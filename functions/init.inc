<?php

$mysqliLink = new MYSQLi(DATABASE_ADDR, DATABASE_USER, DATABASE_PASSWORD, DATABASE_NAME) or die(mysqli_error());
if ($mysqliLink->connect_errno) {
    error_log("MySQL hiba:" . $mysqliLink->connect_error);
    die(-1);
}
$mysqliLink->set_charset("utf8");
$now = time();
define("VERSION", "3.0");
if (!isset($redirected)) {
    $redirected = false;
}
$existingTables = array();
$sql = 'SELECT * FROM information_schema.TABLES WHERE TABLE_SCHEMA = "' . DATABASE_NAME . '";';
$res = $mysqliLink->query($sql);
while ($res AND $row = $res->fetch_assoc()) {
    $existingTables[] = $row['TABLE_NAME'];
}

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
include_once FUNCTIONS . 'select_members.inc';
include_once FUNCTIONS . 'create_mysql_views.inc';
//include_once INCLUDES . 'setup_init.inc';

if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
    opcache_reset();
}
if (getOptionValue(OPTIONS_NAME_ERROR_LOG) > 0) {
    opcache_reset();
    if (isset($_SERVER['CONTENT_LENGTH'])) {
        error_log('Post size:' . $_SERVER['CONTENT_LENGTH']);
    } else {
        error_log('No posts at all');
    }
    foreach ($_POST as $key => $value) {
        if (is_array($value)) {
            error_log($key . '->' . print_r($value, true));
        } else {
            error_log($key . '->' . $value);
        }
        $$key = $value;
    }
    if (getOptionValue(OPTIONS_NAME_ERROR_LOG_SESSIONS) > 0) {
        foreach ($_SESSION as $key => $value) {
            if (is_array($value)) {
                error_log('Session:' . $key . '->' . print_r($value, true));
            } else {
                error_log('Session:' . $key . '->' . $value);
            }
            $$key = $value;
        }
    }
    if (getOptionValue(OPTIONS_NAME_ERROR_LOG_COOKIE) > 0) {
        foreach ($_COOKIE as $key => $value) {
            if (is_array($value)) {
                error_log('Cookie:' . $key . '->' . print_r($value, true));
            } else {
                error_log('Cookie:' . $key . '->' . $value);
            }
            $$key = $value;
        }
    }
}

$fee_type_names = [];
foreach ($fee_type as $key => $value) {
    $fee_type_names[$key] = $value[0];
}
if (VERSION != getOptionValue(OPTIONS_NAME_VERSION_CURRENT)) {
    updateOption(OPTIONS_NAME_VERSION_CURRENT, VERSION);
    updateOption(OPTIONS_NAME_VERSION_UPDATED, $now);
}