<?php

define('CONGRESS_POLL_VOTE_TABLE', "congress_poll_vote");
define('CONGRESS_POLL_VOTE_ID', "id");
define('CONGRESS_POLL_VOTE_POLL_ID', "poll_id");
define('CONGRESS_POLL_VOTE_DELEGATE', "mem_id");
define('CONGRESS_POLL_VOTE_ANONIM_COUNTER', "anonim_counter");
define('CONGRESS_POLL_VOTE_OPTION_ID', "option_id");
define('CONGRESS_POLL_VOTE_VOTED_TIME', "voted");

class CongressPollVote extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_POLL_VOTE_TABLE);
        $this->setTableFields([
            CONGRESS_POLL_VOTE_ID,
            CONGRESS_POLL_VOTE_POLL_ID,
            CONGRESS_POLL_VOTE_DELEGATE,
            CONGRESS_POLL_VOTE_OPTION_ID,
            CONGRESS_POLL_VOTE_VOTED_TIME,
            CONGRESS_POLL_VOTE_ANONIM_COUNTER,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_POLL_VOTE_TABLE . '` (`'
                . CONGRESS_POLL_VOTE_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . CONGRESS_POLL_VOTE_OPTION_ID . '` int(8) NOT NULL DEFAULT -1, `'
                . CONGRESS_POLL_VOTE_POLL_ID . '` int(8) NOT NULL DEFAULT -1, `'
                . CONGRESS_POLL_VOTE_DELEGATE . '` int(8) NOT NULL DEFAULT -1, `'
                . CONGRESS_POLL_VOTE_ANONIM_COUNTER . '` int NOT NULL DEFAULT 0, `'
                . CONGRESS_POLL_VOTE_VOTED_TIME . '` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1), '
                . 'UNIQUE KEY `' . CONGRESS_POLL_VOTE_ID . '` (`' . CONGRESS_POLL_VOTE_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_POLL_VOTE_ID);
    }

    function getNumberOfVotes($congress_id, $poll_id) {
        global $mysqliLink;

        $congressDelegates = new CongressDelegates();
        $congressDelegates->setSelectedCongress($congress_id);

        $num = 0;
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_VOTE_TABLE . '` WHERE '
                . '`' . CONGRESS_POLL_VOTE_POLL_ID . '`=' . $poll_id . ' AND `' . CONGRESS_POLL_VOTE_DELEGATE . '`>0 '
                . ' GROUP BY `' . CONGRESS_POLL_VOTE_DELEGATE . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            if (isset($row[CONGRESS_POLL_VOTE_DELEGATE])) {
                $num += max(1, $congressDelegates->getMandateNumber($row[CONGRESS_POLL_VOTE_DELEGATE]));
            } else {
                error_log('getNumberOfVotes: congress_id:' . $congress_id . ', poll_id: ' . $poll_id);
            }
        }
        return $num;
    }

    function getVotedOptions($poll_id) {
        global $mysqliLink;
        $congressPoll = new CongressPoll();
        $congressPoll->getItemById($poll_id);
        $congress_id = $congressPoll->getCongressId();
        $congressDelegates = (new CongressDelegates())->setSelectedCongress($congress_id);
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_VOTE_TABLE . '` WHERE '
                . '`' . CONGRESS_POLL_VOTE_POLL_ID . '`=' . $poll_id . ' AND `' . CONGRESS_POLL_VOTE_DELEGATE . '`>0  ;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $mem_id = $row[CONGRESS_POLL_VOTE_DELEGATE];
            $mandat = max($congressDelegates->getMandateNumber($mem_id), 1); // Ahany mandátum van, annyiszor szerepeltetjuk a tombben. Legalab a            
            while ($mandat-- > 0) {
                $ret[] = $row;
            }
        }
        return $ret;
    }

    function getAnonimVotes($poll_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_VOTE_TABLE . '` WHERE '
                . '`' . CONGRESS_POLL_VOTE_POLL_ID . '`=' . $poll_id . ' AND `' . CONGRESS_POLL_VOTE_DELEGATE . '`<0  ;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row;
        }
        return $ret;
    }

    function setVote($poll_id, $mem_id, $option_id, $isByName) {
        if ($isByName) {
            $this->insertElement([
                CONGRESS_POLL_VOTE_POLL_ID => intval($poll_id),
                CONGRESS_POLL_VOTE_DELEGATE => intval($mem_id),
                CONGRESS_POLL_VOTE_OPTION_ID => intval($option_id),
            ]);
        } else {
            $congressDelegates = (new CongressDelegates())->setCongressIdByPollId($poll_id);
            $mandat = $congressDelegates->getMandateNumber($mem_id);
            if ($mandat < 0) {
                error_log('Nem letezo kuldott (' . $mem_id . ') a ' . $poll_id . ' szavazason');
                return;
            }
            $this->insertElement([
                CONGRESS_POLL_VOTE_POLL_ID => intval($poll_id),
                CONGRESS_POLL_VOTE_DELEGATE => intval($mem_id),
                CONGRESS_POLL_VOTE_OPTION_ID => -1,
            ]);
            $this->updateAnonimCounter($poll_id, $option_id, max($mandat, 1));
        }
    }

    function updateAnonimCounter($poll_id, $option_id, $mandat) {
        $votes = $this->getElementsByArray([
            CONGRESS_POLL_VOTE_POLL_ID => intval($poll_id),
            CONGRESS_POLL_VOTE_OPTION_ID => intval($option_id),
            CONGRESS_POLL_VOTE_DELEGATE => -1,
        ]);
        if (empty($votes)) { // Elso szavazat
            $this->createAnonimCounter($poll_id, $option_id, $mandat);            
        } else { // Novelni kell a szamlalot
            $vote = reset($votes);
            $new = $vote[CONGRESS_POLL_VOTE_ANONIM_COUNTER] + $mandat;
            $this->updateElementById($vote[CONGRESS_POLL_VOTE_ID], CONGRESS_POLL_VOTE_ANONIM_COUNTER, $new);
            $this->updateElementById($vote[CONGRESS_POLL_VOTE_ID], CONGRESS_POLL_VOTE_VOTED_TIME, time());
        }
    }
    function createAnonimCounter($poll_id, $option_id, $mandat) {
        $this->insertElement([
                CONGRESS_POLL_VOTE_POLL_ID => intval($poll_id),
                CONGRESS_POLL_VOTE_DELEGATE => -1,
                CONGRESS_POLL_VOTE_OPTION_ID => intval($option_id),
                CONGRESS_POLL_VOTE_ANONIM_COUNTER => intval($mandat),
            ]);
    }
    function removeVotedOption($id) {
        $this->updateElementById($id, CONGRESS_POLL_VOTE_OPTION_ID, -1);
    }

    function setSelected($voteId) {
        $this->itemId = $voteId;
        $this->getItemById($voteId);
        return $this;
    }

    function getPollId() {
        return $this->getValue(CONGRESS_POLL_VOTE_POLL_ID);
    }

    function getOptionId() {
        return $this->getValue(CONGRESS_POLL_VOTE_OPTION_ID);
    }

    function getVotedTime() {
        return $this->getValue(CONGRESS_POLL_VOTE_VOTED_TIME);
    }

    function getVotesOfMember($mem_id) {
        $ret = array();
        $votes = $this->getElementsBy(CONGRESS_POLL_VOTE_DELEGATE, $mem_id);
        foreach ($votes as $vote) {
            $ret[] = $vote[CONGRESS_POLL_VOTE_ID];
        }
        return $ret;
    }

    function getVotesOfPoll($poll_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_VOTE_TABLE . '` WHERE `' . CONGRESS_POLL_VOTE_POLL_ID . '`=' . $poll_id . '  AND `' . CONGRESS_POLL_VOTE_DELEGATE . '` >0 '
                . 'ORDER BY `' . CONGRESS_POLL_VOTE_DELEGATE . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row;
        }
        return $ret;
    }

    function getVotesOfOption($option_id) {
        return $this->getElementsBy(CONGRESS_POLL_VOTE_OPTION_ID, $option_id);
    }

    function getVoteOptionsOfMember($mem_id, $poll_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_VOTE_TABLE . '` WHERE `' . CONGRESS_POLL_VOTE_POLL_ID . '`=' . $poll_id
                . ' AND `' . CONGRESS_POLL_VOTE_DELEGATE . '`=' . $mem_id . ' ;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row[CONGRESS_POLL_VOTE_OPTION_ID];
        }
        return $ret;
    }

    function isVoteAlready($mem_id, $poll_id, $option_id) {
        $vote = $this->getElementsByArray([
            CONGRESS_POLL_VOTE_DELEGATE => intval($mem_id),
            CONGRESS_POLL_VOTE_OPTION_ID => intval($option_id),
            CONGRESS_POLL_VOTE_POLL_ID => intval($poll_id),
        ]);
        return !empty($vote);
    }

    function isMemberAlreadyVoted($mem_id, $poll_id) {
        $vote = $this->getElementsByArray([
            CONGRESS_POLL_VOTE_DELEGATE => intval($mem_id),
            CONGRESS_POLL_VOTE_POLL_ID => intval($poll_id),
        ]);
        return !empty($vote);
    }

    function resetVotes($mem_id, $poll_id) {
        global $mysqliLink;
        $sql = 'DELETE FROM `' . CONGRESS_POLL_VOTE_TABLE . '` WHERE `' . CONGRESS_POLL_VOTE_POLL_ID . '`=' . $poll_id
                . ' AND `' . CONGRESS_POLL_VOTE_DELEGATE . '`=' . $mem_id . ';';
        $mysqliLink->query($sql);
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
