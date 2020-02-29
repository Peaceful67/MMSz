<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Command
 *
 * @author Peaceful
 */

class Command extends BasicElement {

    public $mem_id, $code, $expire, $name, $value;

    function __construct() {
        parent::__construct();
        $this->setTableName(COMMAND_TABLE);
        $this->setTableFields([COMMAND_MEMBER_ID, COMMAND_CODE, COMMAND_EXPIRE, COMMAND_NAME, COMMAND_VALUE]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `command` (
                    `mem_id` int(8) NOT NULL,
                    `code` varchar(8) NOT NULL,
                    `expire` timestamp NOT NULL,
                    `name` varchar(8) NOT NULL,
                    `value` varchar(32) NOT NULL,
                    UNIQUE KEY `code` (`code`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8; '
        );
        $this->removeExpiredCommands();
    }

    public function getItemOnce($code) {
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . COMMAND_TABLE . '` WHERE `' . COMMAND_CODE . '`="' . $code . '";';
        $res = $mysqliLink->query($sql);
        if ($res) {
            $row = $res->fetch_assoc();
            $this->mem_id = $row[COMMAND_MEMBER_ID];
            $this->code = $row[COMMAND_CODE];
            $this->name = $row[COMMAND_NAME];
            $this->value = $row[COMMAND_VALUE];
            $sql = 'DELETE FROM `' . COMMAND_TABLE . '` WHERE `' . COMMAND_CODE . '`="'.$code.'";';
            $mysqliLink->query($sql);
            return true;
        } else {
            return false;
        }
    }

    public function putPassword($mem_id, $passwd) {
        $exp = 'DAY,1';
        $code = $this->putItem($mem_id, COMMAND_NAME_PASSWORD, $passwd, $exp);
        return $code;
    }

    public function putItem($mem_id, $name, $value, $expire) {
        global $mysqliLink;
        $code = randomPassword();  //Ez itt nem jelszo, hanem csak egy random azonosito
        $sql = 'INSERT INTO `' . COMMAND_TABLE . '` '
                . '(`' . COMMAND_MEMBER_ID . '`, `' . COMMAND_CODE . '`,`' . COMMAND_EXPIRE . '`,`' . COMMAND_NAME . '`,`' . COMMAND_VALUE . '`) '
                . ' VALUES (' . $mem_id . ', "' . $code . '", TIMESTAMPADD(' . $expire . ',CURRENT_TIMESTAMP()), "' . $name . '", "' . $value . '");';
        $mysqliLink->query($sql);
        return $code;
    }

    private function removeExpiredCommands() {
        global $mysqliLink;
        $sql = 'DELETE FROM `' . COMMAND_TABLE . '` WHERE `' . COMMAND_EXPIRE . '` < CURRENT_TIMESTAMP();';
        $mysqliLink->query($sql);
    }

    public function isCommandPassword() {
        return isset($this->name) AND $this->name == COMMAND_NAME_PASSWORD;
    }

}
