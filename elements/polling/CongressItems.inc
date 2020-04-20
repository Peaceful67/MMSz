<?php

define("CONGRESS_ITEMS_TABLE", 'congress_items');
define("CONGRESS_ITEMS_ID", 'id');
define("CONGRESS_ITEMS_CONGRESS_ID", 'congress_id');
define("CONGRESS_ITEMS_TITLE", 'title');
define("CONGRESS_ITEMS_DESCRIPTION", 'desc');
define("CONGRESS_ITEMS_CLOSED", 'closed');
define("CONGRESS_ITEMS_OPTIONS", 'options');
define("CONGRESS_ITEMS_DEPENDECY", 'dependency');

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
                . CONGRESS_ITEMS_DEPENDECY . '` varchar(256) NOT NULL, '
                . 'UNIQUE KEY `' . CONGRESS_ITEMS_ID . '` (`' . CONGRESS_ITEMS_ID . '`)'
                . ') ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_ITEMS_ID);
    }

    function setSelectedCongress($selectedCongress) {
        $this->selectedCongress = $selectedCongress;
    }

    function setSelectedItem($itemId) {
        $this->selectedItem = $itemId;
        $this->itemId = $itemId;
    }

    function getSelectionOfCongress() {
        $ret = array();
        $items = $this->getElementsBy(CONGRESS_ITEMS_CONGRESS_ID, $this->selectedCongress);
        foreach ($items as $item) {
            $ret[$item[CONGRESS_ITEMS_ID]] = $item[CONGRESS_ITEMS_TITLE];
        }
        return $ret;
    }

    function getSelectionOfCongressOpen() {
        $ret = array();
        $items = $this->getElementsBy(CONGRESS_ITEMS_CONGRESS_ID, $this->selectedCongress);
        foreach ($items as $item) {
            if (is_null($item[CONGRESS_ITEMS_CLOSED])) {
                $ret[$item[CONGRESS_ITEMS_ID]] = $item[CONGRESS_ITEMS_TITLE];
            }
        }
        return $ret;
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
            CONGRESS_ITEMS_TITLE => 'Napirend címe',
            CONGRESS_ITEMS_DESCRIPTION => 'Napirend részletes leírása',
            CONGRESS_ITEMS_CONGRESS_ID => $this->selectedCongress,
            CONGRESS_ITEMS_OPTIONS => $congresOptions,
        ]);
        $this->setSelectedItem($newItem);
        return $newItem;
    }

    function modifyItemTitle($title) {
        $this->updateElement(CONGRESS_ITEMS_TITLE, $title);
    }

    function modifyItemDescription($desc) {
  //      error_log("Modify desc:" . $this->itemId);
        $this->updateElement(CONGRESS_ITEMS_DESCRIPTION, $desc);
    }

    function setItemOption($option) {
        $this->updateElement(CONGRESS_ITEMS_OPTIONS, $option);
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

    function getCongressItem() {
        return $this->getItemById($this->selectedItem);
    }

    function getCongressItemDescription() {
        return ($this->getItemById($this->selectedItem)[CONGRESS_ITEMS_DESCRIPTION]);
    }

    function setDependency($arrItems) {
        $this->updateElement(CONGRESS_ITEMS_DEPENDECY, serialize($arrItems));
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
        $this->setTime(CONGRESS_ITEMS_CLOSED);
        $congress = new CongressOnline();
    }

    function getPendingDependencies() {
        $depArr = $this->getDependency();
        $ret = '';
        foreach ($depArr as $itemId) {
            if (!$this->isItemClosed($itemId)) {
                $ret .= $this->getItemById($itemId)[CONGRESS_ITEMS_TITLE] . ';';
            }
        }
        return $ret;
    }

    function isReadyToVote() {
        $ret = true;
        if (strlen($this->getPendingDependencies()) > 0) {
            //          error_log('isReadyToVote()' . 'Dependency');
            return false;
        }
        $poll = new CongressPoll();
        $poll_id = $poll->getPollIdByItem($this->itemId);
        if ($poll_id < 0) {
            //          error_log('isReadyToVote()' . 'Error, itemId:' . $this->itemId);
            return false;
        }
        $poll_option = new CongressPollOption();
        $poll_option->setSelectedPoll($poll_id);
        if (count($poll_option->getPollOptions()) < 2) {
            //         error_log('isReadyToVote()' . 'less than 2');
            return false;
        }
        //     error_log('isReadyToVote()' . ' Ready');
        return true;
    }

    function deleteCongressItem() {
        $this->removeItemFromDependecy();
        $this->deleteElementBy($this->key, $this->selectedItem);
        $this->selectedItem = $this->itemId = -1;
    }

    function deleteCongress($congress_id) {
        $this->deleteElementBy(CONGRESS_ITEMS_CONGRESS_ID, $congress_id);
        $this->selectedItem = $this->itemId = -1;
        $this->selectedCongress = -1;
    }

    function insertMadatoryItemToCongress() {
        $itemsOfCongress = new CongressItems();
        foreach ($this->getSelectionOfCongress() as $item_id => $item_title) {
            if ($item_id != $this->selectedItem) {
                $itemsOfCongress->setSelectedItem($item_id);
                $depArr = $itemsOfCongress->getDependency();
                $depArr[] = $this->selectedItem;
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
                if ($this->selectedItem == $dep) {
                    unset($depArr[$id]);
                }
            }
            unset($depArr[$this->selectedItem]);
            $itemsOfCongress->setDependency($depArr);
        }
    }

}
