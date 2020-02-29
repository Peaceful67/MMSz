<?php

if (!isset($already)) {
    $already = -1;
}
if (isset($create_bill)) {
    redirect("start_payment");
}
if (!isset($veznev)) {
    $veznev = '';
}
if (!isset($kernev)) {
    $kernev = '';
}
if (!isset($email)) {
    $email = '';
}
if (!isset($born)) {
    $born = '0000-00-00';
}

if (isset($base_data_save)) {
    $error = false;
    if (strlen($kernev) < 3) {
        warning("Túl rövid keresztnév");
        $error = true;
    }
    if (strlen($veznev) < 3) {
        warning("Túl rövid vezetéknév");
        $error = true;
    }
    if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        warning("Helytelen email cím !");
        $error = true;
    }
    if (!validateDate($born)) {
        warning("Helytelen születési dátum !");
        $error = true;
    }
    if (!isset($already_override) OR $already_override != 'on') {
        $already_members = similarMembers($veznev, $kernev, $email, $born);
        $already_max = ($base_data_id > 0) ? 1 : 0; // Uj vagy modositas
        if (count($already_members) > $already_max) {
            $already = 1;
            $ids = '';
            foreach ($already_members as $already_id => $already_mem) {
                error_log('Already: ' . $already_id);
                $ids .= $already_id . '  ';
            }
            warning('A rendszer szerint ez a tag a ' . $ids . ' azonsítójú tagként már létezik');
            $error = true;
        }
    }

    if (!$error) {
        if ($base_data_id <= 0) {
            $edit_member = $base_data_id = createNewMember();
            logger($member_id, $base_data_id, LOGGER_MEMBER_MOD, ' azonosítójú tag létrehozása');
            if (isOnlyClubleader()) {
                $clubs = getClubsOfLeader($member_id);
                if (count($clubs) > 0) { 
                    reset($clubs);
                    $club_id = key($clubs);
                } else {
                    $club_id = 0;
                }
                setClubOfNewMember($base_data_id, $club_id);
            } else {
                setClubOfNewMember($base_data_id, 0);
            }
        } else {
            logger($member_id, $base_data_id, LOGGER_MEMBER_MOD, " azonosítójú tag adatmódosítása");
        }
        $sql = 'UPDATE `' . MEMBER_TABLE . '` SET ';
        $sql .= ' `' . MEMBER_VEZNEV . '` ="' . $veznev . '" ,';
        $sql .= ' `' . MEMBER_KERNEV . '` ="' . $kernev . '" ,';
        $sql .= ' `' . MEMBER_BORN . '` ="' . $born . '" ,';
        if ($base_data_id == $member_id) {
            $sql .= ' `' . MEMBER_BIRTH_CITY . '` ="' . $birth_city . '" ,';
            $sql .= ' `' . MEMBER_MOTHER . '` ="' . $mother . '" ,';
        }
        $sql .= ' `' . MEMBER_ADDR_POST . '` ="' . $addr_post . '" ,';
        $sql .= ' `' . MEMBER_ADDR_CITY . '` ="' . $addr_city . '" ,';
        $sql .= ' `' . MEMBER_ADDR_STREET . '` ="' . $addr_street . '" ,';
        $sql .= ' `' . MEMBER_EMAIL . '` ="' . $email . '" ,';
        $sql .= ' `' . MEMBER_TEL . '` ="' . $telefon . '" ,';
        $sql .= ' `' . MEMBER_HOMEPAGE . '` ="' . $homepage . '" ,';
        $sql .= ' `' . MEMBER_NICKNAME . '` ="' . $nickname . '" ,';
        $sql .= ' `' . MEMBER_SEX . '` ="' . $sex . '" ';
        $sql .= ' WHERE `' . MEMBER_ID . '`="' . $base_data_id . '"';
        $mysqliLink->query($sql);
        if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) == 1 AND isFaiMember($base_data_id)) {
            updateFaiMember($base_data_id);
        }
        reloadEditMember();
    } else {
        
    }
}
if ($base_data_id < 0) {
    $m = array(
        MEMBER_SEX => 'M',
        MEMBER_BORN => '',
        MEMBER_ADDR_CITY => '',
        MEMBER_ADDR_POST => '',
        MEMBER_ADDR_STREET => '',
        MEMBER_KERNEV => $kernev,
        MEMBER_VEZNEV => $veznev,
        MEMBER_EMAIL => $email,
        MEMBER_TEL => '',
        MEMBER_HOMEPAGE => '',
        MEMBER_NICKNAME => '',
        MEMBER_LICENCE => '',
        MEMBER_BIRTH_CITY => '',
        MEMBER_MOTHER => '',
    );
} else {
    $m = retriveMember($base_data_id);
}
$isMale = ($m[MEMBER_SEX] != "F");
$output .= '<div class="keret"><div class="editor">';
$output .= '<form  method="post">';
$output .= printPermissions($base_data_id);
if ($already > 0) {
    $output .= '<br><font color="red"><label>Azonos név elfogadása:</label></font><input type="checkbox" name="already_override">';
}
$output .= '<br><label for="veznev">Vezetéknév: </label><input type="text" style="border-color:red"  name="veznev" value="' . $m[MEMBER_VEZNEV] . '"><br>';
$output .= '<label for="kernev">Keresztnév: </label><input type="text" style="border-color:red"  name="kernev" value="' . $m[MEMBER_KERNEV] . '"><br>';
$output .= '<label for="szulev">Születési dátum: </label><input type="date" style="border-color:red" name="born" value="' . $m[MEMBER_BORN] . '"><br>';
if ($base_data_id == $member_id) { // Sajat adatokat szerkeszt
    $output .= '<label >Születési hely: </label><input type="text" maxlength="32" name="birth_city" value="' . $m[MEMBER_BIRTH_CITY] . '"><br>';
    $output .= '<label >Anyja neve: </label><input type="text" maxlength="32" name="mother" value="' . $m[MEMBER_MOTHER] . '"><br>';
}
$output .= '<label for="addr_post">Írányítószám: </label><input type="text" name="addr_post" value="' . $m[MEMBER_ADDR_POST] . '"><br>';
$output .= '<label for="addr_city">Város :</label><input type="text" name="addr_city" value="' . $m[MEMBER_ADDR_CITY] . '"><br>';
$output .= '<label for="addr_street">Lakcím: </label><input type="text" name="addr_street" value="' . $m[MEMBER_ADDR_STREET] . '"><br>';
$output .= '<label for="telefon">Telefon: </label><input type="text" name="telefon" value="' . $m[MEMBER_TEL] . '"><br>';
$output .= '<label for="email">Email :</label><input type="text" style="border-color:red"  name="email" value="' . $m[MEMBER_EMAIL] . '"><br>';
$output .= '<label>Honlap: </label><input type="text" name="homepage" value="' . $m[MEMBER_HOMEPAGE] . '"><br>';
$output .= '<label>Becenév: </label><input type="text" name="nickname" value="' . $m[MEMBER_NICKNAME] . '"><br>';
$output .= '<label>Nem: </label>Férfi: <input type="radio" name="sex" value="M" ' . ($isMale ? ' checked ' : '') . '> ';
$output .= 'Nő: <input type="radio" name="sex" value="F" ' . (!$isMale ? ' checked ' : '') . '> <br>';
$output .= '<label>Klub: </label>' . getMemberClubNames($base_data_id) . '<br>';
$output .= '<label>Kategóriák: </label>' . getListMemberCategories($base_data_id) . '<br>';
$output .= '<label>Nemzetközi azonosító: </label>';
$fai_payment = getFaiPaymentDate($base_data_id);
$fai_mem = popFaiRow($base_data_id);
if (empty($fai_payment) OR empty($fai_mem)) {
    $output .= $m[MEMBER_LICENCE] . '+';
} else {
    $fai_print = getOptionValue(OPTIONS_NAME_FAI_PRINT) . $fai_mem[FAI_IDLICENCE];
    $output .= '<a href="' . $fai_print . '" target="_blank">' . $fai_mem[FAI_LICENCEE] . '</a>';
}
$output .= '<br>';
$fms2bill = getUnpaidFMOfMember($base_data_id);
$fms2bill_sum = 0;
$fms2bill_num = 0;
if (!empty($fms2bill)) {
    $collected_fms = array();
    include INCLUDES . 'print_fms2bill.inc';
    $_SESSION[SESSION_FEE_TO_BILL] = serialize($collected_fms);
}

if ($fms2bill_sum > 0) {
    $output .= '<input value="Fizetés:' . $fms2bill_sum . ' Ft"  type="submit" name="create_bill"'
            . ' onClick="return confirm(\'Biztosan el akarod a ' . $fms2bill_sum . ' összegű fizetést?\')">&nbsp;&nbsp;&nbsp;';
} else {
    $output .= '<input value="Rögzítés"  type="submit" name="base_data_save">';
}
$output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
$output .= '<input type="hidden" name="already" value="' . $already . '">';

$output .= '</form></div></div>';

