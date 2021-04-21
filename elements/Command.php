<?php

define("COMMAND_TABLE", 'command');
define("COMMAND_MEMBER_ID", 'mem_id');
define("COMMAND_CODE", 'code');
define("COMMAND_EXPIRE", 'expire');
define("COMMAND_NAME", 'name');
define("COMMAND_VALUE", 'value');

class Command extends BasicElement {

    const PASSWORD = 'password';
    const EMAIL_VOTE = 'email_vote';

    private static $definedCommands = [
        self::PASSWORD => [
            'expire' => 'TIMESTAMPADD(DAY,1 ,CURRENT_TIMESTAMP())',
            'valueIsArray' => false,
            'module_name' => 'profile',
            'fixed' => [
                'login_by_email' => true,
                'login_normal' => 'coded',
                'radio_edit_member' => 'passwd',
                'remember_me' => 'false',
            ],
            'variables' => [
                'member_pass' => NULL,
            ],
        ],
        self::EMAIL_VOTE => [
            'valueIsArray' => true,
            'module_name' => 'vote_by_email',
            'variables' => [
                'member_pass' => NULL,
            ],
        ],
    ];
    private $mem_id, $code, $name, $value, $expire;

    function __construct($name = '') {
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
        $this->expire = '"' . date('Y-m-d I:m:s' . time()) . '"';
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
            return true;
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
        if (isset(self::$definedCommands[$this->name]['expire'])) {
            $this->expire = self::$definedCommands[$this->name]['expire'];
        }
        return $this->putElement($mem_id, $value);
    }

    private function putElement($mem_id, $value) {
        global $mysqliLink;
        if (is_array($value)) {
            $escaped_value = $mysqliLink->real_escape_string(serialize($value));
        } else {
            $escaped_value = $mysqliLink->real_escape_string($value);
        }
        while ($this->isCodeAlready($code = randomPassword()));  //Ez itt nem jelszo, hanem csak egy random azonosito, ha létezik már, újra generáljuk
        $sql = 'INSERT INTO `' . COMMAND_TABLE . '` '
                . '(`' . COMMAND_MEMBER_ID . '`, `' . COMMAND_CODE . '`,`' . COMMAND_EXPIRE . '`,`' . COMMAND_NAME . '`,`' . COMMAND_VALUE . '`) '
                . ' VALUES (' . $mem_id . ', "' . $code . '", ' . $this->expire . ' , "' . $this->name . '", "' . $escaped_value . '");';
        $mysqliLink->query($sql);
        return $code;
    }

    private function isCodeAlready($code) { // Elvileg már létezhet a véletlenül generált.
        return !empty($this->getElementBy(COMMAND_CODE, $code));
    }

    private function removeExpiredCommands() {
        global $mysqliLink;
        $sql = 'DELETE FROM `' . COMMAND_TABLE . '` WHERE `' . COMMAND_EXPIRE . '` < CURRENT_TIMESTAMP();';
        $mysqliLink->query($sql);
    }

    public function setExpire($expire) {
        $this->expire = $expire;
        return $this;
    }

    public function putPatternCommand($value) {
        return $this->putElement(-1, $value);
    }

    public function putItemByPattern($mem_id, $pattern_code) {
        if (empty($this->getItemById($pattern_code))) {
            error_log('Pattern code not found in Command:' . $pattern_code);
            return '--------';
        }
        $value = $this->tableRow[COMMAND_VALUE];
        $this->expire = '"' . $this->tableRow[COMMAND_EXPIRE] . '"';
        return $this->putItem($mem_id, $value);
    }

    public function getItems() {
        
    }

    public function getMemId() {
        return $this->mem_id;
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
