<?php

define("DOCU_PIDAO_DOCUS_TABLE", 'docu_pidao_docus');
define("DOCU_PIDAO_DOCUS_ID", 'id');
define("DOCU_PIDAO_DOCUS_PIDAO_ID", 'pidao_id');
define("DOCU_PIDAO_DOCUS_KK_ID", 'kk_id');
define("DOCU_PIDAO_DOCUS_VARIABLES", 'variables');
define("DOCU_PIDAO_DOCUS_EXPIRE", 'expire'); // Lejáró tipusú-e
define("DOCU_PIDAO_DOCUS_KK_DOCU_ID", 'kk_docu_id'); // Sablonból átvett dokumentum id
define("DOCU_PIDAO_DOCUS_MEMBER_SO", 'member_so'); // Specialis szakoktató, ha a képzésvezető nem tudja oktatni
define("DOCU_PIDAO_DOCUS_DOCU_ID", 'docu_id'); // Sablonból átvett dokumentum id
define("DOCU_PIDAO_DOCUS_EXPIRE_DATE", 'expire_date'); // Lejárat időpontje
define("DOCU_PIDAO_DOCUS_STATUS", 'status'); // Hibás, hiányos, alapértelmezetten: Rendben
define("DOCU_PIDAO_DOCUS_APPROVED", 'approved'); // Jóváhagyva
define("DOCU_PIDAO_DOCUS_PUBLIC", 'public');
define("DOCU_PIDAO_DOCUS_OWN", 'own');
define("DOCU_PIDAO_DOCUS_OWN_NAME", 'own_name');
define("DOCU_PIDAO_DOCUS_MODIFIED_TIME", 'modified');
define("DOCU_PIDAO_DOCUS_MODIFIED_MEMBER", 'modified_by');
define("DOCU_PIDAO_DOCUS_APPROVED_TIME", 'approved_time');
define("DOCU_PIDAO_DOCUS_APPROVED_MEMBER", 'approved_by');
define("DOCU_PIDAO_DOCUS_WARNING_TIME", 'warning_time');
define("DOCU_PIDAO_DOCUS_ALERT_TIME", 'alert_time');

/**
 * Description of DocuPIDAO
 *   Képzés dokumentumai
 * @author Peaceful
 */
