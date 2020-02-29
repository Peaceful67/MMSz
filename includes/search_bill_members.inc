
<?php

if (!isset($listed_member)) {
    $listed_member = 0;
}
if (!isset($search)) {
    $search = '';
}

if (!isset($selected_fee)) {
    $selected_fee = -1;
}

if (!isset($selected_club)) {
    if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
        $selected_club = 0;
    } else {
        $selected_club = isClubLeader();
    }
}

$sql = 'SELECT `' . MEMBER_TABLE. '`.* FROM ((`' . MEMBER_TABLE . '`';

if ($selected_club > 0) {
    $sql .= ' INNER JOIN `' . ROLE_TABLE . '` ON (`' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`="' . $selected_club . '" '
            . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '` '
            . ' AND (CURDATE() BETWEEN `' . ROLE_TABLE . '`.`' . ROLE_FROM . '` AND `' . ROLE_TABLE . '`.`' . ROLE_TO . '`)))';
} else {
    $sql .= ')';
}
if ($selected_fee >= 0) {
    $sql .= ' INNER JOIN `' . FM_TABLE . '` AS `pay_fm` ON ('
            . ' `pay_fm`.`' . FM_BILL_ID . '`="0" '
            . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`pay_fm`.`' . FM_MEMEBER_ID . '` )'
            . ' INNER JOIN `' . FEE_TABLE . '` AS `pay_fee` ON ('
            . ' `pay_fm`.`' . FM_FEE_ID . '`=`pay_fee`.`' . FEE_ID . '` '
            . ' AND `pay_fee`.`' . FEE_TYPE . '`="' . $selected_fee . '" '
            . ' )) ';
} else {
    $sql .= ' INNER JOIN `' . FM_TABLE . '` AS `pay_fm` ON ('
            . ' `pay_fm`.`' . FM_BILL_ID . '`="0" '
            . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`pay_fm`.`' . FM_MEMEBER_ID . '` )) ';
}

if (strlen($search) < 3 AND ! empty($search)) {
    warning('Legalább 3 karakter adj meg a szűréshez');
}
if (strlen($search) >= 3) {
    $sql .= ' WHERE (`' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '` LIKE "%' . $search . '%"  OR '
            . '`' . MEMBER_TABLE . '`.`' . MEMBER_KERNEV . '` LIKE "%' . $search . '%")';
}

$sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`'
        . ' ORDER BY `' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '` ';

$res = $mysqliLink->query($sql);
$total_member = $res ? $res->num_rows : 0;
$output .= '<div class="keret"><div class="search"> 

<label for="search">Név: </label>
<input type="text" name="search"  id="search" value="' . $search . '"  size="10">';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Találat :' . $total_member . ' tag&nbsp;&nbsp;&nbsp;&nbsp';
$output .= '<input type="submit" name="search_submit" value="Szűrés">';
if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $output .= '<br><br>Egyesület: <select  name="selected_club" onChange="submit();">';
    $output .= '<option value="0">Válassz egyesületet !</option>';
    $clubs = getClubNameList();
    foreach ($clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }
    $output .= '</select>&nbsp;&nbsp;&nbsp;&nbsp';
} else {
    $selected_club = putSelectionOfClub('selected_club');
}
$output .= 'Díj tipus: ';
$output .= '<select name="selected_fee" onChange="submit();">';
$output .= '<option value="-1" ' . (($selected_fee == -1) ? "selected" : "") . '>Összes</option>';
foreach ($fee_type as $type_key => $type_name) {
    $output .= '<option value="' . $type_key . ' " ' . (($selected_fee == $type_key) ? "selected" : "") . '>' . $type_name[0] . '</option>';
}

if (isset($pager)) {
    switch ($pager) {
        case '<<':
            $listed_member = 0;
            break;
        case '<':
            $listed_member -= PAGE_MEMBERS;
            if ($listed_member < 0) {
                $listed_member = 0;
            }
            break;
        case '>':
            $listed_member += PAGE_MEMBERS;
            if ($listed_member + PAGE_MEMBERS >= $total_member) {
                $listed_member = $total_member - PAGE_MEMBERS;
            }
            break;
        case '>>':
            $listed_member = $total_member - PAGE_MEMBERS;
            break;

        default:
            break;
    }
}


$output .= '<input type="hidden" name="listed_member" value="' . $listed_member . '">    ';
$output .= '<input type="hidden" name="total_member" value="' . $total_member . '">    ';

if ($total_member > PAGE_MEMBERS) {
    $output .= '&nbsp;&nbsp;&nbsp;&nbspLapozás:<input type="submit" name="pager" value="<<">    ';
    $output .= '<input type="submit" name="pager" value="<">    ';
    $output .= $listed_member + 1 . '/' . $total_member;
    $output .= '<input type="submit" name="pager" value=">">    ';
    $output .= '<input type="submit" name="pager" value=">>">    ';
}

$output .= '</div>';
$output .= '</div>';

