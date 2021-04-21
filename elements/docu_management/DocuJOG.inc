<?php

define("DOCU_JOG_TABLE", 'docu_jog');
define("DOCU_JOG_ID", 'id');
define("DOCU_JOG_IS_TRAINER", 'trainer');
define("DOCU_JOG_LOWER", 'lower');
define("DOCU_JOG_NAME", 'name');

class DocuJOG extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_JOG_TABLE);
        $this->setTableFields([
            DOCU_JOG_ID,
            DOCU_JOG_IS_TRAINER,
            DOCU_JOG_LOWER,
            DOCU_JOG_NAME,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_JOG_TABLE . '` (`'
                . DOCU_JOG_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_JOG_IS_TRAINER . '` int(8) NOT NULL DEFAULT 0,`'
                . DOCU_JOG_NAME . '` varchar(255) NULL DEFAULT NULL, `'
                . DOCU_JOG_LOWER . '` int(8) NOT NULL DEFAULT -1,'
                . 'UNIQUE KEY `' . DOCU_JOG_ID . '` (`' . DOCU_JOG_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_JOG_ID);
    }

    function getJOGById($id) {
        return $this->getItemById($id);
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_JOG_NAME);
    }

    function deleteJOG($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
                    DOCU_JOG_TABLE => DOCU_JOG_LOWER,
                    DOCU_DAO_TABLE => DOCU_DAO_JOG_ID_O,
                    DOCU_DAO_TABLE => DOCU_DAO_JOG_ID_T,
                    DOCU_JOG_LJK_TABLE => DOCU_JOG_LJK_JOG_ID,
                    DOCU_TEMP_KK_TABLE => DOCU_TEMP_KK_JOG_ID_O,
                    DOCU_TEMP_PD_TABLE => DOCU_TEMP_PD_JOG_ID,
        ]);
    }

    function updateJOG($item_id, $name, $is_trainer, $lower_id) {
        $this->updateElementById($item_id, DOCU_JOG_NAME, $name);
        $this->updateElementById($item_id, DOCU_JOG_IS_TRAINER, $is_trainer);
        $this->updateElementById($item_id, DOCU_JOG_LOWER, $lower_id);
    }

    function InsertJOG() {
        return $this->insertElement([
        ]);
    }
    function isTrainer($jog_id) {
       $jog = $this->getItemById($jog_id);
       if(empty($jog)) {
           return false;
       } else {
           return $jog[DOCU_JOG_IS_TRAINER]==1;
       }
    }
    function getNames() {
        $ret = [];
        foreach ($this->getElementsOrderBy(DOCU_JOG_NAME) as $jog_id => $jog) {
            $ret[$jog_id] = $jog[DOCU_JOG_NAME];
        }
        return $ret;
    }

    function getName($id) {
        return $id > 0 ? $this->getItemById($id)[DOCU_JOG_NAME] : "";
    }

    function getTrainerNames() {
        $ret = [];
        foreach ($this->getElementsOrderBy(DOCU_JOG_NAME) as $jog_id => $jog) {
            if ($jog[DOCU_JOG_IS_TRAINER] > 0) {
                $ret[$jog_id] = $jog[DOCU_JOG_NAME];
            }
        }
        return $ret;
    }

}
