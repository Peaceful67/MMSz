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
if (!isset($expand_general)) {
    $expand_general = 0;
}
if (!isset($expand_szamlazz)) {
    $expand_szamlazz = 0;
}
if (!isset($expand_fai)) {
    $expand_fai = 0;
}
if (!isset($role_as_member)) {
    $role_as_member = $member_id;
}
if (isset($delegate_add_member) AND isset($delegate_add_range)) {
    $ranges = unserialize(getOptionValue(OPTIONS_NAME_DELEGATE_RANGE));
    $ranges[$delegate_add_range] = intval($delegate_add_member, 10);
    sort($ranges);
    $new = serialize(array_values($ranges));
    changeValue($new, OPTIONS_NAME_DELEGATE_RANGE);
}
if (isset($delegate_remove_range)) {
    $ranges = unserialize(getOptionValue(OPTIONS_NAME_DELEGATE_RANGE));
    unset($ranges[$delegate_remove_range]);
    sort($ranges);
    $new = serialize(array_values($ranges));
    changeValue($new, OPTIONS_NAME_DELEGATE_RANGE);
}

if (isset($save_options)) {
    if ($expand_fai != 0) {
        if (!isset($fai_ena)) {
            $fai_ena = 'off';
        }
        changeValue($fai_ena, OPTIONS_NAME_FAI_ENABLED);
        changeValue($fai_user, OPTIONS_NAME_FAI_AUTH_USER);
        changeValue($fai_passwd, OPTIONS_NAME_FAI_AUTH_PASSWD);
        changeValue($fai_url, OPTIONS_NAME_FAI_URL);
        changeValue($fai_view, OPTIONS_NAME_FAI_VIEW);
        changeValue($fai_print, OPTIONS_NAME_FAI_PRINT);
    }
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
    if ($expand_general != 0) {
        if (!isset($development)) {
            $development = 'off';
        }
        if (!isset($password_link)) {
            $password_link = 'off';
        }
        if (!isset($delegate_mod)) {
            $delegate_mod = 'off';
        }
        if ($delegate_mod == 'on') {
            if (!isset($delegate_step)) {
                $delegate_step = 10;
            }
            changeValue($delegate_step, OPTIONS_NAME_DELEGATE_STEP);
        }
        if ($role_as_member != $member_id) {
            $_SESSION[SESSION_ROLE_AS] = $role_as_member;
        }
        changeValue($delegate_mod, OPTIONS_NAME_DELEGATE_MOD);
        changeValue($development, OPTIONS_NAME_DEVELOPMENT);
        changeValue($password_link, OPTIONS_NAME_PASSWORD_LINK);
    }
    if ($expand_company != 0) {
        changeValue($company_name, OPTIONS_NAME_COMPANY_NAME);
        changeValue($company_long, OPTIONS_NAME_COMPANY_LONG_NAME);
        changeValue($company_addr, OPTIONS_NAME_COMPANY_ADDR);
        changeValue($company_telefon, OPTIONS_NAME_COMPANY_TELEFON);
        changeValue($company_bank_name, OPTIONS_NAME_COMPANY_BANK_NAME);
        changeValue($company_bank_account, OPTIONS_NAME_COMPANY_BANK_ACCOUNT);
    }
    if ($expand_szamlazz != 0) {
        if (!isset($szamlazz_ena)) {
            $szamlazz_ena = 'off';
        }
        changeValue($szamlazz_ena, OPTIONS_NAME_SZAMLAZZ_ENA);
        changeValue($szamlazz_user, OPTIONS_NAME_SZAMLAZZ_USER);
        changeValue($szamlazz_passwd, OPTIONS_NAME_SZAMLAZZ_PASSWD);
        changeValue($szamlazz_prefix, OPTIONS_NAME_SZAMLAZZ_PREFIX);
    }
    $expand_company = $expand_emails = $expand_fai = $expand_general = $expand_newsletter = $expand_szamlazz = 0;
}

