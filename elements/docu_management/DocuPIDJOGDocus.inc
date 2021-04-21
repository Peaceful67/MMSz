<?php

define("DOCU_PIDJOG_DOCUS_TABLE", 'docu_pidjog_docus');
define("DOCU_PIDJOG_DOCUS_ID", 'id');
define("DOCU_PIDJOG_DOCUS_PIDJOG_ID", 'pidjog_id');
define("DOCU_PIDJOG_DOCUS_PD_ID", 'pd_id');
define("DOCU_PIDJOG_DOCUS_VARIABLES", 'variables');
define("DOCU_PIDJOG_DOCUS_EXPIRE", 'expire'); // Lejáró tipusú-e
define("DOCU_PIDJOG_DOCUS_PD_DOCU_ID", 'pd_docu_id');
define("DOCU_PIDJOG_DOCUS_SDN_ID", 'sdn_id');
define("DOCU_PIDJOG_DOCUS_DOCU_ID", 'docu_id');
define("DOCU_PIDJOG_DOCUS_EXPIRE_DATE", 'expire_date');
define("DOCU_PIDJOG_DOCUS_OWN", 'own');
define("DOCU_PIDJOG_DOCUS_OWN_NAME", 'own_name');
define("DOCU_PIDJOG_DOCUS_PUBLIC", 'public');
define("DOCU_PIDJOG_DOCUS_MODIFIED_TIME", 'modified');
define("DOCU_PIDJOG_DOCUS_MODIFIED_MEMBER", 'modified_by');
define("DOCU_PIDJOG_DOCUS_WARNING_TIME", 'warning_time');
define("DOCU_PIDJOG_DOCUS_ALERT_TIME", 'alert_time');

/**
 * Description of DocuPIDAO
 *   Képzés dokumentumai
 * @author Peaceful
 */
