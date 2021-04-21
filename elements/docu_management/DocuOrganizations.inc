<?php

define("DOCU_ORG_TABLE", 'docu_orgs');
define("DOCU_ORG_ID", 'id');
define("DOCU_ORG_NAME", 'name');
define("DOCU_ORG_TYPE", 'type');
define("DOCU_ORG_ASSOCIATION_ID", 'assoc');

define("DOCU_ORG_TYPE_EDU", '1');
define("DOCU_ORG_TYPE_FLF", '2');

$docu_org_types_name = [
    DOCU_ORG_TYPE_EDU => 'Képzési szervezet',
    DOCU_ORG_TYPE_FLF => 'FLF szervezet',
];

class DocuOrganizations extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_ORG_TABLE);
        $this->setTableFields([
            DOCU_ORG_ID,
            DOCU_ORG_NAME,
            DOCU_ORG_TYPE,
            DOCU_ORG_ASSOCIATION_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_ORG_TABLE . '` (`'
                . DOCU_ORG_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_ORG_ASSOCIATION_ID . '` int(8) NULL DEFAULT -1,`'
                . DOCU_ORG_TYPE . '` int(8) NULL DEFAULT -1,`'
                . DOCU_ORG_NAME . '` varchar(255) NULL DEFAULT NULL, '
                . 'UNIQUE KEY `' . DOCU_ORG_ID . '` (`' . DOCU_ORG_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_ORG_ID);
    }

    function setName($name) {
        $this->updateElement(DOCU_ORG_NAME, $name);
    }

    function getOrgById($id) {
        return $this->getItemById($id);
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_ORG_NAME);
    }

    function getNames() {
        $ret = [];
        foreach ($this->getElementsOrderBy(DOCU_ORG_NAME) as $org_id => $org) {
            $ret[$org_id] = $org[DOCU_ORG_NAME];
        }
        return $ret;
    }

    function getEduNames() {
        $ret = [];
        foreach ($this->getElementsOrderBy(DOCU_ORG_NAME) as $org_id => $org) {
            if ($org[DOCU_ORG_TYPE] == DOCU_ORG_TYPE_EDU) {
                $ret[$org_id] = $org[DOCU_ORG_NAME];
            }
        }
        return $ret;
    }

    function deleteOrg($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
                    DOCU_EVO_TABLE => DOCU_EVO_ORG_ID,
                    DOCU_SDN_TABLE => DOCU_SDN_ORG_ID,
                    DOCU_DAO_TABLE => DOCU_DAO_ORG_ID,
        ]);
    }

    function updateOrg($item_id, $name, $type, $assoc) {
        $this->updateElementById($item_id, DOCU_ORG_NAME, $name);
        $this->updateElementById($item_id, DOCU_ORG_TYPE, $type);
        $this->updateElementById($item_id, DOCU_ORG_ASSOCIATION_ID, $assoc);
    }

    function InsertOrg() {
        return $this->insertElement([
                    DOCU_ORG_NAME => '',
                    DOCU_ORG_TYPE => DOCU_ORG_TYPE_EDU,
        ]);
    }

}
