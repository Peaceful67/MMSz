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
$congress_dates[$today] = 'Ma';
while ($res AND $row = $res->fetch_assoc()) {
    $enter_day = date('Y-m-d', strtotime($row[CONGRESS_ENTER]));
    if (!isset($congress_dates[$enter_day])) {
        $congress_dates[$enter_day] = $enter_day;
    }
}
if (isset($check_in)) {
    $mem_id = 0;
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
    $mem_id = 0;
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

if (!isset($selected_day)) {
    $selected_day = $today;
}
$view->submitDropDownChooser('Nap', $congress_dates, 'selected_day', true);
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
    $view->newKeret();
    $view->inputClub('Egyesület', [-1 => 'Válassz!!!'] + getClubNameList(), 'selected_club', true);
    if ($selected_club >= 0) {
        $club = getClub($selected_club);
        $mandat = $club[CLUB_MANDATUM];
        $delegatesOfClub = getDelegatesOfClub($selected_club);
        if ($mandat > 0) {
            if (count($delegatesOfClub) > 1) {
                $view->danger('Több delegált és megadott mandátum konfliktus !');
            } elseif (count($delegatesOfClub) == 0) {
                $view->danger('Az egyesületnek nincs küldött beállítva!');
            }
        }
        foreach ($delegatesOfClub as $del_id) {
            if (!array_key_exists($del_id, $checked_in_delegates)) {
                $view->submitButton('check_in', getMemberName($del_id), $del_id, 'Belép a küldöttgyűlésre.');
            }
        }
        if ($mandat > 0) {
            $view->putText($mandat, 'Mandátum szám');
        }
    }
    $view->newDiv('row');
    $view->inputMemberId('mem_id', 'Azonosító');
    $view->submitButton('check', 'Lekérdezés');
    $view->endDiv();
    if (isValidMemberId($mem_id)) {
        $view->putText(getMemberName($mem_id), 'Név');
        $club_id = getDelegateClub($mem_id);
        if ($club_id > 0) {
            $club = getClub($club_id);
            $view->putText($club[CLUB_NAME], 'Egyesület');
            if ($club[CLUB_MANDATUM] > 0) {
                $view->put('&nbsp;&nbsp;&nbsp;&nbsp;Mandátum szám: ' . $club[CLUB_MANDATUM]);
            }
            if (array_key_exists($mem_id, $checked_in_delegates)) {
                $view->submitButton('check_out', 'Kimegy', $mem_id, 'A küldött elhagyja a termet, kiveszsük a jelenlévő küldöttek listájából.');
            } else {
                $view->submitButton('check_in', 'Bejön', $mem_id, 'A küldött visszajött a gyűlésterembe, ismét részt vesz a szavazásokon');
            }
            $view->putHidden('club_id');
        }
    } else {
        $view->warning('Érvénytelen azonosító');
    }
//$output .= '&nbsp;&nbsp;&nbsp;&nbsp;<button name="refresh" value="' . $mem_id . '">Frissít</button>';
    $view->endKeret();
}
if (!empty($checked_out_delegates)) {
    $view->newKeret();
    $view->putText('<h2>Bent volt, de kiment: ' . count($checked_out_delegates) . '</h2>');
    foreach ($checked_out_delegates as $delegate_id => $delegate_name) {
        if ($selected_day == $today) {
            $view->submitButton('check_in', 'Visszajön', $delegate_id);
        }
        $club_id = getDelegateClub($delegate_id);
        $club = getClub($club_id);
        $output .= '(' . $delegate_id . ') ' . $delegate_name . (isset($club[CLUB_NAME]) ? (' :' . $club[CLUB_NAME]) : '');
        if (isset($club[CLUB_MANDATUM]) AND $club[CLUB_MANDATUM] > 0) {
            $output .= '&nbsp;&nbsp;&nbsp;&nbsp;(' . $club[CLUB_MANDATUM] . ' mandátum)';
        }
    }
    $view->endKeret();
}
$view->newKeret();
if ($selected_day == $today) {
    $view->submitRefresh('refresh');
}
$delegates_sum = 0;
foreach ($checked_in_delegates as $delegate_id => $delegate_name) {
    $club_id = getDelegateClub($delegate_id);
    if ($club_id > 0) {
        $club = getClub($club_id);
        $num = $club[CLUB_MANDATUM];
        $delegates_sum += max($num, 1);
    } else {
        $delegates_sum++;
    }
}
$view->putText('<h2>A teremben : ' . count($checked_in_delegates) . ' küldött '
        . '( ' . $delegates_sum . ' mandátum) '
        . ($selected_day == $today ? 'tartózkodik' : 'tartózkodott') . '</h2>');

foreach ($checked_in_delegates as $delegate_id => $delegate_name) {
    $view->newDiv("row");
    if ($selected_day == $today) {
        $view->submitButton('check_out', 'Kimegy', $delegate_id, $delegate_name . ' kimegy a gyűlésteremből, a szavazásokon nem vesz részt');
    }
    $club_id = getDelegateClub($delegate_id);
    if ($club_id > 0) {
        $club = getClub($club_id);
        $num = $club[CLUB_MANDATUM];
    } else {
        $num = 0;
    }
    $view->putText('(' . $delegate_id . ') ' . $delegate_name . ' : ' . getClubName(getDelegateClub($delegate_id)));
    if ($num > 0) {
        $view->put('&nbsp;&nbsp;&nbsp;&nbsp;(' . $num . ' mandátum)');
    }
    $view->endDiv();
}
$view->endKeret();
