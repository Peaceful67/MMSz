<?php

$view = new View();
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
            $view->danger('Add meg az összeget !');
        }
    } else {
        $view->warning('Válassz díjtípus !');
    }
    unset($fee_from);
}

if (!isset($selected_name))
    $selected_name = -1;
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
$prior_name = $selected_name;
$view->newKeret();
$view->putHidden('editing_fee_id');
$view->putHidden('prior_name');
if ($editing_fee_id < 0) {
    $view->submitDropDownChooser($fee_type_names[$edited_type], [-1 => 'Válassz!!!'] + $edited_names, 'selected_name', true);
    if ($selected_name >= 0) {
        $fees = getFees($edited_type, $selected_name);
        $last_fee_id = -1;
        foreach ($fees as $fee_item) {
            $view->newDiv('row');
            $view->putElement('Rögzítve: '.$fee_item[1] . ' : ' . $fee_item[2] . ' = ' . $fee_item[3] . ' Ft ' );
            $view->submitEdit('edit_fee', $fee_item[0]);
            if ($last_fee_id < 0) {
                $last_fee_id = $fee_item[0];
                if (isFeeDeleteable($last_fee_id)) {
                    $view->submitDelete('delete_last', $last_fee_id);
                }
            }
            $view->endDiv();
        }

        $first_undefined = getFirstUndefinedFeeDate($edited_type, $selected_name);
        if (!isset($fee_from)) {
            $fee_from = $first_undefined;
        } else {
            $fee_from = max($fee_from, $first_undefined);
        }
        $continues_period = true;
        if ($fee_type[$edited_type][FEE_TYPE_PERIOD] == FEE_PERIOD_DAY) {
            $free_from = $today;
            $continues_period = false;
        }
        if ($first_undefined == VERY_FIRST_DATE) {
            $continues_period = false;
        }
        if (!$continues_period) {
            $view->inputDate('fee_from');
        } else {
            $view->putElement('Új időszak: '.$fee_from );
            $view->putHidden('fee_from');
        }
        $view->putElement('Új időszak'. addPeriodToDate($fee_from, $fee_type[$edited_type][FEE_TYPE_PERIOD]) );
        $view->inputNumber('fee_sum', 'Új ' . $fee_periods[$fee_type[$edited_type][FEE_TYPE_PERIOD]] . ' díj', 'Ft');
        $view->submitButton('submit_fee', 'Rögzít');
    }
} else {
    $fee = getFeeById($editing_fee_id);
    $view->putElement('Időszak: '.$fee[FEE_FROM] . ':' . $fee[FEE_TO]);
    $view->inputNumber('fee_sum', 'Új ' . $fee_periods[$fee_type[$edited_type][FEE_TYPE_PERIOD]] . ' díj', 'Ft');    
    $view->submitButton('submit_fee', 'Módosít');
  /*
    $output .= '<label>Időszak: </label>' . $fee[FEE_FROM] . ':' . $fee[FEE_TO] . '<br>';
    $output .= '<label>' . $fee_periods[$fee_type[$edited_type][FEE_TYPE_PERIOD]] . ' díj : </label>'
            . '<input type="number" name="fee_sum" size="5" value="' . $fee_sum . '"> Ft';
    $output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="submit_fee" value="Módosít">';
   * 
   */
}
$view->endKeret();
