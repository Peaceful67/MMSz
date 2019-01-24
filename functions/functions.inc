<?php

function retriveSession() {
    global $_SESSION;
    global $edit_member;
    if (isset($_SESSION[SESSION_EDIT_MEMBER])) {
        $edit_member = $_SESSION[SESSION_EDIT_MEMBER];
        unset($_SESSION[SESSION_EDIT_MEMBER]);
    }
}

function conv_array($in_data) {
    $ret = array();
    foreach ($in_data as $data) {
        $ret[] = iconv("utf-8", "iso-8859-2", $data);
    }
    return $ret;
}

function redirect($id) {
    global $admin_menu;
    $_SESSION[SESSION_MOD] = $admin_menu[$id][SETUP_MOD_ID];
    header("Location: " . PROGI);
}

function reloadEditMember() {
    global $_SESSION;
    global $edit_member;
    $_SESSION[SESSION_EDIT_MEMBER] = $edit_member;
    header("Content-type:text/html; charset=UTF-8;");
    echo '<meta http-equiv="refresh" content= "0;" />';
    exit(0);
}

function reloadPage() {
    global $mysqliLink;
    header("Content-type:text/html; charset=UTF-8;");
    echo '<meta http-equiv="refresh" content= "0;" />';
    $mysqliLink->close();
    exit(0);
}

function getLastMemberId() {
    global $mysqliLink;
    $sql = 'SELECT `' . MEMBER_ID . '` FROM `' . MEMBER_TABLE . '` ORDER BY `' . MEMBER_ID . '` DESC LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res->num_rows > 0) {
        return $res->fetch_assoc()[MEMBER_ID];
    } else {
        return -1;
    }
}

function isGdpr($id) {
    global $mysqliLink;
    $sql = 'SELECT `' . MEMBER_GDPR . '` FROM `' . MEMBER_TABLE . '` WHERE `' . MEMBER_ID . '`=' . $id . ' LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res->num_rows > 0) {
        $gdpr = $res->fetch_assoc()[MEMBER_GDPR];
        return $gdpr > 0 ? true : false;
    } else {
        return false;
    }
}

function setGdpr($id) {
    global $mysqliLink;
    $sql = 'UPDATE `' . MEMBER_TABLE . '` SET `' . MEMBER_GDPR . '` = CURRENT_TIMESTAMP WHERE `' . MEMBER_ID . '`=' . $id;
    $mysqliLink->query($sql);
}

function isValidMemberId($id) {
    $ret = true;
    if ($id <= 0) {
        $ret = false;
    }
    if ($id > getLastMemberId()) {
        $ret = false;
    }
    return $ret;
}

function isEditAllowed() {
    global $permission;
    return (($permission & (ACCESS_ADMIN | ACCESS_CLUBLEADER | ACCESS_MMSZ | ACCESS_MMSZ_PRESIDENT)) != 0);
}

function isPermitted($perm) {
    global $permission;
    if ($perm == ACCESS_PUBLIC)
        return true;
    if (($permission & $perm) != 0)
        return true;
    else
        return false;
}

function theEnd($output) {
    global $mysqliLink;
    header("Content-type:text/html; charset=UTF-8;");
    echo $output;
    $mysqliLink->close();
    exit(0);
}

function randomPassword() {
    $alphabet = "abcdefghijklmnopqrstuwxyzABCDEFGHIJKLMNOPQRSTUWXYZ0123456789";
    $len = mb_strlen($alphabet) - 1;
    $pass = '';
    for ($i = 0; $i < 8; $i++) {
        $n = rand(0, $len);
        $pass .= $alphabet[$n];
    }
    return $pass;
}

