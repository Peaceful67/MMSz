<?php

$now = time();
/// Az index.php es a rss.php lefutasa utan ez is lefut.
// Utemezett korlevel kuldes 
updateOption(OPTIONS_NAME_LAST_CRONTAB, $now);
if (!isset($options[OPTIONS_NAME_LAST_NEWSLETTER]) OR 0 == $options[OPTIONS_NAME_LAST_NEWSLETTER]) {
//    updateOption(OPTIONS_NAME_LAST_NEWSLETTER, $now);
    start_sending_newsletters();
    sending_emails();
}
$next = $options[OPTIONS_NAME_LAST_NEWSLETTER] + $options[OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER];
if ($next < $now) {
// Átkerült a levélküldéshez, hogy csak akkor frissítsük, ha tényleg küldünk levelet
//    updateOption(OPTIONS_NAME_LAST_NEWSLETTER, $now);
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

if (isset($admin_menu['congress_online']) AND $admin_menu['congress_create'][SETUP_MOD_ENABLED] > 0) {
    include_once POLLING . 'CongressPoll.inc';
    if (class_exists('CongressPoll')) {
        $cPoll = new CongressPoll();
        foreach ($cPoll->getPollsToBeClosed() as $poll_id) {
            if ($cPoll->setPollClosed($poll_id)) {
                logger(-1, -1, LOGGER_CONGRESS_ONLINE, 'A ' . $poll_id . 'azonosítójú határozat szavazása (congress_poll) automatikus lezárásra került.');
            }
        }
    }
}