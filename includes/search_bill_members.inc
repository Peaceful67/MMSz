
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

$sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM ((`' . MEMBER_TABLE . '`';

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
    $view->danger('Legalább 3 karakter adj meg a szűréshez');
}
if (strlen($search) >= 3) {
    $sql .= ' WHERE (`' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '` LIKE "%' . $search . '%"  OR '
            . '`' . MEMBER_TABLE . '`.`' . MEMBER_KERNEV . '` LIKE "%' . $search . '%")';
}

$sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`'
        . ' ORDER BY `' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '` ';

$res = $mysqliLink->query($sql);
$total_member = $res ? $res->num_rows : 0;
$view->newKeret();
$view->newDiv('row');
$view->inputText('search', 'Név');
$view->submitButton('search_submit', 'Szűrés', 0, 'Szűrés a megadott feltételek szerint.');
$view->success('Találat: ' . $total_member . ' tag');
$view->endDiv()->newDiv('row');
if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $clubs = getClubNameList();
    $view->inputClub('Egyesület', [0 => 'Válassz!!!'] + $clubs, 'selected_club', true);
} else {
    $selected_club = putSelectionOfClub('selected_club');
}
$view->submitDropDownChooser('Díj tipus', [-1 => 'Összes'] + $fee_type, 'selected_fee');
$view->endDiv();
$view->putHidden('listed_member');
$view->putHidden('total_member');
$view->endKeret();

