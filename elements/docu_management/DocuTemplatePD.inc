<?php

define("DOCU_TEMP_PD_TABLE", 'docu_temp_pd');
define("DOCU_TEMP_PD_ID", 'id');
define("DOCU_TEMP_PD_JOG_ID", 'jog_id');
define("DOCU_TEMP_PD_SDN_ID", 'sdn_id');
define("DOCU_TEMP_PD_DOCU_ID", 'docu_id');
define("DOCU_TEMP_PD_PUBLIC", 'public');
define("DOCU_TEMP_PD_EXPIRE", 'expire');

class DocuTemplatePD extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_TEMP_PD_TABLE);
        $this->setTableFields([
            DOCU_TEMP_PD_ID,
            DOCU_TEMP_PD_JOG_ID,
            DOCU_TEMP_PD_SDN_ID,
            DOCU_TEMP_PD_DOCU_ID,
            DOCU_TEMP_PD_PUBLIC,
            DOCU_TEMP_PD_EXPIRE,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_TEMP_PD_TABLE . '` (`'
                . DOCU_TEMP_PD_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_TEMP_PD_JOG_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_TEMP_PD_SDN_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_TEMP_PD_DOCU_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_TEMP_PD_EXPIRE . '` TINYINT NOT NULL DEFAULT 1,`'
                . DOCU_TEMP_PD_PUBLIC . '` TINYINT NOT NULL DEFAULT 0,'
                . 'UNIQUE KEY `' . DOCU_TEMP_PD_ID . '` (`' . DOCU_TEMP_PD_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_TEMP_PD_ID);
    }

    function getTempPDById($id) {
        return $this->getItemById($id);
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_TEMP_PD_ID);
    }

    function deleteTempPD($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
        ]);
    }

    function updateTempPD($item_id, $sdn_id, $expire, $public) {
        $this->updateElementById($item_id, DOCU_TEMP_PD_SDN_ID, $sdn_id);
        $this->updateElementById($item_id, DOCU_TEMP_PD_EXPIRE, $expire);
        $this->updateElementById($item_id, DOCU_TEMP_PD_PUBLIC, $public);
    }

    function getPDsOfJog($jog_id) {
        return $this->getElementsBy(DOCU_TEMP_PD_JOG_ID, $jog_id);
    }

    function InsertTempPD($docu_id, $jog_id) {
        return $this->insertElement([
                    DOCU_TEMP_PD_DOCU_ID => $docu_id,
                    DOCU_TEMP_PD_JOG_ID => $jog_id,
                    DOCU_TEMP_PD_PUBLIC => 0,
                    DOCU_TEMP_PD_EXPIRE => 1,
        ]);
    }

    function getCurrentDocuFile($id) {
        $item = $this->getItemById($id);
        $docu_id = $item[DOCU_TEMP_PD_DOCU_ID];
        if ($docu_id < 0) {
            return [];
        }
        $docuFile = new DocuFile();
        $files = $docuFile->getFilesByDocuId($docu_id);
        return end($files);
    }
    function getName($pd_id) {
        $pd = $this->getItemById($pd_id);
        if(isset($pd[DOCU_TEMP_PD_SDN_ID])) {
            $docuSDN = new DocuStandard();
            return $docuSDN->getName($pd[DOCU_TEMP_PD_SDN_ID]);
        } else {
            return '';
        }
    }

    function getNames() {
        $ret = [];
        return $ret;
    }

}
