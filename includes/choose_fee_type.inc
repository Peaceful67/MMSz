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
                warning("Már van nem törölhető " . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " tagsági díjtétel ebben az időszakban !");
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
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $member_insurances[$fee[FEE_NAME]]);
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
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $organizations[$fee[FEE_NAME]]);
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
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $associations[$fee[FEE_NAME]]);
                unset($selected_assoc_period);
            } else {
                warning("Már van nem törölhető " . $fee_type[FEE_TYPE_ASSOCIATION][0] . " díjtétel ebben az időszakban !");
            }
        }
    }
     if (isset($selected_occasion) && $selected_occasion >= 0) {
        if ($selected_occasion_period > 0) {
            if (isFeeChangeableInFM($fee_member_id, $selected_occasion_period)) {
                deleteFeeNameIfExists($fee_member_id, FEE_TYPE_OCCASION, $selected_occasion_period);
                $fee = getFeeById($selected_occasion_period);
                $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`,`' . FM_FEE_ID . '` , `' . FM_FROM . '`, `' . FM_TO . '`)
                    VALUES ("' . $fee_member_id . '","' . $selected_occasion_period . '",'
                        . ' "' . $fee[FEE_FROM] . '", "' . $fee[FEE_TO] . '")';
                $mysqliLink->query($sql);
                $isSucceed = true;
                logger($member_id, $fee_member_id, LOGGER_FM_EDIT, $fee_type[FEE_TYPE_OCCASION][0]
                        . ' ' . $fee[FEE_FROM] . '-' . $fee[FEE_TO] . ' időszakbani meghatározása: ' . $occasions[$fee[FEE_NAME]]);
                unset($selected_occasion_period);
            } else {
                warning("Már van nem törölhető " . $fee_type[FEE_TYPE_OCCASION][0] . " díjtétel ebben az időszakban !");
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
include_once INCLUDES . 'edit_fees_member.inc';
$deletable_fm = true;
$sf_member = $fee_member_id;
include_once INCLUDES . 'show_fees_member.inc';
$output .= '</form>';