function isIdUnique($id) {
    global $mysqliLink;
    $res = $mysqliLink->query('SELECT * FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_NUMBER . '`="' . $id . '"');
    if ($res AND ( 0 < $res->num_rows)) {
        return false;
    }
    $res = $mysqliLink->query('SELECT * FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_NUMBER . '`="DR_' . $id . '"');
    if ($res AND ( 0 < $res->num_rows)) {
        return false;
    }
    $res = $mysqliLink->query('SELECT * FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_NUMBER . '`="SC_' . $id . '"');
    if ($res AND ( 0 < $res->num_rows)) {
        return false;
    }
    return true;
}

function retriveMember($id) {
    $res = searchById($id);
    if ($res) {
        return $res->fetch_assoc();
    } else {
        return array();
    }
}

function getMemberAddr($mem_id) {
    $mem = retriveMember($mem_id);
    if (empty($mem)) {
        return "";
    }
    return $mem[MEMBER_ADDR_POST] . ' ' . $mem[MEMBER_ADDR_CITY] . ', ' . $mem[MEMBER_ADDR_STREET];
}

function setPassword($id, $pass) {
    global $mysqliLink;
    $password = md5($pass . SECRET);
    unset($_COOKIE[COOKIE_MEMBER_PASS]);
    unset($_COOKIE[COOKIE_REMEMBER_ME]);
    global $output;
    $mod_string = 'UPDATE `members` SET `' . MEMBER_PASSWORD . '` = "' . $password . '" WHERE `' . MEMBER_ID . '`=' . $id;
    if ($mysqliLink->query($mod_string)) {
        header("Location:" . PROGI);
    } else {
        $output .= '<br><a  title="Nem sikerült a jelszó beállítani, vissza !" href="' . PROGI . '"><img width="48" weight="48"src="plus.jpg"></a>';
    }
}

function getPassword($id) {
    return retriveMember($id)[MEMBER_PASSWORD];
}

function getMemberName($id) {
    $m = retriveMember($id);
    return empty($m) ? '' : $m[MEMBER_VEZNEV] . ' ' . $m[MEMBER_KERNEV];
}

function getMemberEmail($id) {
    $m = retriveMember($id);
    return $m[MEMBER_EMAIL];
}

function warning($msg) {
    global $output;
    $output .= '<div class="warning">  ' . $msg . '</div><br><br>';
}

function searchById($id) {
    global $mysqliLink;
    return $mysqliLink->query("SELECT * FROM `" . MEMBER_TABLE . "` WHERE `id`=" . $id . " LIMIT 1");
}

function logger($id, $related, $category, $action) {
    global $mysqliLink;
    $sql = 'INSERT INTO `logs`(`member`, `related_id`, `log_category`, `activity`) '
            . 'VALUES ("' . $id . '","' . $related . '","' . $category . '","' . $action . '")';
    $mysqliLink->query($sql);
    $sql = 'DELETE FROM `logs` WHERE `time_stamp` < DATE_SUB(NOW(), INTERVAL 2 YEAR);';
    $mysqliLink->query($sql);
}

function createNewMember() {
    global $mysqliLink;
    $mysqliLink->query('INSERT INTO `' . MEMBER_TABLE . '` () VALUES ()');
    return $mysqliLink->insert_id;
}

function getClubName($cl_id) {
    global $mysqliLink;
    $ret = '';
    $res = $mysqliLink->query('SELECT `' . CLUB_NAME . '` FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ID . '`="' . $cl_id . '" LIMIT 1');
    if ($row = $res->fetch_assoc()) {
        $ret = $row[CLUB_NAME];
    }return $ret;
}

function getClub($cl_id) {
    global $mysqliLink;
    $res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ID . '`="' . $cl_id . '" LIMIT 1');
    if ($res AND $res->num_rows > 0) {
        return $res->fetch_assoc();
    } else {
        return array();
    }
}

function delegatesNumberToString($club_id) {
    $num = getNumberOfDelegateClub($club_id);
    $max = getMaxNumberOfDelegateClub($club_id);
    if ($num > $max) {
        $ret = '<span style="color:red;">(Akt/Max: ' . $num . '/' . $max . ')</span>';
    } else {
        $ret = '(Akt/Max: ' . $num . '/' . $max . ')';
    }
    return $ret;
}

function getClubNameList() {
    global $mysqliLink;
    $ret = array();
    $res = $mysqliLink->query('SELECT `' . CLUB_ID . '`, `' . CLUB_NAME . '` FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ACT . '`="1" ORDER BY ' . CLUB_NAME);
    while ($row = $res->fetch_assoc()) {
        $ret[$row[CLUB_ID]] = $row[CLUB_NAME];
    }
    return $ret;
}

function isClubLeader() {
    global $member_id;
    if (!isPermitted(ACCESS_CLUBLEADER)) {
        return 0;
    }
    return getRoleClubleader($member_id);
}

function getCategories($branch = -1) {
    global $mysqliLink;
    $list = array();
    $res = $mysqliLink->query('SELECT `' . CATEGORIES_SHORT . '`, `' . CATEGORIES_ID . '` FROM `' . CATEGORIES_TABLE . '` WHERE ' . ($branch < 0 ? '1' : '`' . CATEGORIES_BRANCH . '`="' . $branch . '"') . ' GROUP BY `' . CATEGORIES_SHORT . '`  ASC');
    while ($row = $res->fetch_assoc()) {
        $list[$row[CATEGORIES_ID]] = $row[CATEGORIES_SHORT];
    }
    return $list;
}

function getMemberById($id) {
    return retriveMember($id);
}

function getMemberCategories($id) {
    global $mysqliLink;
    $list = array();
    $res = $mysqliLink->query('SELECT `' . CM_CATEGORY . '` FROM `' . CM_TABLE . '` WHERE `' . CM_MEMBER_ID . '`=' . $id);
    while ($row = $res->fetch_assoc()) {
        $list[] = $row[CM_CATEGORY];
    }
    return $list;
}

function getCategoryById($cat_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . CATEGORIES_TABLE . '` WHERE `' . CATEGORIES_ID . '`=' . $cat_id;
    $res = $mysqliLink->query($sql);
    if ($res AND $row = $res->fetch_assoc()) {
        $ret = $row;
    }
    return $ret;
}

function getClubById($cl_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ID . '`=' . $cl_id;
    $res = $mysqliLink->query($sql);
    $row = $res->fetch_assoc();
    if ($row) {
        $ret = $row;
    }
    return $ret;
}

function getListMemberCategories($id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT  * '
            . 'FROM `' . CATEGORIES_TABLE . '`, `' . CM_TABLE . '` '
            . 'WHERE `' . CM_TABLE . '`.`' . CM_MEMBER_ID . '`="' . $id . '" '
            . 'AND `' . CATEGORIES_TABLE . '`.`' . CATEGORIES_ID . '` = `' . CM_TABLE . '`.`' . CM_CATEGORY . '`';
    $res = $mysqliLink->query($sql);
    if ($res->num_rows == 0) {
        return 'Nincs megadva!';
    }
    while ($row = $res->fetch_assoc()) {
        $cat_id = $row[CATEGORIES_ID];
        $file_en = RULES . '_' . $cat_id . '_EN.pdf';
        $file_hu = RULES . '_' . $cat_id . '_HU.pdf';
        if (is_file($file_hu)) {
            $ret .= '<a title = "Magyar nyelvű szabályzat" target = "_blank" href = "' . $file_hu . '">' . $row[CATEGORIES_SHORT] . '</a>, ';
        } elseif (is_file($file_en)) {
            $ret .= '<a title = "Angol nyelvű szabályzat" target = "_blank" href = "' . $file_en . '">' . $row[CATEGORIES_SHORT] . '</a>, ';
        } else {
            $ret .= '<a title = "Nincs feltöltve szabályzat" >' . $row[CATEGORIES_SHORT] . '</a>, ';
        }
    }
    return $ret;
}

function getListMemberOldCategories($id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT  `' . CATEGORIES_TABLE . '`.* '
            . 'FROM `' . CATEGORIES_TABLE . '`, `' . CM_TABLE . '` '
            . 'WHERE `' . CM_TABLE . '`.`' . CM_MEMBER_ID . '`="' . $id . '" '
            . 'AND `' . CATEGORIES_TABLE . '`.`' . CATEGORIES_ID . '` = `' . CM_TABLE . '`.`' . CM_CATEGORY . '`';
    $res = $mysqliLink->query($sql);
    while ($row = $res->fetch_assoc()) {
        $ret .= $row[CATEGORIES_SHORT] . ',';
    }
    return $ret;
}

function isCategoryInUse($cat) {
    global $mysqliLink;
    $res = $mysqliLink->query('SELECT `' . CM_CATEGORY . '` FROM `' . CM_TABLE . '` WHERE `' . CM_CATEGORY . '`=' . $cat);
    return ($res AND ( $res->num_rows > 0)) ? true : false;
}

function isCategoryNameUsed($short, $branch, $cat_id) {
    global $mysqliLink;
    if (!isset($cat_id)) {
        $cat_id = -1;
    }
    $sql = 'SELECT * FROM `' . CATEGORIES_TABLE . '` WHERE '
            . '`' . CATEGORIES_SHORT . '`="' . $short . '" AND '
            . ' NOT(`' . CATEGORIES_ID . '`="' . $cat_id . '") '
            . ' AND `' . CATEGORIES_BRANCH . '`="' . $branch . '"';
    $res = $mysqliLink->query($sql);
    return ($res AND ( $res->num_rows > 0)) ? true : false;
}

function printPermissions($id) {
    global $access;
    $p = getPermission($id);
    $ret = "Jogosultságok: ";
    foreach ($access as $k => $a) {
        if (($p & $k) != 0) {
            $ret .= $a . ', ';
        }
    }
    return $ret;
}

function isOnlyClubleader() {// Ha klubvezeto de nem magasabb jogosultsagu
    return isPermitted(ACCESS_CLUBLEADER) &&
            !isPermitted(ACCESS_ADMIN) && !isPermitted(ACCESS_MMSZ) && !isPermitted(ACCESS_MMSZ_PRESIDENT);
}

function memberAlreadyExists($vez, $ker, $email) {
    global $mysqliLink;
    $ret = -1;
    $sql = 'SELECT * FROM `' . MEMBER_TABLE . '` WHERE (`' . MEMBER_KERNEV . '`="' . $ker . '" '
            . 'AND `' . MEMBER_VEZNEV . '`="' . $vez . '" ) OR '
            . '(`' . MEMBER_EMAIL . '`="' . $email . '" AND `' . MEMBER_EMAIL . '`!="")';
    $res = $mysqliLink->query($sql);
    if ($res AND $row = $res->fetch_assoc()) {
        $ret = $row[MEMBER_ID];
    }
    return $ret;
}

function push_mod($new) {
    if (isset($_SESSION[SESSION_LAST_MOD])) {
        $hist = unserialize($_SESSION[SESSION_LAST_MOD]);
        if ($new != end($hist)) {
            $hist[] = $new;
        }
    } else {
        $hist = array();
    }
    $_SESSION[SESSION_LAST_MOD] = serialize($hist);
}

function reset_mod() {
    unset($_SESSION[SESSION_LAST_MOD]);
}

function get_back() {
    $ret = "";
    if (isset($_SESSION[SESSION_LAST_MOD])) {
        $hist = unserialize($_SESSION[SESSION_LAST_MOD]);
        if (count($hist) >= 2) {
            array_pop($hist);
            $ret = end($hist);
            $_SESSION[SESSION_LAST_MOD] = serialize($hist);
        }
    }
    return $ret;
}

function downloadCardOfMember($mem_id) {
    global $this_year;
    require_once (MPDF . "mpdf.php");
    $file = '.' . AVATAR . $mem_id . '.jpg';
    $mem = retriveMember($mem_id);
    if (!is_file($file)) {
        $file = "./" . ((retriveMember($mem_id)[MEMBER_SEX] == "F") ?
                PATH_IMAGES . 'female.jpg' :
                PATH_IMAGES . 'male.jpg');
    }

    $avatar = imagecreatetruecolor(CARD_AVATAR_WIDTH, CARD_AVATAR_HEIGHT);
    $source = imagecreatefromjpeg($file);
    $temp_file = tempnam(sys_get_temp_dir(), "_") . ".jpg";
    $check = getimagesize($file);
    list($width, $height) = $check;
    imagecopyresized($avatar, $source, 0, 0, 0, 0, CARD_AVATAR_WIDTH, CARD_AVATAR_HEIGHT, $width, $height);
    imagejpeg($avatar, $temp_file, 100);

    $stylesheet = file_get_contents('design/card.css');
    $body = '';
    $body .= '<div class="avatar">';
    $body .= '<img src="' . $temp_file . '"></div>';

    $body .= '<div class="mmsz_hun"><b>MAGYAR MODELLEZŐ SZÖVETSÉG</b></div>'
            . '<div class="mmsz_eng">HUNGARIAN ASSOCIATION OF MODELLERS</div>';

    $body .= '<div class="name">';
    $body .= getMemberName($mem_id) . '&nbsp;&nbsp;-&nbsp;&nbsp;' . $mem_id;
    $body .= '</div><div class="data">';
    $body .= getMemberClubNames($mem_id);
    if ($mem[MEMBER_LICENCE] != '') {
        $body .= '&nbsp;&nbsp;&nbsp;FAI:&nbsp;' . $mem[MEMBER_LICENCE];
    }
    $body .= '</div><div class="year">' . $this_year . '</div>';

    $pdf = new mPDF($mode = 'utf-8', $format = 'A4', $default_font_size = 0, $default_font = '', $mgl = 1, $mgr = 1, $mgt = 1, $mgb = 1, $mgh = 1, $mgf = 1, $orientation = 'L');
    $pdf->img_dpi = $pdf->dpi = CARD_IMG_DPI;
    $pdf->nonPrintMargin = '0';
    $pdf->WriteHTML($stylesheet, 1);
    $pdf->WriteHTML($body, 2);
    $pdf->Output('card_' . $mem_id . '_' . date("Y-m-d_H-i-s") . '.pdf', 'D');
    unlink($temp_file);
}

function notificationFAI($fm_id) {
    global $this_year;
    $fm = getFmById($fm_id);
    $mem_id = $fm[FM_MEMEBER_ID];
    $mem = getMemberById($mem_id);
    $fee = getFeeById($fm[FM_FEE_ID]);
    if ($fee[FEE_TYPE] == FEE_TYPE_ORGANIZATION AND
            $fee[FEE_NAME] == FEE_ORG_NAME_FAI) { // Valaki egy FAI díjat fizetett be éppen
        $body = 'A ' . $mem_id . ' azonosítójú tag befizette FAI díját a '
                . $fm[FM_FROM] . '-' . $fm[FM_TO] . ' időszakra.<br>'
                . 'A tag részletes adatai a mellékletben';
        $filename = TMP_DIR . 'FAI_' . $mem_id . '.xlsx';
        include_once CLASSES . 'PHPExcel.php';
        include_once CLASSES . 'PHPExcel/Writer/Excel2007.php';
        $objPHPExcel = new PHPExcel();
        $objPHPExcel->getProperties()->setCreator(COMPANY_NAME . ' Tagnyilvántartó');


        $objPHPExcel->setActiveSheetIndex(0);
        $objPHPExcel->getActiveSheet()->setTitle($mem[MEMBER_KERNEV] . ' ' . $mem[MEMBER_VEZNEV] . ' ' . $this_year);


        $objPHPExcel->getActiveSheet()->setCellValue('A1', 'FAI ID');
        $objPHPExcel->getActiveSheet()->setCellValue('A2', $mem[MEMBER_LICENCE]);

        $objPHPExcel->getActiveSheet()->setCellValue('B1', 'Licence Number');
        $objPHPExcel->getActiveSheet()->setCellValue('B2', 'HUN-' . $mem_id);

        $objPHPExcel->getActiveSheet()->setCellValue('C1', 'Validity From');
        $objPHPExcel->getActiveSheet()->setCellValue('C2', date('d.m.Y'));

        $objPHPExcel->getActiveSheet()->setCellValue('D1', 'Validity To');
        $objPHPExcel->getActiveSheet()->setCellValue('D2', '31.12.' . date('Y', time()));

        $objPHPExcel->getActiveSheet()->setCellValue('E1', 'Air Sport Discipline');
        $objPHPExcel->getActiveSheet()->setCellValue('E2', 'Aeromodelling and Spacemodelling');

        $objPHPExcel->getActiveSheet()->setCellValue('F1', 'First Name');
        $objPHPExcel->getActiveSheet()->setCellValue('F2', $mem[MEMBER_KERNEV]);

        $objPHPExcel->getActiveSheet()->setCellValue('G1', 'Last Name');
        $objPHPExcel->getActiveSheet()->setCellValue('G2', $mem[MEMBER_VEZNEV]);

        $objPHPExcel->getActiveSheet()->setCellValue('H1', 'Gender');
        $objPHPExcel->getActiveSheet()->setCellValue('H2', $mem[MEMBER_SEX]);

        $objPHPExcel->getActiveSheet()->setCellValue('I1', 'Date of Birth');
        $objPHPExcel->getActiveSheet()->setCellValue('I2', date('d.m.Y', strtotime($mem[MEMBER_BORN])));

        $objPHPExcel->getActiveSheet()->setCellValue('J1', 'Nationality');
        $objPHPExcel->getActiveSheet()->setCellValue('J2', 'HUN');

        $objPHPExcel->getActiveSheet()->setCellValue('K1', 'Country of Residence');
        $objPHPExcel->getActiveSheet()->setCellValue('K2', 'HUN');

        $objPHPExcel->getActiveSheet()->setCellValue('L1', 'Address1');
        $objPHPExcel->getActiveSheet()->setCellValue('L2', $mem[MEMBER_ADDR_STREET]);

        $objPHPExcel->getActiveSheet()->setCellValue('M1', 'Address2');
        $objPHPExcel->getActiveSheet()->setCellValue('M2', $mem[MEMBER_ADDR_POST]);

        $objPHPExcel->getActiveSheet()->setCellValue('N1', 'Address3');
        $objPHPExcel->getActiveSheet()->setCellValue('N2', $mem[MEMBER_ADDR_CITY]);

        $objPHPExcel->getActiveSheet()->setCellValue('O1', 'Country of addresse');
        $objPHPExcel->getActiveSheet()->setCellValue('O2', 'Hungary');

        $objPHPExcel->getActiveSheet()->setCellValue('P1', 'E-mail address');
        $objPHPExcel->getActiveSheet()->setCellValue('P2', $mem[MEMBER_EMAIL]);

        $objPHPExcel->getActiveSheet()->setCellValue('Q1', 'Phone (home)');
        $objPHPExcel->getActiveSheet()->setCellValue('Q2', '');

        $objPHPExcel->getActiveSheet()->setCellValue('R1', 'Phone (office)');
        $objPHPExcel->getActiveSheet()->setCellValue('R2', '');

        $objPHPExcel->getActiveSheet()->setCellValue('S1', 'Phone (mobile)');
        $objPHPExcel->getActiveSheet()->setCellValue('S2', '');

        $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
//        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');

        $objWriter->save($filename);
        error_log('FAI Notif filename:' . $filename);
        if (DEVELOPMENT) {
            send_mail_to("postafiok@emg.hu", "FAI befizetés érkezett (" . $mem_id . ")", $body, $filename);
        } else {
            send_mail_to(FAI_NOTIF_EMAIL, "FAI befizetés érkezett (" . $mem_id . ")", $body, $filename);
        }
    }
}

function getNewId($type) {
    global $mysqliLink;
    $sql = 'SELECT MAX(`' . TYPES_TYPE_ID . '`) as `max` FROM `' . TYPES_TABLE . '` WHERE `' . TYPES_TYPE . '`=' . $type . ';';
    $res = $mysqliLink->query($sql);
    $max = 0;
    if ($res && $res->num_rows > 0) {
        $max = 1 + $res->fetch_assoc()['max'];
        $res->close();
    }
    return $max;
}

function fill_type($default, $type) {
    global $mysqliLink;
    foreach ($default as $key => $value) {
        $sql = 'INSERT INTO `' . TYPES_TABLE . '` '
                . '(`' . TYPES_TYPE . '`, `' . TYPES_TYPE_NAME . '`, `' . TYPES_TYPE_ID . '`) '
                . ' VALUES (' . $type . ', "' . $value . '", ' . $key . ');';
        $mysqliLink->query($sql);
    }
    return $default;
}

function addDatePeriod($begin, $period) {
    switch ($period) {
        case FEE_PERIOD_DAY:
            $diff = ' +1 day';
            break;
        case FEE_PERIOD_WEEK:
            $diff = ' +7 day';
            break;
        case FEE_PERIOD_MONTH:
            $diff = ' +1 Month';
            break;
        case FEE_PERIOD_QUATER:
            $diff = ' +3 Months';
            break;
        case FEE_PERIOD_HALF:
            $diff = ' +6 Months';
            break;
        case FEE_PERIOD_YEAR:
            $diff = ' +1 Year';
            break;
        case FEE_PERIOD_2YEARS:
            $diff = ' +2 Years';
            break;
        case FEE_PERIOD_3YEARS:
            $diff = ' +3 Years';
            break;
        default:
            $diff = '';
            break;
    }
    $final = date("Y-m-d", strtotime($begin . $diff));
    return $final;
}

function get_name($type, $name) {
    global $member_types, $member_insurances, $organizations;
    global $club_categories, $associations, $associations, $administrations, $licences;
    $ret = '---';
    switch ($type) {
        case FEE_TYPE_MEMBERSHIP:
            $list = $member_types;
            break;
        case FEE_TYPE_INSURANCE:
            $list = $member_insurances;
            break;
        case FEE_TYPE_ORGANIZATION:
            $list = $organizations;
            break;
        case FEE_TYPE_CLUB:
            $list = $club_categories;
            break;
        case FEE_TYPE_ASSOCIATION:
            $list = $associations;
            break;
        case FEE_TYPE_ADMINISTRATION:
            $list = $administrations;
            break;
        case FEE_TYPE_LICENCES:
            $list = $licences;
            break;
        default:
            return $ret;
    }
    if (isset($list[$name])) {
        $ret = $list[$name];
    }
    return $ret;
}

function getMemberIdByEmail($email_addr) {
    global $mysqliLink;
    $sql = 'SELECT `' . MEMBER_ID . '` FROM `' . MEMBER_TABLE . '` WHERE `' . MEMBER_EMAIL . '`="' . trim($email_addr) . '" LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res AND $row = $res->fetch_assoc()) {
        return $row[MEMBER_ID];
    } else {
        return 0;
    }
}

?>