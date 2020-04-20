<?php

include_once FUNCTIONS . 'vote_functions.inc';

if (!isset($selected_open_congress)) {
    $selected_open_congress = -1;
}
if (!isset($selected_open_congress_old)) {
    $selected_open_congress_old = $selected_open_congress;
}
if ($selected_open_congress != $selected_open_congress_old) {
    unset($congress_description);
    unset($congress_title);
    unset($congress_part);
    $selected_open_congress_old = $selected_open_congress;
}
if (!isset($congress_part)) {
    $congress_part = '';
}
if (isset($congress_new)) {
    $selected_open_congress_old = $selected_open_congress = $congressOnline->newCongress();
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójó szavazás létrhozva');
}
if (isset($congress_delete)) {
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójó szavazás törölve');
    $congressOnline->deleteCongress($selected_open_congress);
    $congressDelegates->deleteCongress($selected_open_congress);
    $congressItems->deleteCongress($selected_open_congress);
    $selected_open_congress_old = $selected_open_congress = -1;
}
$output .= '<div class="editor">';
$openCongresses = $congressOnline->getOpenCongresses();
$congressOnline->setCongressEdit($selected_open_congress);
$congressItems->setSelectedCongress($selected_open_congress);
output_hidden('congress_part');
output_hidden('selected_open_congress');
output_hidden('selected_open_congress_old');
if (!empty($openCongresses)) {
    output_label("Szerkesztendő");
    output_selector($openCongresses, 'open_congress');
} else {
    $output .= 'Nincs nyitott szavazás !';
}
output_spaces(5);
$output .= '<button title="Új szavazás létrhozása" name="congress_new"><img src="' . IMAGES . 'add.jpg"></button>';
if ($selected_open_congress > 0) {
    output_spaces(5);
    $output .= '<button title="Szavazás törlése" name="congress_delete"><img src="' . IMAGES . 'delete.jpg"></button>';
}
output_ln(2);
$congress_ready = true;
$error_arr = array();
if (isset($selected_open_congress) AND $selected_open_congress >= 0) {
    if ($congressDelegates->countOfMembers($selected_open_congress) < 3) {
        $congress_ready = false;
        $error_arr[] = 'Túl kevés a szavazásra kijelölt tag.';
    }
    if (strlen($congressOnline->getCongressTitle()) < 5) {
        $congress_ready = false;
        $error_arr[] = 'Túl rövid küldöttgyűlés címe.';
    }
    if (strlen($congressOnline->getCongressDescription()) < 20) {
        $congress_ready = false;
        $error_arr[] = 'Túl rövid küldöttgyűlés Leírása.';
    }
    if (empty($congressItems->getSelectionOfCongress())) {
        $congress_ready = false;
        $error_arr[] = 'Nincsenek napirendi pontok!';
    }
    if ($congressOnline->isCongressAuthMandatory()) {
        if ($congressOnline->getCongressAuth1() < 0 OR $congressOnline->getCongressAuth2() < 0) {
            $congress_ready = false;
            $error_arr[] = 'Nincs megadva a két jegyzőkönyv hitelesítő!';
        }
    }
    if(!is_writable(CONGRESS)) {
        $error_arr[] = 'Nem írható a dokumentum könyvtár';
        $congress_ready = false;
    }
} else {
    $congress_ready = false;
}
if ($selected_open_congress > 0) {
    $output .= '<div class="keret">';
    $output .= '<button title="Erről fogunk beszélni" name="congress_part" '
            . (($congress_part == 'congress') ? ' class="pressed_button" ' : '')
            . ' value="congress">Téma</button>';
    output_spaces(5);
    $output .= '<button name="congress_part" '
            . (($congress_part == 'members') ? ' class="pressed_button" ' : '')
            . (($selected_open_congress < 0) ? ' disabled ' : '')
            . ' value="members">Résztvevők</button>';
    output_spaces(5);
    $output .= '<button name="congress_part" '
            . (($congress_part == 'items') ? ' class="pressed_button" ' : '')
            . (($selected_open_congress < 0) ? ' disabled ' : '')
            . ' value="items">Napirend</button>';
    output_spaces(5);
    if ($congress_ready) {
        output_spaces(5);
        $output .= '<button name="congress_part" '
                . (($congress_part == 'summerize') ? ' class="pressed_button" ' : '')
                . (($selected_open_congress < 0) ? ' disabled ' : '')
                . ' value="summerize">Összegzés</button>';
    }
    $output .= '</div>';

    switch ($congress_part) {
        case 'congress':
            $output .= '<div class="keret">';
            include_once INCLUDES . 'congress_congress.inc';
            $output .= '</div>';
            break;
        case 'members':
            $output .= '<div class="keret">';
            include_once INCLUDES . 'congress_members.inc';
            $output .= '</div>';
            break;
        case 'items':
            $output .= '<div class="keret">';
            include_once INCLUDES . 'congress_items.inc';
            $output .= '</div>';
            break;
        case 'summerize':
            $output .= '<div class="keret">';
            include_once INCLUDES . 'congress_summerize.inc';
            $output .= '</div>';
            break;
    }
}
$output .= '</div>';
