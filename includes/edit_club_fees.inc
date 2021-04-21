<?php


if (isset($delete_last)) {
    $sql = 'DELETE FROM `' . FEE_TABLE . '` WHERE `' . FEE_ID . '`="' . $delete_last . '"';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_FEE_EDIT, 'A ' . $delete_last . ' azonosítójú díjtétel törlése.');
    unset($club_fee_from);
}
if (isset($edit_fee)) {
    $fee = getFeeById($edit_fee);
    $club_fee_sum = $fee[FEE_SUM];
    $editing_fee_id = $edit_fee;
}

if (isset($submit_club)) {
    if ($editing_fee_id > 0) {
        $sql = 'UPDATE `' . FEE_TABLE . '` SET `' . FEE_SUM . '`="' . $club_fee_sum . '" '
                . 'WHERE `' . FEE_ID . '`="' . $editing_fee_id . '"';
        $mysqliLink->query($sql);
        logger($member_id, -1, LOGGER_FEE_EDIT, ' A ' . $editing_fee_id . ' díj módosítása. Az új összeg: ' . $fee_sum . ' Ft');
        unset($editing_fee_id);
    } elseif ($selected_club >= 0) {
        if ($club_fee_sum > 0) {
            $fee_from = $club_fee_from;
            $fee_to = addPeriodToDate($fee_from, $fee_type[FEE_TYPE_CLUB][FEE_TYPE_PERIOD]);
            $sql = 'INSERT INTO `' . FEE_TABLE . '` (`' . FEE_TYPE . '`, `' . FEE_NAME . '`, `' . FEE_FROM . '`, `' . FEE_TO . '`, `' . FEE_SUM . '`, `' . FEE_CLUB . '`) '
                    . 'VALUES ("' . FEE_TYPE_CLUB . '", "' . $selected_club_category . '", "' . $fee_from . '", "' . $fee_to . '", "' . $club_fee_sum . '", "' . $selected_club . '");';
            $mysqliLink->query($sql);
            $l_id = $mysqliLink->insert_id;
            logger($member_id, -1, LOGGER_FEE_EDIT, ' A ' . $selected_club . ' azonosítójú egyesület '
                    . $fee_from . '-' . $fee_to . ' időszakú, ' . $club_categories[$selected_club_category] . ' tagdíjának felvitele. Azonosító: ' . $l_id);
        } else {
            $view->danger('Add meg az összeget !');
        }
    } else {
        $view->danger('Válassz klubbot !');
    }
    unset($club_fee_from);
}
if (!isset($selected_club)) {
    $selected_club = -1;
}

if (!isset($selected_club_category)) {
    $selected_club_category = -1;
}

if (!isset($editing_fee_id)) {
    $editing_fee_id = -1;
}
if (!isset($prior_club))
    $prior_club = $selected_club;
if (!isset($prior_category))
    $prior_category = $selected_club_category;

if ($prior_club != $selected_club || $prior_category != $selected_club_category) {
    $club_fee_from = VERY_FIRST_DATE;
    $prior_club = $selected_club;
    $prior_category = $selected_club_category;
    $editing_fee_id = -1;
    $club_fee_sum = 0;
}
$view->newKeret();
$view->putHidden('editing_fee_id');
$view->putHidden('prior_club');
$view->putHidden('prior_category');
if ($editing_fee_id < 0) {
    if (isset($club_limited)) {
        $selected_club = $club_limited;
        $view->putHidden('selected_club');
    } else {
        $clubs = getClubNameList();
        $view->inputClub('Egyesület', [-1 => 'Válassz!!!'] + $clubs, 'selected_club', true);
    }
    if ($selected_club >= 0) {
        $view->submitDropDownChooser('Díj kategória', [-1 => 'Válassz!!!'] + $club_categories, 'selected_club_category');
        if (!isset($club_fee_sum))
            $club_fee_sum = 0;
        if ($selected_club_category >= 0) {
            $club_fees = getFeesOfClub($selected_club, $selected_club_category);
            $last_fee_id = -1;
            foreach ($club_fees as $fee_item) {
                $view->newDiv('row');
                $view->putText($fee_item[1] . ' : ' . $fee_item[2] . ' = ' . $fee_item[3] . ' Ft ', 'Rögzítve');
                $view->submitEdit('edit_fee', $fee_item[0]);
                if (isFeeDeleteable($fee_item[0])) {
                    $view->submitDelete('delete_last', $fee_item[0]);
                }
                $view->endDiv();
            }
            $first_undefined = getFirstUndefinedClubFeeDate($selected_club, $selected_club_category);
            if (!isset($club_fee_from) || $club_fee_from = VERY_FIRST_DATE) {
                $club_fee_from = $first_undefined;
            } else {
                $club_fee_from = max($club_fee_from, $first_undefined);
            }
            if ($first_undefined == VERY_FIRST_DATE) {
                $view->inputDate('club_fee_from', 'Új dőszak kezdete');
            } else {
                $view->putText($club_fee_from, 'Új időszak kezdete');
                $view->putHidden('club_fee_from');
            }
            $view->putText(addPeriodToDate($club_fee_from, $fee_type[FEE_TYPE_CLUB][FEE_TYPE_PERIOD]), 'Új időszak vége');
            $view->inputNumber('club_fee_sum', $fee_periods[$fee_type[FEE_TYPE_CLUB][FEE_TYPE_PERIOD]] . ' díj', ' Ft');
            $view->submitSave('submit_club');
        }
    }
} else {
    $fee = getFeeById($editing_fee_id);
    $view->putText($fee[FEE_FROM] . ':' . $fee[FEE_TO], 'Időszak');
    $view->inputNumber('club_fee_sum', $fee_periods[$fee_type[FEE_TYPE_CLUB][FEE_TYPE_PERIOD]] . ' díj', ' Ft');
    $view->submitSave('submit_club');
}
$view->endKeret();
