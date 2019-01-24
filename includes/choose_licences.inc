<?php

define("CLM_CHK", "lm_chk_");
define("CLM_FROM", "date_from_");
define("CLM_TO", "date_to_");
if(isset($lm_save)) {    
    $log ='';
    foreach ($licences as $key => $value) {
        $chk = CLM_CHK.$key;
        $from = CLM_FROM.$key;
        $to = CLM_TO.$key;
        $sql = 'DELETE FROM `'.LM_TABLE.'` WHERE ' // Mindenkepp kitoroljuk
                . '`'.LM_LICENCE.'`='.$key.' AND `'.LM_MEMBER.'`='.$licences_member_id.';';
        $mysqliLink->query($sql);
        if(isset($$chk) AND $$chk=="on") {
            $sql = 'INSERT INTO `'.LM_TABLE.'` '
                    . '(`'.LM_LICENCE.'`, `'.LM_MEMBER.'`, `'.LM_FROM.'`,`'.LM_TO.'`) VALUES '
                    . '('.$key.', '.$licences_member_id.', "'.$$from.'", "'.$$to.'");';
            $mysqliLink->query($sql);
            $log .= $value.':'.$$from.'-'.$$to.', ';
        } 
    }
    logger($member_id, $licences_member_id, LOGGER_LICENCES, 'Jogosítások beállítva:'.$log);
}

$output .= '<div class="keret">';
$output .= '<form  method="post" enctype="multipart/form-data">';
foreach ($licences as $key => $value) {
    $output .= '<label>' . $value . ':</label>';
    $lm = getLicenceMember($key, $licences_member_id);
    if (empty($lm)) {
        $lm_from = $today;
        $lm_to = addDatePeriod($today, $fee_type[FEE_TYPE_LICENCES][1]);
        $lm_checked = false;
    } else {
        $lm_from = $lm[LM_FROM];
        $lm_to = $lm[LM_TO];
        $lm_checked = true;
    }
    $output .= '<input type="checkbox" name="' . CLM_CHK . $key . '" '.($lm_checked ? "checked":"").'>';
    $output .= '<input type="date" name="'.CLM_FROM. $key . '" size="8" value="' . $lm_from . '">'
            . '<input type="date" name="'.CLM_TO . $key . '" size="8" value="' . $lm_to . '">';

    $output .= '<br>';
}
$output .= '<input type="submit" name="lm_save" value="Rögzítés">';
$output .= '</form></div>';