class DocuPIDAODocus extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_PIDAO_DOCUS_TABLE);
        $this->setTableFields([
            DOCU_PIDAO_DOCUS_ID,
            DOCU_PIDAO_DOCUS_PIDAO_ID,
            DOCU_PIDAO_DOCUS_KK_ID,
            DOCU_PIDAO_DOCUS_VARIABLES,
            DOCU_PIDAO_DOCUS_EXPIRE,
            DOCU_PIDAO_DOCUS_KK_DOCU_ID,
            DOCU_PIDAO_DOCUS_MEMBER_SO,
            DOCU_PIDAO_DOCUS_DOCU_ID,
            DOCU_PIDAO_DOCUS_EXPIRE_DATE,
            DOCU_PIDAO_DOCUS_STATUS,
            DOCU_PIDAO_DOCUS_APPROVED,
            DOCU_PIDAO_DOCUS_PUBLIC,
            DOCU_PIDAO_DOCUS_OWN,
            DOCU_PIDAO_DOCUS_OWN_NAME,
            DOCU_PIDAO_DOCUS_MODIFIED_TIME,
            DOCU_PIDAO_DOCUS_MODIFIED_MEMBER,
            DOCU_PIDAO_DOCUS_APPROVED_TIME,
            DOCU_PIDAO_DOCUS_APPROVED_MEMBER,
            DOCU_PIDAO_DOCUS_WARNING_TIME,
            DOCU_PIDAO_DOCUS_ALERT_TIME,
        ]);
        $this->setVariablesName(DOCU_PIDAO_DOCUS_VARIABLES);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_PIDAO_DOCUS_TABLE . '` (`'
                . DOCU_PIDAO_DOCUS_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_PIDAO_DOCUS_PIDAO_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_KK_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_EXPIRE . '` TINYINT NOT NULL DEFAULT 0,`'
                . DOCU_PIDAO_DOCUS_KK_DOCU_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_MEMBER_SO . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_DOCU_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_EXPIRE_DATE . '` DATE NULL DEFAULT NULL, `'
                . DOCU_PIDAO_DOCUS_STATUS . '` VARCHAR(254) NOT NULL DEFAULT "",  `'
                . DOCU_PIDAO_DOCUS_APPROVED . '` TINYINT NOT NULL DEFAULT 0,`'
                . DOCU_PIDAO_DOCUS_PUBLIC . '` TINYINT NOT NULL DEFAULT 1,`'
                . DOCU_PIDAO_DOCUS_OWN . '` TINYINT NOT NULL DEFAULT 0,`'
                . DOCU_PIDAO_DOCUS_OWN_NAME . '` VARCHAR(254) NOT NULL DEFAULT "",`'
                . DOCU_PIDAO_DOCUS_MODIFIED_TIME . '` TIMESTAMP NULL DEFAULT NULL, `'
                . DOCU_PIDAO_DOCUS_MODIFIED_MEMBER . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_APPROVED_TIME . '` TIMESTAMP NULL DEFAULT NULL, `'
                . DOCU_PIDAO_DOCUS_APPROVED_MEMBER . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_WARNING_TIME . '` TIMESTAMP NULL DEFAULT NULL, `'
                . DOCU_PIDAO_DOCUS_ALERT_TIME . '` TIMESTAMP NULL DEFAULT NULL, '
                . 'UNIQUE KEY `' . DOCU_PIDAO_DOCUS_ID . '` (`' . DOCU_PIDAO_DOCUS_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_PIDAO_DOCUS_ID);
    }

    public function createPidaoDocu($pidao_id, $pidao_kk_id, $pidao_kk_docu_id, $is_expire, $is_own = false) {

        return $this->insertElement([
                    DOCU_PIDAO_DOCUS_PIDAO_ID => $pidao_id,
                    DOCU_PIDAO_DOCUS_KK_ID => $pidao_kk_id,
                    DOCU_PIDAO_DOCUS_KK_DOCU_ID => $pidao_kk_docu_id,
                    DOCU_PIDAO_DOCUS_EXPIRE => $is_expire,
                    DOCU_PIDAO_DOCUS_OWN => ($is_own ? 1 : 0),
        ]);
    }

    public function setDocuId($docu_id) {
        $this->updateElement(DOCU_PIDAO_DOCUS_DOCU_ID, $docu_id);
    }

    public function updatePidaoDocu($is_expire, $expire_date, $is_public, $own_name) {
        global $member_id;
        $this->updateValue(DOCU_PIDAO_DOCUS_MODIFIED_MEMBER, $member_id);
        $this->setTimeCurrent(DOCU_PIDAO_DOCUS_MODIFIED_TIME);
        $this->updateElement(DOCU_PIDAO_DOCUS_EXPIRE, $is_expire?1:0);
        $this->updateElement(DOCU_PIDAO_DOCUS_EXPIRE_DATE, $expire_date);
        $this->updateElement(DOCU_PIDAO_DOCUS_PUBLIC, $is_public?1:0);
        $this->updateElement(DOCU_PIDAO_DOCUS_OWN_NAME, $own_name);
    }

    public function getPidaoDocusOfPidao($pidao_id) {
        return $this->getElementsBy(DOCU_PIDAO_DOCUS_PIDAO_ID, $pidao_id);
    }

    public function deletePIDAODocus($id) {
        return $this->deleteElementById($id);
    }

    public function getPidaoDocusById($id) {
        return $this->getItemById($id);
    }

    public function isExpire() {
        return $this->tableRow[DOCU_PIDAO_DOCUS_EXPIRE] > 0;
    }

    public function isPublic() {
        return $this->tableRow[DOCU_PIDAO_DOCUS_PUBLIC] > 0;
    }

    public function getExpireDate() {
        return $this->tableRow[DOCU_PIDAO_DOCUS_EXPIRE_DATE];
    }

    public function isOwn() {
        return $this->tableRow[DOCU_PIDAO_DOCUS_OWN] > 0;
    }

    public function getOwnName() {
        return $this->tableRow[DOCU_PIDAO_DOCUS_OWN_NAME];
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        return true;
    }

}
