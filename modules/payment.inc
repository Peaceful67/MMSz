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
    $output .= '<div class="lists"><form  method="post">'
            . '<table><tr><th>Azonosító</th><th>Indított</th><th>Összeg</th><th>Tartalom</th><th>Beérkezett</th></tr>';
    foreach ($unpaied as $bill) {
        $output .= '<tr><td>' . $bill . '</td><td>' . getTimeOfBills($bill) . '</td><td>' . getSumOfBills($bill) . ' Ft</td>';
        $output .= '<td>' . printBillItems($bill) . '</td>';
        $output .= '<td><button type="submit" name="pay_bill" value="' . $bill . '" '
                . 'onClick="return confirm(\'Biztosan beérkeztek a befizetések ?\')">Befizetes</button>&nbsp;&nbsp;&nbsp;&nbsp;'
                . '<button type="submit" name="storno_bill" value="' . $bill . '" '
                . 'onClick="return confirm(\'Biztosan sztornózod a tételt?\')">Sztorno</button></td></tr>';
    }
    $output .= '</table>';
    $output .= '</form></div>';
} else {
    $output .= 'Nincsenek kifizetetlen tételek.';
}
