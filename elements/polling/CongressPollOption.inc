<?php

define('CONGRESS_POLL_OPTION_TABLE', "congress_poll_option");
define('CONGRESS_POLL_OPTION_ID', "id");
define('CONGRESS_POLL_OPTION_POLL_ID', "poll_id");
define('CONGRESS_POLL_OPTION_TITLE', "title");
define('CONGRESS_POLL_OPTION_DESC', "description");


define('CONGRESS_POLL_OPTION_ABSTAIN_ID', 1);
define('CONGRESS_POLL_OPTION_YES_ID', 2);
define('CONGRESS_POLL_OPTION_NO_ID', 3);

class CongressPollOption extends BasicElement {

    private $congressPollId;
    public static $predefinedPollOptions = [
        CONGRESS_POLL_OPTION_ABSTAIN_ID => [
            CONGRESS_POLL_OPTION_TITLE => 'Tartózkodás',
            CONGRESS_POLL_OPTION_DESC => 'Jelen vagyok, de egyik lehetőségre sem szavazok',
            CONGRESS_POLL_OPTION_POLL_ID => -1,
        ],
        CONGRESS_POLL_OPTION_YES_ID => [
            CONGRESS_POLL_OPTION_TITLE => 'Elfogadom',
            CONGRESS_POLL_OPTION_DESC => 'Egyetértek a határozati javaslattal',
            CONGRESS_POLL_OPTION_POLL_ID => -1,
        ],
        CONGRESS_POLL_OPTION_NO_ID => [
            CONGRESS_POLL_OPTION_TITLE => 'Nem fogadom el',
            CONGRESS_POLL_OPTION_DESC => 'Nem értek egyet a határozati javaslattal',
            CONGRESS_POLL_OPTION_POLL_ID => -1,
        ],
    ];
    private $autoAbstain = false;

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
                . ' AUTO_INCREMENT=10 ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_POLL_OPTION_ID);
    }

    function setSelectedPoll($poll_id) {
        $this->congressPollId = $poll_id;
        return $this;
    }

    function setSelectedPollOption($optionId) {
        $this->itemId = $optionId;
        return $this;
    }

    function getPollOptions() {
        $pollOptions = $this->getElementsBy(CONGRESS_POLL_OPTION_POLL_ID, $this->congressPollId);
        return $pollOptions;
    }

    function getTitle($option_id) {
        if (isset(self::$predefinedPollOptions[$option_id])) {
            return self::$predefinedPollOptions[$option_id][CONGRESS_POLL_OPTION_TITLE];
        } else {
            $this->getItemById($option_id);
            return $this->getPollOptionTitle();
        }
    }

    function getDescription($option_id) {
        if (isset(self::$predefinedPollOptions[$option_id])) {
            return self::$predefinedPollOptions[$option_id][CONGRESS_POLL_OPTION_DESC];
        } else {
            $this->getItemById($option_id);
            return $this->getPollOptionDescription();
        }
    }

    function getPollOptionTitle() {
        return $this->getBasicItem()[CONGRESS_POLL_OPTION_TITLE];
    }

    function getPollOptionDescription() {
        return $this->getBasicItem()[CONGRESS_POLL_OPTION_DESC];
    }

    function setPollOptionTitle($title) {
        $this->updateValue(CONGRESS_POLL_OPTION_TITLE, $title);
        return $this;
    }

    function setPollOptionDescription($desc) {
        $this->updateValue(CONGRESS_POLL_OPTION_DESC, $desc);
        return $this;
    }

    function addPollOption() {
        $item_id = $this->insertElement([
            CONGRESS_POLL_OPTION_POLL_ID => $this->congressPollId,
        ]);
        $this->setItemId($item_id);
        $this->updateItem();
        return $this;
    }

    function incrementNumAnonim($mandat) {
        
    }

    function getNumAnonim() {
        
    }

    function delPollOption($optionId) {
        $this->deleteElementById($optionId);
    }

    function addYesNo() {
        if ($this->congressPollId > 0) {
            $this->insertElement([
                CONGRESS_POLL_OPTION_POLL_ID => $this->congressPollId,
                CONGRESS_POLL_OPTION_TITLE => 'Elfogadom',
                CONGRESS_POLL_OPTION_DESC => 'Egyetértek a határozati javaslattal',
            ]);
            $this->insertElement([
                CONGRESS_POLL_OPTION_POLL_ID => $this->congressPollId,
                CONGRESS_POLL_OPTION_TITLE => 'Nem fogadom el',
                CONGRESS_POLL_OPTION_DESC => 'Nem értek egyet a határozati javaslattal',
            ]);
        }
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
