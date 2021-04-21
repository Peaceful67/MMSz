<?php

include_once FUNCTIONS . 'vote_functions.inc';


if (!isset($selected_open_congress)) {
    $selected_open_congress = -1;
}
if (!isset($selected_open_congress_old)) {
    $selected_open_congress_old = -1;
}
if ($selected_open_congress != $selected_open_congress_old) {
    unset($congress_description);
    unset($congress_title);
    unset($congress_part);
    unset($congress_part_old);
    $selected_open_congress_old = $selected_open_congress;
}
if (!isset($congress_part)) {
    $congress_part = 'congress';
}
if (!isset($congress_part_old)) {
    $congress_part_old = '';
}
if (!isset($selected_role)) {
    $selected_role = -1;
}

if (isset($congress_new)) {
    $selected_open_congress = $congressOnline->newCongress();
//    $congress_part_old = ''; // Hogy ne legyen mentes
    $selected_open_congress_old = $selected_open_congress;
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójó szavazás létrhozva');
}
if (isset($congress_delete)) {
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $congress_delete . ' azonosítójó szavazás törölve');
    $congressOnline->deleteCongress($congress_delete);
    $congressDelegates->deleteCongress($congress_delete);
    $congressItems->deleteCongress($congress_delete);
    $selected_open_congress_old = $selected_open_congress = -1;
}

$congressOnline->setSelectedCongress($selected_open_congress);
$congressItems->setSelectedCongress($selected_open_congress);

if ($congress_part == $congress_part_old) {
    include_once INCLUDES . 'congress_save.inc';
    $congressOnline->setSelectedCongress($selected_open_congress);
}
$openCongresses = $congressOnline->getOpenCongresses();

$view->newDiv('row');
$view->submitAdd('congress_new', -1, 'Új szavazás létrhozása');
if (empty($openCongresses)) {
    $selected_open_congress = -1;
} elseif (count($openCongresses) == 1) {
    $selected_open_congress = $selected_open_congress_old = key($openCongresses);
    $congressOnline->setSelectedCongress($selected_open_congress);
    $congressItems->setSelectedCongress($selected_open_congress);
    $view->putText($openCongresses[$selected_open_congress], 'Címe', true);
    $view->putHidden('selected_open_congress');
} else {
    if ($selected_open_congress < 0 OR strlen($congressOnline->getCongressTitle()) >= 5) {
        $view->submitDropDownChooser("Szerkesztendő", [-1 => 'Válassz!!!'] + $openCongresses, 'selected_open_congress', true);
    } else {
        $view->putHidden('selected_open_congress');
    }
}

if ($selected_open_congress > 0) {
    if (!isset($selected_auth1)) {
        $selected_auth1 = $congressOnline->getCongressAuth1();
    }
    if (!isset($selected_auth2)) {
        $selected_auth2 = $congressOnline->getCongressAuth2();
    }
    $view->submitDelete('congress_delete', $selected_open_congress, 'Szavazás törlése', 'Biztosan törlöd a ' . $congressOnline->getCongressTitle() . ' című szavazást?');
}
$view->endDiv();

