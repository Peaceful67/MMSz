<?php

define('FAI_LICENCE_SEARCH', 'licences');
define('FAI_LICENCE_GET', 'licence');
define('FAI_LICENCE_CREATE', 'create');
define('FAI_LICENCE_UPDATE', 'create');


define("FAI_ANS_LICENCE", 'idlicence');
//define("FAI_ANS_LICENCEE", 'idlicencee_lic');
define("FAI_ANS_LICENCEE", 'idlicencee');
define("FAI_ANS_MMSZ", 'licencenumber_lic');
define("FAI_ANS_VALID", 'validuntil_lic');
define("FAI_ANS_BIRTH", 'birthdate_lip');
define("FAI_ANS_FIRSTNAME", 'givenname_lip');
define("FAI_ANS_SURNAME", 'surname_lip');
define("FAI_ANS_ISSUE", 'dateissued_lic');
define("FAI_ANS_GENDER", 'gender_lip');
define("FAI_ANS_SUCCESS", "success");


define("FAI_SEND_LICENCE", 'idlicence');
define("FAI_SEND_LICENCEE", 'idlicencee_lic');
define("FAI_SEND_MMSZ", 'licencenumber_lic');
define("FAI_SEND_VALID", 'validuntil_lic');
define("FAI_SEND_BIRTH", 'birthdate_lip');
define("FAI_SEND_FIRSTNAME", 'givenname_lip');
define("FAI_SEND_SURNAME", 'surname_lip');
define("FAI_SEND_ISSUE", 'dateissued_lic');
define("FAI_SEND_GENDER", 'gender_lip');

function isFaiMember($mem_id) {
    return !empty(popFaiRow($mem_id));
}

function updateFaiAvatar($mem_id) {
    $body = 'A ' . mem2FaiMem($mem_id) . ' azonosítójú tag a mellékelt képre módosította az arcképét.<br>'
            . 'A tag szerepel FAI adatbázisában, de a képet egyelőre automatikusan nem lehet frissíteni.<br>'
            . 'Kérjük, töltsd fel a képet manuálisan a FAI rendszerébe !';
    $subject = mem2FaiMem($mem_id) . ' azonosítójú tag frissítése';
    $filename = '.' . AVATAR_SIZED . $mem_id . '.jpg';
    if (!is_file($filename)) {
        $filename = '.' . AVATAR . $mem_id . '.jpg';
    }
    if (!is_file($filename)) {
        $filename = '';
    }
    if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
        $addr = getOptionValue(OPTIONS_NAME_ADMIN_EMAIL);
    } else {
        $addr = getOptionValue(OPTIONS_NAME_FAI_NOTIF_EMAIL);
    }
    send_mail_to($addr, $subject, $body, $filename);
}

function getAllFaiMembersId() { // Ez csak az átmenethez kell
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . FAI_HUNID . '` FROM `' . FAI_TABLE . '`  GROUP BY `' . FAI_HUNID . '`;';
    $res = $mysqliLink->query($sql);
    if (!$res OR $res->num_rows == 0) {
        $sql = 'SELECT `' . MEMBER_ID . '` FROM `' . MEMBER_TABLE . '` WHERE `' . MEMBER_LICENCE . '`!=""';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row[MEMBER_ID];
        }
    } else {
        while ($res AND $row = $res->fetch_assoc()) {
            $ret[] = $row[FAI_HUNID];
        }
    }
    return $ret;
}

function countFaiMembersOfClub($club_id, $date) {
    global $mysqliLink;
    if (!isset($date)) {
        global $today;
        $date = $today;
    }
    $fee = getFee(FEE_TYPE_ORGANIZATION, FEE_ORG_NAME_FAI, $date);
    $fee_id = $fee[FEE_ID];
    $sql = 'SELECT *  FROM `' . FM_TABLE . '` INNER JOIN `' . ROLE_TABLE . '` AS `club_mem` ON ('
            . '`club_mem`.`' . ROLE_CLUB . '`=' . $club_id . ' '
            . 'AND `' . FM_TABLE . '`.`' . FM_MEMEBER_ID . '`=`club_mem`.`member_id`)'
            . 'WHERE `' . FM_TABLE . '`.`' . FM_FEE_ID . '`=' . $fee_id . '  AND '
            . '("' . $date . '" BETWEEN `' . FM_TABLE . '`.`' . FM_FROM . '` AND `' . FM_TABLE . '`.`' . FM_TO . '`)'
            . 'GROUP BY `' . FM_TABLE . '`.`' . FM_MEMEBER_ID . '`;';
    $ret = 0;
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = $res->num_rows;
    }
    return $ret;
}

