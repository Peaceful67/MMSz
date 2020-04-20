<?php

define("CONGRESS_ITEMS_TABLE", 'congress_items');
define("CONGRESS_ITEMS_ID", 'id');
define("CONGRESS_ITEMS_CONGRESS_ID", 'congress_id');
define("CONGRESS_ITEMS_TITLE", 'title');
define("CONGRESS_ITEMS_DESCRIPTION", 'desc');
define("CONGRESS_ITEMS_CLOSED", 'closed');
define("CONGRESS_ITEMS_OPTIONS", 'options');



class CongressItems extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_ITEMS_TABLE);
        $this->setTableFields([
            CONGRESS_ITEMS_ID,
            CONGRESS_ITEMS_CONGRESS_ID,
            CONGRESS_ITEMS_TITLE,
            CONGRESS_ITEMS_DESCRIPTION,
            CONGRESS_ITEMS_CLOSED,
            CONGRESS_ITEMS_OPTIONS,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `'.CONGRESS_ITEMS_TABLE.'` (
                `'.CONGRESS_ITEMS_ID.'` int(8) NOT NULL AUTO_INCREMENT,
                `'.CONGRESS_ITEMS_CONGRESS_ID.'` int(8) NULL DEFAULT NULL,
                `'.CONGRESS_ITEMS_TITLE.'` varchar(256) NOT NULL,
                `'.CONGRESS_ITEMS_DESCRIPTION.'` text NOT NULL,
                `'.CONGRESS_ITEMS_CLOSED.'` timestamp(1) NULL DEFAULT NULL ,
                `'.CONGRESS_ITEMS_OPTIONS.'` int(11) NOT NULL,
                UNIQUE KEY `'.CONGRESS_ITEMS_ID.'` (`'.CONGRESS_ITEMS_ID.'`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->key = CONGRESS_ITEMS_ID;
    }

}

