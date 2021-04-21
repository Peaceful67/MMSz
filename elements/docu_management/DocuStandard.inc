<?php

define("DOCU_SDN_TABLE", 'docu_sdn');
define("DOCU_SDN_ID", 'id');
define("DOCU_SDN_NAME", 'name');
define("DOCU_SDN_SHORT", 'short');
define("DOCU_SDN_ENG_NAME", 'eng_name');
define("DOCU_SDN_ENG_SHORT", 'eng_short');
define("DOCU_SDN_EXPIRE", 'expire');
define("DOCU_SDN_TYPE", 'type');
define("DOCU_SDN_ORG_ID", 'org_id');

define("DOCU_SDN_TYPE_DAO", 1);
define("DOCU_SDN_TYPE_JOG", 2);
define("DOCU_SDN_TYPE_KK", 3);
define("DOCU_SDN_TYPE_PD", 4);

class DocuStandard extends BasicElement {

    protected static $sdn_types = [
        DOCU_SDN_TYPE_DAO => 'Képzés',
        DOCU_SDN_TYPE_JOG => 'Képzettségek',
        DOCU_SDN_TYPE_KK => 'Képzés dokumentum',
        DOCU_SDN_TYPE_PD => 'Pilóta sablonok',
    ];

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_SDN_TABLE);
        $this->setTableFields([
            DOCU_SDN_ID,
            DOCU_SDN_NAME,
            DOCU_SDN_SHORT,
            DOCU_SDN_ENG_NAME,
            DOCU_SDN_ENG_SHORT,
            DOCU_SDN_EXPIRE,
            DOCU_SDN_TYPE,
            DOCU_SDN_ORG_ID,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_SDN_TABLE . '` (`'
                . DOCU_SDN_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_SDN_ORG_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_SDN_TYPE . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_SDN_NAME . '` varchar(255) NOT NULL DEFAULT "", `'
                . DOCU_SDN_SHORT . '` varchar(255) NOT NULL DEFAULT "", `'
                . DOCU_SDN_ENG_NAME . '` varchar(255) NOT NULL DEFAULT "", `'
                . DOCU_SDN_ENG_SHORT . '` varchar(255) NOT NULL DEFAULT "", `'
                . DOCU_SDN_EXPIRE . '` int(8) NOT NULL DEFAULT 0, '
                . 'UNIQUE KEY `' . DOCU_SDN_ID . '` (`' . DOCU_SDN_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_SDN_ID);
    }

    function getSDNById($id) {
        return $this->getItemById($id);
    }

    function getName($id) {
        return $this->getSDNById($id)[DOCU_SDN_NAME];
    }

    function getItems() {
        return $this->getElementsOrderBy(DOCU_SDN_TYPE);
    }

    function getNames() {
        $ret = [];
        foreach ($this->getElementsOrderBy(DOCU_SDN_SHORT) as $sdn_id => $sdn) {
            $ret[$sdn_id] = $sdn[DOCU_SDN_NAME];
        }
        return $ret;
    }

    function isUnique($docu_sdn_id, $name, $eng_short) { // Egyedi-e a rövid megnevezés
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . DOCU_SDN_TABLE . '` WHERE (`' . DOCU_SDN_NAME . '`="' . $name . '" AND `' . DOCU_SDN_ID . '`!=' . $docu_sdn_id . ') OR'
                . '(`' . DOCU_SDN_ENG_SHORT . '`="' . $eng_short . '" AND `' . DOCU_SDN_ID . '`!=' . $docu_sdn_id . ')';
        $res = $mysqliLink->query($sql);
        if ($res AND $res->num_rows > 0) {
            return false;
        } else {
            return true;
        }
    }

    function isExpire($sdn_id) {
        $sdn = $this->getItemById($sdn_id);
        if (empty($sdn)) {
            return false;
        } else {
            return $sdn[DOCU_SDN_EXPIRE] > 0;
        }
    }

    function getNamesOfType($type) {
        $ret = [];
        foreach ($this->getElementsBy(DOCU_SDN_TYPE, $type) as $sdn_id => $sdn) {
            $ret[$sdn_id] = $sdn[DOCU_SDN_NAME];
        }
        return $ret;
    }

    function getTypes() {
        return self::$sdn_types;
    }

    function getTypeName($type) {
        if ($type < 0) {
            return 'Nincs meghatározva';
        }
        return self::$sdn_types[$type];
    }

    function deleteSDN($id) {
        $this->deleteElementById($id);
        return true;
    }

    function isDeletable($item_id) {
        $this->itemId = $item_id;
        return $this->isElementDeletable([
                    DOCU_DAO_TABLE => DOCU_DAO_SDN_ID,
                    DOCU_FILE_TABLE => DOCU_FILE_SDN_ID,
        ]);
    }

    function getSDNsOfType($type) {
        $this->getElementsBy(DOCU_SDN_TYPE, $type);
    }

    function updateSDN($item_id, $name, $short, $eng_name, $eng_short, $expire, $org_id, $type) {
        $this->updateElementById($item_id, DOCU_SDN_NAME, $name);
        $this->updateElementById($item_id, DOCU_SDN_SHORT, $short);
        $this->updateElementById($item_id, DOCU_SDN_ENG_NAME, $eng_name);
        $this->updateElementById($item_id, DOCU_SDN_ENG_SHORT, $eng_short);
        $this->updateElementById($item_id, DOCU_SDN_EXPIRE, $expire);
        $this->updateElementById($item_id, DOCU_SDN_ORG_ID, $org_id);
        $this->updateElementById($item_id, DOCU_SDN_TYPE, $type);
    }

    function InsertSDN() {
        return $this->insertElement([
        ]);
    }

}
