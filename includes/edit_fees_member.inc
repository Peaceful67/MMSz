<?php

if (!isset($editable_fees)) {
    $editable_fees = array();
}

$output .= '<div class="keret">';
if (in_array(FEE_TYPE_MEMBERSHIP, $editable_fees)) {
    if (!isset($selected_member_name)) {
        $selected_member_name = getFeeName($fee_member_id, $today, FEE_TYPE_MEMBERSHIP);
    }

    $output .= '<label>' . $fee_type[FEE_TYPE_MEMBERSHIP][0] . ':</label><select name="selected_member_name" onChange="submit();">';
    $output .= '<option value="-1">Válassz !</option>';
    foreach ($member_types as $index => $name) {
        $output .= '<option value="' . $index . '"'
                . ($selected_member_name == $index ? " selected " : "")
                . '>' . $name . '</option>';
    }
    $output .= '</select>';
    if ($selected_member_name >= 0) {
        $fees = getFees(FEE_TYPE_MEMBERSHIP, $selected_member_name);
        if (!isset($selected_member_period)) {
            $selected_member_period = -1;
        }
        foreach ($fees as $fee) {
            if (!isFeeMember($fee_member_id, $fee[0]) && ($selected_member_period < 0) && ($fee[1] < $today) && ($fee[2] > $today)) {
                $selected_member_period = $fee[0];
            }
        }

        $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_member_period">';
        $output .= '<option value="-1">Válassz !</option>';
        foreach ($fees as $fee) {
                $output .= '<option value="' . $fee[0] . '" ' .
                        ($selected_member_period == $fee[0] ? "selected" : "") . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
        }
        $output .= '</select>';
    }
    $output .= '<br>';
}
if (in_array(FEE_TYPE_INSURANCE, $editable_fees)) {
    if (!isset($selected_insurance_name)) {
        $selected_insurance_name = getFeeName($fee_member_id, $today, FEE_TYPE_INSURANCE);
    }
    $output .= '<label>' . $fee_type[FEE_TYPE_INSURANCE][0] . ':</label><select name="selected_insurance_name" onChange="submit();">';
    $output .= '<option value="-1">Válassz !</option>';
    foreach ($member_insurances as $index => $name) {
        $output .= '<option value="' . $index . '"'
                . ($selected_insurance_name == $index ? " selected " : "")
                . '>' . $name . '</option>';
    }
    $output .= '</select>';
    if ($selected_insurance_name >= 0) {
        if (!isset($selected_insurance_period)) {
            $selected_insurance_period = -1;
        }
        $fees = getFees(FEE_TYPE_INSURANCE, $selected_insurance_name);
        foreach ($fees as $fee) {
            if (!isFeeMember($fee_member_id, $fee[0]) && ($selected_insurance_period < 0) && ($fee[1] < $today) && ($fee[2] > $today)) {
                $selected_insurance_period = $fee[0];
            }
        }

        $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_insurance_period">';
        $output .= '<option value="-1">Válassz !</option>';
        foreach ($fees as $fee) {
            $output .= '<option value="' . $fee[0] . '" '
                    . ($selected_insurance_period == $fee[0] ? " selected " : "")
                    . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
        }
        $output .= '</select>';
    }
    $output .= '<br>';
}
if (in_array(FEE_TYPE_ORGANIZATION, $editable_fees)) {
    if (!isset($selected_organ)) {
        $selected_organ = getFeeName($fee_member_id, $today, FEE_TYPE_ORGANIZATION);
    }
    $output .= '<label>' . $fee_type[FEE_TYPE_ORGANIZATION][0] . ':</label><select name="selected_organ" onChange="submit();">';
    $output .= '<option value="-1">Válassz !</option>';
    foreach ($organizations as $index => $name) {
        $output .= '<option value="' . $index . '"'
                . ($selected_organ == $index ? " selected " : "")
                . '>' . $name . '</option>';
    }
    $output .= '</select>';
    if ($selected_organ >= 0) {
        if (!isset($selected_organ_period)) {
            $selected_organ_period = -1;
        }
        $fees = getFees(FEE_TYPE_ORGANIZATION, $selected_organ);
        foreach ($fees as $fee) {
            if (!isFeeMember($fee_member_id, $fee[0]) && ($selected_organ_period < 0) && ($fee[1] < $today) && ($fee[2] > $today)) {
                $selected_organ_period = $fee[0];
            }
        }

        $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_organ_period" onChange="submit();">';
        $output .= '<option value="-1">Válassz !</option>';
        foreach ($fees as $fee) {
            $output .= '<option value="' . $fee[0] . '" '
                    . ($selected_organ_period == $fee[0] ? "selected " : "")
                    . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
        }
        $output .= '</select>';
    }
    $output .= '<br>';
}
$organ_licence = retriveMember($fee_member_id)[MEMBER_LICENCE];
$output .= '<label>Nemzetközi azonosító:</label><input  style="width: 150px;" type="text" name="organ_licence" value="' . $organ_licence . '"><br>';

