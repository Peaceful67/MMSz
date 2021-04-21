<?php

define("BK_ALL", 1);
define("BK_OFFLINE", 2);
define("BK_ONLINE", 3);

if (!isset($selected_bk)) {
    $selected_bk = BK_ONLINE;
}

if (isset($print_bill)) {
    downloadPdfOfBill($print_bill);
    setBillPrinted($print_bill);
}
if (!isset($search_from)) {
    $search_from = date('Y-m-d', strtotime($today . ' -1 month'));
}
if (!isset($search_to)) {
    $search_to = $today;
}
$view->newDiv('row align-items-end');
$view->submitDropDownChooser('', [BK_ALL => 'Összes', BK_OFFLINE => 'Átadott', BK_ONLINE => 'Rendezetlen'], 'selected_bk');
$view->inputDate('search_from', 'Időszak');
$view->inputDate('search_to');
$view->endDiv();
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
$sql .= ' AND (`' . BILLING_TIME . '` BETWEEN "' . $search_from . ' 00:00:00" AND "' . $search_to . ' 23:59:59" )';
$sql .= ' GROUP BY  `' . BILLING_NUMBER . '` ORDER BY `' . BILLING_TIME . '` DESC';
$res = $mysqliLink->query($sql);
if ($res AND $res->num_rows > 0) {
    while ($row = $res->fetch_assoc()) {
        $number = $row[BILLING_NUMBER];
        $payment = getPaymentByBillNumber($number);
        $printed = current(getBillsByNumber($number))[BILLING_OFFLINE];
        $view->newDiv('row d-flex align-items-center');
        $view->submitPrint('print_bill', $number, ($printed == 0 ? 'Tétel nyomtatása és ezzel a könyvelésnek átadottnak rögzítve' : 'Tétel ismételt nyomtatása'));
        $view->newDiv('col');
        if ($printed > 0) {
            $view->success('Nyomtatva: ' . $printed);
        }
        $view->putText($row[BILLING_TIME] . '&nbsp;&nbsp;&nbsp;'
                . getMemberName($row[BILLING_CREATOR]) . ' ˙(' . $row[BILLING_CREATOR] . ')', 'Kiállítva');
        $view->putText($payment[PAYMENT_TIME], 'Beérkezett');
        $view->putText(getSumOfBills($number) . ' Ft', 'Összeg');
        if ($payment[PAYMENT_BANK_REFNO] != 0) {
            $view->putText($payment[PAYMENT_BANK_REFNO], 'Kártyás fizetés, bankazonosító');
        }
        $view->endDiv()->newDiv('col');
        $view->putLabel('Tételek', true);
        foreach (billItems($number) as $bill) {
            $fm = getFmById($bill[BILLING_FM_ID]);
            $fee = getFeeById($fm[FM_FEE_ID]);
            $view->putText(FM_to_String($fm[FM_ID]));
        }
        $view->endDiv();
        $view->endDiv()->putElement('<hr>');
    }
} else {
    $view->warning('Jelenleg nincs ilyen elem');
}
