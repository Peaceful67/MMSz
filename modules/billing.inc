<?php

if(!isset($fees_in_bill)) {
    $fees_in_bill = array();    
}
$output .= '<div class="keret">';
$output .= '<form  method="post" enctype="multipart/form-data">';
$output .= '<input type="hidden" name="fees_in_bill[]" value="'.$fees_in_bill.'">';
$output .= '<input value="Hozzáadás"  type="submit" name="add_fee">';

$output .= '</form></div>';

