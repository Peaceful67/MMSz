<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of BasicElement
 *
 * @author Peaceful
 */
class BasicElement {

    private $tableName;
    private $tableFields;
    private $tableRow;

    function __construct() {
        $this->tableName = '';
        $this->tableFields = array();
        $this->tableRow = array();
    }

    protected function setTableName($name) {
        $this->tableName = $name;
    }

    protected function setTableFields($fields) {
        foreach ($fields as $field) {
            $this->tableFiels[] = $field;
        }
    }

    protected function createTableIfNotExists($sql) {
        global $mysqliLink;
        $mysqliLink->query($sql);
    }

    protected function getElementBy($key, $value) {
        global $mysqliLink;
        if (!$this->isInitalized()) {
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

    private function isInitalized() {
        if (isempty($this->tableName)) {
            return false;
        }
        if (!in_array($key, $this->tableFields)) {
            return false;
        }
        return true;
    }

    protected function insertElement($row) {
        global $mysqliLink;
        if (!$this->isInitalized()) {
            return false;
        }
        $sql = 'INSERT INTO `' . $this->tableName . '` ';
        $keys = '';
        $values = '';
        $num = 0;
        foreach ($row as $key => $value) {
            if (!in_array($key, $this->tableFields)) {
                return false;
            }
            $keys .= '`' . $key . '`';
            $values .= '"' . $value . '"';
            if (++$num < count($row)) {
                $keys .= ', ';
                $values .= ', ';
            }
        }
        $sql .= $keys . ' VALUES (' . $values . ');';
        return $mysqliLink->query($sql);
    }

}
