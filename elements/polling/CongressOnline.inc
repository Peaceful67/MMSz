<?php

define("CONGRESS_ONLINE_TABLE", 'congress_online');
define("CONGRESS_ONLINE_ID", 'id');
define("CONGRESS_ONLINE_TITLE", 'title');
define("CONGRESS_ONLINE_DESCRIPTION", 'desc');
define("CONGRESS_ONLINE_CREATOR_ID", 'creator');
define("CONGRESS_ONLINE_CREATED", 'created');
define("CONGRESS_ONLINE_CLOSED", 'closed');
define("CONGRESS_ONLINE_VARIABLES", 'variables');
define("CONGRESS_ONLINE_OPTIONS", 'options');
define("CONGRESS_ONLINE_AUTH_ID_1", 'auth1');
define("CONGRESS_ONLINE_AUTH_ID_2", 'auth2');
define("CONGRESS_ONLINE_BY_ROLE", 'by_role');
define("CONGRESS_ONLINE_BY_GROUP", 'by_group');
define("CONGRESS_ONLINE_MANDATE", 'mandate_type');

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
            CONGRESS_ONLINE_CLOSED,
            CONGRESS_ONLINE_OPTIONS,
            CONGRESS_ONLINE_VARIABLES,
            CONGRESS_ONLINE_AUTH_ID_1,
            CONGRESS_ONLINE_AUTH_ID_2,
            CONGRESS_ONLINE_BY_ROLE,        // A tagok kiválasztása ez alapján történt
            CONGRESS_ONLINE_BY_GROUP,       // A tagok kiválasztása ez alapján történt
            CONGRESS_ONLINE_MANDATE,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_ONLINE_TABLE . '` (`'
                . CONGRESS_ONLINE_ID . '` int(8) NOT NULL AUTO_INCREMENT, `'
                . CONGRESS_ONLINE_TITLE . '` varchar(256) NOT NULL, `'
                . CONGRESS_ONLINE_DESCRIPTION . '` text NOT NULL,`'
                . CONGRESS_ONLINE_CREATOR_ID . '` int(8) NOT NULL,`'
                . CONGRESS_ONLINE_AUTH_ID_1 . '` int(8)  NOT NULL DEFAULT "-1",`'
                . CONGRESS_ONLINE_AUTH_ID_2 . '` int(8) NOT NULL DEFAULT "-1",`'
                . CONGRESS_ONLINE_BY_ROLE . '` int(11)  NOT NULL DEFAULT "-1",`'
                . CONGRESS_ONLINE_BY_GROUP . '` int(11) NOT NULL DEFAULT "-1",`'
                . CONGRESS_ONLINE_CREATED . '` timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_ONLINE_CLOSED . '`  timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_ONLINE_MANDATE . '` int(8)  NOT NULL DEFAULT ' . CONGRESS_MANDATE_TYPE_ONE . ',`'
                . CONGRESS_ONLINE_VARIABLES . '` varchar(256) NOT NULL  DEFAULT "", `'
                . CONGRESS_ONLINE_OPTIONS . '` int(11) NOT NULL, '
                . 'UNIQUE KEY `' . CONGRESS_ONLINE_ID . '` (`' . CONGRESS_ONLINE_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_ONLINE_ID);
    }

    public function setSelectedCongress($current) {
        $this->getItemById($current);
        return $this;
    }

    public function setCongressAuth1($mem_id) {
        $this->updateValue(CONGRESS_ONLINE_AUTH_ID_1, $mem_id);
    }

    public function setCongressAuth2($mem_id) {
        $this->updateValue(CONGRESS_ONLINE_AUTH_ID_2, $mem_id);
    }

    public function setCongressByRole($sel_role) {
        $this->updateValue(CONGRESS_ONLINE_BY_ROLE, $sel_role);
    }

    public function setCongressByGroup($sel_group) {
        $this->updateValue(CONGRESS_ONLINE_BY_GROUP, $sel_group);
    }

    public function setCongressMandateType($mand_type) {
        $this->updateValue(CONGRESS_ONLINE_MANDATE, $mand_type);
    }

    public function getCongressAuth1() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_AUTH_ID_1]);
    }

    public function getMandateType() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_MANDATE]);
    }

    public function getCongressByRole() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_BY_ROLE]);
    }

    public function getCongressByGroup() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_BY_GROUP]);
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

    function getCreatedCongressesOfInvited() {
        global $mysqliLink, $member_id;
        $ret = array();
        $sql = 'SELECT `' . CONGRESS_ONLINE_TABLE . '`.* FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . 'INNER JOIN `' . CONGRESS_INVITED_TABLE . '` AS `invited` ON '
                . '(`invited`.`' . CONGRESS_INVITED_MEMBER_ID . '`=' . $member_id . ' AND'
                . '`invited`.`' . CONGRESS_INVITED_CONGRESS_ID . '`=`' . CONGRESS_ONLINE_TABLE . '`.`' . CONGRESS_ONLINE_ID . '`)'
                . ' WHERE  `' . CONGRESS_ONLINE_CREATED . '` IS NOT NULL AND   `' . CONGRESS_ONLINE_CLOSED . '` IS NULL '
                . 'ORDER BY `' . CONGRESS_ONLINE_TITLE . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    function getCongressesOfDelegate() {
        global $mysqliLink, $member_id;
        $ret = array();
        $sql = 'SELECT `' . CONGRESS_ONLINE_TABLE . '`.* FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . 'INNER JOIN `' . CONGRESS_DELEGATES_TABLE . '` AS `delegate` ON '
                . '(`delegate`.`' . CONGRESS_DELEGATES_MEMBER_ID . '`=' . $member_id . ' AND'
                . '`delegate`.`' . CONGRESS_DELEGATES_CONGRESS_ID . '`=`' . CONGRESS_ONLINE_TABLE . '`.`' . CONGRESS_ONLINE_ID . '`)'
                . ' WHERE  1 '
                . 'ORDER BY `' . CONGRESS_ONLINE_ID . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    function getCongressesOfInvited() {
        global $mysqliLink, $member_id;
        $ret = array();
        $sql = 'SELECT `' . CONGRESS_ONLINE_TABLE . '`.* FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . 'INNER JOIN `' . CONGRESS_INVITED_TABLE . '` AS `delegate` ON '
                . '(`delegate`.`' . CONGRESS_INVITED_MEMBER_ID . '`=' . $member_id . ' AND'
                . '`delegate`.`' . CONGRESS_INVITED_CONGRESS_ID . '`=`' . CONGRESS_ONLINE_TABLE . '`.`' . CONGRESS_ONLINE_ID . '`)'
                . ' WHERE  1 '
                . 'ORDER BY `' . CONGRESS_ONLINE_ID . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    function getCongressesOfCreator($mem_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . ' WHERE  `' . CONGRESS_ONLINE_CREATOR_ID . '`=' . $mem_id
                . ' ORDER BY `' . CONGRESS_ONLINE_ID . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    function getPublicCongresses() {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . ' WHERE  (`' . CONGRESS_ONLINE_OPTIONS . '`& ' . CONGRESS_OPTIONS_PUBLIC . ' ) >0 '
                . ' ORDER BY `' . CONGRESS_ONLINE_ID . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ONLINE_ID]] = $row[CONGRESS_ONLINE_TITLE];
        }
        return $ret;
    }

    function getOnlineCongressesByCreator($mem_id) {
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

    function getCreatedCongressesByCreator($mem_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_ONLINE_TABLE . '` '
                . ' WHERE  `' . CONGRESS_ONLINE_CREATED . '` IS NOT NULL  AND '
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
            CONGRESS_ONLINE_TITLE => '',
            CONGRESS_ONLINE_DESCRIPTION => '',
            CONGRESS_ONLINE_CREATOR_ID => $member_id,
            CONGRESS_ONLINE_OPTIONS => (CONGRESS_OPTIONS_BY_NAME) | (CONGRESS_OPTIONS_BY_EMAIL) | (CONGRESS_OPTIONS_AUTH_MANDATORY) | (CONGRESS_OPTIONS_APPENDABLE) | (CONGRESS_OPTIONS_AUTO_ITEM),
        ]);
        $this->setSelectedCongress($new);
        return $new;
    }

    public function modifyCongressTitle($title) {
        $this->updateValue(CONGRESS_ONLINE_TITLE, $title);
    }

    public function getCongressTitle() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_TITLE]);
    }

    public function getCongressCreator() {
        return ($this->getBasicItem()[CONGRESS_ONLINE_CREATOR_ID]);
    }

    public function modifyCongressDescription($description) {
        $this->updateValue(CONGRESS_ONLINE_DESCRIPTION, $description);
    }

    public function getCongressDescription() {
        return $this->getBasicItem()[CONGRESS_ONLINE_DESCRIPTION];
    }

    public function setCongressOption($option_value) {
        $this->updateValue(CONGRESS_ONLINE_OPTIONS, $option_value);
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

    public function amICongressDelegated() {
        $congressesDelegated = $this->getCongressesOfDelegate();
        return (isset($congressesDelegated[$this->itemId]) ? true : false);
    }

    public function isCongressByEmail() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_BY_EMAIL) > 0) ? true : false;
    }

    public function isCongressBaseVoted() {
        return (($this->getCongressOption() & CONGRESS_OPTIONS_POLL_BASE_VOTED) > 0) ? true : false;
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

    public function setCongressClosed() { // Csak akkor zarja be, ha az osszes poll decided
        $items = (new CongressItems())->setSelectedCongress($this->itemId)->getSelectionOfCongress();
        $cPoll = new CongressPoll();
        $congressDecided = true;
        foreach (array_keys($items) as $item_id) {
            if (!$cPoll->getPollByItem($item_id)->isDecided()) {
                $congressDecided = false;
                break;
            }
        }
        if ($congressDecided) {
            $this->setTimeCurrent(CONGRESS_ONLINE_CLOSED);
        }
        return $congressDecided;
    }

    public function createCongress() {
        $this->setTimeCurrent(CONGRESS_ONLINE_CREATED);
        (new CongressDelegates())->setMandateOfDelegates($this->itemId);
    }

    public function deleteCongress($congress_id) {
        $this->deleteElementBy(CONGRESS_ONLINE_ID, $congress_id);
        $this->itemId = -1;
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

    public function setVariable($varname, $value) {
        if (isset($this->itemId) AND $this->itemId > 0) {
            $arr = unserialize($this->getValue(CONGRESS_ONLINE_VARIABLES));
            $arr[$varname] = $value;
            $this->updateElement(CONGRESS_ONLINE_VARIABLES, serialize($arr));
            return true;
        } else {
            return false;
        }
    }

    public function getVaraible($varname) {
        if (isset($this->itemId) AND $this->itemId > 0) {
            $arr = unserialize($this->getValue(CONGRESS_ONLINE_VARIABLES));
            if (isset($arr[$varname])) {
                $arr[$varname];
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

}
