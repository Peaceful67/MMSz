<?php

$view->submitDropDownChooser('Év', $years, SEARCH_FEE_YEAR);
$view->submitDropDownChooser('Típus', [-1 => ['Válassz!!!', 0]] + $fee_type, SEARCH_FEE_TYPE);
$fee_sum = -1;
if ($search_fee_type == FEE_TYPE_CLUB) {
    if (!isset($search_fee_selected_club)) {
        $search_fee_selected_club = -1;
    }
    $search_fee_clubs = getClubsWithFeeInYear($search_fee_year);
    if ($search_fee_selected_club > 0) {
        if (!isset($search_fee_club_category)) {
            $search_fee_club_category = -1;
        }
        $search_fee_clubfees = getFeesOfClubInYear($search_fee_selected_club, $search_fee_year);
        $view->submitDropDownChooser('Kategória', [-1 => 'Válassz !!!'] + $search_fee_clubfees, 'search_fee_club_category');
        if ($search_fee_club_category > 0) {
            $selected_fee_id = $search_fee_club_category;
        } else {
            $selected_fee_id = -1;
        }
    }
} else {
    if ($search_fee_type >= 0) {
        $view->submitDropDownChooser('Típus', [-1 => 'Válassz !!!'] + get_list_fee_name($search_fee_type), SEARCH_FEE_NAME);
    } else {
        $search_fee_name = -1;
    }

    if ($search_fee_name >= 0) {
        $search_fee = getFee($search_fee_type, $search_fee_name, $search_fee_year . '-01-01');
        if (!empty($search_fee)) {
            $fee_sum = $search_fee[FEE_SUM];
            $selected_fee_id = $search_fee[FEE_ID];
        }
    } else {
        $selected_fee_id = -1;
    }
}
if ($selected_fee_id > 0) {
    $view->success('A díjtétel kiválasztva ! (id: ' . $selected_fee_id . ') ' . ($fee_sum >= 0 ? ($fee_sum . ' Ft') : ''));
} else {
    $view->warning('Válaszd ki a díjtételt !');
}
$view->putHidden('selected_fee_id');


