<?php


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
if ($search_fee_type == FEE_TYPE_CLUB) {
    if (!isset($search_fee_selected_club)) {
        $search_fee_selected_club = -1;
    }
    $output .= 'Egyesület: <select  name="search_fee_selected_club" onChange="submit();">';
    $output .= '<option value="-1">Válassz egyesületet !</option>';
    $search_fee_clubs = getClubsWithFeeInYear($search_fee_year);
    foreach ($search_fee_clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $search_fee_selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }
    $output .= '</select>&nbsp;&nbsp;&nbsp;';
    if ($search_fee_selected_club > 0) {
        if (!isset($search_fee_club_category)) {
            $search_fee_club_category = -1;
        }
        $search_fee_clubfees = getFeesOfClubInYear($search_fee_selected_club, $search_fee_year);
        $output .= 'Kategória: <select name="search_fee_club_category" onChange="submit();">';
        $output .= '<option value=-1 ' . (($search_fee_club_category == -1) ? ' selected ' : '') . '>Válassz !</option>';
        foreach ($search_fee_clubfees as $clubfee) {
            $output .= '<option value=' . $clubfee[FEE_ID] . '  ' . (($search_fee_club_category == $clubfee[FEE_ID]) ? ' selected ' : '') . '>'
                    . $club_categories[$clubfee[FEE_NAME]] . '</option>';
        }
        $output .= '</select>';
        if ($search_fee_club_category > 0) {
            $selected_fee_id = $search_fee_club_category;
        }
    }
} else {
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

    if ($search_fee_name >= 0) {
        $search_fee = getFee($search_fee_type, $search_fee_name, $search_fee_year . '-01-01');
        if (!empty($search_fee)) {
            $output .= $search_fee[FEE_SUM] . ' Ft';
            $selected_fee_id = $search_fee[FEE_ID];
        }
    }
}
$output .= '&nbsp;&nbsp;&nbsp;';
if ($selected_fee_id > 0) {
    $output .= '<font color="green">A díjtétel kiválasztva ! ('.$selected_fee_id.')</font>';
} else {
    $output .= '<font color="red">Válaszd ki a díjtételt !</font>';
}
$output .= '<br><br>';


