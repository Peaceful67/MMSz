<?php

define("CONGRESS_ITEM_TOPIC_TABLE", 'congress_topic');
define("CONGRESS_ITEM_TOPIC_ID", 'id');
define("CONGRESS_ITEM_TOPIC_ITEM_ID", 'item_id');
define("CONGRESS_ITEM_TOPIC_DESCRIPTION", 'desc');

/**
 * Description of CongressItemTopic
 * A napirendi pontok temakorei
 * @author Peaceful
 */
class CongressItemTopic extends BasicElement {

    private $congressItem = -1;
    public static $defaultTopic = [
        CONGRESS_ITEM_TOPIC_ID => -1,
        CONGRESS_ITEM_TOPIC_ITEM_ID => -1,
        CONGRESS_ITEM_TOPIC_DESCRIPTION => 'Általános',
    ];

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_ITEM_TOPIC_TABLE);
        $this->setTableFields([
            CONGRESS_ITEM_TOPIC_ID,
            CONGRESS_ITEM_TOPIC_ITEM_ID,
            CONGRESS_ITEM_TOPIC_DESCRIPTION,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_ITEM_TOPIC_TABLE . '` (`'
                . CONGRESS_ITEM_TOPIC_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . CONGRESS_ITEM_TOPIC_ITEM_ID . '` int(8) NOT NULL DEFAULT -1, `'
                . CONGRESS_ITEM_TOPIC_DESCRIPTION . '` varchar(256) NOT NULL DEFAULT "", '
                . 'UNIQUE KEY `' . CONGRESS_ITEM_TOPIC_ID . '` (`' . CONGRESS_ITEM_TOPIC_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_ITEM_TOPIC_ID);
    }

    function setSelectedItem($item_id) {
        $this->congressItem = $item_id;
    }

    function setSelectedTopic($topicId) {
        $this->itemId = $topicId;
        $this->getItemById($topicId);
        return $this;
    }

    function addTopicToItem() {
        if ($this->congressItem < 0) {
            return;
        }
        $this->itemId = $this->insertElement([CONGRESS_ITEM_TOPIC_ITEM_ID => $this->congressItem]);
        return $this;
    }

    function getTopicDescription() {
        return $this->getValue(CONGRESS_ITEM_TOPIC_DESCRIPTION);
    }

    function setTopicDescription($desc) {
        if ($this->itemId < 0 OR $this->congressItem < 0) {
            return;
        }
        $this->updateValue(CONGRESS_ITEM_TOPIC_DESCRIPTION, $desc);
        return $this;
    }

    function isTopicsOfItem($itemId) {
        return (emtpy($this->getTopicsOfItem($itemId)) ? false : true);
    }

    function isTopicDeletable($topic_id) {
        return (empty((new CongressComments())->getCommentsOfTopic($topic_id))) ? true : false;
    }

    function getTopicsOfItem($itemId) {
        $topics = $this->getElementsBy(CONGRESS_ITEM_TOPIC_ITEM_ID, $itemId);
        $ret = array();
        foreach ($topics as $topic) {
            $ret[$topic[CONGRESS_ITEM_TOPIC_ID]] = $topic[CONGRESS_ITEM_TOPIC_DESCRIPTION];
        }
        return $ret;
    }

    function deleteTopic($topic_id) {
        $this->deleteElementById($topic_id);
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
