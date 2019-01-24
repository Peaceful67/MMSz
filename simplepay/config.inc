<?php

if (DEVELOPMENT) {
    $config = array(
        'HUF_MERCHANT' => SIMPLE_PAY_MERCHANT, //merchant account ID (HUF)
        'HUF_SECRET_KEY' => SIMPLE_PAY_KEY, //secret key for account ID (HUF)	
        'EUR_MERCHANT' => "", //merchant account ID (EUR)
        'EUR_SECRET_KEY' => "", //secret key for account ID (EUR)
        'USD_MERCHANT' => "", //merchant account ID (USD)
        'USD_SECRET_KEY' => "", //secret key for account ID (USD)
        'CURL' => true, //use cURL or not
        'SANDBOX' => true, //true: sandbox transaction, false: live transaction
        'PROTOCOL' => 'http', //http or https
        'BACK_REF' => $_SERVER['HTTP_HOST'] . '?simplepay=card_backref', //url of payment backref page
        'TIMEOUT_URL' => $_SERVER['HTTP_HOST'] . '?sp_timeout=1', //url of payment timeout page
        'GET_DATA' => $_GET,
        'POST_DATA' => $_POST,
        'SERVER_DATA' => $_SERVER,
        'LOGGER' => true, //basic transaction log
        'LOG_PATH' => 'log', //path of log file
        'DEBUG_LIVEUPDATE_PAGE' => true, //Debug message on demo LiveUpdate page (only for development purpose)
        'DEBUG_LIVEUPDATE' => true, //LiveUpdate debug into log file
        'DEBUG_BACKREF' => true, //BackRef debug into log file
        'DEBUG_IPN' => true, //IPN debug into log file
        'DEBUG_IRN' => false, //IRN debug into log file
        'DEBUG_IDN' => false, //IDN debug into log file
        'DEBUG_IOS' => false, //IOS debug into log file
        'DEBUG_ONECLICK' => false, //OneClick debug into log file
        'DEBUG_ALU' => false, //ALU debug into log file
    );
} else {
    $config = array(
        'HUF_MERCHANT' => SIMPLE_PAY_MERCHANT, //merchant account ID (HUF)
        'HUF_SECRET_KEY' => SIMPLE_PAY_KEY, //secret key for account ID (HUF)	
        'EUR_MERCHANT' => "", //merchant account ID (EUR)
        'EUR_SECRET_KEY' => "", //secret key for account ID (EUR)
        'USD_MERCHANT' => "", //merchant account ID (USD)
        'USD_SECRET_KEY' => "", //secret key for account ID (USD)
        'CURL' => true, //use cURL or not
        'SANDBOX' => false, //true: sandbox transaction, false: live transaction
        'PROTOCOL' => 'http', //http or https
        'BACK_REF' => $_SERVER['HTTP_HOST'] . '?simplepay=card_backref', //url of payment backref page
        'TIMEOUT_URL' => $_SERVER['HTTP_HOST'] . '?sp_timeout=1', //url of payment timeout page
        'GET_DATA' => $_GET,
        'POST_DATA' => $_POST,
        'SERVER_DATA' => $_SERVER,
        'LOGGER' => true, //basic transaction log
        'LOG_PATH' => 'log', //path of log file
        'DEBUG_LIVEUPDATE_PAGE' => true, //Debug message on demo LiveUpdate page (only for development purpose)
        'DEBUG_LIVEUPDATE' => true, //LiveUpdate debug into log file
        'DEBUG_BACKREF' => true, //BackRef debug into log file
        'DEBUG_IPN' => true, //IPN debug into log file
        'DEBUG_IRN' => false, //IRN debug into log file
        'DEBUG_IDN' => false, //IDN debug into log file
        'DEBUG_IOS' => false, //IOS debug into log file
        'DEBUG_ONECLICK' => false, //OneClick debug into log file
        'DEBUG_ALU' => false, //ALU debug into log file
    );
}
$payment_status = array(
    'SUCCESSFUL_CARD_AUTHORIZATION' => '<font color="orange">Sikeres kártya ellenőrzés !</font>',
    'IN_PROGRESS' => '<font color="orange">Feldolgozás alatt, megerősítésre vár !</font>',
    'PAYMENT_AUTHORIZED' => '<font color="orange">Fizetés feldolgozva, megerősítésre vár !</font>',
    'TIMEOUT' => '<font color="red">Időtúllépés a fizetőoldalon</font>',
    'CANCELLED' => '<font color="red">A fizetőoldalon megszakított fizetés</font>',
    'FRAUD' => '<font color="red">Csalás gyanú</font>',
    'WAITING_PAYMENT' => '<font color="yellow">Bankkártya fizetés közben, '
    . 'vagy átutalás esetén a befizetésre várakozás</font>',
    'COMPLETE' => '<font color="green">Fizetés feldolgozva.</font>',
    'CONFIRMED' => '<font color="green">A tranzakció sikeresen lezajlott !<br>'
    . 'A teljesítésről rövidesen emailben megerősítést fogsz kapni.</font>',
    'EMPTY RESULT' => '<font color="orange">Még nem érkezett válasz a OTP fizető szerverétől!</font>', 
    'CARD_NOTAUTHORIZED' => '<font color="red">A bank a tranzakciót elutasította !<br>'
    . 'Kérjük, ellenőrizze a tranzakció során megadott adatok helyességét !<br>'
    . 'Amennyiben minden adatot helyesen adott meg, a visszautasítás okának kivizsgálása '
    . 'érdekében kérjük, szíveskedjen kapcsolatba lépni kártyakibocsátó bankjával.</font>',
);
?>