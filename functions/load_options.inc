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
define("OPTIONS_NAME_COMPANY_BANK", "company_bank");
define("OPTIONS_NAME_COMPANY_TELEFON", "company_telefon");


$options = [OPTIONS_NAME_LAST_NEWSLETTER => 0,
    OPTIONS_NAME_MAX_NEWSLETTERS => 300,
    OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER => 600,
    OPTIONS_NAME_ADMIN_EMAIL => 'peaceful.app@gmail.com',
    OPTIONS_NAME_FAI_NOTIF_EMAIL => FAI_NOTIF_EMAIL,
    OPTIONS_NAME_MMSZ_EMAIL => MMSZ_EMAIL,
    OPTIONS_NAME_MMSZ_ELNOK_EMAIL => MMSZ_ELNOK_EMAIL,
    OPTIONS_NAME_EMAIL_SENDER => MMSZ_EMAIL,
    OPTIONS_NAME_EMAIL_REPLY => MMSZ_EMAIL,
    OPTIONS_NAME_EMAIL_SIGNATURE => 'Az MMSz csapata',
    OPTIONS_NAME_COMPANY_NAME => 'MMSz',
    OPTIONS_NAME_COMPANY_LONG_NAME => 'Magyar Modelező Szövetség',
    OPTIONS_NAME_COMPANY_BANK => '12345678-12345678-12345678',
    OPTIONS_NAME_COMPANY_TELEFON => '70-1234567',
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
}

function getOptionValue($key) {
    global $options;
    return isset($options[$key]) ? $options[$key] : '';
}
