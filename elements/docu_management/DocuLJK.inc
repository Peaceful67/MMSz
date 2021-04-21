<?php

define("DOCU_LJK_TABLE", 'docu_ljk');
define("DOCU_LJK_ID", 'id');
define("DOCU_LJK_NAME", 'name');
define("DOCU_LJK_BRANCH", 'branch');

class DocuLJK extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_LJK_TABLE);
        $this->setTableFields([
            DOCU_LJK_ID,
            DOCU_LJK_NAME,
            DOCU_LJK_BRANCH,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_LJK_TABLE . '` (`'
                . DOCU_LJK_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_LJK_BRANCH . '` int(8) NULL DEFAULT -1,`'
                . DOCU_LJK_NAME . '` varchar(255) NULL DEFAULT NULL, '
                . 'UNIQUE KEY `' . DOCU_LJK_ID . '` (`' . DOCU_LJK_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_LJK_ID);
    }

    function getLJKById($id) {
        return $this->getItemById($id);
    }

    function getLJKs() {
        return $this->getElementsOrderBy(DOCU_LJK_NAME);
    }

    function getNames() {
        $ret = [];
        foreach ($this->getLJKs() as $ljk_id => $ljk) {
            $ret[$ljk_id] = $ljk[DOCU_LJK_NAME];
        }
        return $ret;
    }

    function deleteLJK($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
                    DOCU_EVO_TABLE => DOCU_EVO_LJK,
                    DOCU_JOG_LJK_TABLE => DOCU_JOG_LJK_LJK_ID,
        ]);
    }

    function updateLJK($item_id, $name, $branch) {
        $this->updateElementById($item_id, DOCU_LJK_NAME, $name);
        $this->updateElementById($item_id, DOCU_LJK_BRANCH, $branch);
    }

    function InsertLJK() {
        return $this->insertElement([
                    DOCU_LJK_NAME => '',
        ]);
    }

    public function getItems() {
        
    }

}
