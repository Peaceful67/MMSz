<?php

if (isset($delete_fm)) {
    $fm = getFmById($delete_fm);
    $sql = 'DELETE FROM `' . FM_TABLE . '` WHERE `' . FM_ID . '`="' . $delete_fm . '" LIMIT 1';
    $mysqliLink->query($sql);
    logger($member_id, $fee_member_id, LOGGER_FM_EDIT, $fm[FM_FROM] . '-' . $fm[FM_TO]
            . ' időszakbani tagságviszony törlése.');
    reloadEditMember();
}
if (isset($fee_type_save)) {
    $isSucceed = false;
    if (isset($selected_member_name) && $selected_member_name >= 0) {
        if ($selected_member_period > 0) {
            if (isFeeChangeableInFM($fee_member_id, $selected_member_period)) {
                deleteFeeNameIfExists($fee_member_id, FEE_TYPE_MEMBERSHIP, $selected_member_period);
                $fee = getFeeById($selected_member_period);
                $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`, `' . FM_FEE_ID . '` , `' . FM_FROM . '`, `' . FM_TO . '`)
                    VALUES ("' . $fee_member_id . '", "' . $selected_member_period . '",'
                        . ' "' . $fee[FEE_FROM] . '", "' . $fee[FEE_TO] . '")';
                $mysqliLink->query($sql);
                $isSucceed = true;
                logger($member_id, $fee_member_id, LOGGER_FM_EDIT, $fee_type[FEE_TYPE_MEMBERSHIP][0]
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $member_types[$fee[FEE_NAME]]);
                unset($selected_member_period);
            } else {
                warning("Már van nem törölhető " . COMPANY_NAME . " tagsági díjtétel ebben az időszakban !");
            }
        }
    }
    if (isset($selected_insurance_name) && $selected_insurance_name >= 0) {
        if ($selected_insurance_period > 0) {
            if (isFeeChangeableInFM($fee_member_id, $selected_insurance_period)) {
                deleteFeeNameIfExists($fee_member_id, FEE_TYPE_INSURANCE, $selected_insurance_period);
                $fee = getFeeById($selected_insurance_period);
                $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`, `' . FM_FEE_ID . '` , `' . FM_FROM . '`, `' . FM_TO . '`)
                    VALUES ("' . $fee_member_id . '", "' . $selected_insurance_period . '",'
                        . ' "' . $fee[FEE_FROM] . '", "' . $fee[FEE_TO] . '")';
                $mysqliLink->query($sql);
                $isSucceed = true;
                logger($member_id, $fee_member_id, LOGGER_FM_EDIT, $fee_type[FEE_TYPE_INSURANCE][0]
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $member_types[$fee[FEE_NAME]]);
                unset($selected_insurance_period);
            } else {
                warning("Már van nem törölhető biztosítási díjtétel ebben az időszakban !");
            }
        }
    }
    if (isset($selected_organ) && $selected_organ >= 0) {
        if ($selected_organ_period > 0) {
            if (isFeeChangeableInFM($fee_member_id, $selected_organ_period)) {
                deleteFeeNameIfExists($fee_member_id, FEE_TYPE_ORGANIZATION, $selected_organ_period);
                $fee = getFeeById($selected_organ_period);
                $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`, `' . FM_LICENCE . '`,`' . FM_FEE_ID . '` , `' . FM_FROM . '`, `' . FM_TO . '`)
                    VALUES ("' . $fee_member_id . '", "' . $organ_licence . '","' . $selected_organ_period . '",'
                        . ' "' . $fee[FEE_FROM] . '", "' . $fee[FEE_TO] . '")';
                $mysqliLink->query($sql);
                $isSucceed = true;
                logger($member_id, $fee_member_id, LOGGER_FM_EDIT, $fee_type[FEE_TYPE_ORGANIZATION][0]
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $member_types[$fee[FEE_NAME]]);
                unset($selected_organ_period);
            } else {
                warning("Már van nem törölhető " . $fee_type[FEE_TYPE_ORGANIZATION][0] . " díjtétel ebben az időszakban !");
            }
        }
    }
    if (isset($selected_assoc) && $selected_assoc >= 0) {
        if ($selected_assoc_period > 0) {
            if (isFeeChangeableInFM($fee_member_id, $selected_assoc_period)) {
                deleteFeeNameIfExists($fee_member_id, FEE_TYPE_ASSOCIATION, $selected_assoc_period);
                $fee = getFeeById($selected_assoc_period);
                $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`,`' . FM_FEE_ID . '` , `' . FM_FROM . '`, `' . FM_TO . '`)
                    VALUES ("' . $fee_member_id . '","' . $selected_assoc_period . '",'
                        . ' "' . $fee[FEE_FROM] . '", "' . $fee[FEE_TO] . '")';
                $mysqliLink->query($sql);
                $isSucceed = true;
                logger($member_id, $fee_member_id, LOGGER_FM_EDIT, $fee_type[FEE_TYPE_ASSOCIATION][0]
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $member_types[$fee[FEE_NAME]]);
                unset($selected_assoc_period);
            } else {
                warning("Már van nem törölhető " . $fee_type[FEE_TYPE_ASSOCIATION][0] . " díjtétel ebben az időszakban !");
            }
        }
    }

    if (isset($selected_club_fee) && $selected_club_fee >= 0) {
        if ($selected_club_category >= 0 && $selected_club_period >= 0) {
            if (isClubFeeChangeableInFM($fee_member_id, $selected_club_period)) {
                deleteClubFeeIfExists($fee_member_id, $selected_club_fee, $selected_club_period);
                $fee = getFeeById($selected_club_period);
                $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`, `' . FM_FEE_ID . '` , `' . FM_FROM . '`, `' . FM_TO . '`)
                    VALUES ("' . $fee_member_id . '", "' . $selected_club_period . '",'
                        . ' "' . $fee[FEE_FROM] . '", "' . $fee[FEE_TO] . '")';
                $mysqliLink->query($sql);
                $isSucceed = true;
                logger($member_id, $fee_member_id, LOGGER_FM_EDIT, $fee_type[FEE_TYPE_CLUB][0]
                        . '(' . $selected_club_fee . ') ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $club_categories[$fee[FEE_NAME]]);
                unset($selected_club_period);
            } else {
                warning("Már van nem törölhető tagsági díjtétele ennek az egyesületnek ebben az időszakban !");
            }
        }
    }
    if ($organ_licence != retriveMember($fee_member_id)[MEMBER_LICENCE]) {
        $isSucceed = true;
        $mysqliLink->query('UPDATE `' . MEMBER_TABLE . '` SET `' . MEMBER_LICENCE . '`="' . $organ_licence . '" '
                . ' WHERE `' . MEMBER_ID . '`="' . $fee_member_id . '"');
        logger($member_id, $fee_member_id, LOGGER_MEMBER_MOD, "Új nemzetközi azonosító: " . $organ_licence);
    }
    if (!$isSucceed) {
        warning("Nem történt rögzítés, mert nem módosítottál semmit !");
    }
}

$output .= '<form method="post">';
$output .= '<div class="keret">';
if (!isset($selected_member_name)) {
    $selected_member_name = getFeeName($fee_member_id, $today, FEE_TYPE_MEMBERSHIP);
}

$output .= '<label>' . $fee_type[FEE_TYPE_MEMBERSHIP][0] . ':</label><select name="selected_member_name" onChange="submit();">';
$output .= '<option value="-1">Válassz</option>';
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
    $output .= '<option value="-1">Válassz</option>';
    foreach ($fees as $fee) {
        $output .= '<option value="' . $fee[0] . '" ' .
                ($selected_member_period == $fee[0] ? "selected" : "") . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
    }
    $output .= '</select>';
}
$output .= '<br>';


if (!isset($selected_insurance_name)) {
    $selected_insurance_name = getFeeName($fee_member_id, $today, FEE_TYPE_INSURANCE);
}
$output .= '<label>' . $fee_type[FEE_TYPE_INSURANCE][0] . ':</label><select name="selected_insurance_name" onChange="submit();">';
$output .= '<option value="-1">Válassz</option>';
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
    $output .= '<option value="-1">Válassz</option>';
    foreach ($fees as $fee) {
        $output .= '<option value="' . $fee[0] . '" '
                . ($selected_insurance_period == $fee[0] ? " selected " : "")
                . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
    }
    $output .= '</select>';
}
$output .= '<br>';

if (!isset($selected_organ)) {
    $selected_organ = getFeeName($fee_member_id, $today, FEE_TYPE_ORGANIZATION);
}
$output .= '<label>' . $fee_type[FEE_TYPE_ORGANIZATION][0] . ':</label><select name="selected_organ" onChange="submit();">';
$output .= '<option value="-1">Válassz</option>';
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
    $output .= '<option value="-1">Válassz</option>';
    foreach ($fees as $fee) {
        $output .= '<option value="' . $fee[0] . '" '
                . ($selected_organ_period == $fee[0] ? "selected " : "")
                . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
    }
    $output .= '</select>';
}
//    $organ_licence = getFeeOfMember($fee_member_id, getFeeById($selected_organ_period)[FEE_FROM], FEE_TYPE_ORGANIZATION)[FM_LICENCE];
$organ_licence = retriveMember($fee_member_id)[MEMBER_LICENCE];
$output .= '<br><label>Nemzetközi azonosító:</label><input  style="width: 150px;" type="text" name="organ_licence" value="' . $organ_licence . '">';

if (!isset($selected_assoc)) {
    $selected_assoc = -2;
}
$output .= '<br><label>' . $fee_type[FEE_TYPE_ASSOCIATION][0] . ':</label>';
$output .= '<select name="selected_assoc" onChange="submit();">';
$output .= '<option value="-1">Válassz</option>';
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
    $output .= '<option value="-1">Válassz</option>';
    foreach ($fees as $fee) {
        $output .= '<option value="' . $fee[0] . '" '
                . ($selected_assoc_period == $fee[0] ? "selected " : "")
                . '>' . $fee[1] . ':' . $fee[2] . ' = ' . $fee[3] . ' Ft</option>';
    }
    $output .= '</select>';
}


$clubsOfMember = getClubsId($fee_member_id);
if (!isset($selected_club_fee)) {
    $selected_club_fee = -2;
}
if (!isset($selected_club_category)) {
    $selected_club_category = -2;
}

$output .= '<br><label>' . $fee_type[FEE_TYPE_CLUB][0] . ':</label>';
$output .= '<select name="selected_club_fee" onChange="submit();">';
$output .= '<option value="-1">Válassz</option>';
foreach ($clubsOfMember as $cl_id => $cl_name) {
    $output .= '<option value="' . $cl_id . '" ' . ($selected_club_fee == $cl_id ? "selected" : "") . '>' . $cl_name . '</option>';
}
$output .= '</select>';

if ($selected_club_fee >= 0) {
    $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select name="selected_club_category" onChange="submit();">';
    $output .= '<option value="-1">Válassz</option>';
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
        $output .= '<option value="-1">Válassz</option>';
        foreach ($fees as $feeOfClub) {
            $output .= '<option value="' . $feeOfClub[0] . '" '
                    . ($selected_club_period == $fee[0] ? "selected " : "")
                    . '>' . $feeOfClub[1] . ':' . $feeOfClub[2] . ' = ' . $feeOfClub[3] . ' Ft</option>';
        }
        $output .= '</select>';
    }
}

$output .= '<br><input value="Rögzítés"  type="submit" name="fee_type_save">';
$output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
$output .= '</div>';
$deletable_fm = true;
$sf_member = $fee_member_id;
include_once INCLUDES . 'show_fees_member.inc';
$output .= '</form>';
