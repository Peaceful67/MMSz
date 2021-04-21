<?php

define('CONGRESS_POLL_TABLE', "congress_poll");
define('CONGRESS_POLL_ID', "id");
define('CONGRESS_POLL_ITEM_ID', "item_id");
define('CONGRESS_POLL_TITLE', "title");
define('CONGRESS_POLL_DESCRIPTION', "desc");
define('CONGRESS_POLL_DECISION', "decision");
define('CONGRESS_POLL_DECISION_NUMBER', "decision_num");
define('CONGRESS_POLL_CREATOR_ID', "creator_id");
define('CONGRESS_POLL_CREATED_TIME', "created");
define('CONGRESS_POLL_CLOSED_TIME', "closed");
define('CONGRESS_POLL_TO_BE_CLOSED', "to_be_closed");
define('CONGRESS_POLL_OPTIONS', "options");
define('CONGRESS_POLL_MAX_OPTION', "max_option");
define('CONGRESS_POLL_VARIABLES', "variables");

class CongressPoll extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_POLL_TABLE);
        $this->setTableFields([
            CONGRESS_POLL_ID,
            CONGRESS_POLL_TITLE,
            CONGRESS_POLL_DESCRIPTION,
            CONGRESS_POLL_DECISION,
            CONGRESS_POLL_DECISION_NUMBER,
            CONGRESS_POLL_CREATOR_ID,
            CONGRESS_POLL_CLOSED_TIME,
            CONGRESS_POLL_TO_BE_CLOSED,
            CONGRESS_POLL_CREATED_TIME,
            CONGRESS_POLL_OPTIONS,
            CONGRESS_POLL_MAX_OPTION,
            CONGRESS_POLL_VARIABLES,
            CONGRESS_POLL_ITEM_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_POLL_TABLE . '` (`'
                . CONGRESS_POLL_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . CONGRESS_POLL_TITLE . '` varchar(256) NOT NULL,`'
                . CONGRESS_POLL_DESCRIPTION . '` text NOT NULL,`'
                . CONGRESS_POLL_DECISION . '` text NOT NULL,`'
                . CONGRESS_POLL_DECISION_NUMBER . '` varchar(32) NOT NULL DEFAULT "",`'
                . CONGRESS_POLL_CREATOR_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . CONGRESS_POLL_ITEM_ID . '` int(8)  NOT NULL DEFAULT -1,`'
                . CONGRESS_POLL_MAX_OPTION . '` int(8)  NOT NULL DEFAULT 0,`'
                . CONGRESS_POLL_CREATED_TIME . '` timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_POLL_CLOSED_TIME . '` timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_POLL_TO_BE_CLOSED . '` timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_POLL_VARIABLES . '` varchar(256) NOT NULL DEFAULT "",`'
                . CONGRESS_POLL_OPTIONS . '` int(11) NOT NULL DEFAULT 0, '
                . 'UNIQUE KEY `' . CONGRESS_POLL_ID . '` (`' . CONGRESS_POLL_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_POLL_ID);
    }

    public function createFromItemIfNotExists($itemId) {
        if ($this->getElementBy(CONGRESS_POLL_ITEM_ID, $itemId)) {
            $this->itemId = $this->getValue(CONGRESS_POLL_ID);
            return $this;
        } else {
            $item = new CongressItems();
            $item->setSelectedItem($itemId);
            global $member_id;
            $this->itemId = $this->insertElement([
                CONGRESS_POLL_TITLE => $item->getCongressItemTitle(),
                CONGRESS_POLL_DESCRIPTION => $item->getCongressItemDescription(),
                CONGRESS_POLL_ITEM_ID => $itemId,
                CONGRESS_POLL_CREATOR_ID => $member_id,
                CONGRESS_POLL_OPTIONS => $item->getItemOption(),
            ]);
            $this->getItemById($this->itemId);
            $this->setPollToBeClosed('DATE_ADD(NOW(), INTERVAL 7 DAY)');
            return $this;
        }
    }

    function openPollOfItem($itemId) {
        if ($this->getElementBy(CONGRESS_POLL_ITEM_ID, $itemId)) {
            $this->setTimeCurrent(CONGRESS_POLL_CREATED_TIME);
            return true;
        } else {
            return false;
        }
    }

    function emailOpenPoll() {
        global $member_id;
        $congress_id = $this->getCongressId();
        $delegates = (new CongressDelegates())->getMembersOfCongress($congress_id);
        $email = new Email(($this->isPollByEmail() ? EMAIL_CONTEXT_VOTE_BY_EMAIL : EMAIL_CONTEXT_VOTE), $this->itemId);
        $email->insertToBody(1, (new CongressItems())->getCongressItemTitleById($this->getCongressItemId()));
        $email->insertToBody(2, $this->getPollTitle());
        $email->insertToBody(3, $this->getPollDescription());
        $email->insertToBody(4, output_time($this->getPollToBeClosed()));
        $email->insertToBody(5, getMemberName($member_id));
        if ($this->isPollByEmail()) {
            $email->insertToBody(6, $this->putVoteOptions2Email());
        }
        $email->sendEmail($delegates);
    }

    function getIdsOfCongress($congress_id) {
        $ret = array();
        $item = new CongressItems();
        $item->setSelectedCongress($congress_id);
        $poll = new CongressPoll();
        foreach ($item->getSelectionOfCongress() as $item_id => $item_title) {
            $ret[$item_id] = $poll->getPollIdByItem($item_id);
        }
        return $ret;
    }

    function getIdsOfCongressByDecisionNum($congress_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT `poll`.* FROM `' . CONGRESS_POLL_TABLE . '` AS `poll` '
                . ' INNER JOIN `' . CONGRESS_ITEMS_TABLE . '` AS `item` ON ('
                . ' `item`.`' . CONGRESS_ITEMS_ID . '`=`poll`.`' . CONGRESS_POLL_ITEM_ID . '` '
                . ' AND `item`.`' . CONGRESS_ITEMS_CONGRESS_ID . '`=' . $congress_id . ')'
                . ' ORDER BY `poll`.`' . CONGRESS_POLL_DECISION_NUMBER . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row[CONGRESS_POLL_ID];
        }
        return $ret;
    }

    function setSelectedPollId($poll_id) {
        $this->itemId = $poll_id;
        $this->getItemById($poll_id);
        return $this;
    }

    function isEdited() {
        return $this->isValueNull(CONGRESS_POLL_CREATED_TIME) AND $this->isValueNull(CONGRESS_POLL_CLOSED_TIME);
    }

    function getOnlineClosed() { // Amikor a szavazas megnyilik, akkor van vege a targyalasnak
        return $this->getValue(CONGRESS_POLL_CREATED_TIME);
    }

    function isOpen() {
        return !$this->isValueNull(CONGRESS_POLL_CREATED_TIME) AND $this->isValueNull(CONGRESS_POLL_CLOSED_TIME);
    }

    function isClosed() {
        return (!$this->isValueNull(CONGRESS_POLL_CLOSED_TIME)) AND ( !$this->isValueNull(CONGRESS_POLL_CREATED_TIME));
    }

    function isDecided() {
        return $this->isClosed() AND ( ($this->getPollOptions() & CONGRESS_OPTIONS_DECISION_DONE) > 0);
    }

    function setDecided() {
        $opt = $this->getPollOptions() | CONGRESS_OPTIONS_DECISION_DONE;
        $this->updateValue(CONGRESS_POLL_OPTIONS, $opt);
        (new CongressOnline())->setSelectedCongress($this->getCongressId())->setCongressClosed();
    }

    function getPollId() {
        return $this->itemId;
    }

    function getCongressItemId() {
        return $this->tableRow[CONGRESS_POLL_ITEM_ID];
    }

    function getCongressId() {
        $item_id = $this->getCongressItemId();
        return (new CongressItems)->setSelectedItem($item_id)->getCongressId();
    }

    function getCongressIdByPollId($poll_id) {
        $this->getItemById($poll_id);
        return $this->getCongressId();
    }

    function getMaxOption() {
        return $this->getBasicItem()[CONGRESS_POLL_MAX_OPTION];
    }

    function getPollTitle() {
        return $this->getBasicItem()[CONGRESS_POLL_TITLE];
    }

    function getPollDescription() {
        return $this->getBasicItem()[CONGRESS_POLL_DESCRIPTION];
    }

    function getPollDecision() {
        return $this->getBasicItem()[CONGRESS_POLL_DECISION];
    }

    function getPollDecisionNumber() {
        return $this->getBasicItem()[CONGRESS_POLL_DECISION_NUMBER];
    }

    function getPollToBeClosed() {
        return $this->getBasicItem()[CONGRESS_POLL_TO_BE_CLOSED];
    }

    function setPollToBeClosed($time_string) {
        $this->updateValue(CONGRESS_POLL_TO_BE_CLOSED, $time_string);
        return $this;
    }

    function setPollTitle($title) {
        $this->updateValue(CONGRESS_POLL_TITLE, $title);
    }

    function setMaxOption($max_option) {
        $this->updateValue(CONGRESS_POLL_MAX_OPTION, $max_option);
        return $this;
    }

    function setPollDescription($desc) {
        $this->updateValue(CONGRESS_POLL_DESCRIPTION, $desc);
    }

    function setPollDecision($deci) {
        $this->updateValue(CONGRESS_POLL_DECISION, $deci);
    }

    function setPollDecisionNumber($deci_num) {
        $this->updateValue(CONGRESS_POLL_DECISION_NUMBER, $deci_num);
    }

    function getPollOptions() {
        return $this->getBasicItem()[CONGRESS_POLL_OPTIONS];
    }

    function isPollMultiChoice() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_MULTICHOICE) > 0;
    }

    function isPollByName() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_BY_NAME) > 0;
    }

    function isPollByEmail() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_BY_EMAIL) > 0;
    }

    function isPollAbstain() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_ABSTAIN_ENABLED) > 0;
    }

    function isPollYesNo() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_YES_NO) > 0;
    }

    function isPollPublic() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_PUBLIC) > 0;
    }

    function isPollDecisionETC() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_DECISION_ETC) > 0;
    }

    function getPollIdByItem($item_id) {
        $poll = $this->getElementBy(CONGRESS_POLL_ITEM_ID, $item_id);
        if (empty($poll) OR ! isset($poll[CONGRESS_POLL_ID])) {
            return -1;
        } else {
            $poll_id = $poll[CONGRESS_POLL_ID];
            $this->setSelectedPollId($poll_id);
            return $poll_id;
        }
    }

    function getPollByItem($item_id) {
        return $this->setSelectedPollId($this->getPollIdByItem($item_id));
    }

    function getCreatedByMember($mem_id) {
        $ret = array();
        $polls = $this->getElementsBy(CONGRESS_POLL_CREATOR_ID, $mem_id);
        foreach ($polls as $poll) {
            $ret[$poll[CONGRESS_POLL_ID]] = $poll[CONGRESS_POLL_TITLE];
        }
        return $ret;
    }

    function isClosable() {
        $totalVotes = (new CongressDelegates)->numOfTotalVotes($this->getCongressId());
        return ($this->countOfVotes() >= $totalVotes AND ! $this->isClosed()) ? true : false;
    }

    function getPollsToBeClosed() {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_TABLE . '` WHERE `' . CONGRESS_POLL_CREATED_TIME . '` IS NOT NULL AND `' . CONGRESS_POLL_CLOSED_TIME . '` IS NULL AND '
                . '`' . CONGRESS_POLL_TO_BE_CLOSED . '`< CURRENT_TIMESTAMP();';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row[CONGRESS_POLL_ID];
        }
        return $ret;
    }

    function setPollClosed($poll_id) {
        include_once POLLING . 'CongressItems.inc';
        $this->setSelectedPollId($poll_id);
        $item_id = $this->getValue(CONGRESS_POLL_ITEM_ID);
        $closed = (new CongressItems())->setSelectedItem($item_id)->getClose();
        if (!is_null($closed)) {
            $this->setTimeCurrent(CONGRESS_POLL_CLOSED_TIME);
            return true;
        } else {
            error_log('Auto close canceled');
            return false;
        }
    }

    function getPollClosed() {
        return $this->getValue(CONGRESS_POLL_CLOSED_TIME);
    }

    function countOfVotes() {
        $congress_id = $this->getCongressId();
        return (new CongressPollVote)->getNumberOfVotes($congress_id, $this->itemId);
    }

    function getPollToVote($mem_id) { // A tagnak ezekben lehet szavaznia
        global $mysqliLink;
        $pollVote = new CongressPollVote();
        $ret = array();
        $sql = 'SELECT `poll`.* FROM (`' . CONGRESS_POLL_TABLE . '` AS `poll`'
                . ' INNER JOIN `' . CONGRESS_DELEGATES_TABLE . '` AS `participate` ON (`participate`.`' . CONGRESS_DELEGATES_MEMBER_ID . '`=' . $mem_id . ') '
                . ' INNER JOIN `' . CONGRESS_ITEMS_TABLE . '` AS `items` ON (`items`.`' . CONGRESS_ITEMS_CONGRESS_ID . '`=`participate`.`' . CONGRESS_DELEGATES_CONGRESS_ID . '` AND '
                . '     `items`.`' . CONGRESS_ITEMS_CLOSED . '` is not null ))'
                . ' WHERE `items`.`' . CONGRESS_ITEMS_ID . '`=`poll`.`' . CONGRESS_POLL_ITEM_ID . '` AND '
                . ' `poll`.`' . CONGRESS_POLL_CREATED_TIME . '` is not null AND'
                . ' `poll`.`' . CONGRESS_POLL_CLOSED_TIME . '` is null '
                . ' GROUP BY `poll`.`' . CONGRESS_POLL_ID . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $poll_id = $row[CONGRESS_POLL_ID];
            $this->getElement($poll_id);
            if (!$this->isPollByName()) { // Ha nem név szerinti és már szavazott, nem adjuk fel
                if ($pollVote->isMemberAlreadyVoted($mem_id, $poll_id)) {
                    continue;
                }
            }
            $ret[$poll_id] = $row[CONGRESS_POLL_TITLE];
        }
        return $ret;
    }

    function getPollsVoting() {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_TABLE . '` WHERE '
                . '`' . CONGRESS_POLL_CREATED_TIME . '` is not null AND'
                . ' `' . CONGRESS_POLL_CLOSED_TIME . '` is null '
                . ' GROUP BY `' . CONGRESS_POLL_ID . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_POLL_ID]] = $row[CONGRESS_POLL_TITLE];
        }
        return $ret;
    }

    function getPollsOfCongress($congress_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT `poll`.* FROM (`' . CONGRESS_POLL_TABLE . '` AS `poll` '
                . 'INNER JOIN `' . CONGRESS_ITEMS_TABLE . '` AS `items` ON (`items`.`' . CONGRESS_ITEMS_CONGRESS_ID . '`=' . $congress_id . ' AND '
                . '     `items`.`' . CONGRESS_ITEMS_ID . '`=`poll`.`' . CONGRESS_POLL_ITEM_ID . '` ))';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_POLL_ID]] = $row[CONGRESS_POLL_TITLE];
        }
        return $ret;
    }

    function getVotes() {
        if ($this->isPollByName()) {
            return (new CongressPollVote)->getVotedOptions($this->itemId);
        } else {
            return (new CongressPollVote)->getAnonimVotes($this->itemId);
        }
    }

    function getResult() {
        /*  Result: 
         *  false : nincs elég szavazat, érvénytelen a szavazás
         *  array [
         *   option_id => true/false nyert/nem nyert
         *   ]
         */
        $congDelegates = new CongressDelegates();
        $congress_id = $this->getCongressId();
        $congDelegates->setSelectedCongress($congress_id);
        if ((new CongressOnline())->setSelectedCongress($congress_id)->isCongressBaseVoted()) {
            $total = $congDelegates->numOfTotalVotes($congress_id);
        } else {
            $total = count($congDelegates->getMembersOfCongress($congress_id));
        }
        $voted_mandate = 0;
        $options = array();
        if ($this->isPollByName()) { 
            $votes = $this->getVotes();
            foreach ($votes as $vote) { // Névszerintinél Szavazat szamlalas
                $del_id = $vote[CONGRESS_POLL_VOTE_DELEGATE];
                $option_id = $vote[CONGRESS_POLL_VOTE_OPTION_ID];
                $mandate = max($congDelegates->getMandateNumber($del_id), 1);
                $voted_mandate += $mandate;
                if (isset($options[$option_id])) {
                    $options[$option_id] += $mandate;
                } else {
                    $options[$option_id] = $mandate;
                }
            }
        } else {
            $votes = (new CongressPollVote())->getAnonimVotes($this->itemId);
            foreach ($votes as $vote) { // Nem névszerintinél elvileg már össze van gyűjtve
                $mandate = $vote[CONGRESS_POLL_VOTE_ANONIM_COUNTER];
                $option_id = $vote[CONGRESS_POLL_VOTE_OPTION_ID];
                $voted_mandate += $mandate;
                $options[$option_id] = $mandate;
            }
        }
        if ($voted_mandate < intval(($total + 1) / 2)) {
            return false;
        }
        $ret = array();
        $limit = getThreshold($total, $this->getPollOptions());
        foreach ($options as $option_id => $num) { // kiértékelés
            switch ($option_id) {
                case CONGRESS_POLL_OPTION_YES_ID:
                    $result = ($limit > 0 AND $num >= $limit) ? true : false;
                    $ret[$option_id] = $result;
                    break;
                case CONGRESS_POLL_OPTION_NO_ID:
                    $result = ($limit > 0 AND $num >= $limit) ? true : false;
                    $ret[$option_id] = $result;
                    break;
                case CONGRESS_POLL_OPTION_ABSTAIN_ID:
                    break;
                default:
                    $ret[$option_id] = ($limit > 0 AND $num >= $limit) ? true : false;
                    break;
            }
            if ($option_id != CONGRESS_POLL_OPTION_ABSTAIN_ID) {
                $ret[$option_id] = ($limit > 0 AND $num >= $limit) ? true : false;
            }
        }
        return $ret;
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

    private function putVoteOptions2Email() {
        $ret = '';
        $pollOption = new CongressPollOption();
        $pollOption->setSelectedPoll($this->itemId);
        $vote_options = $pollOption->getPollOptions();
        $comm = new Command(Command::EMAIL_VOTE);
        $comm->setExpire('"' . $this->getPollToBeClosed() . '"');
        if ($this->isPollAbstain()) {
            $code = $comm->putPatternCommand($this->createCommandValue(CONGRESS_POLL_OPTION_ABSTAIN_ID));
            $ret .= $this->createVoteLink($code, $pollOption->getTitle(CONGRESS_POLL_OPTION_ABSTAIN_ID), $pollOption->getDescription(CONGRESS_POLL_OPTION_ABSTAIN_ID));
        }
        if ($this->isPollYesNo()) {
            $code = $comm->putPatternCommand($this->createCommandValue(CONGRESS_POLL_OPTION_YES_ID));
            $ret .= $this->createVoteLink($code, $pollOption->getTitle(CONGRESS_POLL_OPTION_YES_ID), $pollOption->getDescription(CONGRESS_POLL_OPTION_YES_ID));
            $code = $comm->putPatternCommand($this->createCommandValue(CONGRESS_POLL_OPTION_NO_ID));
            $ret .= $this->createVoteLink($code, $pollOption->getTitle(CONGRESS_POLL_OPTION_NO_ID), $pollOption->getDescription(CONGRESS_POLL_OPTION_NO_ID));
        } else {
            foreach ($vote_options as $option_id => $option) {
                $code = $comm->putPatternCommand($this->createCommandValue($option_id));
                $ret .= $this->createVoteLink($code, $pollOption->getTitle($option_id), $pollOption->getDescription($option_id));
            }
        }
        $ret .= '<br>';
        if ($this->isPollMultiChoice()) {
            $ret .= 'Ez egy több választási lehetőséget lehetővé szavazás. Amennyiben több lehetőséget is ke szeretne választani, mindazokra a linkekre kattintson, amelyekre szavazni akar !<br>';
            if ($this->getMaxOption() > 0) {
                $ret .= 'Ennél a szavazásnál a maximásisan választató lehetőségek száma: ' . $this->getMaxOption() . ', ennél többet nem fogad el a rendszer.<br>';
            }
        } else {
            $ret .= 'A fenti lehetőségek közül csak egy választhat. Mindig az utoljára kattintott szavazása lesz az érvényes.<br>';
        }
        return $ret;
    }

    private function createVoteLink($code, $title, $desc) {
        $ret = '<b><a title="' . $desc . '" href="' . URL . '?c=_#_' . $code . '_#_">' . $title . '</a></b>';
        $ret .= '<br>';
        return $ret;
    }

    private function createCommandValue($option_id) {
        $arr = array();
        $arr[CONGRESS_POLL_VOTE_OPTION_ID] = $option_id;
        $arr[CONGRESS_POLL_VOTE_POLL_ID] = $this->itemId;
        return $arr;
    }

}
