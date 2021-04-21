<?php

define("DOCU_DAO_TABLE", 'docu_dao');
define("DOCU_DAO_ID", 'id');
define("DOCU_DAO_ORG_ID", 'org_id');
define("DOCU_DAO_SDN_ID", 'sdn_id');
define("DOCU_DAO_JOG_ID_O", 'jogo_id'); // A képzés minimum ilyen oktatói képesítéssel végezhető
define("DOCU_DAO_JOG_ID_T", 'jogt_id'); // A képzéssel ilyen képzettség szerezhető
define("DOCU_DAO_ACTIVE", 'active');

class DocuDAO extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_DAO_TABLE);
        $this->setTableFields([
            DOCU_DAO_ID,
            DOCU_DAO_ORG_ID,
            DOCU_DAO_SDN_ID,
            DOCU_DAO_JOG_ID_O,
            DOCU_DAO_JOG_ID_T,
            DOCU_DAO_ACTIVE
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_DAO_TABLE . '` (`'
                . DOCU_DAO_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_DAO_ORG_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_DAO_SDN_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_DAO_ACTIVE . '` int(8) NOT NULL DEFAULT 0,`'
                . DOCU_DAO_JOG_ID_O . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_DAO_JOG_ID_T . '` int(8) NOT NULL DEFAULT -1,'
                . 'UNIQUE KEY `' . DOCU_DAO_ID . '` (`' . DOCU_DAO_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_DAO_ID);
    }

    function getDAOById($id) {
        return $this->getItemById($id);
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_DAO_ID);
    }

    function deleteDAO($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
                    DOCU_TEMP_KK_TABLE => DOCU_TEMP_KK_DAO_ID,
        ]);
    }

    function isExpire($dao_id) {
        $dao = $this->getDAOById($dao_id);
        if (empty($dao)) {
            return false;
        } else {
            $sdn_id = $dao[DOCU_DAO_SDN_ID];
            return (new DocuStandard())->isExpire($sdn_id);
        }
    }

    function updateDAO($item_id, $org_id, $sdn_id, $jog_id_o, $jog_id_t, $active) {
        $this->updateElementById($item_id, DOCU_DAO_ORG_ID, $org_id);
        $this->updateElementById($item_id, DOCU_DAO_SDN_ID, $sdn_id);
        $this->updateElementById($item_id, DOCU_DAO_JOG_ID_O, $jog_id_o);
        $this->updateElementById($item_id, DOCU_DAO_JOG_ID_T, $jog_id_t);
        $this->updateElementById($item_id, DOCU_DAO_ACTIVE, $active);
    }

    function InsertDAO() {
        return $this->insertElement([
                    DOCU_DAO_ORG_ID => 1,
        ]);
    }

    function getName($id) {
        $dao = $this->getItemById($id);
        if (isset($dao[DOCU_DAO_SDN_ID]) AND $dao[DOCU_DAO_SDN_ID] > 0) {
            return (new DocuStandard())->getName($dao[DOCU_DAO_SDN_ID]);
        } else {
            return 'Nincs meghatározva';
        }
    }

    function getNames() {
        $ret = [];
        $docuSDN = new DocuStandard();
        foreach ($this->getElementsOrderBy(DOCU_DAO_ID) as $dao_id => $dao) {
            $ret[$dao_id] = $docuSDN->getName($dao[DOCU_DAO_SDN_ID]);
        }
        return $ret;
    }

    function getActiveNames() {
        $ret = [];
        $docuSDN = new DocuStandard();
        $docuJOG = new DocuJOG();
        foreach ($this->getElementsOrderBy(DOCU_DAO_ID) as $dao_id => $dao) {
            if ($dao[DOCU_DAO_ACTIVE] > 0) {
                $ret[$dao_id] = $docuSDN->getName($dao[DOCU_DAO_SDN_ID]) . ' (' . $docuJOG->getName($dao[DOCU_DAO_JOG_ID_T]) . ')';
            }
        }
        return $ret;
    }

}
