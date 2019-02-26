<?php

$now = time();
if (!isset($options[OPTIONS_NAME_LAST_NEWSLETTER]) OR 0 == $options[OPTIONS_NAME_LAST_NEWSLETTER]) {
    start_sending_newsletters();
}
$next = $options[OPTIONS_NAME_LAST_NEWSLETTER] + $options[OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER];
if ($next < $now) {
    $lock = updateOption(OPTIONS_NAME_LAST_NEWSLETTER, $now);
    if (retrieveOptionValue(OPTIONS_NAME_LAST_NEWSLETTER) == $lock) {
        start_sending_newsletters();
    }
}

if (!isset($options[OPTIONS_NAME_LAST_CRON_DAILY]) OR 0 == $options[OPTIONS_NAME_LAST_CRON_DAILY]) {
    updateOption(OPTIONS_NAME_LAST_CRON_DAILY, $now);
    include_once INCLUDES . 'cron_daily.inc';
} else {
    $next = (60 * 60 * 24) + $options[OPTIONS_NAME_LAST_CRON_DAILY] ;
    if ($next < $now) {
        $lock = updateOption(OPTIONS_NAME_LAST_CRON_DAILY, $now);
        if (retrieveOptionValue(OPTIONS_NAME_LAST_CRON_DAILY) == $lock) {
            include_once INCLUDES . 'cron_daily.inc';
        }
    }
}

