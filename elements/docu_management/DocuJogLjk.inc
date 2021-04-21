<?php

define("DOCU_JOG_LJK_TABLE", 'docu_jog_ljk');
define("DOCU_JOG_LJK_ID", 'id');
define("DOCU_JOG_LJK_JOG_ID", 'jog_id');
define("DOCU_JOG_LJK_LJK_ID", 'ljk_id');

class DocuJogLjk extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_JOG_LJK_TABLE);
        $this->setTableFields([
            DOCU_JOG_LJK_ID,
            DOCU_JOG_LJK_JOG_ID,
            DOCU_JOG_LJK_LJK_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_JOG_LJK_TABLE . '` (`'
                . DOCU_JOG_LJK_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_JOG_LJK_JOG_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_JOG_LJK_LJK_ID . '` int(8) NOT NULL DEFAULT -1,'
                . 'UNIQUE KEY `' . DOCU_JOG_LJK_ID . '` (`' . DOCU_JOG_LJK_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_JOG_LJK_ID);
    }

    function getJogLjkById($id) {
        return $this->getItemById($id);
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_JOG_LJK_ID);
    }

    function deleteJogLjk($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
            ]);
    }

    function updateJogLjk($item_id, $name, $jog_id, $ljk_id) {
        $this->updateElementById($item_id, DOCU_JOG_LJK_NAME, $name);
        $this->updateElementById($item_id, DOCU_JOG_LJK_JOG_ID, $jog_id);
        $this->updateElementById($item_id, DOCU_JOG_LJK_LJK_ID, $ljk_id);
    }

    function InsertJogLjk() {
        return $this->insertElement([
        ]);
    }

    function getNames() {
        $ret = [];
        return $ret;
    }

}
