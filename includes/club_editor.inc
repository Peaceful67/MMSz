<?php

if (!isset($club_name))
    $club_name = '';
if (!isset($club_postcode))
    $club_postcode = '';
if (!isset($club_city))
    $club_city = '';
if (!isset($club_street))
    $club_street = '';
if (!isset($club_email))
    $club_email = '';
if (!isset($club_comment))
    $club_comment = '';
if (!isset($club_homepage))
    $club_homepage = '';
if (!isset($club_act))
    $club_act = '';
if (!isset($club_bank))
    $club_bank = '';

if (isset($cancel)) {
    unset($edit_club);
    return;
}
if (isset($save_club)) {
    if (empty($club_name)) {
        warning('Add meg a klub nevét !');
    } else {
        if ($edit_club < 0) {
            $sql = 'INSERT INTO `' . CLUB_TABLE . '` SET '
                    . ' `' . CLUB_NAME . '`="' . $club_name . '", '
                    . ' `' . CLUB_ADDR_POST . '`="' . $club_postcode . '", '
                    . ' `' . CLUB_ADDR_CITY . '`="' . $club_city . '", '
                    . ' `' . CLUB_ADDR_STREET . '`="' . $club_street . '", '
                    . ' `' . CLUB_EMAIL . '`="' . $club_email . '", '
                    . ' `' . CLUB_HOMEPAGE . '`="' . $club_homepage . '", '
                    . ' `' . CLUB_BANK . '`="' . $club_bank . '", '
                    . ' `' . CLUB_COMMENT . '`="' . $club_comment . '"; ';
            $mysqliLink->query($sql);
            $edit_club = $mysqliLink->insert_id;
            logger($member_id, -1, LOGGER_CLUB_EDIT, $edit_club . ' azonosítójú egyesület létrehozva.');
        } else {
            $sql = 'UPDATE `' . CLUB_TABLE . '` SET '
                    . ' `' . CLUB_NAME . '`="' . $club_name . '", '
                    . ' `' . CLUB_ADDR_POST . '`="' . $club_postcode . '", '
                    . ' `' . CLUB_ADDR_CITY . '`="' . $club_city . '", '
                    . ' `' . CLUB_ADDR_STREET . '`="' . $club_street . '", '
                    . ' `' . CLUB_HOMEPAGE . '`="' . $club_homepage . '", '
                    . ' `' . CLUB_EMAIL . '`="' . $club_email . '", '
                    . ' `' . CLUB_BANK . '`="' . $club_bank . '", '
                    . ' `' . CLUB_COMMENT . '`="' . $club_comment . '", '
                    . ' `' . CLUB_ACT . '`="' . (($club_act == 'on') ? '1' : '0') . '" '
                    . ' WHERE `' . CLUB_ID . '`="' . $edit_club . '"';
            $mysqliLink->query($sql);
            logger($member_id, -1, LOGGER_CLUB_EDIT, $edit_club . ' azonosítójú egyesület adatai módosítva.');
        }
    }
}

if ($edit_club > 0) {
    $res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ID . '`=' . $edit_club . ' LIMIT 1');
    if ($res) {
        $club = $res->fetch_assoc();
        $club_name = $club[CLUB_NAME];
        $club_postcode = $club[CLUB_ADDR_POST];
        $club_city = $club[CLUB_ADDR_CITY];
        $club_street = $club[CLUB_ADDR_STREET];
        $club_email = $club[CLUB_EMAIL];
        $club_comment = $club[CLUB_COMMENT];
        $club_homepage = $club[CLUB_HOMEPAGE];
        $club_bank = $club[CLUB_BANK];
        $club_act = $club[CLUB_ACT] == 1 ? 'on' : 'off';
        $club_leader = getClubLeaders($edit_club);
        $club_president = getClubPresidents($edit_club);
        $club_delegate = getClubDelegates($edit_club);
    }
}

$output .= '<div class="editor"><div class="keret"><form method="post">';
$output .= '<label for="club_name">Klub név:</label><input name="club_name" type="text" value="' . $club_name . '"><br>';
$output .= '<label for="club_postcode">Irányítószám:</label><input name="club_postcode" type="text" value="' . $club_postcode . '"><br>';
$output .= '<label for="club_city">Város:</label><input name="club_city" type="text" value="' . $club_city . '"><br>';
$output .= '<label for="club_street">Utca:</label><input name="club_street" type="text" value="' . $club_street . '"><br>';
$output .= '<label for="club_email">Klub email:</label><input name="club_email" type="text" value="' . $club_email . '"><br>';
$output .= '<label for="club_comment">Megjegyzés:</label><input name="club_comment" type="text" value="' . $club_comment . '"><br>';
$output .= '<label for="club_comment">Honlap:</label><input name="club_homepage" type="text" value="' . $club_homepage . '"><br>';
$output .= '<label for="club_bank">Bankszámla:</label><input name="club_bank" type="text" value="' . $club_bank . '"><br>';
$output .= '<label for="club_act">Aktív:</label><input name="club_act" type="checkbox"  ' . ($club_act == 'on' ? "checked" : "") . '><br>';
if (isset($club_leader)) {
    $output .= '<label>Vezető:</label>' . ((strlen($club_leader) > 1) ? $club_leader : 'Nincs megadva !') . '<br>';
}
if (isset($club_president)) {
    $output .= '<label>Elnök:</label>' . ((strlen($club_president) > 1) ? $club_president : 'Nincs megadva !') . '<br>';
}
if (isset($club_delegate)) {
    $output .= '<label>Küldött:</label>' . ((strlen($club_delegate) > 1) ? $club_delegate : 'Nincs megadva !');
    $output .= ' ' . delegatesNumberToString($edit_club);
    $output .= '<br>';
}


$output .= '<input type="hidden" name="edit_club" value="' . $edit_club . '">'
        . '<input type="submit" name="save_club" value="Rögzít"> '
        . '<input type="submit" name="cancel" value="Mégsem">';
$output .= '</form></div></div>';
