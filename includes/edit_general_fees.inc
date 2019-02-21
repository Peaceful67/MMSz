<?php

if (isset($delete_last)) {
    $sql = 'DELETE FROM `' . FEE_TABLE . '` WHERE `' . FEE_ID . '`="' . $delete_last . '"';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_FEE_EDIT, 'A ' . $delete_last . ' azonosítójú díjtétel törlése.');
    unset($fee_from);
    unset($delete_last);
}

if (isset($edit_fee)) {
    $fee = getFeeById($edit_fee);
    $fee_sum = $fee[FEE_SUM];
    $editing_fee_id = $edit_fee;
}
if (isset($submit_fee)) {
    if ($editing_fee_id > 0) {
        $sql = 'UPDATE `' . FEE_TABLE . '` SET `' . FEE_SUM . '`="' . $fee_sum . '" '
                . 'WHERE `' . FEE_ID . '`="' . $editing_fee_id . '"';
        $mysqliLink->query($sql);
        logger($member_id, -1, LOGGER_FEE_EDIT, ' A ' . $editing_fee_id . ' díj módosítása. Az új összeg: ' . $fee_sum . ' Ft');
        unset($editing_fee_id);
    } elseif ($selected_name >= 0) {
        if ($fee_sum >= 0) {
            $fee_to = addPeriodToDate($fee_from, $fee_type[$edited_type][FEE_TYPE_PERIOD]);
            $sql = 'INSERT INTO `' . FEE_TABLE . '` (`' . FEE_TYPE . '`, `' . FEE_NAME . '`, `' . FEE_FROM . '`, `' . FEE_TO . '`, `' . FEE_SUM . '`) '
                    . 'VALUES ("' . $edited_type . '", "' . $selected_name . '", "' . $fee_from . '", "' . $fee_to . '", "' . $fee_sum . '");';
            $mysqliLink->query($sql);
            $l_id = $mysqliLink->insert_id;
            logger($member_id, -1, LOGGER_FEE_EDIT, ' A ' . $edited_names[$selected_name] . ' díj ' . $fee_from . '-' . $fee_to . ' időszakú felvitele. Azonosító: ' . $l_id);
            unset($fee_sum);
        } else {
            warning('Add meg az összeget !');
        }
    } else {
        warning('Válassz díjtípus !');
    }
    unset($fee_from);
}

if (!isset($selected_name))
    $selected_name = -2;
if (!isset($editing_fee_id))
    $editing_fee_id = -1;
if (!isset($fee_sum))
    $fee_sum = 0;
if (!isset($prior_name))
    $prior_name = $selected_name;
if ($prior_name != $selected_name) {
    $fee_from = VERY_FIRST_DATE;
    $prior_name = $selected_name;
    $editing_fee_id = -1;
    $fee_sum = 0;
}
$output .= '<div class="keret"><form  method="post">';
$output .= '<input type="hidden" name="editing_fee_id" value="' . $editing_fee_id . '" >';
$output .= '<input type="hidden" name="prior_name" value="' . $selected_name . '" >';
if ($editing_fee_id < 0) {
    $output .= '<label>' . $fee_type[$edited_type][FEE_TYPE_NAME] . ': </label>'
            . '<select id="selected_name" name="selected_name" onChange="submit();">';
    $output .= '<option value="-1">Válassz !</option>';
    foreach ($edited_names as $id => $name) {
        $output .= '<option value="' . $id . '" ' . (($selected_name == $id) ? " selected " : "") . '>' . $name . '</option>';
    }
    $output .= '</select>';
    if ($selected_name >= 0) {
        $fees = getFees($edited_type, $selected_name);
        $last_fee_id = -1;
        foreach ($fees as $fee_item) {
            $output .= '<br><label>Rögzítve:</label>' . $fee_item[1] . ' : ' . $fee_item[2] . ' = ' . $fee_item[3] . ' Ft ';
            $output .= '<button type="submit" name="edit_fee" value="' . $fee_item[0] . '">'
                    . '<img src="' . IMAGES . 'edit.jpg" title="Szerkesztés: ' . $fee_item[0] . '"></button>';
            $last_fee_id = $fee_item[0];
        }
        if ($last_fee_id > 0 AND isFeeDeleteable($last_fee_id)) {
            $output .= '<button type="submit"  name="delete_last" value="' . $last_fee_id . '">'
                    . '<img src="' . IMAGES . 'delete.jpg" title="Törlés: ' . $last_fee_id . '"></button>';
        }
        $first_undefined = getFirstUndefinedFeeDate($edited_type, $selected_name);
        if (!isset($fee_from)) {
            $fee_from = $first_undefined;
        } else {
            $fee_from = max($fee_from, $first_undefined);
        }
        $output .= '<br><label>Új időszak: </label>';
        $continues_period = true;
        if ($fee_type[$edited_type][FEE_TYPE_PERIOD] == FEE_PERIOD_DAY) {
            $free_from = $today;
            $continues_period = false;
        }
        if ($first_undefined == VERY_FIRST_DATE) {
            $continues_period = false;
        }
        if (!$continues_period) {
            $output .= '<input type="date" name="fee_from" value="' . $fee_from . '" onChange="submit();">';
        } else {
            $output .= $fee_from;
            $output .= '<input type="hidden" name="fee_from" value="' . $fee_from . '" >';
        }
        $output .= ' : ' . addPeriodToDate($fee_from, $fee_type[$edited_type][FEE_TYPE_PERIOD]) . '<br>';
        $output .= '<label>Új ' . $fee_periods[$fee_type[$edited_type][FEE_TYPE_PERIOD]] . ' díj : </label>'
                . '<input type="number" name="fee_sum" size="5" value="' . $fee_sum . '"> Ft';
        $output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="submit_fee" value="Rögzít">';
    }
} else {
    $fee = getFeeById($editing_fee_id);
    $output .= '<label>Időszak: </label>' . $fee[FEE_FROM] . ':' . $fee[FEE_TO] . '<br>';
    $output .= '<label>' . $fee_periods[$fee_type[$edited_type][FEE_TYPE_PERIOD]] . ' díj : </label>'
            . '<input type="number" name="fee_sum" size="5" value="' . $fee_sum . '"> Ft';
    $output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="submit_fee" value="Módosít">';
}
$output .= '</form></div>';
