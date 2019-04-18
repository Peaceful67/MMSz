<?php

$mysqliLink = new mysqli(DATABASE_ADDR, DATABASE_USER, DATABASE_PASSWORD, DATABASE_NAME);
if ($mysqliLink->connect_errno) {
    error_log("MySQL:" . $mysqliLink->connect_error);
    die(-1);
}
$mysqliLink->set_charset("utf8");

include_once FUNCTIONS . 'config_tables.inc';
include_once FUNCTIONS . 'config.inc';
include_once FUNCTIONS . 'menus.inc';
include_once 'texts.hu.inc';
include_once FUNCTIONS . 'functions.inc';
include_once FUNCTIONS . 'create_elements.inc';
include_once FUNCTIONS . 'email_functions.inc';
include_once FUNCTIONS . 'role_functions.inc';
include_once FUNCTIONS . 'fee_functions.inc';
include_once FUNCTIONS . 'chaptha.inc';
include_once FUNCTIONS . 'load_options.inc';
include_once FUNCTIONS . 'bill_functions.inc';
include_once FUNCTIONS . 'lm_functions.inc';
include_once INCLUDES . 'setup_load.inc';
include_once INCLUDES . 'crontab.inc';



