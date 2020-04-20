<?php

define('CONGRESS_POLL_OPTION_TABLE', "congress_poll_option");
define('CONGRESS_POLL_OPTION_ID', "id");
define('CONGRESS_POLL_OPTION_POLL_ID', "poll_id");
define('CONGRESS_POLL_OPTION_TITLE', "title");
define('CONGRESS_POLL_OPTION_DESC', "description");

class CongressPollOption extends BasicElement {

    private $congressPollId;

    function __construct() {

        parent::__construct();
        $this->setTableName(CONGRESS_POLL_OPTION_TABLE);
        $this->setTableFields([
            CONGRESS_POLL_OPTION_ID,
            CONGRESS_POLL_OPTION_TITLE,
            CONGRESS_POLL_OPTION_DESC,
            CONGRESS_POLL_OPTION_POLL_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_POLL_OPTION_TABLE . '` (`'
                . CONGRESS_POLL_OPTION_ID . '` int(8) NOT NULL AUTO_INCREMENT, `'
                . CONGRESS_POLL_OPTION_TITLE . '` varchar(256) NOT NULL,`'
                . CONGRESS_POLL_OPTION_DESC . '` varchar(256) NOT NULL,`'
                . CONGRESS_POLL_OPTION_POLL_ID . '` int(8) NOT NULL DEFAULT -1, '
                . 'UNIQUE KEY `' . CONGRESS_POLL_OPTION_ID . '` (`' . CONGRESS_POLL_OPTION_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_POLL_OPTION_ID);
    }

    function setSelectedPoll($poll_id) {
        $this->congressPollId = $poll_id;
    }

    function setSelectedPollOption($optionId) {
        $this->itemId = $optionId;
    }

    function getPollOptions() {
        return $this->getElementsBy(CONGRESS_POLL_OPTION_POLL_ID, $this->congressPollId);
    }

    function getPollOptionTitle() {
        return $this->getBasicItem()[CONGRESS_POLL_OPTION_TITLE];
    }

    function getPollOptionDescription() {
        return $this->getBasicItem()[CONGRESS_POLL_OPTION_DESC];
    }

    function setPollOptionTitle($title) {
        $this->updateValue(CONGRESS_POLL_OPTION_TITLE, $title);
    }

    function setPollOptionDescription($desc) {
   //     error_log('Set:'.$this->itemId);
        $this->updateValue(CONGRESS_POLL_OPTION_DESC, $desc);
    }

    function addPollOption() {
        $this->insertElement([
            CONGRESS_POLL_OPTION_POLL_ID => $this->congressPollId,
        ]);
    }

    function delPollOption($optionId) {
        $this->deleteElementById($optionId);
    }

}
