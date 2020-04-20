<?php

define("CONGRESS_ONLINE_TABLE", 'congress_online');
define("CONGRESS_ONLINE_ID", 'id');
define("CONGRESS_ONLINE_TITLE", 'title');
define("CONGRESS_ONLINE_DESCRIPTION", 'desc');
define("CONGRESS_ONLINE_CREATOR_ID", 'creator');
define("CONGRESS_ONLINE_CREATED", 'created');
define("CONGRESS_ONLINE_TO_BE_CLOSED", 'to_close');
define("CONGRESS_ONLINE_OPTIONS", 'options');

define("CONGRESS_OPTIONS_MULTICHOICE", 1);
define("CONGRESS_OPTIONS_BY_NAME", 2);
define("CONGRESS_OPTIONS_PUBLIC", 4);
define("CONGRESS_OPTIONS_BY_EMAIL", 8);
define("CONGRESS_OPTIONS_APPENDABLE", 16);

class CongressOnline extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_ONLINE_TABLE);
        $this->setTableFields([
            CONGRESS_ONLINE_ID,
            CONGRESS_ONLINE_TITLE,
            CONGRESS_ONLINE_DESCRIPTION,
            CONGRESS_ONLINE_CREATOR_ID,
            CONGRESS_ONLINE_CREATED,
            CONGRESS_ONLINE_TO_BE_CLOSED,
            CONGRESS_ONLINE_OPTIONS
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_ONLINE_TABLE . '` (
                `' . CONGRESS_ONLINE_ID . '` int(8) NOT NULL AUTO_INCREMENT,
                `' . CONGRESS_ONLINE_TITLE . '` varchar(256) NOT NULL,
                `' . CONGRESS_ONLINE_DESCRIPTION . '` text NOT NULL,
                `' . CONGRESS_ONLINE_CREATOR_ID . '` int(8) NOT NULL,
                `' . CONGRESS_ONLINE_CREATED . '` timestamp(1) NULL DEFAULT NULL,
                `' . CONGRESS_ONLINE_TO_BE_CLOSED . '` timestamp(1) NULL DEFAULT NULL,
                `' . CONGRESS_ONLINE_OPTIONS . '` int(11) NOT NULL,
                UNIQUE KEY `' . CONGRESS_ONLINE_ID . '` (`' . CONGRESS_ONLINE_ID . '`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->key = CONGRESS_ONLINE_ID;
    }

    public function getOpenCongresses() {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_ONLINE_TABLE . '` WHERE `' . CONGRESS_ONLINE_CREATED . '` IS NULL;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    public function newCongress() {
        global $member_id;
        return $this->insertElement([
                    CONGRESS_ONLINE_CREATOR_ID => $member_id,
        ]);
    }

    public function modifyCongressTitle($id, $title) {
        $this->updateElements(CONGRESS_ONLINE_ID, $id, CONGRESS_ONLINE_TITLE, $title);
    }

    public function getCongressTitle($id) {
        return $this->getItemById($id)[CONGRESS_ONLINE_TITLE];
    }

    public function modifyCongressDescription($id, $description) {
        $this->updateElements(CONGRESS_ONLINE_ID, $id, CONGRESS_ONLINE_DESCRIPTION, $description);
    }

    public function getCongressDescription($id) {
        return $this->getItemById($id)[CONGRESS_ONLINE_DESCRIPTION];
    }

    public function closeCongress($id) {
        global $now;
        $this->updateElements(CONGRESS_ONLINE_ID, $id, CONGRESS_ONLINE_CREATED, $now);
    }

}
