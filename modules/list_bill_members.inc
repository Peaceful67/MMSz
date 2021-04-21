<?php


if (isset($list_create_bill)) {
    redirect("start_payment");
}
$collected_fms = array();
if (!isset($check_all)) {
    $check_all = '';
}
if (!isset($check_all_prior)) {
    $check_all_prior = '';
}

include_once INCLUDES . 'search_bill_members.inc';
$fms2bill_sum = 0;
$fms2bill_num = 0;

$all_fms2bill = array();
$turn_on = ($check_all == "on" AND $check_all_prior != "on") ? true : false;
$turn_off = ($check_all != "on" AND $check_all_prior == "on") ? true : false;

while ($res AND $row = $res->fetch_assoc()) {
    if ($selected_fee < 0) {
        $fms2bill = getUnpaidFMOfMember($row[MEMBER_ID]);
    } else {
        $fms2bill = getUnpaidFMOfMemberFeeType($row[MEMBER_ID], $selected_fee);
    }
    foreach ($fms2bill as $fm2bill) {
        $all_fms2bill[] = $fm2bill;
    }
}

if (count($all_fms2bill) > 0) {

    if (!isset($check_all)) {
        $check_all = '';
    }
    if (!isset($check_all_prior)) {
        $check_all_prior = $check_all;
    }
    $check_all_prior = $check_all == "on" ? "on" : "off";
    $view->putHidden('check_all_prior');
    foreach ($all_fms2bill as $fm2bill) {
        $chk = FM_CHECKBOX . '[' . $fm2bill[0] . ']';
        if ($turn_on OR ( (isset($$chk) AND ( $$chk) == "on") AND ! $turn_off)) {
            $fms2bill_sum += $fm2bill[5];
            $fms2bill_num++;
            $collected_fms[] = array($fm2bill[0], $fm2bill[5]);
        }
    }
    if ($fms2bill_sum > 0) {
        $view->newKeret();
        $view->newDiv('row');
        $view->submitButton('list_create_bill', 'Fizetés', 0, 'A ' . $fms2bill_sum . ' Ft összegű fizetés indítása.', 'Biztosan el akarod indítani a ' . $fms2bill_sum . ' Ft összegű fizetést?');
        $view->info('Tételszám: ' . $fms2bill_num . ',&nbsp;&nbsp;összeg: ' . $fms2bill_sum . ' Ft');
        $view->endDiv();
        $view->endKeret();
    }
    $view->newTable('Csak a szűrési feltételeknek megfelelő beállított díjakkal rendelkezők jelennek meg !');
    $view->putThead('', ['Azonosító', 'Név', 'Született',
        '<div class="mx-3"><label class="form-check-label" for="check_all"><input type="checkbox" class="form-check-input" name="check_all" id="check_all" ' . ($check_all == "on" ? "checked" : "") . ' onchange="submit();">Összes díj</label></div>']);
    foreach ($all_fms2bill as $fm2bill) {
        $fm_id = $fm2bill[0];
        $disabled_msg = isReadyForFAI($fm2bill);
        $chk = FM_CHECKBOX . '[' . $fm2bill[0] . ']';
        if (!isset($$chk) OR $turn_off) {
            $$chk = 'off';
        }
        if ($turn_on OR $ $chk == 'on') {
            $$chk = 'checked';
        }
        if (!empty($disabled_msg)) {
            $$chk = 'disabled';
        }
        $mem_id = getFmById($fm_id)[FM_MEMEBER_ID];
        $mem = retriveMember(getFmById($fm_id)[FM_MEMEBER_ID]);
        $view->newTableTr($$chk == 'on' ? 'bg-success' : '');
        $view->putElement("<td>" . $mem[MEMBER_ID] . "</td>");
        $view->putElement('<td>' . $mem[MEMBER_VEZNEV] . " " . $mem[MEMBER_KERNEV] . "</td>");
        $view->putElement('<td>' . $mem[MEMBER_BORN] . "</td>");
        $view->newElement('<td>');
        //     $view->newElement('<pre>');
        $view->newDiv('mx-3');
        $view->putElement('<input type="checkbox" class="form-check-input" id="' . $chk . '" name="' . $chk . '" ' . $$chk . ' onChange="submit();"/>');
        $view->endDiv()->newDiv('mx-3');
        $fm2bill_text = $fee_type[$fm2bill[1]][0].' / ';
        switch ($fm2bill[1]) {
            case FEE_TYPE_CLUB:
                $fm2bill_text .= $club_categories[$fm2bill[2]];
                break;
            case FEE_TYPE_MEMBERSHIP:
                $fm2bill_text .= $member_types[$fm2bill[2]];
                break;
            case FEE_TYPE_INSURANCE:
                $fm2bill_text .= $member_insurances[$fm2bill[2]];
                break;
            case FEE_TYPE_ORGANIZATION:
                $fm2bill_text .= $organizations[$fm2bill[2]];
                break;
        }
        $fm2bill_text .= ' / '.$fm2bill[3] . ':' . $fm2bill[4] .' / '. $fm2bill[5] . ' Ft';
        $view->putElement($fm2bill_text);
        if (!empty($disabled_msg)) {
            $view->danger($disabled_msg);
        }
        $view->endElement('</td>');
        $view->endTableTr();
    }
    $view->endTable();
    $_SESSION[SESSION_FEE_TO_BILL] = serialize($collected_fms);
} else {
    $view->warning("A szűrési feltételek szerint nincs fizethető díj");
}

