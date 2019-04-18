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

    function __construct() {
        $this->tableName = '';
        $this->tableFields = array();
        $this->tableRow = array();
    }

    function setTableName($name) {
        $this->tableName = $name;
    }

    function setTableFields($fields) {
        foreach ($fields as $field) {
            $this->tableFields[] = $field;
        }
    }

    protected function createTableIfNotExists($sql) {
        global $mysqliLink;
        $mysqliLink->query($sql);
    }

    protected function getElementBy($key, $value) {
        global $mysqliLink;
        if (!$this->isInitalized($key)) {
            return false;
        }
        $sql = 'SELECT * FROM `' . $this->tableName . '` WHERE `' . $key . '`="' . $value . '";';
        $res = $mysqliLink->query($sql);
        if ($res) {
            $this->tableRow = $res->fetch_assoc();
            return true;
        } else {
            return false;
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

    private function isInitalized($key) {
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
                error_log('A '.$key.' mezo nem szerepel a tablaban !');
                return false;
            }
            $keys .= '`' . $key . '`';
            $values .= '"' . $value . '"';
            if (++$num < count($row)) {
                $keys .= ', ';
                $values .= ', ';
            }
        }
        $sql .= '( '.$keys . ') VALUES (' . $values . ');';
        return $mysqliLink->query($sql);
    }

    protected function deleteElementBy($key, $value) {
        global $mysqliLink;
        $sql = 'DELETE FROM `' . $this->tableName . '` WHERE `'.$key.'`="'.$value.'";';
        $mysqliLink->query($sql);
    }
    
    protected function updateElements($whereKey, $whereValue, $updateKey, $updateValue) {
        global $mysqliLink;
        $sql = 'UPDATE `' . $this->tableName . '` SET `'.$updateKey.'`="'.$updateValue.'" '
                . 'WHERE `'.$whereKey.'`="'.$whereValue.'";';
        $mysqliLink->query($sql);
        
    }
}
