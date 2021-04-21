<?php

define("CONGRESS_DELEGATES_TABLE", 'congress_delegates');
define("CONGRESS_DELEGATES_ID", 'id');
define("CONGRESS_DELEGATES_MEMBER_ID", 'member_id');
define("CONGRESS_DELEGATES_CONGRESS_ID", 'congress_id');
define("CONGRESS_DELEGATES_MANDATE", 'mandate');

class CongressDelegates extends BasicElement {

    private $selectedCongress = -1;

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_DELEGATES_TABLE);
        $this->setTableFields([
            CONGRESS_DELEGATES_ID,
            CONGRESS_DELEGATES_MEMBER_ID,
            CONGRESS_DELEGATES_CONGRESS_ID,
            CONGRESS_DELEGATES_MANDATE,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_DELEGATES_TABLE . '` (`'
                . CONGRESS_DELEGATES_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . CONGRESS_DELEGATES_CONGRESS_ID . '` int(8) NULL DEFAULT NULL,`'
                . CONGRESS_DELEGATES_MANDATE . '` int(8) NOT NULL DEFAULT 0,`'
                . CONGRESS_DELEGATES_MEMBER_ID . '` int(11) NOT NULL, '
                . 'UNIQUE KEY `' . CONGRESS_DELEGATES_ID . '` (`' . CONGRESS_DELEGATES_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_DELEGATES_ID);
    }

    function setSelectedCongress($congress_id) {
        $this->selectedCongress = $congress_id;
        return $this;
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

    function alterMember($congress_id, $mem_id) {
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . CONGRESS_DELEGATES_TABLE . '` WHERE `'
                . CONGRESS_DELEGATES_CONGRESS_ID . '`=' . $congress_id . ' AND `' . CONGRESS_DELEGATES_MEMBER_ID . '`=' . $mem_id . ';';
        $res = $mysqliLink->query($sql);
        if ($res AND $row = $res->fetch_assoc()) {
            $this->deleteElementBy($this->key, $row[CONGRESS_DELEGATES_ID]);
        } else {
            $this->addMember($congress_id, $mem_id);
        }
    }

    function countOfMembers($congress_id) {
        return count($this->getMembersOfCongress($congress_id));
    }

    function numOfTotalVotes($congress_id) {
        $ret = 0;
        foreach ($this->getElementsBy(CONGRESS_DELEGATES_CONGRESS_ID, $congress_id) as $delegate) {
            $ret += max($delegate[CONGRESS_DELEGATES_MANDATE], 1);
        }
        return $ret;
    }

    function deleteCongress($congress_id) {
        $this->deleteElementBy(CONGRESS_DELEGATES_CONGRESS_ID, $congress_id);
    }

    function getMandateNumber($del_id) {
        $items = $this->getElementsByArray([
            CONGRESS_DELEGATES_CONGRESS_ID => intval($this->selectedCongress),
            CONGRESS_DELEGATES_MEMBER_ID => intval($del_id),
        ]);
        if (empty($items)) {
            return -1;
        } else {
            return array_values($items)[0][CONGRESS_DELEGATES_MANDATE];
        }
    }

    function setCongressIdByPollId($poll_id) {
        $congPoll = new CongressPoll();
        $congPoll->setSelectedPollId($poll_id);
        $congress_id = $congPoll->getCongressId();
        $this->setSelectedCongress($congress_id);
        return $this;
    }

    function setMandateOfDelegates($congress_id) {
        $mand_type = (new CongressOnline())->setSelectedCongress($congress_id)->getMandateType();
        foreach ($this->getMembersOfCongress($congress_id) as $del_id => $del_name) {
            $num = 0;
            switch ($mand_type) {
                case CONGRESS_MANDATE_TYPE_ONE:
                    break;
                case CONGRESS_MANDATE_TYPE_RANGE:
                    $clubs = getClubsOfDelegate($del_id);
                    $num = 0;
                    foreach ($clubs as $club_id => $club_name) {
                        $num += getMandateOfClubByRange($club_id);
                    }
                    break;
                case CONGRESS_MANDATE_TYPE_CLUB_2:
                    $club_mandate_index = CLUB_MANDATUM2;
                case CONGRESS_MANDATE_TYPE_CLUB:
                    if (!isset($club_mandate_index)) {
                        $club_mandate_index = CLUB_MANDATUM;
                    }
                    $clubs = getClubsOfDelegate($del_id);
                    $num = 0;
                    foreach ($clubs as $club_id => $club_name) {
                        $num += getClubById($club_id)[$club_mandate_index];
                    }
                    $num = max($num, 1);
                    break;
            }
            $this->updateElementsByArr(
                    [
                CONGRESS_DELEGATES_CONGRESS_ID => $congress_id,
                CONGRESS_DELEGATES_MEMBER_ID => $del_id
                    ], CONGRESS_DELEGATES_MANDATE, $num);
        }
    }

    function getMembersOfCongress($congress_id) {
        $ret = array();
        foreach ($this->getElementsBy(CONGRESS_DELEGATES_CONGRESS_ID, $congress_id) as $delegate) {
            $del_id = $delegate[CONGRESS_DELEGATES_MEMBER_ID];
            $ret[$del_id] = getMemberName($del_id);
        }
        return $ret;
    }

    function getCongressOfMember($mem_id) {
        $ret = array();
        foreach ($this->getElementsBy(CONGRESS_DELEGATES_MEMBER_ID, $mem_id) as $congress) {
            $ret[] = $congress[CONGRESS_DELEGATES_CONGRESS_ID];
        }
        return $ret;
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
