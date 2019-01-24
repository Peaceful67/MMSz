<?php

require_once SIMPLEPAY . 'config.inc';
require_once SIMPLEPAY . 'SimplePayment.class.inc';

$orderCurrency = 'HUF';
$lu = new SimpleLiveUpdate($config, $orderCurrency);
$lu->setField("ORDER_REF", $simplepay_number);

$lu->addProduct(array(
    'name' => 'Dijfizetés', //product name [ string ]
    'code' => $simplepay_number, //merchant systemwide unique product ID [ string ]
    'info' => COMPANY_NAME.' díjfizetés', //product description [ string ]
    'price' => $simplepay_sum, //product price [ HUF: integer | EUR, USD decimal 0.00 ]
    'vat' => 0, //product tax rate [ in case of gross price: 0 ] (percent)
    'qty' => 1                                         //product quantity [ integer ] 
));

//Billing data
$mem = retriveMember($member_id);
$lu->setField("BILL_FNAME", $mem[MEMBER_KERNEV]);
$lu->setField("BILL_LNAME", $mem[MEMBER_VEZNEV]);
$lu->setField("BILL_EMAIL", $mem[MEMBER_EMAIL]);
$lu->setField("BILL_PHONE", $mem[MEMBER_TEL]);
$lu->setField("BILL_COUNTRYCODE", "HU");
$lu->setField("BILL_STATE", "Magyarország");
$lu->setField("BILL_CITY", $mem[MEMBER_ADDR_CITY]);
$lu->setField("BILL_ADDRESS", $mem[MEMBER_ADDR_STREET]);
$lu->setField("BILL_ZIPCODE", $mem[MEMBER_ADDR_POST]);


//Delivery data
$lu->setField("DELIVERY_FNAME", $mem[MEMBER_KERNEV]);
$lu->setField("DELIVERY_LNAME", $mem[MEMBER_VEZNEV]);
$lu->setField("DELIVERY_EMAIL", $mem[MEMBER_EMAIL]);
$lu->setField("DELIVERY_PHONE", $mem[MEMBER_TEL]);
$lu->setField("DELIVERY_COUNTRYCODE", "HU");
$lu->setField("DELIVERY_STATE", "Magyarország");
$lu->setField("DELIVERY_CITY", $mem[MEMBER_ADDR_CITY]);
$lu->setField("DELIVERY_ADDRESS", $mem[MEMBER_ADDR_STREET]);
$lu->setField("DELIVERY_ZIPCODE", $mem[MEMBER_ADDR_POST]);

$msg = $lu->createHtmlForm('SimplePayForm', 'auto', 'Átirányítás a fizetési oldalra');   // format: link, button, auto (auto is redirects to payment page immediately )


$lu->errorLogger();
foreach ($lu->errorMessage as $error_msg) {
    error_log("card_pay error: " . $error_msg);
}

theEnd($msg);

?>

