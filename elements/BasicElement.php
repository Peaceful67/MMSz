<?php

/**
 * Description of BasicElement: very plain database element
 *
 * @author Peaceful
 */
abstract class BasicElement {

    protected $tableName;
    protected $tableFields;
    protected $tableRow;
    protected $key;
    protected $itemId;
    private $variables;

    abstract public function getItems();

    abstract public function isDeletable($id);

    abstract public function getNames();

    function __construct() {
        $this->tableName = '';
        $this->tableFields = array();
        $this->tableRow = array();
        $this->key = '';
        $this->itemId = -1;
        $this->variables = '';
    }

    protected function setTableName($name) {
        $this->tableName = $name;
    }

    protected function setPrimaryKey($key) {
        $this->key = $key;
    }

    protected function setVariablesName($variables) {
        $this->variables = $variables;
        $this->tableFields[] = $variables;
    }

    protected function setTableFields($fields) {
        foreach ($fields as $field) {
            $this->tableFields[] = $field;
        }
    }

    protected function setItemId($id) {
        $this->itemId = $id;
    }

    protected function updateItem() {
        if ($this->itemId > 0) {
            $this->getItemById($this->itemId);
        }
        return $this;
    }

    protected function getElement($id) {
        if ($this->itemId == $id) {
            if (empty($this->tableRow)) {
                $this->getItemById($id);
            }
        } else {
            $this->getItemById($id);
        }
        return $this;
    }

    protected function getNextId($key) {
        global $mysqliLink;
        $sql = 'SELECT `' . $key . '` FROM `' . $this->tableName . '` ORDER BY `' . $key . '` ASC LIMIT 1;';
        $res = $mysqliLink->query($sql);
        $ret = 1;
        if ($res AND $row = $res->fetch_assoc()) {
            $ret = intval($row[$key]) + 1;
        }
        return $ret;
    }

    protected function createTableIfNotExists($sql) {
        global $mysqliLink, $existingTables;
        if (!in_array($this->tableName, $existingTables)) {
            error_log('BasicElement/createTableIfNotExists: ' . $sql);
            $mysqliLink->query($sql);
        }
        if (!empty($this->variables)) {
            $mysqliLink->query('ALTER TABLE `' . $this->tableName . '` ADD `' . $this->variables . '` VARCHAR(254) NOT NULL DEFAULT "";');
        }
    }

    protected function getElementBy($key, $value) {
        global $mysqliLink;
        if (!$this->isInitalized($key)) {
            return false;
        }
        $sql = 'SELECT * FROM `' . $this->tableName . '` WHERE `' . $key . '`="' . $value . '" LIMIT 1;';
        $res = $mysqliLink->query($sql);
        if ($res) {
            $this->tableRow = $res->fetch_assoc();
            $this->itemId = $this->tableRow[$this->key];
            return $this->tableRow;
        } else {
            return array();
        }
    }

