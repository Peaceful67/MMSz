<?php

define("CONGRESS_INVITED_TABLE", 'congress_invited');
define("CONGRESS_INVITED_ID", 'id');
define("CONGRESS_INVITED_MEMBER_ID", 'member_id');
define("CONGRESS_INVITED_CONGRESS_ID", 'congress_id');

class CongressInvited extends BasicElement {

    protected $selectedCongress;

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_INVITED_TABLE);
        $this->setTableFields([
            CONGRESS_INVITED_ID,
            CONGRESS_INVITED_MEMBER_ID,
            CONGRESS_INVITED_CONGRESS_ID
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_INVITED_TABLE . '` (`'
                . CONGRESS_INVITED_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . CONGRESS_INVITED_CONGRESS_ID . '` int(8) NULL DEFAULT NULL,`'
                . CONGRESS_INVITED_MEMBER_ID . '` int(11) NOT NULL, '
                . 'UNIQUE KEY `' . CONGRESS_INVITED_ID . '` (`' . CONGRESS_INVITED_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_INVITED_ID);
    }

    function setSelectedCongress($congress_id) {
        $this->selectedCongress = $congress_id;
        return $this;
    }

    function removeAllMembers($congress_id) {
        $this->deleteElementBy(CONGRESS_DELEGATES_CONGRESS_ID, $congress_id);
    }

    function addMember($congress_id, $mem_id) {
        return $this->insertElement([
            CONGRESS_INVITED_CONGRESS_ID => $congress_id,
            CONGRESS_INVITED_MEMBER_ID => $mem_id,
        ]);
    }
    function deleteMember($congress_id, $mem_id) {
        global $mysqliLink;
        $sql = 'DELETE FROM `'.CONGRESS_INVITED_TABLE.'` WHERE '
                . '`'.CONGRESS_INVITED_MEMBER_ID.'`='.$mem_id.' AND `'.CONGRESS_INVITED_CONGRESS_ID.'`='.$congress_id.'; ';
        $mysqliLink->query($sql);
    }
    

    function deleteCongress($congress_id) {
        $this->deleteElementBy(CONGRESS_INVITED_CONGRESS_ID, $congress_id);
    }

    function getMembersOfCongress($congress_id) {
        $ret = array();
        foreach ($this->getElementsBy(CONGRESS_INVITED_CONGRESS_ID, $congress_id) as $invited) {
            $mem_id = $invited[CONGRESS_INVITED_MEMBER_ID];
            $ret[$mem_id] = getMemberName($mem_id);
        }
        return $ret;
    }

    function amIInvited($congress_id) {
        global $member_id;
        $invited = $this->getMembersOfCongress($congress_id);
        return isset($invited[$member_id]);
    }

    function getCongressOfMember($mem_id) {
        $ret = array();
        foreach ($this->getElementsBy(CONGRESS_INVITED_MEMBER_ID, $mem_id) as $congress) {
            $ret[] = $congress[CONGRESS_INVITED_CONGRESS_ID];
        }
        return $ret;
    }
function alterMember($congress_id, $mem_id) {
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . CONGRESS_INVITED_TABLE . '` WHERE `'
                . CONGRESS_INVITED_CONGRESS_ID . '`=' . $congress_id . ' AND `' . CONGRESS_INVITED_MEMBER_ID . '`=' . $mem_id . ';';
        $res = $mysqliLink->query($sql);
        if ($res AND $row = $res->fetch_assoc()) {
             $this->deleteMember($congress_id, $mem_id);
        } else {
            $this->addMember($congress_id, $mem_id);
        }
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
