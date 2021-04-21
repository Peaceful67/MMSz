<?php

define("DOCU_TEMP_KK_TABLE", 'docu_temp_kk');
define("DOCU_TEMP_KK_ID", 'id');
define("DOCU_TEMP_KK_DAO_ID", 'dao_id');
define("DOCU_TEMP_KK_DOCU_ID", 'docu_id');
define("DOCU_TEMP_KK_JOG_ID_O", 'trainer_id');
define("DOCU_TEMP_KK_PATTERN", 'pattern');
define("DOCU_TEMP_KK_EXPIRE", 'expire');

class DocuTemplateKK extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_TEMP_KK_TABLE);
        $this->setTableFields([
            DOCU_TEMP_KK_ID,
            DOCU_TEMP_KK_DAO_ID,
            DOCU_TEMP_KK_DOCU_ID,
            DOCU_TEMP_KK_JOG_ID_O,
            DOCU_TEMP_KK_PATTERN,
            DOCU_TEMP_KK_EXPIRE,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_TEMP_KK_TABLE . '` (`'
                . DOCU_TEMP_KK_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_TEMP_KK_DAO_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_TEMP_KK_DOCU_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_TEMP_KK_PATTERN . '` tinyint NOT NULL DEFAULT 0,`'
                . DOCU_TEMP_KK_EXPIRE . '` tinyint NOT NULL DEFAULT 1,`'
                . DOCU_TEMP_KK_JOG_ID_O . '` int(8) NOT NULL DEFAULT -1,'
                . 'UNIQUE KEY `' . DOCU_TEMP_KK_ID . '` (`' . DOCU_TEMP_KK_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_TEMP_KK_ID);
    }

    function getTempKKById($id) {
        return $this->getItemById($id);
    }

    function getTempsOfDao($dao_id) {
        return $this->getElementsBy(DOCU_TEMP_KK_DAO_ID, $dao_id);
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_TEMP_KK_ID);
    }

    function deleteTempKK($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
                    DOCU_TEMP_KK_TABLE => DOCU_TEMP_KK_DOCU_ID,
        ]);
    }

    function setDocuId($docu_id) {
        $this->updateElementById($item_id, DOCU_TEMP_KK_DOCU_ID, $docu_id);
    }

    function updateTempKK($item_id, $trainer_id, $pattern, $expire) {
        $this->updateElementById($item_id, DOCU_TEMP_KK_JOG_ID_O, $trainer_id);
        $this->updateElementById($item_id, DOCU_TEMP_KK_PATTERN, $pattern);
        $this->updateElementById($item_id, DOCU_TEMP_KK_EXPIRE, $expire);
    }

    function InsertTempKK($docu_id, $dao_id) {
        return $this->insertElement([
                    DOCU_TEMP_KK_DOCU_ID => $docu_id,
                    DOCU_TEMP_KK_DAO_ID => $dao_id,
                    DOCU_TEMP_KK_PATTERN => 0,
                    DOCU_TEMP_KK_EXPIRE => 1,
        ]);
    }

    function getKKsOfDao($dao_id) {
        return $this->getElementsBy(DOCU_TEMP_KK_DAO_ID, $dao_id);
    }

    function getCurrentDocuFile($id) {
        $item = $this->getItemById($id);
        $docu_id = $item[DOCU_TEMP_KK_DOCU_ID];
        if ($docu_id < 0) {
            return [];
        }
        $docuFile = new DocuFile();
        $files = $docuFile->getFilesByDocuId($docu_id);
        return end($files);
    }

    function getNames() {
        $ret = [];
        return $ret;
    }

    function getActives() {
        return $this->getElementsBy(DOCU_TEMP_KK_ACTIVE, 1);
    }

}
