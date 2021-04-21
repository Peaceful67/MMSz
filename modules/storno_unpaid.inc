<?php

$fm_ids = getUnpaidBillsCreatedBy($member_id);

if (empty($fm_ids)) {
    $view->info('Nincsenek sztornózható számláid !');
    return;
}
if (isset($storno)) {
    $email_msg = 'Az alábbi számlákat törölted:<br><br>';
    foreach ($fm_ids as $fm_id) {
        $chk = FM_CHECKBOX . '[' . $fm_id . ']';
        if (isset($$chk) AND $ $chk == "on") {
            $fm = getFmById($fm_id);
            $bill = getBillById($fm[FM_BILL_ID]);
            $email_msg .= 'Azonosító: ' . $bill[BILLING_NUMBER] . ', összeg: ' . $bill[BILLING_PRICE] . ' Ft<br>';
            logger($member_id, $fm[FM_MEMEBER_ID], LOGGER_BILL, $bill[BILLING_NUMBER] . ' azonosítójú, ' . $bill[BILLING_PRICE] . ' Ft összegű számla törlése.');
            stornoBill($fm_id);
        }
    }
    send_mail("Fizetés törlése", $email_msg);
    $fm_ids = getUnpaidBillsCreatedBy($member_id);
}
$view->newTable('Törölhető tételek');
$collected = array();
$view->putThead('', ['Törlendő', 'Tag', 'Díj típus', 'Tétel', 'Időszak', 'Összeg']);
foreach ($fm_ids as $fm_id) {
    $fm = getFeeOfMemberById($fm_id);
    $fee = getFeeById($fm[FM_FEE_ID]);
    $chk = FM_CHECKBOX . '[' . $fm_id . ']';
    if (!isset($$chk)) {
        $$chk = '';
    }
//    $output_chk = '<input type="checkbox" name="' . $chk . '" ';
    if ($$chk == "on") {
        $checked = ' checked ';
        $collected[] = $fm_id;
    } else {
        $checked = '';
    }
 //   $output_chk .= ' onchange="submit();" >';
    $view->newTableTr();
    $view->newElement('<td><input type="checkbox" name="' . $chk . '" '.$checked.' ></td>');
    $output .= $output_chk . '</td>';
    $view->putElement('<td>' . getMemberName($fm[FM_MEMEBER_ID]) . '</td>');
    $view->putElement('<td>' . $fee_type[$fee[FEE_TYPE]][0] . '</td>');
    switch ($fee[FEE_TYPE]) {
        case FEE_TYPE_CLUB:
            $view->putElement('<td>'.$club_categories[$fee[FEE_NAME]].'</td>');    
            $output .= $club_categories[$fee[FEE_NAME]];
            break;
        case FEE_TYPE_MEMBERSHIP:
            $view->putElement('<td>'.$member_types[$fee[FEE_NAME]].'</td>');    
            $output .= $member_types[$fee[FEE_NAME]];
            break;
        case FEE_TYPE_INSURANCE:
            $view->putElement('<td>'.$member_insurances[$fee[FEE_NAME]].'</td>');    
            break;
        case FEE_TYPE_ORGANIZATION:
            $view->putElement('<td>'.$or[$fee[FEE_NAME]].'</td>');    
            break;
        default:
            $view->putElement('<td></td>');    
    }
    $view->putElement('<td>' . $fm[FM_FROM] . ':' . $fm[FM_TO] . '</td>');
    $view->putElement('<td>' . getBillById($fm[FM_BILL_ID])[BILLING_PRICE] . ' Ft</td>');
    $view->endTableTr();
}
$view->submitButton('storno', 'Sztorno');
$view->endTable();
