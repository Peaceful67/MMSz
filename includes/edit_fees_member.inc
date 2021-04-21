<?php

if (!isset($editable_fees)) {
    $editable_fees = array();
}

$view->newKeret();

if (in_array(FEE_TYPE_MEMBERSHIP, $editable_fees)) {
    feePeriodSelector($view, FEE_TYPE_MEMBERSHIP, $fee_member_id, $member_types, 'selected_member_name', 'selected_member_period');
}
if (in_array(FEE_TYPE_INSURANCE, $editable_fees)) {
    feePeriodSelector($view, FEE_TYPE_INSURANCE, $fee_member_id, $member_insurances, 'selected_insurance_name', 'selected_insurance_period');
}
if (in_array(FEE_TYPE_ORGANIZATION, $editable_fees) AND count($organizations) > 0) {
    feePeriodSelector($view, FEE_TYPE_ORGANIZATION, $fee_member_id, $organizations, 'selected_organ', 'selected_organ_period');
}
if (in_array(FEE_TYPE_ASSOCIATION, $editable_fees)) {
    feePeriodSelector($view, FEE_TYPE_ASSOCIATION, $fee_member_id, $associations, 'selected_assoc', 'selected_assoc_period');
}

if (in_array(FEE_TYPE_OCCASION, $editable_fees)) {
    feePeriodSelector($view, FEE_TYPE_OCCASION, $fee_member_id, $occasions, 'selected_occasion', 'selected_occasion_period');
}
if (in_array(FEE_TYPE_ADMINISTRATION, $editable_fees)) {
    feePeriodSelector($view, FEE_TYPE_ADMINISTRATION, $fee_member_id, $administrations, 'selected_administration', 'selected_administration_period');
}
/* Nem hasznaljuk. Arra való, hogy a szövetség szedje be az egyesületi tagdíjakat
  if (in_array(FEE_TYPE_CLUB, $editable_fees)) {
  $clubsOfMember = getClubsId($fee_member_id);
  if (!isset($selected_club_fee)) {
  $selected_club_fee = -2;
  }
  if (!isset($selected_club_category)) {
  $selected_club_category = -2;
  }

  $output .= '<label>' . $fee_type[FEE_TYPE_CLUB][0] . ':</label>';
  $output .= '<select name="selected_club_fee" onChange="submit();">';
  $output .= '<option value="-1">Válassz !</option>';
  foreach ($clubsOfMember as $cl_id => $cl_name) {
  $output .= '<option value="' . $cl_id . '" ' . ($selected_club_fee == $cl_id ? "selected" : "") . '>' . $cl_name . '</option>';
  }
  $output .= '</select>';

  if ($selected_club_fee >= 0) {
  $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_club_category" onChange="submit();">';
  $output .= '<option value="-1">Válassz !</option>';
  foreach ($club_categories as $club_cat_id => $club_cat_name) {
  $output .= '<option value="' . $club_cat_id . '" ' . ($selected_club_category == $club_cat_id ? "selected" : "") . '>'
  . $club_cat_name . '</option>';
  }
  $output .= '</select>';
  if ($selected_club_category >= 0) {
  if (!isset($selected_club_period)) {
  $selected_club_period = -1;
  }
  $fees = getFeesOfClub($selected_club_fee, $selected_club_category);
  foreach ($fees as $fee) {
  if (!isFeeMember($fee_member_id, $fee[0]) && ($selected_club_period < 0) && ($fee[1] < $today) && ($fee[2] > $today)) {
  $selected_club_period = $fee[0];
  }
  }

  $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_club_period" onChange="submit();">';
  $output .= '<option value="-1">Válassz !</option>';
  foreach ($fees as $feeOfClub) {
  $output .= '<option value="' . $feeOfClub[0] . '" '
  . ($selected_club_period == $fee[0] ? "selected " : "")
  . '>' . $feeOfClub[1] . ':' . $feeOfClub[2] . ' = ' . $feeOfClub[3] . ' Ft</option>';
  }
  $output .= '</select>';
  $output .= printFeeInfo($fees, $selected_club_period);
  }
  }
  $output .= '<br>';
  } */


$view->newDiv('row')->submitButton('fee_type_save', 'Rögzítés', $edit_member, 'Kiválasztott díjtételek felvétele')->endDiv();
$view->endKeret();
$view->putHidden('edit_member');

function feePeriodSelector($view, $fee_name, $mem_id, $arr, $selected_name, $period_name) {
    if (isFeeNameUsed($fee_name)) {
        global $$selected_name, $$period_name, $today, $fee_type;
        if (!isset($$selected_name)) {
            $$selected_name = getFeeName($mem_id, $today, $fee_name);
        }
        $view->newDiv('form_inline row');
        $view->newDiv('col-md-4')->submitDropDownChooser($fee_type[$fee_name][0], [-1 => 'Válassz !!!'] + $arr, $selected_name, true)->endDiv();
        if ($$selected_name >= 0) {
            if (!isset($$period_name)) {
                $$period_name = -1;
            }
            $periods = [-1 => 'Válassz !!!'];
            $fees = getFees($fee_name, $$selected_name);
            foreach ($fees as $fee) {
                $periods[$fee[0]] = $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft';
                if (!isFeeMember($mem_id, $fee[0]) && ($$period_name < 0) && ($fee[1] < $today) && ($fee[2] > $today)) {
                    $$period_name = $fee[0];
                }
            }
            $view->newDiv('col-sm-4')->submitDropDownChooser('', $periods, $period_name, true)->endDiv();
            if (isFeeChangeableInFM($mem_id, $$period_name)) {
                printFeeInfo($fees, $$period_name);
            } else {
                $view->newDiv('col-sm-4')->danger('Már van díjtétel ebben az időszakban !')->endDiv();
            }
        }
        $view->endDiv();
    }
}

function printFeeInfo($fees, $period) {
    global $today, $endOfYear, $view;
    $ret = '';
    $view->newDiv('col-sm-3');
    foreach ($fees as $fee) {
        if ($period == $fee[0]) {
            if ($fee[1] > $today) {
                $view->info('Jövőbeni időszak !');
                return;
            }
            if ($fee[2] < $today) {
                $view->warning('Elmúlt időszak !');
                return;
            }
            $view->success('Díjtétel kiválasztva !');
        }
    }
    $view->endDiv();
}
