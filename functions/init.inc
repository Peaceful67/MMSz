<?php

$mysqliLink = new mysqli(DATABASE_ADDR, DATABASE_USER, DATABASE_PASSWORD, DATABASE_NAME);
if ($mysqliLink->connect_errno) {
    error_log("MySQL:" . $mysqliLink->connect_error);
    die(-1);
}
$mysqliLink->set_charset("utf8");

require_once FUNCTIONS . 'config_tables.inc';
require_once FUNCTIONS . 'config.inc';
require_once FUNCTIONS . 'menus.inc';
require_once 'texts.hu.inc';
require_once FUNCTIONS . 'functions.inc';
require_once FUNCTIONS . 'email_functions.inc';
require_once FUNCTIONS . 'role_functions.inc';
require_once FUNCTIONS . 'fee_functions.inc';
require_once FUNCTIONS . 'chaptha.inc';
require_once FUNCTIONS . 'bill_functions.inc';
require_once FUNCTIONS . 'lm_functions.inc';
require_once INCLUDES . 'setup_load.inc';


