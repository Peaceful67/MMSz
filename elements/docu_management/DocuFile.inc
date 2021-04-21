<?php

define("DOCU_FILE_TABLE", 'docu_files');
define("DOCU_FILE_ID", 'id');
define("DOCU_FILE_OWNER", 'owner');
define("DOCU_FILE_DOCU_ID", 'docu_id');
define("DOCU_FILE_ORIGIN", 'origin');
define("DOCU_FILE_REAL_PATH", 'real_path');
define("DOCU_FILE_UPLOADED", 'uploaded');
define("DOCU_FILE_VERSION", 'version');
define("DOCU_FILE_DESCRIPTION", 'description');
define("DOCU_FILE_SDN_ID", 'sdn_id');

define("DOCU_FILE_TYPE_PDF", 'pdf');
define("DOCU_FILE_TYPE_DOCUMENTS", 'documents');

class DocuFile extends BasicElement {

    const DOCU_FILE_OWN_DOCU_ID = 0;

    private $relativepath;
    private $real_path = NULL;
    private $description = NULL;
    private $docu_enabled_types = [
        DOCU_FILE_TYPE_PDF => [
            "application/pdf",
        ],
        DOCU_FILE_TYPE_DOCUMENTS => [
            "application/pdf",
            "image/jpeg",
            "image/pjpeg",
            "image/png",
            "application/msword",
            "application/excel",
        ],
    ];

