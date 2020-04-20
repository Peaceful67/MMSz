<?php

/**
 * Description of BasicElement: very plain database element
 *
 * @author Peaceful
 */
class BasicElement {

    protected $tableName;
    protected $tableFields;
    protected $tableRow;
    protected $key;
    protected $itemId;

    function __construct() {
        $this->tableName = '';
        $this->tableFields = array();
        $this->tableRow = array();
        $this->key = '';
        $this->itemId = -1;
    }

    protected function setTableName($name) {
        $this->tableName = $name;
    }

    protected function setPrimaryKey($key) {
        $this->key = $key;
    }

    protected function setTableFields($fields) {
        foreach ($fields as $field) {
            $this->tableFields[] = $field;
        }
    }

    protected function setItemId($id) {
        $this->itemId = $id;
    }

    protected function createTableIfNotExists($sql) {
        global $mysqliLink;
        //      error_log($sql);
        $mysqliLink->query($sql);
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

    protected function getItemById($id) {
        if (!empty($this->key) AND $this->getElementBy($this->key, $id)) {
            return $this->tableRow;
        } else {
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
            return '';
        }
        if (isset($this->tableRow[$key])) {
            return $this->tableRow[$key];
        } else {
            return '';
        }
    }

    protected function isValueNull($key) {
        return is_null($this->getValue($key));
    }

    protected function updateValue($updateKey, $updateValue) {
        if (empty($this->tableRow)) {
            error_log('Update value: Not set');
            return;
        }
        if (isset($this->tableRow[$updateKey])) {
            $this->tableRow[$updateKey] = $updateValue;
            $this->updateElements($this->key, $this->tableRow[$this->key], $updateKey, $updateValue);
        }
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
            $values .= '"' . $mysqliLink->real_escape_string($value) . '"';
            if (++$num < count($row)) {
                $keys .= ', ';
                $values .= ', ';
            }
        }
        $sql .= '( ' . $keys . ') VALUES (' . $values . ');';
//        error_log($sql);
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

    protected function setTime($key) {
        global $mysqliLink;
        $sql = 'UPDATE `' . $this->tableName . '` SET `' . $key . '`=CURRENT_TIME() '
                . 'WHERE `' . $this->key . '`="' . $this->itemId . '";';
        return $mysqliLink->query($sql);
    }

    protected function updateElements($whereKey, $whereValue, $updateKey, $updateValue) {
        global $mysqliLink;
        $value = $mysqliLink->real_escape_string($updateValue);
        $sql = 'UPDATE `' . $this->tableName . '` SET `' . $updateKey . '`="' . $value . '" '
                . 'WHERE `' . $whereKey . '`="' . $whereValue . '";';
        //       error_log($sql);
        return $mysqliLink->query($sql);
    }

    protected function updateElement($updateKey, $updateValue) {
        return $this->updateElements($this->key, $this->itemId, $updateKey, $updateValue);
    }

}
