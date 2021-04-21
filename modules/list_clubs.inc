<?php

if (isset($edit_subclubs)) {
    $edit_club = $edit_subclubs;
    include_once INCLUDES . 'edit_subclubs.inc';
    if (isset($edit_subclubs)) {
        $view->putHidden('edit_subclubs');
        return;
    }
}

if (isset($new_club)) {
    $edit_club = -1;
    include_once INCLUDES . 'club_editor.inc';
} elseif (isset($edit_club)) {
    include_once INCLUDES . 'club_editor.inc';
}

if (isset($edit_club)) {
    $view->putHidden('edit_club');
    return;
}

if (isset($delete_club)) {
    $sql = 'DELETE FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ID . '`="' . $delete_club . '"';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_CLUB_EDIT, $delete_club . ' azonosítójú egyesület törölve.');
}
if (!isset($order_by)) {
    $order_by = CLUB_NAME;
}

if (!isset($act_club)) {
    $act_club = "off";
}
$col_width = array('5%;', '35%;', '40%;', '10%;', '10%;');
$del_act = $del_max = 0;
$numberOfDelegate = array();
$maxNumberOfDelegate = array();
foreach (getClubNameList() as $cl_id => $cl_name) {
    $del_act += $numberOfDelegate[$cl_id] = intval(getNumberOfDelegateClub($cl_id));
    $del_max += $maxNumberOfDelegate[$cl_id] = intval(getMaxNumberOfDelegateClub($cl_id));
}
$res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE ' . ($act_club == "on" ? ('`' . CLUB_ACT . '`="1"') : '1') . ' ORDER BY `' . $order_by . '`');
$view->info('Aktív egyesületek küldöttei: ' . $del_act . '/' . $del_max);
$view->newDiv('row m-sm-1');
$view->submitAdd('new_club', -1, 'Új egyesület felvétele');
$view->inputCheckbox('act_club', 'Csak az aktív egyesületek listázása', true);
$view->endDiv();
$view->newDiv('table-responsive');
$view->newElement('<table class="table table-striped table-bordered" ><caption>Egyesületek listája</caption>');
$view->newElement('<thead class="thead-dark">');
$view->newTableTr();
$view->putElement('<th scope="col">Nr.</th><th scope="col">Név</th><th scope="col">Cím</th><th scope="col">Tagok</th><th scope="col">Művelet</th>');
$view->endTableTr();
$view->endElement('</thead>')->newElement('<tbody>');

while ($res AND $row = $res->fetch_assoc()) {
    $files = scandir(DOCUMENTS);
    $club_id = $row[CLUB_ID];
    $needle = 'club_' . $club_id . '_';
    $docus = ' ';
    foreach ($files as $f) {
        if (substr($f, 0, strlen($needle)) == $needle) {
            $docus .= '<a target="_blank" href="' . DOCUMENTS . $f . '"><img style="width:20px;" src="' . IMAGES . 'pdf.jpg"></a>';
        }
    }

    $view->newElement('<tr ' . ((isClubMembership($club_id) > 0) ? 'class="table-success"' : '') . '>');
    /*
      . ((!is_null($row[CLUB_SUBCLUBS]) AND ! empty($row[CLUB_SUBCLUBS])) ? ' background-color:orange;' : '')
      . '">';
     */
    $view->putElement('<td>' . $club_id . '</td>');
    $view->putElement('<td>' . $row[CLUB_NAME] . '</td>');
    $view->putElement('<td>' . $row[CLUB_ADDR_POST] . ' ' . $row[CLUB_ADDR_CITY] . ', ' . $row[CLUB_ADDR_STREET] . '</td>');
    $num_members = getNumMemberOfClub($club_id);
    if (!isset($numberOfDelegate[$club_id])) {
        $numberOfDelegate[$club_id] = 0;
    }
    if (($row[CLUB_MANDATUM] > 0) AND $numberOfDelegate[$club_id] > 1) {
        $member_background = ' style="color:red;" ';
    } else {
        $member_background = '';
    }
    $view->putElement('<td' . $member_background . '>'
            . (($row[CLUB_ACT]) ? ($num_members . green_red_String($numberOfDelegate[$club_id], $maxNumberOfDelegate[$club_id])) : 'Inaktív')
            . '</td>');
    $view->putElement('<td>');
    $view->submitEdit('edit_club', $row[CLUB_ID]);
    if (!isClubUsed($club_id)) {
        $view->submitDelete('delete_club', $row[CLUB_ID], 'Törlés', 'Biztosan tölni akarod a ' . $club_id . ' azonosítójú egyesületet?');
    }
    $output .= $docus . '</td>';
    $view->endTableTr();
}
$view->endElement('</tbody>');
$view->endElement('</table>')->endDiv();