    function __construct() {
        parent::__construct();
        $this->relativepath = getcwd() . LOCALFILE_PATH . '/';
        $this->setTableName(DOCU_FILE_TABLE);
        $this->setTableFields([
            DOCU_FILE_ID,
            DOCU_FILE_DOCU_ID,
            DOCU_FILE_OWNER,
            DOCU_FILE_ORIGIN,
            DOCU_FILE_REAL_PATH,
            DOCU_FILE_DESCRIPTION,
            DOCU_FILE_SDN_ID,
            DOCU_FILE_UPLOADED,
            DOCU_FILE_VERSION,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_FILE_TABLE . '` (`'
                . DOCU_FILE_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . DOCU_FILE_DOCU_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_FILE_OWNER . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_FILE_ORIGIN . '` varchar(255) NULL DEFAULT NULL, `'
                . DOCU_FILE_REAL_PATH . '` varchar(255) NULL DEFAULT NULL, `'
                . DOCU_FILE_DESCRIPTION . '` varchar(255) NULL DEFAULT NULL, `'
                . DOCU_FILE_SDN_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . DOCU_FILE_UPLOADED . '` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ,`'
                . DOCU_FILE_VERSION . '` int(8) NOT NULL DEFAULT 0 ,'
                . 'UNIQUE KEY `' . DOCU_FILE_ID . '` (`' . DOCU_FILE_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_FILE_ID);
    }

    function createUniquePath() {
        global $member_id;
        $date_dir = date('Y/m/d/');
        $full_dir = $this->relativepath . $date_dir;
        if (!is_dir($full_dir)) {
            mkdir($full_dir, 0777, true);
        }
        return $this->real_path = $date_dir . uniqid($member_id . '_', true);
    }

    function setItemId($id) {
        $this->itemId = $id;
        return $this;
    }

    function setDescription($desc) {
        $this->description = $desc;
        $this->updateElement(DOCU_FILE_DESCRIPTION, $desc);
        return $this;
    }

    function setSDN($sdn_id) {
        $this->updateElement(DOCU_FILE_SDN_ID, $sdn_id);
        return $this;
    }

    function setSdnId($sdn_id) {
        $this->updateElement(DOCU_FILE_SDN_ID, $sdn_id);
        return $this;
    }

    function getDescription($id) {
        $item = $this->getFileById($id);
        if (empty($item)) {
            return '';
        } else {
            if ($item[DOCU_FILE_SDN_ID] > 0) {
                return (new DocuStandard())->getSDNById($item[DOCU_FILE_SDN_ID])[DOCU_SDN_NAME];
            } else {
                return $item[DOCU_FILE_DESCRIPTION];
            }
        }
    }

    function getNameByFileId($id) {
        $file = $this->getFileById($id);
        if (!empty($file[DOCU_FILE_DESCRIPTION])) {
            return $file[DOCU_FILE_DESCRIPTION];
        } elseif ($file[DOCU_FILE_SDN_ID] > 0) {
            return (new DocuStandard())->getName($file[DOCU_FILE_SDN_ID]);
        } else {
            return $file[DOCU_FILE_ORIGIN];
        }
    }

    function getUploadedById($id) {
        return $this->getFileById($id)[DOCU_FILE_UPLOADED];
    }

    function getDocuIdsOfOwner($mem_id) {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT * FROM `' . $this->tableName . '` WHERE `' . DOCU_FILE_OWNER . '`=' . $mem_id . ' ORDER BY `' . DOCU_FILE_UPLOADED . '` DESC';
        $res = $mysqliLink->query($sql);
        $ret = array();
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[DOCU_FILE_DOCU_ID]][] = $row[DOCU_FILE_ID];
        }
        return $ret;
    }

    function getFileById($id) {
        return $this->getItemById($id);
    }

    function getFileByDocuId($docu_id) {
        $files = $this->getFilesByDocuId($docu_id);
        return end($files);
    }

    function getSdnIdByDocuId($docu_id) {
        $file = $this->getFileByDocuId($docu_id);
        return $file[DOCU_FILE_SDN_ID];
    }

    function getFileIdByDocuId($docu_id) {
        $file = $this->getFileByDocuId($docu_id);
        if ($file) {
            return $file[DOCU_FILE_ID];
        } else {
            return -1;
        }
    }

    function getOwners() {
        global $mysqliLink;
        $ret = array();
        $sql = 'SELECT `' . DOCU_FILE_OWNER . '` FROM `' . $this->tableName . '`  GROUP BY `' . DOCU_FILE_OWNER . '`;';
        $res = $mysqliLink->query($sql);
        $ret = array();
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[$row[DOCU_FILE_OWNER]] = getMemberName($row[DOCU_FILE_OWNER]);
        }
        return $ret;
    }

    function setSdnDescOfFile($file_id, $sdn_id, $desc) {
        $this->updateElementById($file_id, DOCU_FILE_SDN_ID, $sdn_id);
        $this->updateElementById($file_id, DOCU_FILE_DESCRIPTION, $desc);
    }

    function deleteFile($id) {
        global $member_id;
        $file = $this->getItemById($id);
        if (unlink($this->relativepath . $file[DOCU_FILE_REAL_PATH])) {
            $this->deleteElementById($id);
            logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, "A " . $id . " azonosítójú dokumentum törlése : " . $file[DOCU_FILE_ORIGIN]);
            return true;
        }
        return false;
    }

    function downloadFile($id) {
        global $member_id;
        if ($this->itemId != $id) {
            $this->getFileById($id);
        }logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, "A " . $id . " azonosítójú dokumentum letöltése : " . $this->tableRow[DOCU_FILE_ORIGIN]);
        downloadFile($this->relativepath . $this->tableRow[DOCU_FILE_REAL_PATH], $this->tableRow[DOCU_FILE_ORIGIN]);
        warning('Nem sikerült a fájlt letölteni');
    }

    function viewFile($id) {
        global $member_id;
        if ($this->itemId != $id) {
            $this->getFileById($id);
        }
        logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, "A " . $id . " azonosítójú dokumentum megjelenítése : " . $this->tableRow[DOCU_FILE_ORIGIN]);
        viewFile($this->relativepath . $this->tableRow[DOCU_FILE_REAL_PATH], $this->tableRow[DOCU_FILE_ORIGIN]);
        warning('Nem sikerült a fájlt megjeleníteni');
    }

    function uploadDocument($docu_file, $type, $docu_id) {
        global $member_id;
        $file_id = -1;
        if (isset($_FILES[$docu_file])) {
            $enabled = false;
            foreach ($this->docu_enabled_types[$type] as $enabled_type) {
                if ($_FILES[$docu_file]["type"] == $enabled_type) {
                    $enabled = true;
                    break;
                }
            }
            if ($enabled) {
                $file = $this->relativepath . $this->createUniquePath();
                $tmp_name = $_FILES[$docu_file]["tmp_name"];
                if (move_uploaded_file($tmp_name, $file)) {
                    $file_id = $this->createDocument($_FILES[$docu_file]['name'], $docu_id);
                    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, "A " . $file_id . " azonosítójú új dokumentum feltöltése : " . $file);
                } else {
                    warning('A feltöltés sikertelen');
                }
            } else {
                warning("A feltöltött fájl nem engedélyezett formátumú !");
            }
        }
        return $file_id;
    }

    function getFilesByDocuId($docu_id) {
        return $this->getElementsBy(DOCU_FILE_DOCU_ID, $docu_id);
    }

    function getNameByDocuID($docu_id) {
        $file_id = $this->getFileIdByDocuId($docu_id);
        if ($file_id < 0 OR $docu_id < 0) {
            return 'Nincs feltöltve';
        }
        return $this->getNameByFileId($file_id);
    }

    function createDocument($origin, $docu_id) {
        global $member_id, $mysqliLink;
        $version = 0;
        if (is_null($this->real_path)) {
            $this->createUniquePath();
        }
        $sql = 'SELECT `' . DOCU_FILE_VERSION . '` FROM `' . DOCU_FILE_TABLE . '` WHERE `' . DOCU_FILE_DOCU_ID . '`=' . $docu_id . ' ORDER BY `' . DOCU_FILE_VERSION . '` DESC LIMIT 1;';
        $res = $mysqliLink->query($sql);
        if ($res AND $row = $res->fetch_assoc()) {
            $version = 1 + intval($row[DOCU_FILE_VERSION]);
        }
        return $this->insertElement([
                    DOCU_FILE_ORIGIN => $origin,
                    DOCU_FILE_DOCU_ID => $docu_id,
                    DOCU_FILE_OWNER => $member_id,
                    DOCU_FILE_REAL_PATH => $this->real_path,
                    DOCU_FILE_DESCRIPTION => $this->description,
                    DOCU_FILE_VERSION => $version,
        ]);
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) { // akkor lehet törölni, ha DOCU_ID-jére nem hívatkozik senki
        $file = $this->getFileById($id);
        if (empty($file) OR ! isset($file[DOCU_FILE_DOCU_ID]) OR $file[DOCU_FILE_DOCU_ID] <= 0) {
            return true;
        }
        $this->itemId = $file[DOCU_FILE_DOCU_ID];
        // error_log("IsDeletebale Docu_id: ".$this->itemId.' File'.print_r($file, true));

        return $this->isElementDeletable([
                    DOCU_PIDAO_DOCUS_TABLE => DOCU_PIDAO_DOCUS_DOCU_ID,
                    DOCU_TEMP_KK_TABLE => DOCU_TEMP_KK_DOCU_ID,
                    DOCU_TEMP_PD_TABLE => DOCU_TEMP_PD_DOCU_ID,
        ]);
    }

    public function getNextDocuId() {
        $next = 1 + getOptionValue(OPTIONS_NAME_DOCU_LAST_ID);
        updateOption(OPTIONS_NAME_DOCU_LAST_ID, $next);
        return $next;
    }

}