$congress_ready = true;
$error_arr = array();
if (isset($selected_open_congress) AND $selected_open_congress >= 0) {
    $delegates = $congressDelegates->getMembersOfCongress($selected_open_congress);
    if (count($delegates) < 3) {
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

    if ($congressOnline->getCongressAuth1() > 0 AND ! isset($delegates[$congressOnline->getCongressAuth1()])) {
        $congress_ready = false;
        $error_arr[] = 'Hitelesítő 1 (' . getMemberName($congressOnline->getCongressAuth1()) . ') nincs a kijelölt tagok között';
    }
    if ($congressOnline->getCongressAuth2() > 0 AND ! isset($delegates[$congressOnline->getCongressAuth2()])) {
        $congress_ready = false;
        $error_arr[] = 'Hitelesítő 2 (' . getMemberName($congressOnline->getCongressAuth2()) . ') nincs a kijelölt tagok között';
    }
    if ($congressOnline->isCongressAuthMandatory()) {
        if ($congressOnline->getCongressAuth1() < 0 OR $congressOnline->getCongressAuth2() < 0) {
            $congress_ready = false;
            $error_arr[] = 'Nincs megadva a két jegyzőkönyv hitelesítő!';
        }
    }
    if ($congressOnline->getCongressAuth1() > 0 AND ( $congressOnline->getCongressAuth1() == $congressOnline->getCongressAuth2())) {
        $congress_ready = false;
        $error_arr[] = 'Két különböző jegyzőkönyv hitelesítőt kell megadni!';
    }
    switch ($congressOnline->getMandateType()) {
        case CONGRESS_MANDATE_TYPE_ONE: // Egy küldött, egy szavazat
        default:
            break;
        case CONGRESS_MANDATE_TYPE_RANGE: // A delegált tag egyesületeinek létszáma szerint sávosan
            // Minden résztvevő delegált legalább egy egyeseületben
            $clubs_already = [];
            foreach ($delegates as $mem_id => $mem_name) {
                if (empty($clubs_of_delegate = getClubsOfDelegate($mem_id))) {
                    $congress_ready = false;
                    $error_arr[] = $mem_name . ' (' . $mem_id . ') egyik egyesületnél sem küldött';
                } else {
                    foreach ($clubs_of_delegate as $cl_id => $cl_name) {
                        if (isset($clubs_already[$cl_id])) {
                            $congress_ready = false;
                            $error_arr[] = $cl_name . ' egyesületből újabb küldött:' . $mem_name . ' (' . $mem_id . ')';
                        } else {
                            $clubs_already[$cl_id] = $cl_name;
                        }
                    }
                }
            }
            // Van azonos egyesületből két delegált           
            break;
        case CONGRESS_MANDATE_TYPE_CLUB_2:
            $club_mandat_index = CLUB_MANDATUM2;
        case CONGRESS_MANDATE_TYPE_CLUB: // A delegált tag egyesületeinek mandátum száma szerint
            // Minden résztvevő delegált legalább egy egyeseületben
            // Egy egyesületben nincs megadva mandátum szám
            // Van azonos egyesületből két delegált
            if (!isset($club_mandat_index)) {
                $club_mandat_index = CLUB_MANDATUM;
            }
            $clubs_already = [];
            foreach ($delegates as $mem_id => $mem_name) {
                if (empty($clubs_of_delegate = getClubsOfDelegate($mem_id))) {
                    $congress_ready = false;
                    $error_arr[] = $mem_name . ' (' . $mem_id . ') egyik egyesületnél sem küldött';
                } else {
                    foreach ($clubs_of_delegate as $cl_id => $cl_name) {
                        if (getClub($cl_id)[$club_mandat_index] == 0) {
                            $congress_ready = false;
                            $error_arr[] = $cl_name . ' egyesületnél nincs megadva mandátumszám.';
                        }
                        if (isset($clubs_already[$cl_id])) {
                            $congress_ready = false;
                            $error_arr[] = $cl_name . ' egyesületből újabb küldött:' . $mem_name . ' (' . $mem_id . ')';
                        } else {
                            $clubs_already[$cl_id] = $cl_name;
                        }
                    }
                }
            }
            break;
    }
} else {
    $congress_ready = false;
}
$view->putHidden('congress_part');
$view->putHidden('selected_open_congress_old');
if ($selected_open_congress > 0) {
    $view->newElement('<ul class="nav nav-tabs">');
    $view->newElement('<li class="nav-item">');
    $view->putElement('<button class="btn mx-1 navbar-btn btn-secondary ' . ($congress_part == 'congress' ? 'active' : '') . '" name="congress_part" title="Erről fogunk beszélni" value="congress">Téma</button>');
    $view->endElement('</li>');
    $view->newElement('<li class="nav-item">');
    $view->putElement('<button class="btn mx-1 navbar-btn btn-secondary ' . ($congress_part == 'members' ? 'active' : '') . '" name="congress_part" title="A gyűlés résztvevőinek kiválasztása" value="members">Résztvevők</button>');
    $view->endElement('</li>');
    $view->newElement('<li class="nav-item">');
    $view->putElement('<button class="btn mx-1  navbar-btn btn-secondary ' . ($congress_part == 'items' ? 'active' : '') . '" name="congress_part" title="Napirendi pontok létrehozása, szerkesztése" value="items">Napirend</button>');
    $view->endElement('</li>');
    if ($congress_ready) {
        $view->newElement('<li class="nav-item">');
        $view->putElement('<button class="btn mx-1  navbar-btn btn-secondary ' . ($congress_part == 'summerize' ? 'active' : '') . '" name="congress_part"  value="summerize">Összegzés</button>');
        $view->endElement('</li>');
    }
    $view->endElement('</ul>');
    if (!empty($error_arr)) {
        foreach ($error_arr as $err) {
            $view->danger($err);
        }
    }
    $view->newKeret();
    switch ($congress_part) {
        case 'congress':
            include_once INCLUDES . 'congress_congress.inc';
            break;
        case 'members':
            include_once INCLUDES . 'congress_members.inc';
            break;
        case 'items':
            include_once INCLUDES . 'congress_items.inc';
            break;
        case 'summerize':
            include_once INCLUDES . 'congress_summerize.inc';
            break;
    }
    $view->endKeret();
}

$congress_part_old = $congress_part;
$view->putHidden('congress_part_old');