class DocuPIDJOGDocus extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_PIDJOG_DOCUS_TABLE);
        $this->setTableFields([
            DOCU_PIDJOG_DOCUS_ID,
            DOCU_PIDJOG_DOCUS_PIDJOG_ID,
            DOCU_PIDJOG_DOCUS_PD_ID,
            DOCU_PIDJOG_DOCUS_VARIABLES,
            DOCU_PIDJOG_DOCUS_PD_DOCU_ID,
            DOCU_PIDJOG_DOCUS_SDN_ID,
            DOCU_PIDJOG_DOCUS_EXPIRE,
            DOCU_PIDJOG_DOCUS_DOCU_ID,
            DOCU_PIDJOG_DOCUS_EXPIRE_DATE,
            DOCU_PIDJOG_DOCUS_PUBLIC,
            DOCU_PIDJOG_DOCUS_MODIFIED_TIME,
            DOCU_PIDJOG_DOCUS_MODIFIED_MEMBER,
            DOCU_PIDJOG_DOCUS_OWN,
            DOCU_PIDJOG_DOCUS_OWN_NAME,
            DOCU_PIDJOG_DOCUS_WARNING_TIME,
            DOCU_PIDJOG_DOCUS_ALERT_TIME,
        ]);
        $this->setVariablesName(DOCU_PIDJOG_DOCUS_VARIABLES);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_PIDJOG_DOCUS_TABLE . '` (`'
                . DOCU_PIDJOG_DOCUS_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_PIDJOG_DOCUS_PIDJOG_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDJOG_DOCUS_PD_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDJOG_DOCUS_SDN_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDJOG_DOCUS_PD_DOCU_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDJOG_DOCUS_EXPIRE . '` TINYINT NOT NULL DEFAULT 0,`'
                . DOCU_PIDJOG_DOCUS_DOCU_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDJOG_DOCUS_EXPIRE_DATE . '` DATE NULL DEFAULT NULL, `'
                . DOCU_PIDJOG_DOCUS_OWN_NAME . '` VARCHAR(254) NOT NULL DEFAULT "",  `'
                . DOCU_PIDJOG_DOCUS_OWN . '` TINYINT NOT NULL DEFAULT 0,`'
                . DOCU_PIDJOG_DOCUS_PUBLIC . '` TINYINT NOT NULL DEFAULT 1,`'
                . DOCU_PIDJOG_DOCUS_MODIFIED_TIME . '` TIMESTAMP NULL DEFAULT NULL, `'
                . DOCU_PIDJOG_DOCUS_MODIFIED_MEMBER . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDAO_DOCUS_WARNING_TIME . '` TIMESTAMP NULL DEFAULT NULL, `'
                . DOCU_PIDAO_DOCUS_ALERT_TIME . '` TIMESTAMP NULL DEFAULT NULL, '
                . 'UNIQUE KEY `' . DOCU_PIDJOG_DOCUS_ID . '` (`' . DOCU_PIDJOG_DOCUS_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_PIDJOG_DOCUS_ID);
    }

    public function createPidjogDocu($pidjog_id, $pidjog_pd_id, $docu_id, $sdn_id, $is_expire, $is_own) {
        return $this->insertElement([
                    DOCU_PIDJOG_DOCUS_PIDJOG_ID => $pidjog_id,
                    DOCU_PIDJOG_DOCUS_PD_ID => $pidjog_pd_id,
                    DOCU_PIDJOG_DOCUS_EXPIRE => ($is_expire ? 1 : 0),
                    DOCU_PIDJOG_DOCUS_PD_DOCU_ID => $docu_id,
                    DOCU_PIDJOG_DOCUS_SDN_ID => $sdn_id,
                    DOCU_PIDJOG_DOCUS_OWN => ($is_own ? 1 : 0),
                    DOCU_PIDJOG_DOCUS_PUBLIC => 1,
        ]);
    }

    public function setDocuId($docu_id) {
        $this->updateElement(DOCU_PIDJOG_DOCUS_DOCU_ID, $docu_id);
    }

    public function getPIDJOGDocusOfPidjog($pidjog_id) {
        return $this->getElementsBy(DOCU_PIDJOG_DOCUS_PIDJOG_ID, $pidjog_id);
    }

    public function deletePIDJOGDocus($id) {
        return $this->deleteElementById($id);
    }

    public function getPIDJOGDocusById($id) {
        return $this->getElement($id);
    }

    public function getItems() {
        
    }

    public function isValid($id) {
        global $today;
        $this->getElement($id);
        if ($this->tableRow[DOCU_PIDJOG_DOCUS_DOCU_ID] < 0) {
            return false;
        }
        if ($this->tableRow[DOCU_PIDJOG_DOCUS_EXPIRE] > 0) {
            return $this->tableRow[DOCU_PIDJOG_DOCUS_EXPIRE_DATE] > $today;
        } else {
            return true;
        }
    }

    public function updateDocu($id, $is_expire, $expire_date, $is_public, $own_name = '') {
        global $member_id;
        $this->getItemById($id);
        $this->updateValue(DOCU_PIDJOG_DOCUS_MODIFIED_MEMBER, $member_id);
        $this->setTimeCurrent(DOCU_PIDJOG_DOCUS_MODIFIED_TIME);
        $this->updateValue(DOCU_PIDJOG_DOCUS_EXPIRE, ($is_expire ? 1 : 0));
        $this->updateValue(DOCU_PIDJOG_DOCUS_EXPIRE_DATE, $expire_date);
        $this->updateValue(DOCU_PIDJOG_DOCUS_PUBLIC, ($is_public ? 1 : 0));
        $this->updateValue(DOCU_PIDJOG_DOCUS_OWN_NAME, $own_name);
    }

    public function getPidjogDocus($id) {
        $this->getItemById($id);
        return $this;
    }

    public function getDocuId() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_DOCU_ID);
    }

    public function getPDDocuId() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_PD_DOCU_ID);
    }

    public function getSDNId() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_SDN_ID);
    }

    public function isOwn() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_OWN) > 0;
    }

    public function isExpire() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_EXPIRE) > 0;
    }

    public function isPublic() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_PUBLIC) > 0;
    }

    public function getOwnName() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_OWN_NAME);
    }

    public function getExpireDate() {
        return $this->getValue(DOCU_PIDJOG_DOCUS_EXPIRE_DATE);
    }

    public function getName($id) {
        $docu = $this->getItemById($id);
        if (empty($docu)) {
            return '';
        } else {
            return (new DocuStandard())->getName($docu[DOCU_PIDJOG_DOCUS_SDN_ID]);
        }
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        return true;
    }

}
