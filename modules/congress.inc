<?php

if (!isset($mem_id)) {
    $mem_id = 0;
}
if (!isset($selected_club)) {
    $selected_club = -1;
}
$res = $mysqliLink->query('SELECT * FROM `' . CONGRESS_TABLE . '` ');
$checked_in_delegates = array();
$checked_out_delegates = array();
$congress_dates = array();
$congress_dates[] = $today;
while ($res AND $row = $res->fetch_assoc()) {
    $enter_day = date('Y-m-d', strtotime($row[CONGRESS_ENTER]));
    if (!in_array($enter_day, $congress_dates)) {
        $congress_dates[] = $enter_day;
    }
}
sort($congress_dates);
if (isset($check_in)) {
    $sql = 'SELECT * FROM `' . CONGRESS_TABLE . '` WHERE `' . CONGRESS_MEMBER_ID . '`=' . $check_in
            . ' AND DATE(`' . CONGRESS_QUIT . '`)=CURDATE() '
            . ' ORDER BY `' . CONGRESS_QUIT . '` DESC LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        $row = $res->fetch_assoc();
        $sql = 'UPDATE `' . CONGRESS_TABLE . '` SET '
                . ' `' . CONGRESS_ENTER . '`=CURRENT_TIMESTAMP(), `' . CONGRESS_QUIT . '`=0 WHERE '
                . '`' . CONGRESS_ID . '`=' . $row[CONGRESS_ID];
        $mysqliLink->query($sql);
        logger($member_id, $check_in, LOGGER_CONGRESS, getMemberName($check_in) . '  küldött visszajött a terembe');
    } else {
        $sql = 'INSERT INTO `' . CONGRESS_TABLE . '` (`' . CONGRESS_MEMBER_ID . '`) VALUES ("' . $check_in . '")';
        $mysqliLink->query($sql);
        logger($member_id, $check_in, LOGGER_CONGRESS, getMemberName($check_in) . '  küldött bejött a terembe');
    }
}
if (isset($check_out)) {
    $sql = 'SELECT * FROM `' . CONGRESS_TABLE . '` WHERE `' . CONGRESS_MEMBER_ID . '`=' . $check_out
            . ' AND DATE(`' . CONGRESS_ENTER . '`)=CURDATE() '
            . ' ORDER BY `' . CONGRESS_ENTER . '` DESC LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        $row = $res->fetch_assoc();
        $sql = 'UPDATE `' . CONGRESS_TABLE . '` SET '
                . ' `' . CONGRESS_QUIT . '`=CURRENT_TIMESTAMP() WHERE '
                . '`' . CONGRESS_ID . '`=' . $row[CONGRESS_ID];
        $mysqliLink->query($sql);
        logger($member_id, $check_out, LOGGER_CONGRESS, getMemberName($check_out) . ' küldött kiment a teremből');
    }
}

$output .= '<div class="card"><form method="post">';
if (!isset($selected_day)) {
    $selected_day = $today;
}
$output .= 'Nap: <select  name="selected_day" onChange="submit();">';
foreach ($congress_dates as $day) {
    $output .= '<option value="' . $day . '" ' . (($selected_day == $day) ? "selected" : "") . '>' . ($day == $today ? "Ma" : $day) . '</option>';
}
$output .= '</select><br>';
$res = $mysqliLink->query('SELECT * FROM `' . CONGRESS_TABLE . '` ');
while ($row = $res->fetch_assoc()) {
    $enter_day = date('Y-m-d', strtotime($row[CONGRESS_ENTER]));
    if ($enter_day == $selected_day) {
        if ($enter_day > $row[CONGRESS_QUIT]) {
            $checked_in_delegates[$row[CONGRESS_MEMBER_ID]] = getMemberName($row[CONGRESS_MEMBER_ID]);
        } else {
            $checked_out_delegates[$row[CONGRESS_MEMBER_ID]] = getMemberName($row[CONGRESS_MEMBER_ID]);
        }
    }
}
asort($checked_in_delegates);
asort($checked_out_delegates);

if ($selected_day == $today) {
    $output .= '<div class="keret"><label>Egyesület : </label>'
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
            if (!array_key_exists($del_id, $checked_in_delegates)) {
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
            if (array_key_exists($mem_id, $checked_in_delegates)) {
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
}
if (!empty($checked_out_delegates)) {
    $output .= '<div class="keret"><h2>Bent volt, de kiment: ' . count($checked_out_delegates) . '</h2>';
    foreach ($checked_out_delegates as $delegate_id => $delegate_name) {
        if ($selected_day == $today) {
            $output .= '<button name="check_in" value="' . $delegate_id . '">Visszajön</button> ';
        }
        $output .= '(' . $delegate_id . ') ' . $delegate_name . ' : ' . getClubName(getDelegateClub($delegate_id)) . '<br>';
    }
    $output .= '</div>';
}
$output .= '<div class="keret">';
$output .= '<h1><input type="image" style="width: 40px; height: 40px;" src="' . IMAGES . 'refresh.png" name="refresh" >';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;A teremben : ' . count($checked_in_delegates) . ' küldött '
        . ($selected_day == $today ? 'tartózkodik' : 'tartózkodott') . '</h1>';
foreach ($checked_in_delegates as $delegate_id => $delegate_name) {
    if ($selected_day == $today) {
        $output .= '<button name="check_out" value="' . $delegate_id . '">Kimegy</button> ';
    }
    $output .= '(' . $delegate_id . ') ' . $delegate_name . ' : ' . getClubName(getDelegateClub($delegate_id)) . '<br>';
}
$output .= '</div></form></div>';
