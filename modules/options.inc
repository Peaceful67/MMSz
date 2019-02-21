<?php

if (!isset($expand_newsletter)) {
    $expand_newsletter = 0;
}
if (!isset($expand_emails)) {
    $expand_emails = 0;
}
if (!isset($expand_company)) {
    $expand_company = 0;
}

if (isset($save_options)) {
    if ($expand_newsletter != 0) {
        changeValue($max_newsletter, OPTIONS_NAME_MAX_NEWSLETTERS);
        changeValue($next_newsletter, OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER);
    }
    if ($expand_emails != 0) {
        changeValue($admin_email, OPTIONS_NAME_ADMIN_EMAIL);
        changeValue($mmsz_email, OPTIONS_NAME_MMSZ_EMAIL);
        changeValue($mmsz_elnok_email, OPTIONS_NAME_MMSZ_ELNOK_EMAIL);
        changeValue($fai_notif_email, OPTIONS_NAME_FAI_NOTIF_EMAIL);
        changeValue($email_sender, OPTIONS_NAME_EMAIL_SENDER);
        changeValue($email_signature, OPTIONS_NAME_EMAIL_SIGNATURE);
        changeValue($email_reply, OPTIONS_NAME_EMAIL_REPLY);
    }
}

$output .= '<div class="editor"><form method="POST">';
$output .= '<input type="hidden" name="expand_newsletter" value="' . $expand_newsletter . '">';
$output .= '<input type="hidden" name="expand_emails" value="' . $expand_emails . '">';
$output .= '<input type="hidden" name="expand_company" value="' . $expand_company . '">';
$output .= '<label>Körlevél beállítások: </label>';
if (0 == $expand_newsletter) {
    $output .= '<button name="expand_newsletter" value="1" ><img src="' . IMAGES . 'expand.jpg"> </button><br>';
} else {
    $output .= '<button name="expand_newsletter" value="0" ><img src="' . IMAGES . 'collapse.jpg"> </button>';
    $output .= '<div class="keret">';
    $output .= '<label>Utolsó körlevél ellenőrzés:</label>' . date("Y.m.d H:i:s", $options[OPTIONS_NAME_LAST_NEWSLETTER]) . '<br>';
    $output .= '<label>Max körlevél szám:</label><input type="text" name="max_newsletter" value="' . $options[OPTIONS_NAME_MAX_NEWSLETTERS] . '"><br>';
    $output .= '<label>Körlevél időköz (mp):</label><input type="text" name="next_newsletter" value="' . $options[OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER] . '"><br>';
    $output .= '</div>';
}

$output .= '<label>Email címek: </label>';
if (0 == $expand_emails) {
    $output .= '<button name="expand_emails" value="1" ><img src="' . IMAGES . 'expand.jpg"> </button><br>';
} else {
    $output .= '<button name="expand_emails" value="0" ><img src="' . IMAGES . 'collapse.jpg"> </button>';
    $output .= '<div class="keret">';
    $output .= '<label>Adminisztrátor:</label><input type="text" name="admin_email" value="' . $options[OPTIONS_NAME_ADMIN_EMAIL] . '"><br>';
    $output .= '<label>Szövetség:</label><input type="text" name="mmsz_email" value="' . $options[OPTIONS_NAME_MMSZ_EMAIL] . '"><br>';
    $output .= '<label>Szövetség elnök:</label><input type="text" name="mmsz_elnok_email" value="' . $options[OPTIONS_NAME_MMSZ_ELNOK_EMAIL] . '"><br>';
    $output .= '<label>Befizetés értesítés:</label><input type="text" name="fai_notif_email" value="' . $options[OPTIONS_NAME_FAI_NOTIF_EMAIL] . '"><br>';
    $output .= '<label>Emailek válaszcíme:</label><input type="text" name="email_reply" value="' . $options[OPTIONS_NAME_EMAIL_REPLY] . '"><br>';
    $output .= '<label>Emailek feladója:</label><input type="text" name="email_sender" value="' . $options[OPTIONS_NAME_EMAIL_SENDER] . '"><br>';
    $output .= '<label>Emailek aláírása:</label><input type="text" name="email_signature" value="' . $options[OPTIONS_NAME_EMAIL_SIGNATURE] . '"><br>';
    $output .= '</div>';
}
$output .= '<label>Szövetség: </label>';
if (0 == $expand_company) {
    $output .= '<button name="expand_company" value="0" ><img src="' . IMAGES . 'expand.jpg"> </button><br>';
} else {
    $output .= '<button name="expand_company" value="0" ><img src="' . IMAGES . 'collapse.jpg"> </button>';
    $output .= '<div class="keret">';
    $output .= '<label>Adminisztrátor:</label><input type="text" name="admin_email" value="' . $options[OPTIONS_NAME_ADMIN_EMAIL] . '"><br>';
    $output .= '<label>Szövetség:</label><input type="text" name="mmsz_email" value="' . $options[OPTIONS_NAME_MMSZ_EMAIL] . '"><br>';
    $output .= '<label>Szövetség elnök:</label><input type="text" name="mmsz_elnok_email" value="' . $options[OPTIONS_NAME_MMSZ_ELNOK_EMAIL] . '"><br>';
    $output .= '<label>Befizetés értesítés:</label><input type="text" name="fai_notif_email" value="' . $options[OPTIONS_NAME_FAI_NOTIF_EMAIL] . '"><br>';
    $output .= '<label>Emailek feladója:</label><input type="text" name="email_sender" value="' . $options[OPTIONS_NAME_EMAIL_SENDER] . '"><br>';
    $output .= '<label>Emailek aláírása:</label><input type="text" name="email_signature" value="' . $options[OPTIONS_NAME_EMAIL_SIGNATURE] . '"><br>';
    $output .= '</div>';
}



$output .= '<br><input type="submit" name="save_options" value="Mentés">';
$output .= '</form></div>';

function changeValue($var, $key) {
    global $options, $member_id;
    if (isset($var) AND $var != $options[$key]) {
        updateOption($key, $var);
        logger($member_id, -1, LOGGER_OPTIONS, $key . ' új értéke: ' . $var);
    }
}
