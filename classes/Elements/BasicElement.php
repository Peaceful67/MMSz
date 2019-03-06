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

    public $tableName;
    public $tableFields = array();

    public function __construct() {
        
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

}
