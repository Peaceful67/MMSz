<?php

$output .= '<div class="keret">';
$output .= 'Év: <select name="search_fee_year"  onchange="submit();">';
$selected_fee_id = -1;
for ($y = YEAR_FROM; $y <= YEAR_TO; $y++) {
    $output .= '<option value="' . $y . '" ' . (($y == $search_fee_year) ? "selected" : "") . '>' . $y . '</option>';
}
$output .= '</select>&nbsp;&nbsp;&nbsp;';
$output .= 'Típus: <select name="search_fee_type" onChange="submit();">';
$output .= '<option value=-1 ' . (($search_fee_type == -1) ? ' selected ' : '') . '>Válassz !</option>';
foreach ($fee_type as $key => $value) {
    $output .= '<option value=' . $key . '  ' . (($search_fee_type == $key) ? ' selected ' : '') . '>'
            . $value[0] . '</option>';
}
$output .= '</select>&nbsp;&nbsp;&nbsp;';
if ($search_fee_type >= 0) {

    $output .= 'Típus: <select name="search_fee_name" onChange="submit();">';
    $output .= '<option value=-1 ' . (($search_fee_name == -1) ? ' selected ' : '') . '>Válassz !</option>';
    foreach (get_list_fee_name($search_fee_type) as $key => $value) {
        $output .= '<option value=' . $key . '  ' . (($search_fee_name == $key) ? ' selected ' : '') . '>'
                . $value . '</option>';
    }
    $output .= '</select>&nbsp;&nbsp;&nbsp;';
} else {
    $search_fee_name = -1;
}
if($search_fee_name >= 0) {
    $search_fee = getFee($search_fee_type, $search_fee_name, $search_fee_year.'-01-01');
    if(!empty($search_fee)) {
        $output .= $search_fee[FEE_SUM].' Ft';
        $selected_fee_id = $search_fee[FEE_ID];
    }
}
$output .= '<br><br>';
if ($selected_fee_id > 0) {
    $output .= '<font color="green">A díjtétel kiválasztva !</font>';
} else {
    $output .= '<font color="red">Válaszd ki a díjtételt !</font>';
}

$output .= '</div>';

