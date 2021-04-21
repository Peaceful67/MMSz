<?php

define("DOCU_PIDJOG_TABLE", 'docu_pidjog');
define("DOCU_PIDJOG_ID", 'id');
define("DOCU_PIDJOG_MEMBER_ID", 'member_id'); // Növendék azonosítója
define("DOCU_PIDJOG_JOG_ID", 'jog_id');      // A képzés, amit el akar végezni
define("DOCU_PIDJOG_LICENCE", 'licence');    // Képzéssel megbízott oktató

/**
 * Description of DocuPIDAO
 *   Képzés személyi azonosító adatok
 * @author Peaceful
 */
class DocuPIDJOG extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_PIDJOG_TABLE);
        $this->setTableFields([
            DOCU_PIDJOG_ID,
            DOCU_PIDJOG_MEMBER_ID,
            DOCU_PIDJOG_JOG_ID,
            DOCU_PIDJOG_LICENCE,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_PIDJOG_TABLE . '` (`'
                . DOCU_PIDJOG_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_PIDJOG_MEMBER_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDJOG_JOG_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_PIDJOG_LICENCE . '` varchar(254) NOT NULL DEFAULT "", '
                . 'UNIQUE KEY `' . DOCU_PIDJOG_ID . '` (`' . DOCU_PIDJOG_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_PIDJOG_ID);
    }

    public function createPidJog($mem_id, $jog_id) {
        $pidjog_id = $this->insertElement([
            DOCU_PIDJOG_MEMBER_ID => $mem_id,
            DOCU_PIDJOG_JOG_ID => $jog_id,
        ]);
        $docuPD = new DocuTemplatePD();

        $docuPidjogDocus = new DocuPIDJOGDocus();
        $pds = $docuPD->getPDsOfJog($jog_id);
        foreach ($pds as $pd) {
            $docuPidjogDocus->createPidjogDocu($pidjog_id, $pd[DOCU_TEMP_PD_ID], $pd[DOCU_TEMP_PD_DOCU_ID], $pd[DOCU_TEMP_PD_SDN_ID], false, false);
        }
    }

    public function deletePIDJOG($id) {
        $this->deleteElementById($id);
        $pidjogDocus = new DocuPIDJOGDocus();
        foreach ($pidjogDocus->getPIDJOGDocusOfPidjog($id) as $docu) {
            $pidjogDocus->deletePIDJOGDocus($docu[DOCU_PIDJOG_DOCUS_ID]);
        }
    }

    public function updatePIDJOG($item_id, $licence) {
        $this->updateElementById($item_id, DOCU_PIDJOG_LICENCE, $licence);
    }

    public function getPidjogById($id) {
        return $this->getItemById($id);
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        return true;
    }

    public function isValid($id) {
        
    }

    public function getPIDJOGsOfMember($mem_id) {
        return $this->getElementsBy(DOCU_PIDJOG_MEMBER_ID, $mem_id);
    }

    public function isTrainer($pidjog_id) {
        $pidjog = $this->getItemById($pidjog_id);
        if (empty($pidjog)) {
            return false;
        }
        return (new DocuJOG())->isTrainer($pidjog[DOCU_PIDJOG_JOG_ID]);
    }

}