$output .= '<div class="editor"><form method="POST">';
$output .= '<input type="hidden" name="expand_newsletter" value="' . $expand_newsletter . '">';
$output .= '<input type="hidden" name="expand_emails" value="' . $expand_emails . '">';
$output .= '<input type="hidden" name="expand_company" value="' . $expand_company . '">';
$output .= '<input type="hidden" name="expand_general" value="' . $expand_general . '">';
$output .= '<input type="hidden" name="expand_szamlazz" value="' . $expand_szamlazz . '">';
$output .= '<input type="hidden" name="expand_fai" value="' . $expand_fai . '">';
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
$output .= '<label>Szövetség adatok: </label>';
if (0 == $expand_company) {
    $output .= '<button name="expand_company" value="1" ><img src="' . IMAGES . 'expand.jpg"> </button><br>';
} else {
    $output .= '<button name="expand_company" value="0" ><img src="' . IMAGES . 'collapse.jpg"> </button>';
    $output .= '<div class="keret">';
    $output .= '<label>Rövid név:</label><input type="text" name="company_name" value="' . $options[OPTIONS_NAME_COMPANY_NAME] . '"><br>';
    $output .= '<label>Hosszú név:</label><input type="text" name="company_long" value="' . $options[OPTIONS_NAME_COMPANY_LONG_NAME] . '"><br>';
    $output .= '<label>Cím:</label><input type="text" name="company_addr" value="' . $options[OPTIONS_NAME_COMPANY_ADDR] . '"><br>';
    $output .= '<label>Telefon:</label><input type="text" name="company_telefon" value="' . $options[OPTIONS_NAME_COMPANY_TELEFON] . '"><br>';
    $output .= '<label>Bank neve:</label><input type="text" name="company_bank_name" value="' . $options[OPTIONS_NAME_COMPANY_BANK_NAME] . '"><br>';
    $output .= '<label>Számla szám:</label><input type="text" name="company_bank_account" value="' . $options[OPTIONS_NAME_COMPANY_BANK_ACCOUNT] . '"><br>';
    $output .= '</div>';
}

$output .= '<label>Általános beállítások: </label>';
if (0 == $expand_general) {
    $output .= '<button name="expand_general" value="1" ><img src="' . IMAGES . 'expand.jpg"> </button><br>';
} else {
    $output .= '<button name="expand_general" value="0" ><img src="' . IMAGES . 'collapse.jpg"> </button>';
    $output .= '<div class="keret">';
    $output .= '<label>Fejlesztői:</label><input type="checkbox" name="development" '
            . (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0 ? "checked" : "") . '><br>';
    $output .= '<label>Felhasználó mint:</label>';
    $output .= '<input type="number" min="1" max="10000" maxlength="4" size="4" name="role_as_member" value="' . $role_as_member . '"> tag<br>';
    $output .= '<label>Belépő link jelszó kérésnél:</label><input type="checkbox" name="password_link" '
            . (getOptionValue(OPTIONS_NAME_PASSWORD_LINK) > 0 ? "checked" : "") . '><br>';
    $delegate_mod = getOptionValue(OPTIONS_NAME_DELEGATE_MOD);
    if ($delegate_mod > 0) {
        $output .= '<label>Küldöttek száma lineáris:</label><input  type="checkbox" name="delegate_mod" '
                . ' checked="checked">';
        $output .= '&nbsp;&nbsp;&nbsp;&nbsp;'
                . '<input type="number" min="1" max="1000" maxlength="4" size="4" name="delegate_step" value="' . $options[OPTIONS_NAME_DELEGATE_STEP] . '"> tagonként<br>';
    } else {
        $output .= '<label>Küldöttek száma sávos:</label><input   type="checkbox" name="delegate_mod" ><br>';
        $ranges = unserialize(getOptionValue(OPTIONS_NAME_DELEGATE_RANGE));
        $del = 0;
        $last_mem = 0;
        if (sizeof($ranges) > 1) {
            while (isset($ranges[$del + 1])) {
                $output .= '<label></label>' . $del . ' küldött ' . $ranges[$del + 1] . ' taglétszámig';
                $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<button type="submit" name="delegate_remove_range" value="' . ($del + 1) . '">'
                        . '<img src="' . IMAGES . 'delete.jpg"></button><br>';
                $del++;
                $last_mem = $ranges[$del];
            }
        } else {
            $output .= '<label></label>Nincs megadva taglétszám sáv.<br>';
        }
        $next_mem = intval($last_mem, 10) + 10;
        $output .= '<label></label>' . $del . ' küldött ';
        $output .= '<input type="number" min="1" max="10000" maxlength="4" size="4" name="delegate_add_member" value="' . $next_mem . '" > taglétszámig.';
        $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<button type="submit" name="delegate_add_range" value="' . (intval($del, 10) + 1) . '">'
                . '<img src="' . IMAGES . 'add.jpg"></button>';
    }

    $output .= '</div>';
}
$output .= '<label>Számlázz.hu: </label>';
if (0 == $expand_szamlazz) {
    $output .= '<button name="expand_szamlazz" value="1" ><img src="' . IMAGES . 'expand.jpg"> </button><br>';
} else {
    $output .= '<button name="expand_szamlazz" value="0" ><img src="' . IMAGES . 'collapse.jpg"> </button>';
    $output .= '<div class="keret">';
    $output .= '<label>Számlázz.hu engedélyezve:</label><input type="checkbox" name="szamlazz_ena" '
            . (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0 ? "checked" : "") . '><br>';
    $output .= '<label>Számlázz felhasználó:</label><input type="text" name="szamlazz_user" value="' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_USER) . '"><br>';
    $output .= '<label>számlázz jelszó:</label><input type="text" name="szamlazz_passwd" value="' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_PASSWD) . '"><br>';
    $output .= '<label>számla előtag:</label><input type="text" name="szamlazz_prefix" value="' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_PREFIX) . '"><br>';

    $output .= '</div>';
}