function createFaiMember($mem_id) {
    global $member_id, $endOfYear;
    if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) != 1) {
        warning('FAI API nincs engedélyezve');
        return;
    }
    $m = getMemberById($mem_id);
    $payment = getFaiPaymentDate($mem_id);
    $str = getOptionValue(OPTIONS_NAME_FAI_URL) . FAI_LICENCE_CREATE . '?'
            . 'auth_username=' . getOptionValue(OPTIONS_NAME_FAI_AUTH_USER)
            . '&auth_password=' . base64_encode(getOptionValue(OPTIONS_NAME_FAI_AUTH_PASSWD))
            . '&licencee_firstname=' . urlencode($m[MEMBER_KERNEV])
            . '&licencee_middlename='
            . '&licencee_lastname=' . urlencode($m[MEMBER_VEZNEV])
            . '&licencee_gender=' . $m[MEMBER_SEX]
            . '&licencee_birthdate=' . $m[MEMBER_BORN]
            . '&licencee_nationality=HUN'
            . '&licencee_residencecountry=HUN'
            . '&address1=' . urlencode($m[MEMBER_ADDR_STREET])
            . '&address2=' . urlencode($m[MEMBER_ADDR_POST])
            . '&address3=' . urlencode($m[MEMBER_ADDR_CITY])
            . '&address_country=HUN'
            . '&licencee_email=' . urlencode($m[MEMBER_EMAIL])
            . '&phone_home=' . urlencode($m[MEMBER_TEL])
            . '&licence_number=' . mem2FaiMem($mem_id)
            . '&dateissued=' . ((empty($payment)) ? "" : $payment)
            . '&validuntil=' . ((empty($payment)) ? "" : $endOfYear)
            . '&discipline=5';
    $json_array = queryFai($str);
    if (!empty($json_array) AND $json_array[FAI_ANS_SUCCESS] == "1") {
        $idlic = $json_array[FAI_ANS_LICENCE];
        $fai_mem = getFaiMemberByIdlicence($idlic);
        $fai_licence = $fai_mem[FAI_ANS_LICENCEE];
        pushFaiRow($json_array[FAI_ANS_LICENCE], $mem_id, $fai_licence);
        logger($member_id, $mem_id, LOGGER_FAI_API, 'Tag adatainak létrehozása a FAI rendszerében');
    } else {
        warning("Nem sikerült létrehozni");
    }
}

function updateFaiMember($mem_id) {
    global $member_id, $endOfYear;
    if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) != 1) {
        warning('FAI API nincs engedélyezve');
        return;
    }
    $m = getMemberById($mem_id);
    $fai_row = popFaiRow($mem_id);
    if (empty($fai_row)) {  // Még nem FAI tag, létrehozzuk
        return createFaiMember($mem_id);
    }
    $payment = getFaiPaymentDate($mem_id);
    $str = getOptionValue(OPTIONS_NAME_FAI_URL) . FAI_LICENCE_CREATE . '?'
            . 'auth_username=' . getOptionValue(OPTIONS_NAME_FAI_AUTH_USER) . ''
            . '&auth_password=' . base64_encode(getOptionValue(OPTIONS_NAME_FAI_AUTH_PASSWD))
            . '&idlicence=' . $fai_row[FAI_IDLICENCE]
            . '&idlicencee=' . $fai_row[FAI_LICENCEE]
            . '&licencee_firstname=' . urlencode($m[MEMBER_KERNEV])
            . '&licencee_middlename='
            . '&licencee_lastname=' . urlencode($m[MEMBER_VEZNEV])
            . '&licencee_gender=' . $m[MEMBER_SEX]
            . '&licencee_birthdate=' . $m[MEMBER_BORN]
            . '&licencee_nationality=HUN'
            . '&licencee_residencecountry=HUN'
            . '&address1=' . urlencode($m[MEMBER_ADDR_STREET])
            . '&address2=' . urlencode($m[MEMBER_ADDR_POST])
            . '&address3=' . urlencode($m[MEMBER_ADDR_CITY])
            . '&licencee_email=' . urlencode($m[MEMBER_EMAIL])
            . '&phone_home=' . urlencode($m[MEMBER_TEL])
            . '&licence_number=' . mem2FaiMem($mem_id)
            . '&dateissued=' . ((empty($payment)) ? "" : $payment)
            . '&validuntil=' . ((empty($payment)) ? "" : $endOfYear)
            . '&discipline=5';
    $json_array = queryFai($str);
    if (!empty($json_array) AND $json_array[FAI_ANS_SUCCESS] != "1") {
        warning("Nem sikerült a frissítés");
    } else {
        logger($member_id, $mem_id, LOGGER_FAI_API, 'Tag adatainak frissítése a FAI rendszerébe');
    }
}

