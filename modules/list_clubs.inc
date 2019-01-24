<?php

if (isset($new_club)) {
    $edit_club = -1;
    include_once INCLUDES . 'club_editor.inc';
} elseif (isset($edit_club)) {
    include_once INCLUDES . 'club_editor.inc';
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
$col_width = array(5, 40,40, 10, 5);
$res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE ' . ($act_club == "on" ? ('`' . CLUB_ACT . '`="1"') : '1') . ' ORDER BY `' . $order_by . '`');
$output .= '<br><div class="scrolled_lists"><form method="post"> '
        . '<input type="submit" name="new_club" value="Új felvitele">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
        . ' <input type="checkbox" name="act_club" ' . ($act_club == "on" ? "checked" : "") . ' onchange="submit();">'
        . 'Csak aktív egyesületek'
        . '<table border="1"><thead>';
$output .= '<tr><th style="width:' . $col_width[0] . '%;">Nr.</th><th style="width:' . $col_width[1] . '%;">Név</th>'
        . '<th style="width:' . $col_width[2] . '%;">Cím</th><th style="width:' . $col_width[3] . '%;">Tagok</th>'
        . '<th style="width:' . $col_width[4] . '%;">Művelet</th></tr></thead><tbody>';

while ($res AND $row = $res->fetch_assoc()) {
    $output .= '<tr style="color: ' . (($row[CLUB_ACT] == 1) ? 'green' : 'red') . ';">';
    $output .= '<td style="width:' . $col_width[0] . '%;">' . $row[CLUB_ID] . '. ';
    if(isClubMembership($row[CLUB_ID])>0) {
        $output .= '<image src="' . IMAGES . 'ok.jpg">';
    }
    $output .= '</td>';
    $output .= '<td style="width:' . $col_width[1] . '%;">' . $row[CLUB_NAME] . '</td>';
    $output .= '<td  style="width:' . $col_width[2] . '%;">' . $row[CLUB_ADDR_POST] . ' ' . $row[CLUB_ADDR_CITY] . ', ' . $row[CLUB_ADDR_STREET] . '</td>';
    $num_members = getNumMemberOfClub($row[CLUB_ID]);
    $output .= '<td style="width:' . $col_width[3] . '%;">' . $num_members . '</td>';
    $output .= '<td  style="width:' . $col_width[4] . '%;">';
    $output .= '<button  type="submit" name="edit_club" value="' . $row[CLUB_ID] . '"><img src="' . IMAGES . 'edit.jpg" title="Szerkesztés"></button>';
    if (!isClubUsed($row[CLUB_ID])) {
        $output .= '<button  type="submit" name="delete_club" value="' . $row[CLUB_ID] . '" onClick="return confirm(\'Biztosan tölni akarod a ' . $row[CLUB_ID] . ' azonosítójú egyesületet?\')"><img src="' . IMAGES . 'delete.jpg" title="Törlés"></button>';
    }
    $output .= '</td>';
    $output .= "</tr>";
}
$output .= "</tbody></table></form></div>";
