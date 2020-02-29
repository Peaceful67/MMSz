<?php

if (!isset($turn_on))
    $turn_on = false;
if (!isset($turn_off))
    $turn_off = false;

foreach ($fms2bill as $fm2bill) {
    error_log("count fm:".$fm2bill[0]);
    $all_fms2bill[] = $fm2bill;
    $chk = FM_CHECKBOX . '[' . $fm2bill[0] . ']';
    if (!isset($$chk)) {
        $$chk = '';
    }
    if (($turn_on OR ( $$chk) == "on") AND ! $turn_off) {
        $fms2bill_num++;
        $fms2bill_sum += $fm2bill[5];
    }
}


$output_chk = '<input type="checkbox" name="' . $chk . '" ';