function getFaiMemberByIdlicence($idlicence) {
    global $member_id;
    if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) != 1) {
        warning('FAI API nincs engedélyezve');
        return;
    }
    $str = getOptionValue(OPTIONS_NAME_FAI_URL) . FAI_LICENCE_GET
            . '/' . $idlicence . '?'
            . 'auth_username="' . getOptionValue(OPTIONS_NAME_FAI_AUTH_USER) . '"'
            . '&auth_password="' . base64_encode(getOptionValue(OPTIONS_NAME_FAI_AUTH_PASSWD)) . '"';
    $json_array = queryFai($str);
    if (is_null($json_array)) {
        warning('Nem sikerült a FAI rendszerrel való kommunikáció');
        error_log("FAI json error: " . json_last_error_msg());
        return array();
    }
    logger($member_id, -1, LOGGER_FAI_API, $idlicence . ' FAI azonosító lekérdezése');
    return $json_array;
}

function pushFaiRow($idlic, $mem_id, $fai_licence) {
    global $mysqliLink;
    if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) != 1) {
        warning('FAI API nincs engedélyezve');
        return;
    }

    $sql = 'INSERT INTO `' . FAI_TABLE . '` '
            . '(`' . FAI_IDLICENCE . '`, `' . FAI_HUNID . '`, `' . FAI_LICENCEE . '`)'
            . 'VALUES ("' . $idlic . '", "' . $mem_id . '", "' . $fai_licence . '");';
    $mysqliLink->query($sql);
}

function popFaiRow($mem_id) {
    global $mysqliLink;
    if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) != 1) {
        warning('FAI API nincs engedélyezve');
        return;
    }
    $sql = 'SELECT * FROM `' . FAI_TABLE . '` WHERE `' . FAI_HUNID . '`=' . $mem_id . ' ORDER BY `' . FAI_IDLICENCE . '` DESC LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res AND $row = $res->fetch_assoc()) {
        return $row;
    }
    return array();
}

function extractArray($arr) {
    global $output;
    $ret = '';
    if (!empty($arr)) {
        foreach ($arr as $key => $value) {
            if (is_array($value)) {
                $ret .= '<hr>';
                $ret .= extractArray($value);
            } else {
                $ret .= $key . ' => ' . $value . '<br>';
            }
        }
        $output .= $ret;
    }
}

function queryFai($str) {
    $answer = file_get_contents($str);
    $json_array = json_decode($answer, true);
    if (is_null($json_array)) {
        warning('Nem sikerült a FAI rendszerrel való kommunikáció');
        error_log("FAI json error: " . json_last_error_msg());
        return array();
    }
    return $json_array;
}

function mem2FaiMem($mem_id) {
    return sprintf('HUN-%04d', $mem_id);
}

function isReadyForFAI($fm2bill) {
    global $today;
    $fm_id = $fm2bill[0];
    $type = $fm2bill[1];
    $name = $fm2bill[2];    
    if ($type == FEE_TYPE_ORGANIZATION AND $name == FEE_ORG_NAME_FAI) {
        $ret = '';
        $fm = getFmById($fm_id);
        $mem = getMemberById($fm[FM_MEMEBER_ID]);
        if(!filter_var($mem[MEMBER_EMAIL], FILTER_VALIDATE_EMAIL)) {
            $ret .= 'Helytelen email cím, ';
        }
        if(!is_file('.' . AVATAR . $mem[MEMBER_ID] . '.jpg')) {
            $ret .= ' Nincs feltöltött fénykép,';
        }
        /*
        if(!isPayedMembersip($mem[MEMBER_ID], $today)) {
            $ret .= ' Tagsági nincs kifizetve';
        }
         */
        return $ret;
    } else {
        return '';
    }
}
