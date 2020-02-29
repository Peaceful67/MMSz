<?php

$output .= '<div class="fees_members"><table>';
/*
 *  FM2BILL format 
 * 0 => FM_ID
 * 1 => FEE type
 * 2 => FEE name
 * 3 => FEE from
 * 4 => FEE to
 * 5 => FEE sum
 */
if (!isset($turn_on)) {
    $turn_on = false;
}
if (!isset($turn_off)) {
    $turn_off = false;
}
foreach ($fms2bill as $fm2bill) {
    $chk = FM_CHECKBOX . '[' . $fm2bill[0] . ']';
    if (!isset($$chk)) {
        $$chk = '';
    }

    $output_chk = '<input type="checkbox" name="' . $chk . '" ';
    $disabled_msg = isReadyForFAI($fm2bill);
    if(!empty($disabled_msg)) {
        $output_chk .= ' disabled ';
    }
    if (($turn_on OR ( $$chk) == "on") AND ! $turn_off) {
        $output_chk .= ' checked ';
        $fms2bill_sum += $fm2bill[5];
        $fms2bill_num++;
        $collected_fms[] = array($fm2bill[0], $fm2bill[5]);
    }
    $output_chk .= ' onchange="submit();" >';
    $output .= '<tr><td>';
    $output .= $output_chk . '</td><td>';
    $output .= $fee_type[$fm2bill[1]][0] . '</td>';
    $output .= '<td>' . get_fee_name($fm2bill[1], $fm2bill[2]) . '</td>';
    $output .= '<td>' . $fm2bill[3] . ':' . $fm2bill[4] . '</td>';
    $output .= '</td><td>' . $fm2bill[5] . ' Ft';
    if(!empty($disabled_msg)) {
        $output .= '<font color="red"> '.$disabled_msg.'</font>';
    }   
    $output .= '</td></tr>';
}
$output .= '</table></div>';

