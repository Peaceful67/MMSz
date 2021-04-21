<?php

$szamlazz_str = isset($_SESSION[SESSION_SZAMLAZZ_RESULT]) ? $_SESSION[SESSION_SZAMLAZZ_RESULT]:'';
$view->newKeret();
if (empty($szamlazz_str)) {
    $view->danger('A számla kiállítása sikertelen volt, azt utólag kell kiállítani !');
} else {
    $res = unserialize($szamlazz_str);
    $szamlazz = $res['szamla'];
    $pdf = $res['pdf_file'];
    $view->success('A számla kiállítása sikeres volt.');
    if(!empty($szamlazz[SZAMLAZZ_EMAIL])) {
        $view->info('A számlát a Szamlazz.hu a '.$szamlazz[SZAMLAZZ_EMAIL].' email címre elküldte.');
    }
    $view->info('<a href="'.$pdf.'" target="_blank">Innen</a> is letölthető.');
}
$view->endKeret();