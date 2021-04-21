<?php

$output .= '<h3>Szavazás beállításai</h3>';
$congress_ready = true;
$error_arr = array();
if (isset($selected_open_congress) AND $selected_open_congress >= 0) {
    if ($congressDelegates->countOfMembers($selected_open_congress) < 2) {
        $congress_ready = false;
        $error_arr[] = 'Túl kevés a szavazásra kijelölt tag.';
    }
    if (strlen($congressOnline->getCongressTitle($selected_open_congress)) < 5) {
        $congress_ready = false;
        $error_arr[] = 'Túl rövid küldöttgyűlés címe.';
    }
    $mandatedClubs = array();
    foreach (getMandatedClubsofDelegate($selected_open_congress) as $club) {
        error_log('Mandated: '.$club[CLUB_NAME]);
        if(isset($mandatedClubs[$club[CLUB_ID]])) {
                $congress_ready = false;
                $error_arr[] = 'A '.$club[CLUB_NAME].' egyesület többszörös mandátummal rendelkezik és több küldött is tartozik hozzá';
        } else {
            $mandatedClubs[$club[CLUB_ID]] = true;
        }
    }
    if (strlen($congressOnline->getCongressDescription($selected_open_congress)) < 20) {
        $congress_ready = false;
        $error_arr[] = 'Túl rövid küldöttgyűlés Leírása.';
    }
} else {
    $error_arr[] = 'Beslő hiba';
    $congress_ready = false;
}

if (!empty($error_arr)) {
    output_label('Hiányok:');
    output_ln();
    foreach ($error_arr as $err) {
        $output .= $err;
        output_ln();
    }
}
if ($congress_ready) {
    $output .= '<button name="close_congress">Lezárás</button>';
} else {
    $output .= '<button name="save_congress">Mentés</button>';
}

