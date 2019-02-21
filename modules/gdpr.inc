<?php

if (isset($gdpr_submit) AND isset($gdpr_accept) AND $gdpr_accept == "on") {
    logger($member_id, $member_id, LOGGER_GPDR, "GDPR elfogadva");
    setGdpr($member_id);
    redirect("profile");
}
$output .= 'Olvasd el a '.COMPANY_LONG_NAME.' adatkezelési szabályzatát ';
$output .= '<a target="_blank" href="' . DOCUMENTS . 'gdpr.pdf">itt</a> és fogadd el !<br><br>';
$output .= '<form method ="post">';
$output .= '<input type="checkbox" name="gdpr_accept">';
$output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="gdpr_submit" value="Elfogadás">';
$output .= '</form>';
