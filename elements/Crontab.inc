<?php

define("DB_CRONTAB_TABLE", 'crontab');
define("DB_CRONTAB_ID", 'id');
define("DB_CRONTAB_NAME", 'name');
define("DB_CRONTAB_NEXT", 'next');
define("DB_CRONTAB_VARIABLE", 'variable');

class Crontab extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DB_CRONTAB_TABLE);
        $this->setTableFields([
            DB_CRONTAB_ID,
            DB_CRONTAB_NAME,
            DB_CRONTAB_NEXT,
            DB_CRONTAB_VARIABLE,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DB_CRONTAB_TABLE . '` (`'
                . DB_CRONTAB_ID . '` int(8) NOT NULL AUTO_INCREMENT, `'
                . DB_CRONTAB_NAME . '` varchar(16) NOT NULL,`'
                . DB_CRONTAB_VARIABLE . '` text NOT NULL, `'
                . DB_CRONTAB_NEXT . '` timestamp(1) NULL DEFAULT NULL ,'
                . 'UNIQUE KEY `' . DB_CRONTAB_ID . '` (`' . DB_CRONTAB_ID . '`)'
                . ') ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DB_CRONTAB_ID);
    }
    function isTaskToDo($day, $name) {
        return true;
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
