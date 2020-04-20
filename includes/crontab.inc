<?php
$now = time();
/// Az index.php es a rss.php lefutasa utan ez is lefut.
// Utemezett korlevel kuldes 
updateOption(OPTIONS_NAME_LAST_CRONTAB, $now);
if (!isset($options[OPTIONS_NAME_LAST_NEWSLETTER]) OR 0 == $options[OPTIONS_NAME_LAST_NEWSLETTER]) {
    updateOption(OPTIONS_NAME_LAST_NEWSLETTER, $now);
    start_sending_newsletters();
    sending_emails();
}
$next = $options[OPTIONS_NAME_LAST_NEWSLETTER] + $options[OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER];
if ($next < $now) {
    updateOption(OPTIONS_NAME_LAST_NEWSLETTER, $now);
    start_sending_newsletters();
    sending_emails();
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

