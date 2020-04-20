<?php

define("OPTIONS_NAME_LAST_NEWSLETTER", 'last_newsletter');
define("OPTIONS_NAME_MAX_NEWSLETTERS", 'max_newsletters');
define("OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER", 'next_newsletter');

define("OPTIONS_NAME_ADMIN_EMAIL", "admin_email");
define("OPTIONS_NAME_FAI_NOTIF_EMAIL", "fai_notif_email");
define("OPTIONS_NAME_MMSZ_EMAIL", "mmsz_email");
define("OPTIONS_NAME_MMSZ_ELNOK_EMAIL", "mmsz_elnok_email");
define("OPTIONS_NAME_EMAIL_SENDER", "email_sender");
define("OPTIONS_NAME_EMAIL_REPLY", "email_reply");
define("OPTIONS_NAME_EMAIL_SIGNATURE", "email_signature");


define("OPTIONS_NAME_COMPANY_NAME", "company_name");
define("OPTIONS_NAME_COMPANY_LONG_NAME", "company_long_name");
define("OPTIONS_NAME_COMPANY_ADDR", "company_addr");
define("OPTIONS_NAME_COMPANY_BANK_NAME", "company_bank_name");
define("OPTIONS_NAME_COMPANY_BANK_ACCOUNT", "company_bank_account");
define("OPTIONS_NAME_COMPANY_TELEFON", "company_telefon");

define("OPTIONS_NAME_DEVELOPMENT", 'development');
define("OPTIONS_NAME_PASSWORD_LINK", 'password_link');
define("OPTIONS_NAME_DELEGATE_MOD", 'delegate_mod');
//define("OPTIONS_NAME_DELEGATE_MOD_LINEAR", 'linear');
//define("OPTIONS_NAME_DELEGATE_MOD_RANGE", 'range');
define("OPTIONS_NAME_DELEGATE_STEP", 'delegate_step');
define("OPTIONS_NAME_DELEGATE_RANGE", 'delegate_range');
define("OPTIONS_NAME_HELP_ENABLED", 'help');

define("OPTIONS_NAME_SZAMLAZZ_ENA", 'szamlazz_ena');
define("OPTIONS_NAME_SZAMLAZZ_USER", 'szamlazz_user');
define("OPTIONS_NAME_SZAMLAZZ_PASSWD", 'szamlazz_passwd');
define("OPTIONS_NAME_SZAMLAZZ_PREFIX", 'szamlazz_prefix');

define("OPTIONS_NAME_FAI_ENABLED", 'fai_enabled');
define('OPTIONS_NAME_FAI_URL', 'fai_licence_url');
define('OPTIONS_NAME_FAI_VIEW', 'fai_licence_view');
define('OPTIONS_NAME_FAI_PRINT', 'fai_licence_print');
define('OPTIONS_NAME_FAI_AUTH_USER', 'fai_auth_user');
define('OPTIONS_NAME_FAI_AUTH_PASSWD', 'fai_auth_passwd');

define("OPTIONS_NAME_LAST_CRON_DAILY", 'cron_daily');
define("OPTIONS_NAME_LAST_CRONTAB", 'last_crontab');


$options = [OPTIONS_NAME_LAST_NEWSLETTER => 0,
    OPTIONS_NAME_MAX_NEWSLETTERS => 300,
    OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER => 600,
    OPTIONS_NAME_ADMIN_EMAIL => 'peaceful.app@gmail.com',
    OPTIONS_NAME_FAI_NOTIF_EMAIL => "",
    OPTIONS_NAME_MMSZ_EMAIL => "",
    OPTIONS_NAME_MMSZ_ELNOK_EMAIL => "",
    OPTIONS_NAME_EMAIL_SENDER => '',
    OPTIONS_NAME_EMAIL_REPLY => '',
    OPTIONS_NAME_EMAIL_SIGNATURE => 'Az MMSz csapata',
    OPTIONS_NAME_COMPANY_NAME => 'MMSz',
    OPTIONS_NAME_COMPANY_LONG_NAME => 'Magyar Modellező Szövetség',
    OPTIONS_NAME_COMPANY_ADDR => 'Addr',
    OPTIONS_NAME_COMPANY_BANK_NAME => 'Bank',
    OPTIONS_NAME_COMPANY_BANK_ACCOUNT => '12345678-12345678-12345678',
    OPTIONS_NAME_COMPANY_TELEFON => '70-1234567',
    OPTIONS_NAME_LAST_CRON_DAILY => 0,
    OPTIONS_NAME_LAST_CRONTAB => 0,
    OPTIONS_NAME_DEVELOPMENT => 0,
    OPTIONS_NAME_PASSWORD_LINK => 1,
    OPTIONS_NAME_DELEGATE_MOD => 1,
    OPTIONS_NAME_DELEGATE_STEP => 10,
    OPTIONS_NAME_DELEGATE_RANGE  => serialize(array(0)),
    OPTIONS_NAME_SZAMLAZZ_ENA => 0,
    OPTIONS_NAME_SZAMLAZZ_USER => '',
    OPTIONS_NAME_SZAMLAZZ_PASSWD => '',
    OPTIONS_NAME_SZAMLAZZ_PREFIX => '',
    OPTIONS_NAME_FAI_ENABLED => 0,
    OPTIONS_NAME_FAI_URL => 'https://sl-test.fai.org/api/v1/',
    OPTIONS_NAME_FAI_VIEW => 'https://sl-test.fai.org/licence/check/',
    OPTIONS_NAME_FAI_PRINT => 'https://sl-test.fai.org/licence?view=faisl&layout=view&tmpl=component&print=1&licence_id=',
    OPTIONS_NAME_FAI_AUTH_USER => 'hun-ciam',
    OPTIONS_NAME_FAI_AUTH_PASSWD => '6EhBft6I',
    OPTIONS_NAME_HELP_ENABLED => 0,
];


$db_options = array();
$sql = 'SELECT * FROM `' . OPTIONS_TABLE . '` ';
$res = $mysqliLink->query($sql);
while ($res AND $row = $res->fetch_assoc()) {
    $db_options[$row[OPTIONS_NAME]] = $row[OPTIONS_VALUE];
}
foreach ($options as $name => $value) {
    if (isset($db_options[$name])) {
        $options[$name] = $db_options[$name];
    } else {
        updateOption($name, $value);
    }
}

function updateOption($name, $value) {
    global $mysqliLink, $options, $db_options;
    if (isset($db_options[$name])) {
        $sql = 'UPDATE `' . OPTIONS_TABLE . '` SET `' . OPTIONS_VALUE . '`="' . $value . '" '
                . 'WHERE `' . OPTIONS_NAME . '`="' . $name . '" ;';
    } else {
        $sql = 'INSERT INTO `' . OPTIONS_TABLE . '` '
                . '(`' . OPTIONS_NAME . '`, `' . OPTIONS_VALUE . '`)'
                . 'VALUES ("' . $name . '", "' . $value . '");';
    }
    $mysqliLink->query($sql);
    $options[$name] = $value;
    return $value;
}

function getOptionValue($key) {
    global $options;
    return isset($options[$key]) ? $options[$key] : '';
}

function retrieveOptionValue($key) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . OPTIONS_TABLE . '` WHERE `' . OPTIONS_NAME . '`="' . $key . '" LIMIT 1; ';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        return $res->fetch_assoc()[OPTIONS_NAME];
    } else {
        return '';
    }
}


