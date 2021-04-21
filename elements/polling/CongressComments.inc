<?php

define("CONGRESS_COMMENTS_TABLE", 'congress_comments');
define("CONGRESS_COMMENTS_ID", 'id');
define("CONGRESS_COMMENTS_MEMBER_ID", 'member_id');
define("CONGRESS_COMMENTS_COMMENT", 'comment');
define("CONGRESS_COMMENTS_TIME", 'comm_time');
define("CONGRESS_COMMENTS_CONGRESS_ITEM_ID", 'item_id');
define("CONGRESS_COMMENTS_TOPIC_ID", 'topic_id');

class CongressComments extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_COMMENTS_TABLE);
        $this->setTableFields([
            CONGRESS_COMMENTS_ID,
            CONGRESS_COMMENTS_MEMBER_ID,
            CONGRESS_COMMENTS_COMMENT,
            CONGRESS_COMMENTS_TIME,
            CONGRESS_COMMENTS_CONGRESS_ITEM_ID,
            CONGRESS_COMMENTS_TOPIC_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_COMMENTS_TABLE . '` (
                `' . CONGRESS_COMMENTS_ID . '` int(8) NOT NULL AUTO_INCREMENT,
                `' . CONGRESS_COMMENTS_MEMBER_ID . '` int(8) NULL DEFAULT NULL,
                `' . CONGRESS_COMMENTS_TOPIC_ID . '` int NULL DEFAULT -1,
                `' . CONGRESS_COMMENTS_COMMENT . '` text NOT NULL,
                `' . CONGRESS_COMMENTS_TIME . '` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1) ,
                `' . CONGRESS_COMMENTS_CONGRESS_ITEM_ID . '` int(11) NOT NULL,
                UNIQUE KEY `' . CONGRESS_COMMENTS_ID . '` (`' . CONGRESS_COMMENTS_ID . '`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_COMMENTS_ID);
    }

    function setSelectedCommment($comment_id) {
        $this->getItemById($comment_id);
        return $this;
    }

    function getCommentsOfItem($itemId) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . CONGRESS_COMMENTS_TABLE . '` WHERE `'
                . CONGRESS_COMMENTS_CONGRESS_ITEM_ID . '`=' . $itemId . ' ORDER BY `' . CONGRESS_COMMENTS_TIME . '` DESC;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row;
        }
        return $ret;
    }

    function addComment($comment, $itemId, $topic_id) {
        global $member_id;
        return $this->insertElement([
                    CONGRESS_COMMENTS_MEMBER_ID => $member_id,
                    CONGRESS_COMMENTS_COMMENT => $comment,
                    CONGRESS_COMMENTS_CONGRESS_ITEM_ID => $itemId,
                    CONGRESS_COMMENTS_TOPIC_ID => $topic_id,
        ]);
    }

    function setTopicOfComment($topic_id) {
        $this->updateValue(CONGRESS_COMMENTS_TOPIC_ID, $topic_id);
    }

    function getCommentsOfCongress($congress_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT `' . CONGRESS_COMMENTS_TABLE . '`.*, `items`.`' . CONGRESS_ITEMS_TITLE . '` FROM `' . CONGRESS_COMMENTS_TABLE . '` '
                . ' INNER JOIN `' . CONGRESS_ITEMS_TABLE . '` AS `items` ON ('
                . ' `items`.`' . CONGRESS_ITEMS_CONGRESS_ID . '`=' . $congress_id . ')'
                . ' WHERE `' . CONGRESS_COMMENTS_TABLE . '`.`' . CONGRESS_COMMENTS_CONGRESS_ITEM_ID . '`=`items`.`' . CONGRESS_ITEMS_ID . '` '
                . ' ORDER BY `' . CONGRESS_COMMENTS_TABLE . '`.`' . CONGRESS_COMMENTS_TIME . '` ASC;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row;
        }
        return $ret;
    }

    function getCommentsOfTopic($topic_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT *  FROM `' . CONGRESS_COMMENTS_TABLE . '` '
                . ' WHERE `' . CONGRESS_COMMENTS_TOPIC_ID . '`=' . $topic_id
                . ' ORDER BY `' . CONGRESS_COMMENTS_TIME . '` ASC;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row;
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
