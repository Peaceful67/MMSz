<?php

chdir('..');
require_once 'params.inc';
include_once FUNCTIONS . 'init.inc';
require_once SIMPLEPAY . 'config.inc';
require_once SIMPLEPAY . 'SimplePayment.class.inc';


$ipn = new SimpleIpn($config, 'HUF');

if ($ipn->validateReceived()) {
    $ipn->confirmReceived();

    if (createBillsFromFMS($REFNOEXT)) { // Legalább egy letrejott sikeresen
        logger(0, -1, LOGGER_BILL, 'A ' . $REFNOEXT . ' azonosítójú számlák létrehozása.');
        pay_bills($REFNOEXT, $REFNO);
        logger(0, -1, LOGGER_SIMPLEPAY, 'A ' . $REFNOEXT . ' azonosítójú számlák befizetésének rögzítése.  Banki azonosító: ' . $REFNO);
    } else {
        logger(0, -1, LOGGER_SIMPLEPAY, 'A ' . $REFNOEXT . ' azonosítójú számlára ismételt IPN érkezett. Rögzítés nem történt. Banki azonosító: ' . $REFNO);
    }
}
?>


