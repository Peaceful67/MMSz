<?php

define("CONGRESS_ONLINE_TABLE", 'congress_online');
define("CONGRESS_ONLINE_ID", 'id');
define("CONGRESS_ONLINE_TITLE", 'title');
define("CONGRESS_ONLINE_DESCRIPTION", 'desc');
define("CONGRESS_ONLINE_CREATOR_ID", 'creator');
define("CONGRESS_ONLINE_CREATED", 'created');
define("CONGRESS_ONLINE_CLOSED", 'closed');
define("CONGRESS_ONLINE_OPTIONS", 'options');
define("CONGRESS_ONLINE_AUTH_ID_1", 'auth1');
define("CONGRESS_ONLINE_AUTH_ID_2", 'auth2');

class CongressOnline extends BasicElement {

    protected $selectedCongress = -1;

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_ONLINE_TABLE);
        $this->setTableFields([
            CONGRESS_ONLINE_ID,
            CONGRESS_ONLINE_TITLE,
            CONGRESS_ONLINE_DESCRIPTION,
            CONGRESS_ONLINE_CREATOR_ID,
            CONGRESS_ONLINE_CREATED,
            CONGRESS_ONLINE_CLOSED,
            CONGRESS_ONLINE_OPTIONS,
            CONGRESS_ONLINE_AUTH_ID_1,
            CONGRESS_ONLINE_AUTH_ID_2,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_ONLINE_TABLE . '` (`'
                . CONGRESS_ONLINE_ID . '` int(8) NOT NULL AUTO_INCREMENT`, '
                . CONGRESS_ONLINE_TITLE . '` varchar(256) NOT NULL, `'
                . CONGRESS_ONLINE_DESCRIPTION . '` text NOT NULL,`'
                . CONGRESS_ONLINE_CREATOR_ID . '` int(8) NOT NULL,`'
                . CONGRESS_ONLINE_AUTH_ID_1 . '` int(8)  NOT NULL DEFAULT "-1",`'
                . CONGRESS_ONLINE_AUTH_ID_2 . '` int(8) NOT NULL DEFAULT "-1",`'
                . CONGRESS_ONLINE_CREATED . '` timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_ONLINE_CLOSED . '`  timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_ONLINE_OPTIONS . '` int(11) NOT NULL, '
                . 'UNIQUE KEY `' . CONGRESS_ONLINE_ID . '` (`' . CONGRESS_ONLINE_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_ONLINE_ID);
    }

    public function setCongressEdit($current) {
        $this->setItemId($current);
        $this->selectedCongress = $current;
    }

    public function setCongressAuth1($mem_id) {
        $this->updateElement(CONGRESS_ONLINE_AUTH_ID_1, $mem_id);
    }

    public function setCongressAuth2($mem_id) {
        $this->updateElement(CONGRESS_ONLINE_AUTH_ID_2, $mem_id);
    }

    public function getCongressAuth1() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_AUTH_ID_1]);
    }

    public function getCongressAuth2() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_AUTH_ID_2]);
    }

    public function getOpenCongresses() {
        global $mysqliLink, $member_id;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_ONLINE_TABLE . '` WHERE '
                . ' `' . CONGRESS_ONLINE_CREATOR_ID . '`=' . $member_id . ' AND '
                . ' `' . CONGRESS_ONLINE_CREATED . '` IS NULL;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    function getCreatedCongressesOfDelegate() {
        global $mysqliLink, $member_id;
        $ret = array();
        $sql = 'SELECT `' . CONGRESS_ONLINE_TABLE . '`.* FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . 'INNER JOIN `' . CONGRESS_DELEGATES_TABLE . '` AS `delegate` ON '
                . '(`delegate`.`' . CONGRESS_DELEGATES_MEMBER_ID . '`=' . $member_id . ' AND'
                . '`delegate`.`' . CONGRESS_DELEGATES_CONGRESS_ID . '`=`' . CONGRESS_ONLINE_TABLE . '`.`' . CONGRESS_ONLINE_ID . '`)'
                . ' WHERE  `' . CONGRESS_ONLINE_CREATED . '` IS NOT NULL AND   `' . CONGRESS_ONLINE_CLOSED . '` IS NULL '
                . 'ORDER BY `' . CONGRESS_ONLINE_TITLE . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    function getCreatedCongressesByCreator($mem_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . ' WHERE  `' . CONGRESS_ONLINE_CREATED . '` IS NOT NULL  AND   `' . CONGRESS_ONLINE_CLOSED . '` IS NULL  AND '
                . '`' . CONGRESS_ONLINE_CREATOR_ID . '`=' . $mem_id
                . ' ORDER BY `' . CONGRESS_ONLINE_TITLE . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    public function newCongress() {
        global $member_id;
        $new = $this->insertElement([
            CONGRESS_ONLINE_CREATOR_ID => $member_id,
            CONGRESS_ONLINE_OPTIONS => CONGRESS_OPTIONS_BY_NAME + CONGRESS_OPTIONS_BY_EMAIL + CONGRESS_OPTIONS_AUTH_MANDATORY,
        ]);
        $this->setCongressEdit($new);
        return $new;
    }

    public function modifyCongressTitle($title) {
        $this->updateElement(CONGRESS_ONLINE_TITLE, $title);
    }

    public function getCongressTitle() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_TITLE]);
    }

    public function getCongressCreator() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_CREATOR_ID]);
    }

    public function modifyCongressDescription($description) {
        $this->updateElement(CONGRESS_ONLINE_DESCRIPTION, $description);
    }

    public function getCongressDescription() {
        return $this->getBasicItem()[CONGRESS_ONLINE_DESCRIPTION];
    }

    public function setCongressOption($option_value) {
        $this->updateElement(CONGRESS_ONLINE_OPTIONS, $option_value);
    }

    public function getCongressOption() {
        return $this->getBasicItem()[CONGRESS_ONLINE_OPTIONS];
    }

    public function getCongressCreated() {
        $item = $this->getBasicItem();
        return empty($item) ? NULL : $item[CONGRESS_ONLINE_CREATED];
    }

    public function getCongressClosed() {
        $item = $this->getBasicItem();
        return empty($item) ? NULL : $item[CONGRESS_ONLINE_CLOSED];
    }

    public function isCongressClosed() {
        return !$this->isValueNull(CONGRESS_ONLINE_CLOSED);
    }

    public function amICongressCreator() {
        global $member_id;
        return $member_id == $this->getCongressCreator();
    }

    public function isCongressByEmail() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_BY_EMAIL) > 0) ? true : false;
    }

    public function isCongressByName() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_BY_NAME) > 0) ? true : false;
    }

    public function isCongressPublic() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_PUBLIC) > 0) ? true : false;
    }

    public function isCongressAppendable() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_APPENDABLE) > 0) ? true : false;
    }

    public function isCongressAutoItem() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_AUTO_ITEM) > 0) ? true : false;
    }

    public function isCongressAuthMandatory() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_AUTH_MANDATORY) > 0) ? true : false;
    }

    public function setCongressClosed() {
        $this->setTime(CONGRESS_ONLINE_CLOSED);
    }

    public function closeCongress() {
        global $mysqliLink;
        $sql = 'UPDATE `' . CONGRESS_ONLINE_TABLE . '` SET `' . CONGRESS_ONLINE_CREATED . '`=CURRENT_TIME() '
                . ' WHERE `' . CONGRESS_ONLINE_ID . '`=' . $this->selectedCongress . ';';
        $mysqliLink->query($sql);
    }

    public function deleteCongress($congress_id) {
        $this->deleteElementBy(CONGRESS_ONLINE_ID, $congress_id);
        $this->selectedCongress = $this->itemId = -1;
    }

}
