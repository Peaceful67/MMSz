<?php

chdir('..');
include_once 'params.inc';
include_once FUNCTIONS . 'init.inc';
include_once(SIMPLEPAY . "config.inc");
include_once(SIMPLEPAY . 'SimplePayment.class.inc');


$bill_number = $_GET['bill_number'];

$ios = new SimpleIos($config, 'HUF', $bill_number);
$ios->errorLogger();
$message = '<label>Azonosító : </label>' . $bill_number . '<br>';
$message .= '<label>Banki azonosító : </label><b>' . $ios->status['REFNO'] . '</b><br/>';
$message .= '<label>Fiztés időpontja : </label><b>' . $ios->status['ORDER_DATE'] . '</b><br/>';
$message .= '<label>Tranzakció állapota :</label><b>';
$message .= $payment_status[$ios->status['ORDER_STATUS']] . '</b><br/>';

if ($ios->status['ORDER_STATUS'] == 'FRAUD' OR
        $ios->status['ORDER_STATUS'] == 'CARD_NOTAUTHORIZED') {
    stornoBillNumber($bill_number);
} elseif (isBillPaied($bill_number)) {
//    error_log("IOS: " . $bill_number);
    $message .= $payment_status['CONFIRMED'];
}
if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
    $sz = new Szamlazz();
    $sz->getElementByBillNumber($bill_number);
    if ($sz->isPdfExists($bill_number)) {
        $message .= '<br><font color="green">A Számlázz.hu a számlát elkészítette és emailben elküldte.<br>';
        $message .= '<a target="_blank" title="Letöltés" href="' . $sz->getPdfURL($bill_number) . '">Innen</a> közvetlenül is letölthető.</font>';
    } else {
        $message .= '<br><br><font color="red">Várjuk a Számlázz.hu számlájának elkészültét.';
    }
}


echo $message;
?>