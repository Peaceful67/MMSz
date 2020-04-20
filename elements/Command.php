<?php

define("COMMAND_TABLE", 'command');
define("COMMAND_MEMBER_ID", 'mem_id');
define("COMMAND_CODE", 'code');
define("COMMAND_EXPIRE", 'expire');
define("COMMAND_NAME", 'name');
define("COMMAND_VALUE", 'value');


class Command extends BasicElement {

    const PASSWORD = 'password';

    private static $definedCommands = [
        self::PASSWORD => [
            'expire' => 'DAY,1',
            'valueIsArray' => false,
            'module_name' => 'profile',
            'fixed' => [
                'login_normal' => 'coded',
                'radio_edit_member' => 'passwd',
                'remember_me' => 'true',
            ],
            'variables' => [
                'member_pass' => NULL,
                'old_pass' => NULL
            ],
        ],
    ];
    private $mem_id, $code, $name, $value;

    function __construct($name) {
        parent::__construct();
        $this->setTableName(COMMAND_TABLE);
        $this->setTableFields([COMMAND_MEMBER_ID, COMMAND_CODE, COMMAND_EXPIRE, COMMAND_NAME, COMMAND_VALUE]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . COMMAND_TABLE . '` (`'
                . COMMAND_MEMBER_ID . '` int(8) NOT NULL, `'
                . COMMAND_CODE . '` varchar(20) NOT NULL, `'
                . COMMAND_EXPIRE . '` timestamp(1) NULL DEFAULT NULL, `'
                . COMMAND_NAME . '` varchar(16) NOT NULL, `'
                . COMMAND_VALUE . '` varchar(256) NOT NULL, '
                . 'UNIQUE KEY `' . COMMAND_CODE . '` (`' . COMMAND_CODE . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8; '
        );
        $this->removeExpiredCommands();
        $this->setPrimaryKey(COMMAND_CODE);
        $this->name = $name;
    }

    public function getItemOnce($code) {
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . COMMAND_TABLE . '` WHERE `' . COMMAND_CODE . '`="' . $code . '";';
        $res = $mysqliLink->query($sql);
        if ($res) {
            $row = $res->fetch_assoc();
            $this->mem_id = $row[COMMAND_MEMBER_ID];
            if ($this->mem_id > 0) {
                global $member_id;
                $member_id = $this->mem_id;
            }
            $this->code = $row[COMMAND_CODE];
            $this->name = $row[COMMAND_NAME];
            $this->value = $row[COMMAND_VALUE];
            if (isset(self::$definedCommands[$this->name])) {
                $this->executeCommand();
            } else {
                $ret = $this->value;
            }
            $sql = 'DELETE FROM `' . COMMAND_TABLE . '` WHERE `' . COMMAND_CODE . '`="' . $code . '";';
            $mysqliLink->query($sql);
            return $this->value;
        } else {
            return false;
        }
    }

    private function executeCommand() {
        $com = self::$definedCommands[$this->name];
        if (isset($com['module_name']) AND strlen($com['module_name']) > 0) {
            global $mod;
            $mod = $com['module_name'];
        }
        if (isset($com['fixed'])) {
            foreach ($com['fixed'] as $var_name => $var_value) {
                global $$var_name;
                $$var_name = $var_value;
                error_log('Fixed: ' . $var_name . '=>' . $var_value);
            }
        }
        if (isset($com['valueIsArray'])) {
            if ($com['valueIsArray']) {
                $var_array = unserialize($this->value);
                foreach ($var_array as $var_name => $var_value) {
                    global $$var_name;
                    $$var_name = $var_value;
                }
            } else {
                foreach ($com['variables'] as $var_name => $var_value) {
                    global $$var_name;
                    if (is_null($var_value)) {
                        $$var_name = $this->value;
                        error_log('Variables: ' . $var_name . '=>' . $this->value);
                    } else {
                        global $$var_value;
                        $$varname = $$var_value;
                    }
                }
            }
        }
    }

    public function putItem($mem_id, $value = '') {
        if (!isset(self::$definedCommands[$this->name])) {
            return false;
        }
        $exp = self::$definedCommands[$this->name]['expire'];
        if (is_array($value)) {
            $stored = serialize($value);
        } else {
            $stored = $value;
        }
        return $this->putElement($mem_id, $this->name, $stored, $exp);
    }

    private function putElement($mem_id, $name, $value, $expire) {
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

}
