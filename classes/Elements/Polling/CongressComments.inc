<?php

define("CONGRESS_COMMENTS_TABLE", 'congress_comments');
define("CONGRESS_COMMENTS_ID", 'id');
define("CONGRESS_COMMENTS_MEMBER_ID", 'member_id');
define("CONGRESS_COMMENTS_TITLE", 'title');
define("CONGRESS_COMMENTS_DESCRIPTION", 'desc');
define("CONGRESS_COMMENTS_TIME", 'comm_time');
define("CONGRESS_COMMENTS_CONGRESS_ITEM_ID", 'item_id');

class CongressComments extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_COMMENTS_TABLE);
        $this->setTableFields([
            CONGRESS_COMMENTS_ID,
            CONGRESS_COMMENTS_MEMBER_ID,
            CONGRESS_COMMENTS_TITLE,
            CONGRESS_COMMENTS_DESCRIPTION,
            CONGRESS_COMMENTS_TIME,
            CONGRESS_COMMENTS_CONGRESS_ITEM_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `'.CONGRESS_COMMENTS_TABLE.'` (
                `'.CONGRESS_COMMENTS_ID.'` int(8) NOT NULL AUTO_INCREMENT,
                `'.CONGRESS_COMMENTS_MEMBER_ID.'` int(8) NULL DEFAULT NULL,
                `'.CONGRESS_COMMENTS_TITLE.'` varchar(256) NOT NULL,
                `'.CONGRESS_COMMENTS_DESCRIPTION.'` varchar(256) NOT NULL,
                `'.CONGRESS_COMMENTS_TIME.'` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1) ,
                `'.CONGRESS_COMMENTS_CONGRESS_ITEM_ID.'` int(11) NOT NULL,
                UNIQUE KEY `'.CONGRESS_COMMENTS_ID.'` (`'.CONGRESS_COMMENTS_ID.'`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->key = CONGRESS_COMMENTS_ID;
    }

}

