<?php

if (!isset($mem_id)) {
    $mem_id = 0;
}
if (!isset($selected_club)) {
    $selected_club = -1;
}
$res = $mysqliLink->query('SELECT * FROM `' . CONGRESS_TABLE . '` ');
$checked_in_delegates = array();
while ($res AND $row = $res->fetch_assoc()) {
    $checked_in_delegates[] = $row[CONGRESS_MEMBER_ID];
}

if (isset($check_in)) {
    $mem_id = $check_in;
    if (!in_array($check_in, $checked_in_delegates)) {
        $checked_in_delegates[] = $check_in;
        $mysqliLink->query('INSERT INTO `' . CONGRESS_TABLE . '` (`' . CONGRESS_MEMBER_ID . '`) VALUES ("' . $check_in . '")');
        logger($member_id, $check_in, LOGGER_CONGRESS, getMemberName($check_in) . '  küldött bejött a termbe');
    }
}
if (isset($check_out)) {
    $mem_id = $check_out;
    $mysqliLink->query('DELETE FROM `' . CONGRESS_TABLE . '` WHERE `' . CONGRESS_MEMBER_ID . '`="' . $check_out . '"');
    if (($key = array_search($check_out, $checked_in_delegates)) !== false) {
        unset($checked_in_delegates[$key]);
        logger($member_id, $check_out, LOGGER_CONGRESS, getMemberName($check_out) . ' küldött kiment a termből');
    }
}
$output .= '<div class="card"><form method="post"><div class="keret">';
$output .= '<label>Egyesület : </label>'
        . '<select id="selected_club" name="selected_club" onchange="submit();">';
$output .= '<option value="-1">Válassz egyesületet !</option>';
$clubs = getClubNameList();
foreach ($clubs as $club_id => $club_name) {
    $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
}
$output .= '</select>&nbsp;&nbsp;&nbsp;&nbsp;';
if ($selected_club >= 0) {
    $delegatesOfClub = getDelegatesOfClub($selected_club);
    foreach ($delegatesOfClub as $del_id) {
        if (!in_array($del_id, $checked_in_delegates)) {
            $output .= '<button name="check_in" value="' . $del_id . '">' . getMemberName($del_id) . '</button>&nbsp;&nbsp;';
        }
    }
}

$output .= '<br><label>Azonosító:</label><input min="0" max="9999" type="number" name="mem_id" value="' . $mem_id . '" onChange="submit();">';
$output .= ' <input type="submit" name="check" value="Lekérdezés">&nbsp;&nbsp;&nbsp;&nbsp;';


if (isValidMemberId($mem_id)) {

    $output .= '<br><label>Név:</label>' . getMemberName($mem_id) . '&nbsp;&nbsp;&nbsp;&nbsp;';
    $club_id = getDelegateClub($mem_id);
    if ($club_id > 0) {
        $output .= '<label>Egyesület:</label>' . getClubName($club_id);
        if (in_array($mem_id, $checked_in_delegates)) {
            $output .= '<br> <button name="check_out" value="' . $mem_id . '">Kimegy</button>';
        } else {
            $output .= '<br> <button name="check_in" value="' . $mem_id . '">Bejön</button>';
        }
        $output .= '<input type="hidden" name="club_id" value="' . $club_id . '">';
    }
} else {
    $output .= '<br>Érvénytelen azonosító';
}
//$output .= '&nbsp;&nbsp;&nbsp;&nbsp;<button name="refresh" value="' . $mem_id . '">Frissít</button>';
$output .= '</div>';
$output .= '<div class="keret">';
$output .= '<h1><input type="image" style="width: 40px; height: 40px;" src="' . IMAGES . 'refresh.png" name="refresh" >';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;A teremben : ' . count($checked_in_delegates) . ' küldött tartózkodik</h1>';
foreach ($checked_in_delegates as $delegate_id) {
    $output .= '(' . $delegate_id . ') ' . getMemberName($delegate_id) . ' : ' . getClubName(getDelegateClub($delegate_id)) . '<br>';
}
$output .= '</div></form></div>';
