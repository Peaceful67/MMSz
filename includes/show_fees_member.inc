<?php

if (!isset($sf_member)) {
    $sf_member = $member_id;
}

$view->newDiv('table-responsive-md');
$view->newElement('<table class="table table-striped table-bordered"><caption>Díjak listája</caption>');
$view->newElement('<tr><th scope="col">Típus</th><th scope="col">Név</th><th scope="col">Kezdet</th><th scope="col">Vég</th><th scope="col">Összeg</th><th scope="col">Állapot</th></tr>');
$fms = getFMsOfMember($sf_member);
foreach ($fms as $fm) {
    $fee = getFeeById($fm[FM_FEE_ID]);
    if ($fm[FM_PAY_ID] != 0) { //Ha ki van fizetve, a bill-bol vesszuk
        $sum = getFmSumFromBill($fm);
    } else { // Ha nincs kifizetve a fee-bol
        $sum = $fee[FEE_SUM];
    }
    $view->newElement('<tr>');
    $view->putElement('<td>' . $fee_type[$fee[FEE_TYPE]][0] . '</td>');
    $view->putElement('<td>' . printFeeName($fee) . '</td>');
    $view->putElement('<td>' . $fm[FM_FROM] . '</td>');
    $view->putElement('<td>' . $fm[FM_TO] . '</td>');
    $view->putElement('<td>' . $sum . ' Ft</td>');
    $view->putElement('<td>');
    if ($fm[FM_BILL_ID] == 0) {
        if (isset($deletable_fm) && $deletable_fm) {
            $view->submitDelete('delete_fm', $fm[FM_ID], 'Díjtétel törlése', 'Biztosan törölni akarod a '.printFeeName($fee).' tételt ?');
        } else {
            $view->put('Beállítva');
        }
    } elseif ($fm[FM_PAY_ID] == 0) {
        $view->put('Fizetésre vár');
    } else {
        $view->put('Rendezve');
    }
    $view->put('</td>');
    $view->endElement('</tr>');
}
$view->endElement('</table>')->endDiv();
