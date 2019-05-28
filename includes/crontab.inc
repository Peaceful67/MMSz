<?php

$now = time();
/// Az index.php es a rss.php lefutasa utan ez is lefut.
// Utemezett korlevel kuldes 
if (!isset($options[OPTIONS_NAME_LAST_NEWSLETTER]) OR 0 == $options[OPTIONS_NAME_LAST_NEWSLETTER]) {
    logger(-1, -1, LOGGER_CRONTAB, "Körlevél köldés cronból 1");
    start_sending_newsletters();
}
$next = $options[OPTIONS_NAME_LAST_NEWSLETTER] + $options[OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER];
if ($next < $now) {
    updateOption(OPTIONS_NAME_LAST_NEWSLETTER, $now);
    logger(-1, -1, LOGGER_CRONTAB, "Körlevél köldés cronból 2");
    start_sending_newsletters();
}


// Napi rutin (havi)
if (!isset($options[OPTIONS_NAME_LAST_CRON_DAILY]) OR 0 == $options[OPTIONS_NAME_LAST_CRON_DAILY]) {
    updateOption(OPTIONS_NAME_LAST_CRON_DAILY, $now);
    include_once INCLUDES . 'cron_daily.inc';
} else {
    $next = (60 * 60 * 24 * 30) + $options[OPTIONS_NAME_LAST_CRON_DAILY];
    if ($next < $now) {
        updateOption(OPTIONS_NAME_LAST_CRON_DAILY, $now);
        include_once INCLUDES . 'cron_daily.inc';
    }
}

