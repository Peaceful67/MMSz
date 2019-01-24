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
            warning('Add meg az összeget !');
        }
    } else {
        warning('Válassz klubbot !');
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

$output .= '<div class="keret"><form  name="select_club_fee_form" id="select_club_fee_form"  method="post" >';
$output .= '<input type="hidden" name="editing_fee_id" value="' . $editing_fee_id . '" >';
$output .= '<input type="hidden" name="prior_club" value="' . $selected_club . '" >';
$output .= '<input type="hidden" name="prior_category" value="' . $selected_club_category . '" >';
if ($editing_fee_id < 0) {
    if (isset($club_limited)) {
        $selected_club = $club_limited;
        $output .= '<input type="hidden"  name="selected_club" value="' . $selected_club . '" >';
    } else {
        $output .= '<label>Egyesület : </label>'
                . '<select id="selected_club" name="selected_club" onChange="submit();">';
        $output .= '<option value="-1">Válassz egyesületet !</option>';
        $clubs = getClubNameList();
        foreach ($clubs as $club_id => $club_name) {
            $output .= '<option value="' . $club_id . '" ' . (($selected_club == $club_id) ? " selected " : "") . '>' . $club_name . '</option>';
        }
        $output .= '</select><br>';
    }

    if ($selected_club >= 0) {
        $output .= '<label>Díj kategória : </label>'
                . '<select id="selected_club_category" name="selected_club_category" onChange="submit();">';
        $output .= '<option value="-1">Válassz kategóriát !</option>';
        foreach ($club_categories as $club_category_id => $club_category_name) {
            $output .= '<option value="' . $club_category_id . '" ' . (($selected_club_category == $club_category_id) ? " selected " : "") . '>' . $club_category_name . '</option>';
        }
        $output .= '</select>';
        if (!isset($club_fee_sum))
            $club_fee_sum = 0;
        if ($selected_club_category >= 0) {
            $club_fees = getFeesOfClub($selected_club, $selected_club_category);
            $last_fee_id = -1;
            foreach ($club_fees as $fee_item) {
                $output .= '<br><label>Rögzítve:</label>' . $fee_item[1] . ' : ' . $fee_item[2] . ' = ' . $fee_item[3] . ' Ft ';
                $last_fee_id = $fee_item[0];
                $output .= '<input type="image" title="Szerkesztés" name="edit_fee" value="' . $fee_item[0] . '" src="' . IMAGES . 'edit.jpg">';
            }
            if ($last_fee_id > 0 AND isFeeDeleteable($last_fee_id)) {
                $output .= '<input type="image" title="Törlés: ' . $last_fee_id . '" name="delete_last" value="' . $last_fee_id . '" src="' . IMAGES . 'delete.jpg">';
            }
            $first_undefined = getFirstUndefinedClubFeeDate($selected_club, $selected_club_category);
            if (!isset($club_fee_from) || $club_fee_from = VERY_FIRST_DATE) {
                $club_fee_from = $first_undefined;
            } else {
                $club_fee_from = max($club_fee_from, $first_undefined);
            }
            $output .= '<br><label>Új időszak: </label>';
            if ($first_undefined == VERY_FIRST_DATE) {
                $output .= '<input type="date" name="club_fee_from" value="' . $club_fee_from . '" onChange="submit();">';
            } else {
                $output .= $club_fee_from;
                $output .= '<input type="hidden" id="club_fee_from" name="club_fee_from" value="' . $club_fee_from . '" >';
            }
            $output .= ' : ' . addPeriodToDate($club_fee_from, $fee_type[FEE_TYPE_CLUB][FEE_TYPE_PERIOD]) . '<br>';
            $output .= '<label>Új ' . $fee_periods[$fee_type[FEE_TYPE_CLUB][FEE_TYPE_PERIOD]] . ' díj : </label>'
                    . '<input type="number" name="club_fee_sum" size="5" value="' . $club_fee_sum . '"> Ft';
            $output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="submit_club" value="Rögzít">';
        }
    }
} else {
    $fee = getFeeById($editing_fee_id);
    $output .= '<label>Időszak: </label>' . $fee[FEE_FROM] . ':' . $fee[FEE_TO] . '<br>';
    $output .= '<label>' . $fee_periods[$fee_type[FEE_TYPE_CLUB][FEE_TYPE_PERIOD]] . ' díj : </label>'
            . '<input type="number" name="club_fee_sum" size="5" value="' . $club_fee_sum . '"> Ft';
    $output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="submit_club" value="Módosít">';
}
$output .= '</form></div>';
