<?php

$mysqliLink = new MYSQLi(DATABASE_ADDR, DATABASE_USER, DATABASE_PASSWORD, DATABASE_NAME) or die(mysqli_error());
if ($mysqliLink->connect_errno) {
    error_log("MySQL hiba:" . $mysqliLink->connect_error);
    die(-1);
}
$mysqliLink->set_charset("utf8");
$now = time();
include_once FUNCTIONS . 'config_tables.inc';
include_once FUNCTIONS . 'config.inc';
include_once FUNCTIONS . 'menus.inc';
include_once 'texts.hu.inc';
include_once FUNCTIONS . 'functions.inc';
include_once FUNCTIONS . 'output.inc';
include_once FUNCTIONS . 'create_elements.inc';
include_once FUNCTIONS . 'email_functions.inc';
include_once FUNCTIONS . 'role_functions.inc';
include_once FUNCTIONS . 'fee_functions.inc';
include_once FUNCTIONS . 'chaptha.inc';
include_once FUNCTIONS . 'load_options.inc';
include_once FUNCTIONS . 'bill_functions.inc';
include_once FUNCTIONS . 'lm_functions.inc';
include_once FUNCTIONS . 'fai.inc';
include_once INCLUDES . 'setup_load.inc';
// include_once INCLUDES . 'crontab.inc';

if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
    opcache_reset();
    foreach ($_POST as $key => $value) {
        if (is_array($value)) {
            error_log($key . '->' . print_r($value, true));
        } else {
            error_log($key . '->' . $value);
        }
        $$key = $value;
    }
}


