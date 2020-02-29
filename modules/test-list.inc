<?php

$col_width = array(70, 300, 300, 70, 70);
$res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE ' . ($act_club == "on" ? ('`' . CLUB_ACT . '`="1"') : '1'));
$output .= '<br><div class="scrolled_lists"><form method="post"> '
        . '<input type="submit" name="new_club" value="Új felvitele">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
        . ' <input type="checkbox" name="act_club" ' . ($act_club == "on" ? "checked" : "") . ' onchange="submit();">'
        . ($act_club == "on" ? "Aktív egyesületek" : "Összes egyesület")
        . '<table><thead>';
$output .= '<tr><th style="width:' . $col_width[0] . 'px;">Nr.</th><th style="width:' . $col_width[1] . 'px;">Név</th>'
        . '<th style="width:' . $col_width[2] . 'px;">Cím</th><th style="width:' . $col_width[3] . 'px;">Tagok</th>'
        . '<th style="width:' . $col_width[4] . 'px;">Művelet</th></tr></thead><tbody>';

while ($res AND $row = $res->fetch_assoc()) {
    $output .= '<tr style="color: ' . (($row[CLUB_ACT] == 1) ? 'green' : 'red') . ';">';
    $output .= '<td  style="width:' . $col_width[0] . 'px;">' . $row[CLUB_ID] . '.</td>';
    $output .= '<td style="width:' . $col_width[1] . 'px;">' . $row[CLUB_NAME] . '</td>';
    $output .= '<td style="width:' . $col_width[2] . 'px;">' . $row[CLUB_ADDR_POST] . ' ' . $row[CLUB_ADDR_CITY] . ', ' . $row[CLUB_ADDR_STREET] . '</td>';
    $num_members = getNumMemberOfClub($row[CLUB_ID]);
    $output .= '<td style="width:' . $col_width[3] . 'px;">' . $num_members . '</td>';
    $output .= '<td style="width:' . $col_width[4] . 'px;">';
    $output .= '<button  type="submit" name="edit_club" value="' . $row[CLUB_ID] . '"><img src="' . IMAGES . 'edit.jpg" title="Szerkesztés"></button>';
    $output .= '</td>';
    $output .= '</tr>';
}
$output .= "</tbody></table></form></div>";
