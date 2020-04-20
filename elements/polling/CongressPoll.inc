<?php

define('CONGRESS_POLL_TABLE', "congress_poll");
define('CONGRESS_POLL_ID', "id");
define('CONGRESS_POLL_ITEM_ID', "item_id");
define('CONGRESS_POLL_TITLE', "title");
define('CONGRESS_POLL_DESCRIPTION', "desc");
define('CONGRESS_POLL_CREATOR_ID', "creator_id");
define('CONGRESS_POLL_CREATED_TIME', "created");
define('CONGRESS_POLL_CLOSED_TIME', "closed");
define('CONGRESS_POLL_OPTIONS', "options");

class CongressPoll extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_POLL_TABLE);
        $this->setTableFields([
            CONGRESS_POLL_ID,
            CONGRESS_POLL_TITLE,
            CONGRESS_POLL_DESCRIPTION,
            CONGRESS_POLL_CREATOR_ID,
            CONGRESS_POLL_CLOSED_TIME,
            CONGRESS_POLL_CREATED_TIME,
            CONGRESS_POLL_OPTIONS,
            CONGRESS_POLL_ITEM_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_POLL_TABLE . '` (`'
                . CONGRESS_POLL_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . CONGRESS_POLL_TITLE . '` varchar(256) NOT NULL,`'
                . CONGRESS_POLL_DESCRIPTION . '` text NOT NULL,`'
                . CONGRESS_POLL_CREATOR_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . CONGRESS_POLL_ITEM_ID . '` int(8)  NOT NULL DEFAULT -1,`'
                . CONGRESS_POLL_CREATED_TIME . '` timestamp(1) NULL DEFAULT NULL,`'
                . CONGRESS_POLL_CLOSED_TIME . '` timestamp(1) NULL DEFAULT NULL,`'
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
            return $this;
        }
    }

    function setSelectedPollId($poll_id) {
        $this->itemId = $poll_id;
    }

    function getPollId() {
        return $this->itemId;
    }

    function getPollTitle() {
        return $this->getBasicItem()[CONGRESS_POLL_TITLE];
    }

    function getPollDescription() {
        return $this->getBasicItem()[CONGRESS_POLL_DESCRIPTION];
    }

    function setPollTitle($title) {
        $this->updateValue(CONGRESS_POLL_TITLE, $title);
    }

    function setPollDescription($desc) {
        $this->updateValue(CONGRESS_POLL_DESCRIPTION, $desc);
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

    function isPollPublic() {
        return ($this->getPollOptions() & CONGRESS_OPTIONS_PUBLIC) > 0;
    }

    function getPollIdByItem($itemId) {
        $poll = $this->getElementBy(CONGRESS_POLL_ITEM_ID, $itemId);
        if (empty($poll) OR !isset($poll[CONGRESS_POLL_ID])) {
            return -1;
        } else {
            return $poll[CONGRESS_POLL_ID];
        }
    }

}
