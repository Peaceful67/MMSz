<?php
$now = time();
if(!isset($options[OPTIONS_NAME_LAST_NEWSLETTER]) OR 0==$options[OPTIONS_NAME_LAST_NEWSLETTER]) {
            start_sending_newsletters();
}
$next = $options[OPTIONS_NAME_LAST_NEWSLETTER]+$options[OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER];
if($next < $now) {
            start_sending_newsletters();
}
