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
$output .= '<form  method="post">';
include_once INCLUDES . 'search_bill_members.inc';

$output .= '<br><div class="lists">';

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

    $output .= '<input type="hidden" name="check_all_prior" value="' . ($check_all == "on" ? "on" : "off") . '" >';
    foreach ($all_fms2bill as $fm2bill) {
        $chk = FM_CHECKBOX . '[' . $fm2bill[0] . ']';
        if ($turn_on OR ( (isset($$chk) AND ( $$chk) == "on") AND ! $turn_off)) {
            $fms2bill_sum += $fm2bill[5];
            $fms2bill_num++;
            $collected_fms[] = array($fm2bill[0], $fm2bill[5]);
        }
    }
    if ($fms2bill_sum > 0) {
        $output .= '<div class="keret">';
        $output .= '&nbsp;&nbsp;<input value="Fizetés"  type="submit" name="list_create_bill" '
                . 'onClick="return confirm(\'Biztosan el akarod indítani a ' . $fms2bill_sum . ' Ft összegű fizetést?\')">';
        $output .= '&nbsp;&nbsp;tételszám: ' . $fms2bill_num . ',&nbsp;&nbsp;összeg: ' . $fms2bill_sum . ' Ft<br>';
        $output .= '</div>';
    }

    $output .= '<table border="1"><caption>Csak a meghatározott díjakkal rendelkezők jelennek meg !</caption>';
    $output .= '<tr><th>Sorszám</th><th>Név</th><th>Született</th><th>Díjak&nbsp;&nbsp;&nbsp;'
            . '<input type="checkbox" name="check_all" ' . ($check_all == "on" ? "checked" : "") . ' onchange="submit();">'
            . '</th></tr>';
    foreach ($all_fms2bill as $fm2bill) {
        $fm_id = $fm2bill[0];
        $mem_id = getFmById($fm_id)[FM_MEMEBER_ID];
        $mem = retriveMember(getFmById($fm_id)[FM_MEMEBER_ID]);
        $output .= "<tr>";
        $output .= "<td>" . $mem[MEMBER_ID] . "</td>";
        $output .= '<td>' . $mem[MEMBER_VEZNEV] . " " . $mem[MEMBER_KERNEV] . "</td>";
        $output .= '<td>' . $mem[MEMBER_BORN] . "</td>";
        $output .= '<td>';

        $chk = FM_CHECKBOX . '[' . $fm2bill[0] . ']';
        if (!isset($$chk)) {
            $$chk = '';
        }
        $output .= '<pre><input type="checkbox" name="' . $chk . '" ';
        $disabled_msg = isReadyForFAI($fm2bill);
        if (!empty($disabled_msg)) {
            $output .= ' disabled ';
        }
        if (($turn_on OR ( $$chk) == "on") AND ! $turn_off) {
            $output .= ' checked ';
        }
        $output .= ' onchange="submit();" >';
        $output .= '&#9;';
        $output .= $fee_type[$fm2bill[1]][0] . '&#9;';
        switch ($fm2bill[1]) {
            case FEE_TYPE_CLUB:
                $output .= $club_categories[$fm2bill[2]];
                break;
            case FEE_TYPE_MEMBERSHIP:
                $output .= $member_types[$fm2bill[2]];
                break;
            case FEE_TYPE_INSURANCE:
                $output .= $member_insurances[$fm2bill[2]];
                break;
            case FEE_TYPE_ORGANIZATION:
                $output .= $organizations[$fm2bill[2]];
                break;
        }
        $output .= '&#9;' . $fm2bill[3] . ':' . $fm2bill[4] . '&#9;';
        $output .= $fm2bill[5] . ' Ft</pre>';
        if (!empty($disabled_msg)) {
            $output .= '<font color="red"> ' . $disabled_msg . '</font>';
        }
    }
    $output .= "</table>";
    $_SESSION[SESSION_FEE_TO_BILL] = serialize($collected_fms);
} else {
    warning("A szűrési feltételek szerint nincs fizethető díj");
}
$output .= "</form>";
$output .= "</div>";

