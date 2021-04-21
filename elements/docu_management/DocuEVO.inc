<?php

define("DOCU_EVO_TABLE", 'docu_evo');
define("DOCU_EVO_ID", 'id');
define("DOCU_EVO_CLUB", 'club_id');
define("DOCU_EVO_ORG_ID", 'org_id');
define("DOCU_EVO_LJK", 'ljk');


class DocuEVO extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_EVO_TABLE);
        $this->setTableFields([
            DOCU_EVO_ID,
            DOCU_EVO_CLUB,
            DOCU_EVO_ORG_ID,
            DOCU_EVO_LJK,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_EVO_TABLE . '` (`'
                . DOCU_EVO_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_EVO_CLUB . '` int(8) NULL DEFAULT -1,`'
                . DOCU_EVO_ORG_ID . '` int(8) NULL DEFAULT -1,`'
                . DOCU_EVO_LJK . '` int(8) NULL DEFAULT -1,'
                . 'UNIQUE KEY `' . DOCU_EVO_ID . '` (`' . DOCU_EVO_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_EVO_ID);
    }

    function getEVOById($id) {
        return $this->getItemById($id);
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_EVO_ID);
    }

    function deleteEVO($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([]);
    }

    function updateEVO($item_id, $club, $org_id, $craft) {
        $this->updateElementById($item_id, DOCU_EVO_ORG_ID, $org_id);
        $this->updateElementById($item_id, DOCU_EVO_CLUB, $club);
        $this->updateElementById($item_id, DOCU_EVO_LJK, $craft);
    }

    function InsertEVO() {
        return $this->insertElement([
        ]);
    }

    function getNames() {
        
    }

}
