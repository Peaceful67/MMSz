<?php

define("CONGRESS_ITEMS_TABLE", 'congress_items');
define("CONGRESS_ITEMS_ID", 'id');
define("CONGRESS_ITEMS_CONGRESS_ID", 'congress_id');
define("CONGRESS_ITEMS_TITLE", 'title');
define("CONGRESS_ITEMS_DESCRIPTION", 'desc');
define("CONGRESS_ITEMS_CLOSED", 'closed');
define("CONGRESS_ITEMS_OPTIONS", 'options');
define("CONGRESS_ITEMS_DEPENDECY", 'dependency');
define("CONGRESS_ITEMS_VARIABLES", 'variables');

class CongressItems extends BasicElement {

    protected $selectedCongress = -1;
    protected $selectedItem = -1;

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_ITEMS_TABLE);
        $this->setTableFields([
            CONGRESS_ITEMS_ID,
            CONGRESS_ITEMS_CONGRESS_ID,
            CONGRESS_ITEMS_TITLE,
            CONGRESS_ITEMS_DESCRIPTION,
            CONGRESS_ITEMS_CLOSED,
            CONGRESS_ITEMS_OPTIONS,
            CONGRESS_ITEMS_VARIABLES,
            CONGRESS_ITEMS_DEPENDECY,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_ITEMS_TABLE . '` (`'
                . CONGRESS_ITEMS_ID . '` int(8) NOT NULL AUTO_INCREMENT, `'
                . CONGRESS_ITEMS_CONGRESS_ID . '` int(8) NULL DEFAULT NULL, `'
                . CONGRESS_ITEMS_TITLE . '` varchar(256) NOT NULL,`'
                . CONGRESS_ITEMS_DESCRIPTION . '` text NOT NULL, `'
                . CONGRESS_ITEMS_CLOSED . '` timestamp(1) NULL DEFAULT NULL ,`'
                . CONGRESS_ITEMS_OPTIONS . '` int(11) NOT NULL,`'
                . CONGRESS_ITEMS_VARIABLES . '` varchar(256) NOT NULL DEFAULT "",`'
                . CONGRESS_ITEMS_DEPENDECY . '` varchar(256) NOT NULL, '
                . 'UNIQUE KEY `' . CONGRESS_ITEMS_ID . '` (`' . CONGRESS_ITEMS_ID . '`)'
                . ') ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_ITEMS_ID);
    }

    function setSelectedCongress($selectedCongress) {
        $this->selectedCongress = $selectedCongress;
        return $this;
    }

    function setSelectedItem($itemId) {
        $this->itemId = $itemId;
        $this->getItemById($itemId);
        return $this;
    }

    function getSelectionOfCongress() {
        $ret = array();
        $items = $this->getElementsBy(CONGRESS_ITEMS_CONGRESS_ID, $this->selectedCongress);
        foreach ($items as $item) {
            $ret[$item[CONGRESS_ITEMS_ID]] = $item[CONGRESS_ITEMS_TITLE];
        }
        return $ret;
    }

    function getSelectionOfCongressOnline() {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_ITEMS_TABLE . '` WHERE'
                . ' `' . CONGRESS_ITEMS_CONGRESS_ID . '`=' . $this->selectedCongress . ' AND '
                . ' `' . CONGRESS_ITEMS_CLOSED . '` IS NOT NULL;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[CONGRESS_ITEMS_ID]] = $row[CONGRESS_ITEMS_TITLE];
        }
        return $ret;
    }

    function getSelectionOfItemIdCongressClose($congress_id) {
        $ret = array();
        $items = $this->getElementsBy(CONGRESS_ITEMS_CONGRESS_ID, $congress_id);
        foreach ($items as $item) {
            if (!is_null($item[CONGRESS_ITEMS_CLOSED])) {
                $ret[] = $item[CONGRESS_ITEMS_ID];
            }
        }
        return $ret;
    }

    function getCongressId() {
        return $this->getValue(CONGRESS_ITEMS_CONGRESS_ID);
    }

    function getCongressItemTitleById($itemId) {
        $item = $this->getItemById($itemId);
        return $item[CONGRESS_ITEMS_TITLE];
    }

    function getCongressItemTitle() {
        return $this->getBasicItem()[CONGRESS_ITEMS_TITLE];
    }

    function addItem($congresOptions) {
        $newItem = $this->insertElement([
            CONGRESS_ITEMS_TITLE => '',
            CONGRESS_ITEMS_DESCRIPTION => '',
            CONGRESS_ITEMS_CONGRESS_ID => $this->selectedCongress,
            CONGRESS_ITEMS_OPTIONS => $congresOptions,
        ]);
        $this->setSelectedItem($newItem);
        return $newItem;
    }

    function modifyItemTitle($title) {
        $this->updateValue(CONGRESS_ITEMS_TITLE, $title);
    }

    function modifyItemDescription($desc) {
        $this->updateValue(CONGRESS_ITEMS_DESCRIPTION, $desc);
    }

    function setItemOption($option) {
        $this->updateValue(CONGRESS_ITEMS_OPTIONS, $option);
    }

    function getItemOption() {
        return $this->getBasicItem()[CONGRESS_ITEMS_OPTIONS];
    }

    function isItemByName() {
        return (($this->getItemOption() & CONGRESS_OPTIONS_BY_NAME) > 0) ? true : false;
    }

    function isItemPublic() {
        return (($this->getItemOption() & CONGRESS_OPTIONS_PUBLIC) > 0) ? true : false;
    }

    function isItemByEmail() {
        return (($this->getItemOption() & CONGRESS_OPTIONS_BY_EMAIL) > 0) ? true : false;
    }

    function isItemMultichoice() {
        return (($this->getItemOption() & CONGRESS_OPTIONS_MULTICHOICE) > 0) ? true : false;
    }

    function isItemYesNo() {
        return (($this->getItemOption() & CONGRESS_OPTIONS_YES_NO) > 0) ? true : false;
    }

    function isItemAbstain() {
        return (($this->getItemOption() & CONGRESS_OPTIONS_ABSTAIN_ENABLED) > 0) ? true : false;
    }

    function getCongressItem() {
        return $this->getItemById($this->itemId);
    }

    function getCongressItemDescription() {
        return ($this->getItemById($this->itemId)[CONGRESS_ITEMS_DESCRIPTION]);
    }

    function setDependency($arrItems) {
        $this->updateValue(CONGRESS_ITEMS_DEPENDECY, serialize($arrItems));
    }

    function getDependency() {
        $dependency = $this->getBasicItem()[CONGRESS_ITEMS_DEPENDECY];
        if (strlen($dependency) == 0) {
            return array();
        } else {
            return unserialize($dependency);
        }
    }

    function getClose() {
        return $this->getValue(CONGRESS_ITEMS_CLOSED);
    }

    function isItemClosed($itemId) {
        $item = $this->getItemById($itemId);
        return(!is_null($item[CONGRESS_ITEMS_CLOSED]));
    }

    function setItemClosed($item_id) {
        $this->getItemById($item_id);
        $this->setTimeCurrent(CONGRESS_ITEMS_CLOSED);
    }

    function getPendingDependencies() {
        $depArr = $this->getDependency();
        $ret = '';
        $otherItem = new CongressItems();
        $otherPoll = new CongressPoll();
        foreach ($depArr as $itemId) {
            if (!$otherItem->isItemClosed($itemId)) {
                $ret .= $otherItem->getItemById($itemId)[CONGRESS_ITEMS_TITLE] . ';';
            } else {
                $poll_id = $otherPoll->getPollIdByItem($itemId);
                if (!($otherPoll->setSelectedPollId($poll_id)->isDecided())) {
                    $ret .= $otherItem->getItemById($itemId)[CONGRESS_ITEMS_TITLE] . ':';
                }
            }
        }
        return $ret;
    }

    function isReadyToVote() {
        $ret = array();
        if (strlen($this->getPendingDependencies()) > 0) {
            $ret[] = 'Vannak függő, le nem zárt szavazások.';
        }

        $poll = new CongressPoll();
        $poll_id = $poll->getPollIdByItem($this->itemId);
        if ($poll_id < 0) {
            $ret[] = 'Nincs létrehozva határozati javaslat';
            return $ret;
        }
        if (time() > strtotime($poll->getPollToBeClosed())) {
            $ret[] = 'A szavazás lezárása a múltba került.';
        }
        $poll->setSelectedPollId($poll_id);
        if (!$poll->isPollYesNo()) {
            $pollOption = new CongressPollOption();
            $pollOption->setSelectedPoll($poll_id);
            if (count($pollOption->getPollOptions()) < 2) {
                $ret[] = 'Legalább két választási lehetőséget létre kell hozni';
            }
        }
        return $ret;
    }

    function deleteCongressItem() {
        $this->removeItemFromDependecy();
        $this->deleteElementBy($this->key, $this->itemId);
        $this->itemId = -1;
    }

    function deleteCongress($congress_id) {
        $this->deleteElementBy(CONGRESS_ITEMS_CONGRESS_ID, $congress_id);
        $this->itemId = -1;
        $this->selectedCongress = -1;
    }

    function insertMadatoryItemToCongress() {
        $itemsOfCongress = new CongressItems();
        foreach ($this->getSelectionOfCongress() as $item_id => $item_title) {
            if ($item_id != $this->itemId) {
                $itemsOfCongress->setSelectedItem($item_id);
                $depArr = $itemsOfCongress->getDependency();
                $depArr[] = $this->itemId;
                $itemsOfCongress->setDependency($depArr);
            }
        }
    }

    function removeItemFromDependecy() {
        $itemsOfCongress = new CongressItems();
        foreach ($this->getSelectionOfCongress() as $item_id => $item_title) {
            $itemsOfCongress->setSelectedItem($item_id);
            $depArr = $itemsOfCongress->getDependency();
            foreach ($depArr as $id => $dep) {
                if ($this->itemId == $dep) {
                    unset($depArr[$id]);
                }
            }
            unset($depArr[$this->itemId]);
            $itemsOfCongress->setDependency($depArr);
        }
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