$output .= '<label>FAI API: </label>';
if (0 == $expand_fai) {
    $output .= '<button name="expand_fai" value="1" ><img src="' . IMAGES . 'expand.jpg"> </button><br>';
} else {
    $output .= '<button name="expand_fai" value="0" ><img src="' . IMAGES . 'collapse.jpg"> </button>';
    $output .= '<div class="keret">';
    $output .= '<label>FAI API  engedélyezve:</label><input type="checkbox" name="fai_ena" '
            . (getOptionValue(OPTIONS_NAME_FAI_ENABLED) > 0 ? "checked" : "") . '><br>';
    $output .= '<label>FAI API felhasználó:</label><input type="text" name="fai_user" value="' . getOptionValue(OPTIONS_NAME_FAI_AUTH_USER) . '"><br>';
    $output .= '<label>FAI API jelszó:</label><input type="text" name="fai_passwd" value="' . getOptionValue(OPTIONS_NAME_FAI_AUTH_PASSWD) . '"><br>';
    $output .= '<label>FAI URL:</label><input type="text" name="fai_url" value="' . getOptionValue(OPTIONS_NAME_FAI_URL) . '"><br>';
    $output .= '<label>FAI view:</label><input type="text" name="fai_view" value="' . getOptionValue(OPTIONS_NAME_FAI_VIEW) . '"><br>';
    $output .= '<label>FAI print:</label><input type="text" name="fai_print" value="' . getOptionValue(OPTIONS_NAME_FAI_PRINT) . '"><br>';

    $output .= '</div>';
}
$output .= '<label>Utolsó crontab:</label>' . date('Y-m-d H:i:s', getOptionValue(OPTIONS_NAME_LAST_CRONTAB));
$output .= '<br><input type="submit" name="save_options" value="Mentés">';
$output .= '</form></div>';

function changeValue($var, $key) {
    global $options, $member_id;
    if (isset($var) AND $var == 'on') {
        $var = 1;
    }
    if (isset($var) AND $var == 'off') {
        $var = 0;
    }
    if (isset($var) AND $var != $options[$key]) {
        updateOption($key, $var);
        logger($member_id, -1, LOGGER_OPTIONS, $key . ' új értéke: ' . $var);
    }
}