    protected function getElementsBy($key, $value) {
        global $mysqliLink;
        if (!$this->isInitalized($key)) {
            return array();
        }
        $sql = 'SELECT * FROM `' . $this->tableName . '` WHERE `' . $key . '`="' . $value . '"';
        $res = $mysqliLink->query($sql);
        $ret = array();
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[$this->key]] = $row;
        }
        return $ret;
    }

    function getElementsByArray($arr) {
        global $mysqliLink;
//        error_log('getElementsByArray: '.print_r($arr, true));
        $sql = 'SELECT * FROM `' . $this->tableName . '` WHERE ';
        $all = true;
        foreach ($arr as $key => $value) {
//            error_log('Key:'.$key.' value:'.$value);
            if (!$this->isInitalized($key)) {
                return array();
            }
            if (is_string($value)) {
                if (strlen($value) > 2) {
                    $sql .= ' `' . $key . '` LIKE "%' . $value . '%" AND';
                    $all = false;
                }
            } elseif ($value > 0) {
                $sql .= ' `' . $key . '`=' . $value . ' AND';
                $all = false;
            }
        }
        if ($all) {
            $sql .= '1;';
        } else {
            $sql = substr($sql, 0, -3);
        }
        //      error_log('getElementsByArray: '.$sql);
        $res = $mysqliLink->query($sql);
        $ret = array();
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[$this->key]] = $row;
        }
        return $ret;
    }

    protected function getElementsOrderBy($key) {
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . $this->tableName . '` ORDER BY `' . $key . '`;';
        $res = $mysqliLink->query($sql);
        $ret = array();
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[$this->key]] = $row;
        }
        return $ret;
    }

    protected function getElementsIsNull($key) {
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . $this->tableName . '` WHERE `' . $key . '` IS NULL;';
        $res = $mysqliLink->query($sql);
        $ret = array();
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[$this->key]] = $row;
        }
        return $ret;
    }

    public function getItemById($id) {
        if (!empty($this->key) AND $this->getElementBy($this->key, $id)) {
            $this->itemId = $id;
            return $this->tableRow;
        } else {
            $this->itemId = -1;
            $this->tableRow = array();
            return array();
        }
    }

    protected function getBasicItem() {
        if ($this->itemId > 0) {
            return $this->getItemById($this->itemId);
        } else {
            return array();
        }
    }

    protected function getValue($key) {
        if (empty($this->tableRow)) {
            $item = $this->getBasicItem();
            if (empty($item)) {
                return '';
            } else {
                return $item[$key];
            }
        }
        if (isset($this->tableRow[$key])) {
            return $this->tableRow[$key];
        } else {
            return '';
        }
    }

    protected function isValueNull($key) {
        return is_null($this->tableRow[$key]);
    }

    public function updateValue($updateKey, $updateValue) {
        if (empty($this->tableRow)) {
            error_log('Update value: Not set: ' . print_r(debug_backtrace(), true));
            return;
        }
        if (in_array($updateKey, $this->tableFields)) {
            $this->tableRow[$updateKey] = $updateValue;
            $this->updateElements($this->key, $this->tableRow[$this->key], $updateKey, $updateValue);
        } else {
            error_log('Fields: ' . print_r($this->tableFields, true));
            error_log('Update value: ' . $updateKey . ' not a set field');
        }
        return $this;
    }

    protected function updateTimestamp($key, $timestamp) {
        global $mysqliLink;
        $sql = 'UPDATE `' . $this->tableName . '` SET `' . $key . '`=' . $timestamp
                . ' WHERE `' . $this->key . '`="' . $this->itemId . '";';
        return $mysqliLink->query($sql);
    }

    protected function isInitalized($key) {
        if (empty($this->tableName)) {
            return false;
        }
        if (!in_array($key, $this->tableFields)) {
            return false;
        }
        return true;
    }

    protected function insertElement($row) {
        global $mysqliLink;
        $sql = 'INSERT INTO `' . $this->tableName . '` ';
        $keys = '';
        $values = '';
        $num = 0;
        foreach ($row as $key => $value) {
            if (!in_array($key, $this->tableFields)) {
                error_log('A ' . $key . ' mezo nem szerepel a tablaban !');
                return false;
            }
            $keys .= '`' . $key . '`';
            if (is_numeric($value)) {
                $values .= $value;
            } else {
                $values .= '"' . $mysqliLink->real_escape_string($value) . '"';
            }
            if (++$num < count($row)) {
                $keys .= ', ';
                $values .= ', ';
            }
        }
        $sql .= '( ' . $keys . ') VALUES (' . $values . ');';
        $mysqliLink->query($sql);
        return $mysqliLink->insert_id;
    }

    protected function deleteElementBy($key, $value) {
        global $mysqliLink;
        $sql = 'DELETE FROM `' . $this->tableName . '` WHERE `' . $key . '`="' . $value . '";';
        $mysqliLink->query($sql);
    }

    protected function deleteElementById($id) {
        $this->deleteElementBy($this->key, $id);
    }

    protected function setTimeCurrent($key) {
        return $this->updateTimestamp($key, 'CURRENT_TIME()');
    }

    protected function updateElements($whereKey, $whereValue, $updateKey, $updateValue) {
        global $mysqliLink;
        if (is_numeric($updateValue)) {
            $value = $updateValue;
        } else {
            $value = '"' . $mysqliLink->real_escape_string($updateValue) . '"';
        }
        $sql = 'UPDATE `' . $this->tableName . '` SET `' . $updateKey . '`=' . $value
                . ' WHERE `' . $whereKey . '`="' . $whereValue . '";';
        //      error_log('Update elements:'.$sql);
        return $mysqliLink->query($sql);
    }

    protected function updateElementsByArr($whereArr, $updateKey, $updateValue) {
        global $mysqliLink;
        if (is_numeric($updateValue)) {
            $value = $updateValue;
        } else {
            $value = '"' . $mysqliLink->real_escape_string($updateValue) . '"';
        }
        $sql = 'UPDATE `' . $this->tableName . '` SET `' . $updateKey . '`=' . $value
                . ' WHERE ';
        if (empty($whereArr)) {
            $sql .= ' 1;';
        } else {
            foreach ($whereArr as $whereKey => $whereValue) {
                $sql .= ' `' . $whereKey . '` = "' . $whereValue . '" AND';
            }
            $sql = substr($sql, 0, -3) . ';';
        }
//       error_log('Update elements:'.$sql);
        return $mysqliLink->query($sql);
    }

    protected function updateElementById($id, $updateKey, $updateValue) {
        return $this->updateElements($this->key, $id, $updateKey, $updateValue);
    }

    protected function updateElement($updateKey, $updateValue) {
        return $this->updateElementById($this->itemId, $updateKey, $updateValue);
    }

    protected function isElementDeletable($key_arr) {
        global $mysqliLink;
        foreach ($key_arr as $table => $key) {
            $sql = 'SELECT * FROM `' . $table . '` WHERE `' . $key . '`=' . $this->itemId . ';';
            $res = $mysqliLink->query($sql);
//            error_log('isElementDeletable' . $sql);
            if ($res AND $res->num_rows > 0) {
                return false;
            }
        }
        return true;
    }

    protected function setVariable($var_name, $value) {
        if (!empty($this->variables)) {
            $arr = unserialize($this->getValue($this->variables));
            $arr[$var_name] = $value;
            $this->updateElement($this->variables, serialize($arr));
        }
    }

    protected function getVariable($var_name) {
        if (empty($this->variables)) {
            return NULL;
        }
        $arr = unserialize($this->getValue($this->variables));
        return isset($arr[$var_name]) ? $arr[$var_name] : NULL;
    }

}
