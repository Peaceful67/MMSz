<?php

$fm_ids = getUnpaidBillsCreatedBy($member_id);

if (empty($fm_ids)) {
    $output .= 'Nincsenek sztornózható számláid !';
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
$output .= '<form method="post"><div class="fees_members"><table><caption>Törölhető tételek:</caption>';
$collected = array();
foreach ($fm_ids as $fm_id) {
    $fm = getFeeOfMemberById($fm_id);
    $fee = getFeeById($fm[FM_FEE_ID]);
    $chk = FM_CHECKBOX . '[' . $fm_id . ']';
    if (!isset($$chk)) {
        $$chk = '';
    }
    $output_chk = '<input type="checkbox" name="' . $chk . '" ';
    if ($$chk == "on") {
        $output_chk .= ' checked ';
        $collected[] = $fm_id;
    }
    $output_chk .= ' onchange="submit();" >';
    $output .= '<tr><td>';
    $output .= $output_chk . '</td>';
    $output .= '<td>' . getMemberName($fm[FM_MEMEBER_ID]) . '</td>';
    $output .= '<td>' . $fee_type[$fee[FEE_TYPE]][0] . '</td>';
    $output .= '<td>';
    switch ($fee[FEE_TYPE]) {
        case FEE_TYPE_CLUB:
            $output .= $club_categories[$fee[FEE_NAME]];
            break;
        case FEE_TYPE_MEMBERSHIP:
            $output .= $member_types[$fee[FEE_NAME]];
            break;
        case FEE_TYPE_INSURANCE:
            $output .= $member_insurances[$fee[FEE_NAME]];
            break;
        case FEE_TYPE_ORGANIZATION:
            $output .= $organizations[$fee[FEE_NAME]];
            break;
    }
    $output .= '</td><td>' . $fm[FM_FROM] . ':' . $fm[FM_TO] . '</td>';
    $output .= '<td>' . getBillById($fm[FM_BILL_ID])[BILLING_PRICE] . ' Ft</td></tr>';
}
$output .= '</table><input type="submit" value="Sztorno" name="storno">';
$output .= '</div></form>';
