<?php

define("BK_ALL", 1);
define("BK_OFFLINE", 2);
define("BK_ONLINE", 3);

if (!isset($selected_bk)) {
    $selected_bk = BK_ONLINE;
}

if(isset($print_bill)) {
    downloadPdfOfBill($print_bill);
    setBillPrinted($print_bill);
}
$output .= '<div class="lists"><form method="post">';
$output .= '<select name="selected_bk" onchange="submit();">';
$output .= '<option value="' . BK_ALL . '" ' . (($selected_bk == BK_ALL) ? "selected" : "") . '>Összes</option>';
$output .= '<option value="' . BK_OFFLINE . '" ' . (($selected_bk == BK_OFFLINE) ? "selected" : "") . '>Átadott</option>';
$output .= '<option value="' . BK_ONLINE . '" ' . (($selected_bk == BK_ONLINE) ? "selected" : "") . '>Rendezetlen</option>';
$output .= '</select>&nbsp;&nbsp;&nbsp;&nbsp;';
if (!isset($search_from)) {
    $search_from = date('Y-m-d', strtotime($today.' -1 month'));
}
if (!isset($search_to)) {
    $search_to = $today;
}
$output .= '<label >Időszak: </label>
            <input type="date" name="search_from"  value="' . $search_from . '" onchange="submit();">
            <input type="date" name="search_to"  value="' . $search_to . '" onchange="submit();"><br>';

$sql = 'SELECT * FROM `' . BILLING_TABLE . '` '
        . 'WHERE `' . BILLING_PAYMENT_ID . '`!="0" ';
switch ($selected_bk) {
    case BK_OFFLINE;
        $sql .= ' AND `' . BILLING_OFFLINE . '`>0 ';
        break;
    case BK_ONLINE;
        $sql .= ' AND `' . BILLING_OFFLINE . '`=0 ';
        break;
    case BK_ALL:
    default:
        break;
}
$sql .= ' AND (`'.BILLING_TIME.'` BETWEEN "'.$search_from.' 00:00:00" AND "'.$search_to.' 23:59:59" )';
$sql .= ' GROUP BY  `' . BILLING_NUMBER . '` ORDER BY `' . BILLING_TIME . '` DESC';
$res = $mysqliLink->query($sql);
if ($res AND $res->num_rows > 0) {
    while ($row = $res->fetch_assoc()) {
        $number = $row[BILLING_NUMBER];
        $payment = getPaymentByBillNumber($number);
        $output .= '<br><button name="print_bill" value="' . $number . '"><img src="' . IMAGES . 'print.jpg" title="Nyomtatás"></button><b>' . $number.'</b>';
        $printed = current(getBillsByNumber($number))[BILLING_OFFLINE];
        if ($printed > 0) {
            $output .= '&nbsp;&nbsp;Nyomtatva: ' . $printed;
        }
        $output .= '<br><label>Kiállítva: </label>'.$row[BILLING_TIME].'&nbsp;&nbsp;&nbsp;'
                .getMemberName($row[BILLING_CREATOR]).' ˙('.$row[BILLING_CREATOR].')';
        $output .= '<br><label>Beérkezett: </label>' . $payment[PAYMENT_TIME] . ', '
                . '<label>Összeg: </label>' . getSumOfBills($number) . ' Ft<br>';
        if($payment[PAYMENT_BANK_REFNO] != 0) {
            $output .= '<label>Kártyás fizetés, bankazonosító: </label>'.$payment[PAYMENT_BANK_REFNO].'<br>';
        }
        $output .= '<b>Tételek : </b><br>';
        foreach (billItems($number) as $bill) {
            $fm = getFmById($bill[BILLING_FM_ID]);
            $fee = getFeeById($fm[FM_FEE_ID]);
            $output .= FM_to_String($fm[FM_ID], ', ') . '<br>';
        }
        $output .= '<br><hr>';
    }
} else {
    warning('Jelenleg nincs ilyen elem');
}
$output .= '</form></div>';
