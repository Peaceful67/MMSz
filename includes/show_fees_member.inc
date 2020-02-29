<?php

if (!isset($sf_member)) {
    $sf_member = $member_id;
}

$output .= '<div class="keret"><div class="lists">';
$output .= '<table><tr><th>Típus</th><th>Név</th><th>Kezdet</th><th>Vég</th><th>Összeg</th><th>Állapot</th>';
$fms = getFMsOfMember($sf_member);
foreach ($fms as $fm) {
    $fee = getFeeById($fm[FM_FEE_ID]);
    if ($fm[FM_PAY_ID] != 0) { //Ha ki van fizetve, a bill-bol vesszuk
        $sum = getFmSumFromBill($fm);
    } else { // Ha nincs kifizetve a fee-bol
        $sum = $fee[FEE_SUM];
    }
    $output .= '<tr><td>' . $fee_type[$fee[FEE_TYPE]][0] . '</td>';
    $output .= '<td>' . printFeeName($fee) . '</td>';
    $output .= '<td>' . $fm[FM_FROM] . '</td>';
    $output .= '<td>' . $fm[FM_TO] . '</td>';
    $output .= '<td>' . $sum . ' Ft</td>';
    $output .= '<td>';
    if ($fm[FM_BILL_ID] == 0) {
        if (isset($deletable_fm) && $deletable_fm) {
            $output .= '<button  type="submit" name="delete_fm" value="' . $fm[FM_ID] . '"><img title="Törlés" src="' . IMAGES . 'delete.jpg"></button>';
        } else {
            $output .= 'Beállítva';
        }
    } elseif ($fm[FM_PAY_ID] == 0) {
        $output .= 'Fizetésre vár';
    } else {
        $output .= 'Rendezve';
    }
    $output .= '</td></tr>';
}

$output .= '</table><div></div>';
