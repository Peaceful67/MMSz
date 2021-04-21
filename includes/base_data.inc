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
if (!isset($is_male)) {
    $is_male = 'off';
}

if (isset($base_data_save)) {
    $error = false;
    if (strlen($kernev) < 3) {
        $view->danger("Túl rövid keresztnév");
        $error = true;
    }
    if (strlen($veznev) < 3) {
        $view->danger("Túl rövid vezetéknév");
        $error = true;
    }
    if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        $view->danger("Helytelen email cím !");
        $error = true;
    }
    if (!validateDate($born)) {
        $view->danger("Helytelen születési dátum !");
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
                $ids .= $already_id . ', ';
            }
            $view->danger('A rendszer szerint ez a tag a ' . $ids . ' azonsítójú tagként már létezik');
            $error = true;
        }
    }

    if (!$error) {
        if ($base_data_id <= 0) {
            $edit_member = $base_data_id = createNewMember();
            logger($member_id, $base_data_id, LOGGER_MEMBER_MOD, ' tag létrehozása');

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
            logger($member_id, $base_data_id, LOGGER_MEMBER_MOD, "Tag alapadat módosítása");
        }
        $sex = (isset($is_male)AND $is_male == 'on') ? 'M' : 'F';
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
        $sql .= ' `' . MEMBER_LICENCE . '` ="' . $international_licence . '" ,';
        $sql .= ' `' . MEMBER_SEX . '` ="' . $sex . '" ';
        $sql .= ' WHERE `' . MEMBER_ID . '`=' . $base_data_id . ';';
        $mysqliLink->query($sql);
        if (isset($is_inform) AND $is_inform == 'on') { // Ha módosítunk, biztosan nem jövünk ide, mert meg sem jeleni a checkbox
            $email = new Email(EMAIL_CONTEXT_INFORM_NEW_MEMBER, $edit_member);
            $email->insertToBody(1, getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME));
            $email->insertToBody(2, $_SERVER['HTTP_HOST']);
            $email->insertToBody(3, getMemberName($member_id));
            $email->sendEmail([$base_data_id => '']);
            logger($member_id, $base_data_id, LOGGER_MEMBER_MOD, ' emailes kiértesítése regisztrálásáról');
        }
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

$view->newKeret();
$view->putPrevent();
$view->info('Szerepek: '.printRoles($base_data_id) );
if ($already > 0) {
    $view->warning('<label>Azonos név elfogadása:</label></font><input type="checkbox" name="already_override">');
}
$veznev = $m[MEMBER_VEZNEV];
$kernev = $m[MEMBER_KERNEV];
$born = $m[MEMBER_BORN];
$birth_city = $m[MEMBER_BIRTH_CITY];
$mother = $m[MEMBER_MOTHER];
$addr_post = $m[MEMBER_ADDR_POST];
$addr_city = $m[MEMBER_ADDR_CITY];
$addr_street = $m[MEMBER_ADDR_STREET];
$telefon = $m[MEMBER_TEL];
$email = $m[MEMBER_EMAIL];
$homepage = $m[MEMBER_HOMEPAGE];
$nickname = $m[MEMBER_NICKNAME];
$is_male = ($m[MEMBER_SEX] == "M") ? 'on' : 'off';
$international_licence = $m[MEMBER_LICENCE];
$view->newDiv('row align-items-end');
$view->newDiv('col-6')->inputText('veznev', 'Név', 'Vezetéknév')->endDiv();
$view->newDiv('col-4')->inputText('kernev', '', 'Keresztnév')->endDiv();
$view->endDiv()->newDiv('row align-items-end');
$view->newDiv('col-6')->inputDate('born', 'Születési dátum')->endDiv();
$view->newDiv('col-6')->inputEmail('email', 'Email')->endDiv();
$view->endDiv();
if ($base_data_id == $member_id) { // Sajat adatokat szerkeszt
    $view->newDiv('row align-items-end');
    $view->newDiv('col-6')->inputText('birth_city', 'Születési hely', 'Város')->endDiv();
    $view->newDiv('col-6')->inputText('mother', 'Anyja neve', 'Lánykori')->endDiv();
    $view->endDiv();
}
$view->newDiv('row align-items-end');
$view->newDiv('col-6')->inputNumber('addr_post', 'Írányítószám')->endDiv();
$view->newDiv('col-6')->inputText('addr_city', 'Város', 'Város')->endDiv();
$view->endDiv()->newDiv('row align-items-end');
$view->newDiv('col-6')->inputText('addr_street', 'Lakcím', 'Utca, házszám')->endDiv();
$view->newDiv('col-6')->inputText('telefon', 'Telefon', 'Lehetőleg mobil')->endDiv();
$view->endDiv()->newDiv('row align-items-end');
$view->newDiv('col-6')->inputText('homepage', 'Honlap', 'URL')->endDiv();
$view->newDiv('col-6')->inputText('nickname', 'Becenév', 'Nickname')->endDiv();
$view->endDiv();
$view->inputSwitchCheckbox('is_male', 'Férfi', 'Nő', 'Nem');

if ($base_data_id < 0) {
    $is_inform = 'on';
    $view->inputCheckbox('is_inform', 'Tag értesítése emailben');
} else {
    $view->putText(getMemberClubNames($base_data_id), 'Egyesület');
    $view->putText(getListMemberCategories($base_data_id), 'Kategória');
    if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) > 0) {
        $view->newDiv('form-inline');
        $fai_payment = getFaiPaymentDate($base_data_id);
        $fai_mem = popFaiRow($base_data_id);
        if (empty($fai_payment) OR empty($fai_mem)) {
            if (empty($m[MEMBER_LICENCE])) {
                $view->putText('Nincs', 'Nemzetközi azonosító');
            } else {
                $view->putText($m[MEMBER_LICENCE], 'Nemzetközi azonosító');
            }
        } else {
            $fai_print = getOptionValue(OPTIONS_NAME_FAI_PRINT) . $fai_mem[FAI_IDLICENCE];
            $view->putText('<a href="' . $fai_print . '" target="_blank">' . $fai_mem[FAI_LICENCEE] . '</a>', 'Nemzetközi azonosító');
        }
        $view->endDiv();
    } else {
        $view->inputText('international_licence', 'Nemzetközi', 'Azonosító');
    }
}
$fms2bill = getUnpaidFMOfMember($base_data_id);
$fms2bill_sum = 0;
$fms2bill_num = 0;
if (!empty($fms2bill)) {
    $collected_fms = array();
    include INCLUDES . 'print_fms2bill.inc';
    $_SESSION[SESSION_FEE_TO_BILL] = serialize($collected_fms);
}

if ($fms2bill_sum > 0) {
    $view->submitButton('create_bill', 'Fizetés', -1, $fms2bill_sum . ' Ft összegű fizetés indítása', 'Biztosan el akarod a ' . $fms2bill_sum . ' összegű fizetést?');
} else {
    $view->submitButton('base_data_save', 'Rögzítés', $base_data_id, $base_data_id < 0 ? 'Új tag felvitele' : 'Adatok módosítása');
}
$view->putHidden('edit_member');
$view->putHidden('already');
$view->endKeret();
