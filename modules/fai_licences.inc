<?php

include_once INCLUDES . 'fai_functions.php';

$fai_op = '';
if (!isset($mem_fai)) {
    $mem_fai = $member_id;
}
if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) != 1) {
    warning('FAI API nincs engedélyezve');
    return;
}

if (isset($get_licence)) {
    $fai_op = FAI_LICENCE_GET;
}
if (isset($update_licence)) {
    $fai_op = 'update';
}
if (isset($create_licence)) {
    $fai_op = 'create';
}
if (isset($search_licence)) {
    $fai_op = FAI_LICENCE_SEARCH;
}
if (isset($fai_upload_)) {
    foreach (getAllFaiMembersId() as $mem_id) {
        error_log("Process: " . $mem_id);
        $fai_mem = FAI_search_last($mem_id);
        if (empty($fai_mem)) {
            error_log("FAI nem ismeri: " . $mem_id);
            createFaiMember($mem_id); // Nem ismeri, feltoltjuk
            $fai_mem = FAI_search_last($mem_id);
            if (empty($fai_mem)) {
                warning("Nem sikerült feltölteni: " . $mem_id);
                break;
            }
        }
        $fai_row = popFaiRow($mem_id);
        if (empty($fai_row)) { // Nalunk nincs meg a tablaban, letrehozzuk
            error_log("Nalunk nincs meg a tablaban, letrehozzuk " . $mem_id);
            pushFaiRow($fai_mem[FAI_ANS_LICENCE], $mem_id, $fai_mem[FAI_ANS_LICENCEE]);
        } elseif (($fai_mem[FAI_ANS_LICENCE] != $fai_row[FAI_IDLICENCE])
                OR ( $fai_mem[FAI_ANS_LICENCEE] != $fai_row[FAI_LICENCEE])) { // Megvan, de rosszul, letre hozzuk
            error_log("Megvan, de rosszul, letre hozzuk " . $mem_id);
            pushFaiRow($fai_mem[FAI_ANS_LICENCE], $mem_id, $fai_mem[FAI_ANS_LICENCEE]);
        } else {
            error_log("Megvan es egyezik: " . $mem_id);
        }
    }
}

if (isset($compare_licence)) {
    $equal = true;
    $output .= 'Id: ' . $mem_fai . ' ';
    $mem = getMemberById($mem_fai);
    $fai_row = popFaiRow($mem_fai);
    $fai_mem = getFaiMemberByIdlicence($fai_row[FAI_IDLICENCE]);
    if ($mem[MEMBER_SEX] != $fai_mem[FAI_ANS_GENDER]) {
        $output .= ' neme ('.$fai_mem[FAI_ANS_GENDER].'), ';
        $equal = false;
    }
    if ($mem[MEMBER_KERNEV] != $fai_mem[FAI_ANS_FIRSTNAME]) {
        $output .= ' keresztnév ('.$fai_mem[FAI_ANS_FIRSTNAME].'), ';
        $equal = false;
    }
    if ($mem[MEMBER_VEZNEV] != $fai_mem[FAI_ANS_SURNAME]) {
        $output .= ' vezetéknév ('.$fai_mem[FAI_ANS_SURNAME].'), ';
        $equal = false;
    }
    if ($mem[MEMBER_BORN] != $fai_mem[FAI_ANS_BIRTH]) {
        $output .= ' szül.('.$fai_mem[FAI_ANS_BIRTH].') ';
        $equal = false;
    }
    $pay_date = getFaiPaymentEnfOfYear($mem_fai);
    if ($pay_date != $fai_mem[FAI_ANS_VALID]) {
        $output .= ' érvényesség ('.$fai_mem[FAI_ANS_VALID].'), ';
        $equal = false;
    }
    $output .= $equal ? ' Minden egyezik' : ' nem egyezik';
    $output .= '<br>';
}
if (isset($fai_compare)) {
    $num_equ = $num_diff = 0;
    foreach (getAllFaiMembersId() as $mem_id) {
        $equal = true;
        $output .= 'Id: ' . $mem_id . ' ';
        $mem = getMemberById($mem_id);
        $fai_row = popFaiRow($mem_id);
        $fai_mem = getFaiMemberByIdlicence($fai_row[FAI_IDLICENCE]);
        if ($mem[MEMBER_SEX] != $fai_mem[FAI_ANS_GENDER]) {
            $output .= ' neme, ';
            $equal = false;
        }
        if ($mem[MEMBER_KERNEV] != $fai_mem[FAI_ANS_FIRSTNAME]) {
            $output .= ' keresztnév, ';
            $equal = false;
        }
        if ($mem[MEMBER_VEZNEV] != $fai_mem[FAI_ANS_SURNAME]) {
            $output .= ' vezetéknév, ';
            $equal = false;
        }
        if ($mem[MEMBER_BORN] != $fai_mem[FAI_ANS_BIRTH]) {
            $output .= ' szül. ';
            $equal = false;
        }
        $pay_date = getFaiPaymentEnfOfYear($mem_id);
        if ($pay_date != $fai_mem[FAI_ANS_VALID]) {
            $output .= ' érvényesség, ';
            $equal = false;
        }
        if ($equal) {
            $num_equ++;
        } else {
            $num_diff++;
        }
        $output .= $equal ? ' Minden egyezik' : ' nem egyezik';
        $output .= '<br>';
    }
    error_log($num_equ . ' tétel egyezik és ' . $num_diff . ' tétel különbözik');
    $output .= $num_equ . ' tétel egyezik és ' . $num_diff . ' tétel különbözik<br>';
}

