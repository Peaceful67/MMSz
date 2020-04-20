<?php

define("ADDR_CHK", "addr_chk_");
$selected_group = array(
    0 => "Egyesületi tisztek",
    1 => "Egyesületi vezetők",
    2 => "Egyesületi küldöttek",
    3 => "Elnökségi tagok",
    4 => "Aktív tagok",
);

$filtered = array();
$sql = 'SELECT * FROM `' . MEMBER_TABLE . '` WHERE `' . MEMBER_EMAIL . '`!="" '
        . ' GROUP BY `' . MEMBER_ID . '`'
        . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';

$res = $mysqliLink->query($sql);
$all = array();
while ($res AND $row = $res->fetch_assoc()) {
    $all[] = $row[MEMBER_ID];
}
if (isset($congress_members_save)) {
    $congressDelegates->removeAllMembers($selected_open_congress);
    foreach ($all as $mem_id) {
        $chk = ADDR_CHK . $mem_id;
        if (isset($$chk) AND $ $chk == 'on') {
            $congressDelegates->addMember($selected_open_congress, $mem_id);
        }
    }
    if (isset($selected_auth1)) {
        $congressOnline->setCongressAuth1($selected_auth1);
    }
    if (isset($selected_auth2)) {
        $congressOnline->setCongressAuth2($selected_auth2);
    }
}

if (!isset($selected_addr)) {
    $selected_addr = -1;
}
if (!isset($old_selected_addr)) {
    $old_selected_addr = $selected_addr;
    $filtered = $congressDelegates->getMembersOfCongress($selected_open_congress);
}

$output .= '<h3>Szavazásban résztvevők kiválasztása</h3>';
output_label('Résztvevők');
output_selector($selected_group, 'addr');
output_spaces(10);


foreach ($all as $mem_id) {
    if (isset($filtered[$mem_id])) {
        $chk = ADDR_CHK . $mem_id;
        $$chk = 'on';
    }
}
if ($old_selected_addr != $selected_addr) {
    foreach ($all as $id) {
        $chk = ADDR_CHK . $id;
        unset($$chk);
    }
    $congressDelegates->removeAllMembers($selected_open_congress);
    $old_selected_addr = $selected_addr;
    $res = retriveAddresses($selected_addr);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $congressDelegates->addMember($selected_open_congress, $row[MEMBER_ID]);
            $chk = ADDR_CHK . $row[MEMBER_ID];
            $$chk = 'on';
        }
    }
}
$output .= '<b>Kiválasztottak száma: ' . $congressDelegates->countOfMembers($selected_open_congress) . '</b>';
$output .= '<div class="congress_selected">';
$delegates = $congressDelegates->getMembersOfCongress($selected_open_congress);
output_label('Hitelesítő 1');
$selected_auth1 = $congressOnline->getCongressAuth1();
output_selector($delegates, 'auth1', false);
output_ln();
output_label('Hitelesítő 2');
$selected_auth2 = $congressOnline->getCongressAuth2();
output_selector($delegates, 'auth2', false);
output_ln();
$mem_num = 0;
foreach ($delegates as $mem_id => $mem_name) {
    $mem = retriveMember($mem_id);
    $output .= '<div class="tooltip">' . $mem_name
            . '<span class="tooltiptext">'
            . 'Azonosító:' . $mem_id
            . '<br>Szül:' . $mem[MEMBER_BORN]
            . '<br>Email:' . $mem[MEMBER_EMAIL]
            . '</span></div>, ';
    if (++$mem_num > 4) {
        $mem_num = 0;
        output_ln();
    }
}
$output .= '</div>';

$output .= '<div class="congress_address">';
$output .= '<ul>';
$selected_num = 0;
$congress_members = array();
foreach ($all as $id) {
    $mem = retriveMember($id);
    $chk = ADDR_CHK . $id;
    if (isset($$chk) AND $ $chk == 'on') {
        $checked = 'checked';
        $selected_num++;
        $congress_members[$id] = $mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV];
    } else {
        $checked = '';
    }
    $output .= '<li><input type="checkbox" name="' . $chk . '" ' . $checked . ' " >'
            . '<a title="Azonosító: ' . $mem[MEMBER_ID] . '">'
            . $mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV] . '</a></li>';
}
$output .= '</ul>';
$output .= '</div>';
output_hidden('old_selected_addr');
output_ln(12);
$output .= '<button name="congress_members_save">Mentés</button>';

