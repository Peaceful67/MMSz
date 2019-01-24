<?php

if (isset($pay_bills)) {
    if (count($check_bills) > 0) {
        foreach ($check_bills as $bill) {
            logger($member_id,-1, LOGGER_PAY,  'A '.$bill.' azonosítójú számlák befizetésének rögzítése');
            pay_bills($bill);
        }
    } else {
        warning("Legalább egy tételt válassz ki !");
    }
}

$unpaied = getUnpaiedBillNumbers();

if (count($unpaied) > 0) {
    $output .= '<div class="lists"><form  method="post">'
            . '<table><tr><th>Azonosító</th><th>Indított</th><th>Összeg</th><th>Tartalom</th><th>Beérkezett</th></tr>';
    foreach ($unpaied as $bill) {
        $output .= '<tr><td>' . $bill . '</td><td>' . getTimeOfBills($bill) . '</td><td>' . getSumOfBills($bill) . ' Ft</td>';
        $output .= '<td>'. printBillItems($bill).'</td>';
        $output .= '<td><input type="checkbox" name="check_bills[]" value="' . $bill . '"></td></tr>';
    }
    $output .= '</table>';
    $output .= '<input type="submit" name="pay_bills" value="Rögzítés" onClick="return confirm(\'Biztosan beérkeztek a befizetések ?\')">'
            . '</form></div>';
} else {
    $output .= 'Nincsenek kifizetetlen tételek.';
}
