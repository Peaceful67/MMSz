<?php

if (isset($pay_bill)) {
    logger($member_id, -1, LOGGER_PAY, 'A ' . $pay_bill . ' azonosítójú számlák befizetésének rögzítése');
    pay_bills($pay_bill);
}

if (isset($storno_bill)) {
    logger($member_id, -1, LOGGER_BILL, 'A ' . $storno_bill . ' azonosítójú számlák sztornózva');
    stornoBillNumber($storno_bill);
}


$unpaied = getUnpaiedBillNumbers();

if (count($unpaied) > 0) {
    $view->newTable('Kifizetetlen tételek');
    $view->putThead('', ['Azonosító', 'Indított', 'Összeg', 'Tartalom', 'Művelet']);
    foreach ($unpaied as $bill) {
        $view->newTableTr();
        $view->putElement('<td>' . $bill . '</td>');
        $view->putElement('<td>' . getTimeOfBills($bill) . '</td>');
        $view->putElement('<td>' . getSumOfBills($bill) . '</td>');
        $view->putElement('<td>' . printBillItems($bill) . '</td>');
        $view->newElement('<td>');
        $view->submitButton('pay_bill', 'Befizetes', $bill, 'A tétel befizetésének lekönyvelése', 'Biztosan beérkeztek a befizetések ?');
        $view->submitButton('storno_bill', 'Sztorno', $bill, 'A tétel sztornózása', 'Biztosan sztornózod a tételt?');
        $view->endElement('</td>');
        $view->endTableTr();
    }
    $view->endTable();
} else {
    $view->warning('Nincsenek kifizetetlen tételek.');
}
