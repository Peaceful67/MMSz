<?php

$szamlazz_str = isset($_SESSION[SESSION_SZAMLAZZ_RESULT]) ? $_SESSION[SESSION_SZAMLAZZ_RESULT]:'';
$output .= '<div class="keret">';
if (empty($szamlazz_str)) {
    $output .= 'A számla kiállítása sikertelen volt, azt utólag kell kiállítani !';
} else {
    $res = unserialize($szamlazz_str);
    $szamlazz = $res['szamla'];
    $pdf = $res['pdf_file'];
    $output .= 'A számla kiállítása sikeres volt<br><br>';
    if(!empty($szamlazz[SZAMLAZZ_EMAIL])) {
        $output .='A számlát a Szamlazz.hu a '.$szamlazz[SZAMLAZZ_EMAIL].' email címre elküldte.<br><br>';
    }
    $output .= '<a href="'.$pdf.'" target="_blank">Innen</a> később is letölthető.';
}
$output .= '</div>';