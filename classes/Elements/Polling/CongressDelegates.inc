<?php

define("CONGRESS_DELEGATES_TABLE", 'congress_delegates');
define("CONGRESS_DELEGATES_ID", 'id');
define("CONGRESS_DELEGATES_MEMBER_ID", 'member_id');
define("CONGRESS_DELEGATES_CONGRESS_ID", 'congress_id');

class CongressDelegates extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_DELEGATES_TABLE);
        $this->setTableFields([
            CONGRESS_DELEGATES_ID,
            CONGRESS_DELEGATES_MEMBER_ID,
            CONGRESS_DELEGATES_CONGRESS_ID
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_DELEGATES_TABLE . '` (
                `' . CONGRESS_DELEGATES_ID . '` int(8) NOT NULL AUTO_INCREMENT,
                `' . CONGRESS_DELEGATES_CONGRESS_ID . '` int(8) NULL DEFAULT NULL,
                `' . CONGRESS_DELEGATES_MEMBER_ID . '` int(11) NOT NULL,
                UNIQUE KEY `' . CONGRESS_DELEGATES_ID . '` (`' . CONGRESS_DELEGATES_ID . '`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->key = CONGRESS_DELEGATES_ID;
    }

    function removeAllMembers($congress_id) {
        $this->deleteElementBy(CONGRESS_DELEGATES_CONGRESS_ID, $congress_id);
    }

    function addMember($congress_id, $mem_id) {
        $this->insertElement([
            CONGRESS_DELEGATES_CONGRESS_ID => $congress_id,
            CONGRESS_DELEGATES_MEMBER_ID => $mem_id,
        ]);
    }

    function countOfMembers($congress_id) {
        return count($this->getMembersOfCongress($congress_id));
    }

    function getMembersOfCongress($congress_id) {
        $ret = array();
        foreach ($this->getElementsBy(CONGRESS_DELEGATES_CONGRESS_ID, $congress_id) as $delegate) {
            $ret[] = $delegate[CONGRESS_DELEGATES_MEMBER_ID];
        }
        return $ret;
    }

}