if (isset($fai_create)) {
    $mysqliLink->query('DELETE * FROM `' . FAI_TABLE . '` WHERE 1;');
    $num = 0;
    foreach (getAllFaiMembersId() as $mem_id) {
        error_log("Process: " . $mem_id);
        $fai_mem = FAI_search_last($mem_id);
        if (empty($fai_mem)) {
            error_log("FAI nem ismeri: " . $mem_id);
            $output .= 'FAI nem ismeri: ' . $mem_id . '<br>';
        } else {
            $num++;
            error_log("Inserted " . $mem_id);
            pushFaiRow($fai_mem[FAI_ANS_LICENCE], $mem_id, $fai_mem[FAI_ANS_LICENCEE]);
        }
    }
    $output .= $num . ' elem saját táblában létrehozva.<br>';
}
if (isset($view_licence)) {
    $fai_row = popFaiRow($mem_fai);
    if (empty($fai_row)) {
        $output .= 'Nem szerepel a FAI táblában<br>';
    } else {
        $output .= '<a href="' . getOptionValue(OPTIONS_NAME_FAI_VIEW) . $fai_row[FAI_IDLICENCE] . '" target="_blank">View</a>';
    }
}
$output .= '<form method="post">';
switch ($fai_op) {
    case FAI_LICENCE_SEARCH:
        extractArray(FAI_search_last($mem_fai));
        break;
    case FAI_LICENCE_GET:
        $fai_row = popFaiRow($mem_fai);
        if (empty($fai_row)) {
            $output .= 'Nem szerepel a FAI táblában<br>';
            break;
        }
        extractArray($fai_row);
        $fai_mem = getFaiMemberByIdlicence($fai_row[FAI_IDLICENCE]);
        extractArray($fai_mem);
        break;
    case 'create':
        createFaiMember($mem_fai);
        break;
    case 'update':
        updateFaiMember($mem_fai);
        break;
}
$fai_row = popFaiRow($mem_fai);
if (empty($fai_row)) {
    $fai_row[FAI_IDLICENCE] = '';
}
$output .= '<label>Azonosító :</label>'
        . '<input type="number" name="mem_fai"  length="10" size="5" value="' . $mem_fai . '">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input type="submit" value="Get" name="get_licence">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<a href="' . getOptionValue(OPTIONS_NAME_FAI_VIEW) . $fai_row[FAI_IDLICENCE] . '" target="_blank">'
        . '<input type="button" class="button" value="View" /></a>'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input type="submit" value="Search" name="search_licence">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input disabled type="submit" value="Create" name="create_licence">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input type="submit" value="Update" name="update_licence">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input type="submit" value="Compare" name="compare_licence">'
        . '<br><br>';
$output .= '<label>Műveletek:'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input disabled type="submit" value="Saját tábla létrehozás" name="fai_create";>'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input disabled type="submit" value="Adat feltöltés" name="fai_updatee";>'
        . '&nbsp;&nbsp;&nbsp;&nbsp;'
        . '<input type="submit" value="Adat összehasonlítás" name="fai_compare";>'
        . '<br>';
$output .= '</form>';

