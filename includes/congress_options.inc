<?php

$output .= '<h3>Szavazás beállításai</h3>';
$congress_ready = true;
$error_arr = array();
if (isset($selected_open_congress) AND $selected_open_congress >= 0) {
  if($congressDelegates->countOfMembers($selected_open_congress)<2) {
      $congress_ready = false;
      $error_arr[] = 'Túl kevés a szavazásra kijelölt tag.';
  }  
  if(strlen($congressOnline->getCongressTitle($selected_open_congress))<5) {
      $congress_ready = false;
      $error_arr[] = 'Túl rövid küldöttgyűlés címe.';
  }
  if(strlen($congressOnline->getCongressDescription($selected_open_congress))<20) {
      $congress_ready = false;
      $error_arr[] = 'Túl rövid küldöttgyűlés Leírása.';
  }
} else {
    $error_arr[] = 'Beslő hiba';
    $congress_ready = false;
}

if(!empty($error_arr)) {
    output_label('Hiányok:');    output_ln();
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

