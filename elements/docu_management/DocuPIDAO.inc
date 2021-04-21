<?php

define("DOCU_PIDAO_TABLE", 'docu_pidao');
define("DOCU_PIDAO_ID", 'id');
define("DOCU_PIDAO_MEMBER_ID", 'member_id'); // Növendék azonosítója
define("DOCU_PIDAO_DAO_ID", 'dao_id');      // A képzés, amit el akar végezni
define("DOCU_PIDAO_MEMBER_OA", 'member_oa');    // Képzéssel megbízott oktató

/**
 * Description of DocuPIDAO
 *   Képzés személyi azonosító adatok
 * @author Peaceful
 */
class DocuPIDAO extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_PIDAO_TABLE);
        $this->setTableFields([
            DOCU_PIDAO_ID,
            DOCU_PIDAO_MEMBER_ID,
            DOCU_PIDAO_DAO_ID,
            DOCU_PIDAO_MEMBER_OA,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_PIDAO_TABLE . '` (`'
                . DOCU_PIDAO_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_PIDAO_MEMBER_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DAO_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_MEMBER_OA . '` int(8) NOT NULL DEFAULT -1, '
                . 'UNIQUE KEY `' . DOCU_PIDAO_ID . '` (`' . DOCU_PIDAO_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_PIDAO_ID);
    }

    public function createPidao($mem_id, $dao_id) {
        $pidao_id = $this->insertElement([
            DOCU_PIDAO_MEMBER_ID => $mem_id,
            DOCU_PIDAO_DAO_ID => $dao_id,
        ]);

        $docuKK = new DocuTemplateKK();
        $docuPidaoDocus = new DocuPIDAODocus();
        $kks = $docuKK->getKKsOfDao($dao_id);
        foreach ($kks as $kk) {
            $docuPidaoDocus->createPidaoDocu($pidao_id, $kk[DOCU_TEMP_KK_ID], $kk[DOCU_TEMP_KK_DOCU_ID], $kk[DOCU_TEMP_PD_EXPIRE], false);
        }
    }

    public function deletePIDAO($id) {
        $this->deleteElementById($id);
        $pidaoDocus = new DocuPIDAODocus();
        $docus = $pidaoDocus->getPidaoDocusOfPidao($id);
        foreach ($docus as $docu) {
            $pidaoDocus->deletePIDAODocus($docu[DOCU_PIDAO_DOCUS_ID]);
        }
    }

    public function updatePIDAO($item_id, $dao_id) {
        $this->updateElementById($item_id, DOCU_PIDAO_DAO_ID, $dao_id);
    }

    public function getPidaoById($id) {
        return $this->getItemById($id);
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        return true;
    }

    public function getPIDAOsOfMember($mem_id) {
        global $mysqliLink;
        $sql = 'SELECT * FROM `' . DOCU_PIDAO_TABLE . '` WHERE `' . DOCU_PIDAO_MEMBER_ID . '`=' . $mem_id . ' GROUP BY `' . DOCU_PIDAO_DAO_ID . '`;';
        $ret = [];
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[DOCU_PIDAO_ID]] = $row;
        }
        return $ret;
    }

}
