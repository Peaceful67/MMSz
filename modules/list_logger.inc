<?php

if (!isset($member)) {
    $member = 0;
}
if (!isset($related)) {
    $related = 0;
}
if (!isset($search_text)) {
    $search_text = '';
}
if (!isset($selected_act)) {
    $selected_act = -1;
}
if (!isset($logger_from)) {
    $logger_from = date('Y-m-d', strtotime($today . ' -1 month'));
}
if (!isset($logger_to)) {
    $logger_to = $today;
}
$view->newKeret();
$view->newDiv('row align-items-end');
$view->newDiv('col-6')->inputMemberId('member', 'Elkövető')->endDiv();
$view->newDiv('col-6')->inputMemberId('related', 'Érintett')->endDiv();
$view->endDiv()->newDiv('row align-items-end');
$view->newDiv('col-6')->inputDropDownChooser('Kategória', [-1 => 'Összes'] + $logger_categories, 'selected_act', false)->endDiv();
$view->newDiv('col-6')->inputText('search_text', 'Szöveg')->endDiv();
$view->endDiv()->newDiv('row align-items-end');
$view->inputDate('logger_from', 'Időszak');
$view->inputDate('logger_to');
$view->endDiv();
$view->submitButton('submit', 'Keresés');
$view->endKeret();
$view->newTable('Aktivitás lista');
$view->putThead('thead-dark', ['Elkövető', 'Érintett', 'Művelet', 'Időpont']);
$view->newElement('<tbody>');
$sql = 'SELECT * FROM `logs` WHERE '
        . '(`time_stamp` BETWEEN "' . $logger_from . ' 00:00:00" AND "' . $logger_to . ' 23:59:59" ) ';
if ($member > 0) {
    $sql .= ' AND`member`="' . $member . '" ';
}
if (strlen($search_text) > 0) {
    $sql .= ' AND `activity` LIKE "%' . $search_text . '%" ';
}
if ($related > 0) {
    $sql .= ' AND `related_id`="' . $related . '" ';
}
if ($selected_act > 0) {
    $sql .= ' AND `log_category`="' . $selected_act . '" ';
}
$sql .= ' ORDER BY `time_stamp` DESC LIMIT 300';
$res = $mysqliLink->query($sql);
while ($res AND $row = $res->fetch_assoc()) {
    $mid = $row["member"];
    $view->newTableTr();
    $view->putElement('<td><a title="' . getMemberName($row["member"]) . '">' . $row["member"] . '</a> </td>');
    if ($row["related_id"] > 0) {
        $view->putElement('<td><a title="' . getMemberName($row["related_id"]) . '">' . $row["related_id"] . '</a> </td>');
    } else {
        $view->putElement('<td>---</td>');
    }
    $view->putElement('<td><a title="' . $logger_categories[$row["log_category"]] . '">' . $row["activity"] . '</a></td>');
    $view->putElement('<td><small>' . $row["time_stamp"] . '</small></td>');
    $view->endTableTr();
}
$view->endTable();
$view->endKeret();