if (in_array(FEE_TYPE_ASSOCIATION, $editable_fees)) {
    if (!isset($selected_assoc)) {
        $selected_assoc = -2;
    }
    $output .= '<label>' . $fee_type[FEE_TYPE_ASSOCIATION][0] . ':</label>';
    $output .= '<select name="selected_assoc" onChange="submit();">';
    $output .= '<option value="-1">Válassz !</option>';
    foreach ($associations as $assoc_id => $assoc_name) {
        $output .= '<option value="' . $assoc_id . '" ' . ($selected_assoc == $assoc_id ? "selected" : "") . '>' . $assoc_name . '</option>';
    }
    $output .= '</select>';
    if ($selected_assoc >= 0) {
        if (!isset($selected_assoc_period)) {
            $selected_assoc_period = -1;
        }
        $fees = getFees(FEE_TYPE_ASSOCIATION, $selected_assoc);
        foreach ($fees as $fee) {
            if (!isFeeMember($fee_member_id, $fee[0]) && ($selected_assoc_period < 0) && ($fee[1] < $today) && ($fee[2] > $today)) {
                $selected_assoc_period = $fee[0];
            }
        }

        $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_assoc_period" onChange="submit();">';
        $output .= '<option value="-1">Válassz !</option>';
        foreach ($fees as $fee) {
            $output .= '<option value="' . $fee[0] . '" '
                    . ($selected_assoc_period == $fee[0] ? "selected " : "")
                    . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
        }
        $output .= '</select>';
    }
    $output .= '<br>';
}

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

            $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_club_period">';
            $output .= '<option value="-1">Válassz !</option>';
            foreach ($fees as $feeOfClub) {
                $output .= '<option value="' . $feeOfClub[0] . '" '
                        . ($selected_club_period == $fee[0] ? "selected " : "")
                        . '>' . $feeOfClub[1] . ':' . $feeOfClub[2] . ' = ' . $feeOfClub[3] . ' Ft</option>';
            }
            $output .= '</select>';
        }
    }
    $output .= '<br>';
}

if (in_array(FEE_TYPE_OCCASION, $editable_fees)) {
    $output .= '<label>' . $fee_type[FEE_TYPE_OCCASION][0] . ':</label>';
    if (!isset($selected_occasion)) {
        $selected_occasion = getFeeName($fee_member_id, $today, FEE_TYPE_OCCASION);
    }
    $output .= '<select name="selected_occasion" onChange="submit();">';
    $output .= '<option value="-1">Válassz !</option>';
    foreach ($occasions as $occ_id => $occ_name) {
        $output .= '<option value="' . $occ_id . '" ' . ($selected_occasion == $occ_id ? "selected" : "") . '>' . $occ_name . '</option>';
    }
    $output .= '</select>';
    if ($selected_occasion >= 0) {
        if (!isset($selected_occasion_period)) {
            $selected_occasion_period = -1;
        }
        $fees = getFees(FEE_TYPE_OCCASION, $selected_occasion);
        foreach ($fees as $fee) {
            if (!isFeeMember($fee_member_id, $fee[0]) && ($selected_occasion_period < 0) && ($fee[1] < $today) && ($fee[2] > $today)) {
                $selected_occasion_period = $fee[0];
            }
        }

        $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_occasion_period" onChange="submit();">';
        $output .= '<option value="-1">Válassz</option>';
        foreach ($fees as $fee) {
            $output .= '<option value="' . $fee[0] . '" '
                    . (($selected_occasion_period == $fee[0]) ? "selected " : "")
                    . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
        }
        $output .= '</select>';
    }
    $output .= '<br>';
}

$output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
$output .= '<br><input value="Rögzítés"  type="submit" name="fee_type_save">';
$output .= '</div>';

