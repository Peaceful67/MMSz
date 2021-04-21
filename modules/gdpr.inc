<?php

if (isset($gdpr_submit) AND isset($gdpr_accept) AND $gdpr_accept == "on") {
    logger($member_id, $member_id, LOGGER_GPDR, "GDPR elfogadva");
    setGdpr($member_id);
    $mod = get_back();
    redirect(empty($mod)? "profile":$mod);
}
$output .= 'Olvasd el a '.getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME).' adatkezelési szabályzatát ';
$output .= '<a target="_blank" href="' . DOCUMENTS . 'gdpr.pdf">itt</a> és fogadd el !<br><br>';
$output .= '<input type="checkbox" name="gdpr_accept">';
$output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="gdpr_submit" value="Elfogadás">';
