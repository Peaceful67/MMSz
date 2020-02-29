<?php

require_once (SIMPLEPAY . 'config.inc');
require_once (SIMPLEPAY . 'SimplePayment.class.inc');

$orderCurrency = (isset($_GET['order_currency'])) ? $_GET['order_currency'] : 'N/A';
$orderRef = (isset($_GET['order_ref'])) ? $_GET['order_ref'] : 'N/A';

$backref = new SimpleBackRef($config, $orderCurrency);
$backref->order_ref = $orderRef;

$backref->errorLogger();
$_SESSION[SESSION_SP_BACKREF] = serialize($backref);
?>
