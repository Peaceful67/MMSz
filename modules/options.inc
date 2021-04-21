<?php

define('DELEGATE_MODE_RANGE', 0);
define('DELEGATE_MODE_LINEAR', 1);


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
if (!isset($role_as_member)) {
    $role_as_member = $member_id;
}
if (isset($save_options)) {
    if (!isset($fai_ena)) {
        $fai_ena = 'off';
    }
    changeValue($fai_ena, OPTIONS_NAME_FAI_ENABLED);
    changeValue($fai_user, OPTIONS_NAME_FAI_AUTH_USER);
    changeValue($fai_passwd, OPTIONS_NAME_FAI_AUTH_PASSWD);
    changeValue($fai_url, OPTIONS_NAME_FAI_URL);
    changeValue($fai_view, OPTIONS_NAME_FAI_VIEW);
    changeValue($fai_print, OPTIONS_NAME_FAI_PRINT);
    changeValue($max_newsletter, OPTIONS_NAME_MAX_NEWSLETTERS);
    changeValue($next_newsletter, OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER);
    changeValue($admin_email, OPTIONS_NAME_ADMIN_EMAIL);
    changeValue($mmsz_email, OPTIONS_NAME_MMSZ_EMAIL);
    changeValue($mmsz_elnok_email, OPTIONS_NAME_MMSZ_ELNOK_EMAIL);
    changeValue($fai_notif_email, OPTIONS_NAME_FAI_NOTIF_EMAIL);
    changeValue($email_sender, OPTIONS_NAME_EMAIL_SENDER);
    changeValue($email_signature, OPTIONS_NAME_EMAIL_SIGNATURE);
    changeValue($email_reply, OPTIONS_NAME_EMAIL_REPLY);
    if (!isset($development)) {
        $development = 'off';
    }
    if (!isset($error_log)) {
        $error_log = 'off';
    }
    if (!isset($html_tabulated)) {
        $html_tabulated = 'off';
    }
    if (!isset($error_session) OR $error_log == 'off') {
        $error_session = 'off';
    }
    if (!isset($error_cookie) OR $error_log == 'off') {
        $error_cookie = 'off';
    }
    if (!isset($help_enabled)) {
        $help_enabled = 'off';
    }

    if (!isset($password_link)) {
        $password_link = 'off';
    }
    if (!isset($delegate_mod)) {
        $delegate_mod = DELEGATE_MODE_LINEAR;
    }
    if ($delegate_mod == DELEGATE_MODE_LINEAR) {
        if (!isset($delegate_step)) {
            $delegate_step = 10;
        }
        changeValue($delegate_step, OPTIONS_NAME_DELEGATE_STEP);
    }

    changeValue($delegate_mod, OPTIONS_NAME_DELEGATE_MOD);
    changeValue($development, OPTIONS_NAME_DEVELOPMENT);
    changeValue($html_tabulated, OPTIONS_NAME_HTML_TABULATED);
    changeValue($error_log, OPTIONS_NAME_ERROR_LOG);
    changeValue($error_session, OPTIONS_NAME_ERROR_LOG_SESSIONS);
    changeValue($error_cookie, OPTIONS_NAME_ERROR_LOG_COOKIE);
    changeValue($password_link, OPTIONS_NAME_PASSWORD_LINK);
    changeValue($help_enabled, OPTIONS_NAME_HELP_ENABLED);
    changeValue($company_name, OPTIONS_NAME_COMPANY_NAME);
    changeValue($company_long, OPTIONS_NAME_COMPANY_LONG_NAME);
    changeValue($company_addr, OPTIONS_NAME_COMPANY_ADDR);
    changeValue($company_telefon, OPTIONS_NAME_COMPANY_TELEFON);
    changeValue($company_bank_name, OPTIONS_NAME_COMPANY_BANK_NAME);
    changeValue($company_bank_account, OPTIONS_NAME_COMPANY_BANK_ACCOUNT);
    changeValue($company_id_format, OPTIONS_NAME_COMPANY_ID_FORMAT);
    if (!isset($szamlazz_ena)) {
        $szamlazz_ena = 'off';
    }
    changeValue($szamlazz_ena, OPTIONS_NAME_SZAMLAZZ_ENA);
    changeValue($szamlazz_user, OPTIONS_NAME_SZAMLAZZ_USER);
    changeValue($szamlazz_passwd, OPTIONS_NAME_SZAMLAZZ_PASSWD);
    changeValue($szamlazz_prefix, OPTIONS_NAME_SZAMLAZZ_PREFIX);

    if ($role_as_member != $member_id AND $role_as_member > 0) {
        $_SESSION[SESSION_ROLE_AS] = $role_as_member;
        redirect('profile');
        exit(0);
    }
}
$view->putPrevent();
$view->newKeret();
$view->putElement('<button class="btn btn-secondary mx-1" type="button" data-toggle="collapse" data-target="#newsletter" '
        . 'aria-expanded="false" aria-controls="newsletter">Körlevelek</button>');
$view->putElement('<button class="btn btn-secondary mx-1" type="button" data-toggle="collapse" data-target="#emails" '
        . 'aria-expanded="false" aria-controls="emails">Email címek</button>');
$view->putElement('<button class="btn btn-secondary mx-1" type="button" data-toggle="collapse" data-target="#company" '
        . 'aria-expanded="false" aria-controls="company">Szövetségi adatok</button>');
$view->putElement('<button class="btn btn-secondary mx-1" type="button" data-toggle="collapse" data-target="#admin" '
        . 'aria-expanded="false" aria-controls="admin">Fejlesztői</button>');
$view->putElement('<button class="btn btn-secondary mx-1" type="button" data-toggle="collapse" data-target="#congress" '
        . 'aria-expanded="false" aria-controls="congress">Gyűlés</button>');
$view->putElement('<button class="btn btn-secondary mx-1" type="button" data-toggle="collapse" data-target="#szamlazz" '
        . 'aria-expanded="false" aria-controls="szamlazz">Számlázz.hu</button>');
$view->putElement('<button class="btn btn-secondary mx-1" type="button" data-toggle="collapse" data-target="#fai" '
        . 'aria-expanded="false" aria-controls="fai">FAI API</button>');


$view->newDiv('collapse multi-collapse', 'newsletter');
$view->newDiv('card card-body');
$view->putText('<h4>Körlevelek<h4>');
$max_newsletter = $options[OPTIONS_NAME_MAX_NEWSLETTERS];
$next_newsletter = $options[OPTIONS_NAME_SECONDS_BEFORE_NEXT_NEWSLETTER];
$view->putText(date("Y.m.d H:i:s", $options[OPTIONS_NAME_LAST_NEWSLETTER]), 'Utolsó ellenőrzés');
$view->inputText('max_newsletter', 'Max körlevél szám');
$view->inputText('next_newsletter', 'Körlevél időköz (mp)');
$view->endDiv()->endDiv();

$view->newDiv('collapse multi-collapse', 'emails');
$view->newDiv('card card-body');
$view->putText('<h4>Email címek<h4>');
$admin_email = $options[OPTIONS_NAME_ADMIN_EMAIL];
$mmsz_email = $options[OPTIONS_NAME_MMSZ_EMAIL];
$mmsz_elnok_email = $options[OPTIONS_NAME_MMSZ_ELNOK_EMAIL];
$fai_notif_email = $options[OPTIONS_NAME_FAI_NOTIF_EMAIL];
$email_reply = $options[OPTIONS_NAME_EMAIL_REPLY];
$email_sender = $options[OPTIONS_NAME_EMAIL_SENDER];
$email_signature = $options[OPTIONS_NAME_EMAIL_SIGNATURE];
$view->inputEmail('admin_email', 'Adminisztrátor');
$view->inputEmail('mmsz_email', 'Szövetség');
$view->inputEmail('mmsz_elnok_email', 'Szövetség elnök');
$view->inputEmail('fai_notif_email', 'Befizetés értesítés');
$view->inputEmail('email_reply', 'Emailek válaszcíme');
$view->inputEmail('email_sender', 'Emailek feladója');
$view->inputText('email_signature', 'Emailek aláírása');
$view->endDiv()->endDiv();

$view->newDiv('collapse multi-collapse', 'company');
$view->newDiv('card card-body');
$view->putText('<h4>Szövetség adatok<h4>');
$company_name = $options[OPTIONS_NAME_COMPANY_NAME];
$company_long = $options[OPTIONS_NAME_COMPANY_LONG_NAME];
$company_addr = $options[OPTIONS_NAME_COMPANY_ADDR];
$company_telefon = $options[OPTIONS_NAME_COMPANY_TELEFON];
$company_bank_name = $options[OPTIONS_NAME_COMPANY_BANK_NAME];
$company_bank_account = $options[OPTIONS_NAME_COMPANY_BANK_ACCOUNT];
$company_id_format = $options[OPTIONS_NAME_COMPANY_ID_FORMAT];
$view->inputText('company_name', 'Rövid név');
$view->inputText('company_long', 'Hosszú név');
$view->inputText('company_addr', 'Cím');
$view->inputText('company_telefon', 'Telefon');
$view->inputText('company_bank_name', 'Bank neve');
$view->inputText('company_bank_account', 'Számla szám');
$view->inputText('company_id_format', 'Azonosító formátum');
$view->endDiv()->endDiv();

$view->newDiv('collapse multi-collapse', 'admin');
$view->newDiv('card card-body');
$view->putText('<h4>Fejlesztői beállítások<h4>');
if (isPermitted(ACCESS_ADMIN)) {
    $development = (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) ? "on" : "off";
    $error_log = (getOptionValue(OPTIONS_NAME_ERROR_LOG) > 0) ? "on" : "off";
    $html_tabulated = (getOptionValue(OPTIONS_NAME_HTML_TABULATED) > 0) ? "on" : "off";
    $error_cookie = (getOptionValue(OPTIONS_NAME_ERROR_LOG_COOKIE) > 0) ? "on" : "off";
    $error_session = (getOptionValue(OPTIONS_NAME_ERROR_LOG_SESSIONS) > 0) ? "on" : "off";
    $help_enabled = (getOptionValue(OPTIONS_NAME_HELP_ENABLED) > 0) ? "on" : "off";
    $password_link = (getOptionValue(OPTIONS_NAME_PASSWORD_LINK) > 0) ? "on" : "off";
    $view->inputCheckbox('development', 'Fejlesztői');
    $view->inputCheckbox('error_log', 'Post variables');
    $view->inputCheckbox('error_session', 'Sessions');
    $view->inputCheckbox('error_cookie', 'Cookies');
    $view->inputCheckbox('html_tabulated', 'HTML tabulálva');
    $view->inputCheckbox('help_enabled', 'Help megjelenítés');
    $view->inputCheckbox('password_link', 'Link jelszó kérésnél');
    $view->inputMemberId('role_as_member', 'Felhasználó mint');
} else {
    $view->warning('Csak adminisztrátori jogosultsággal érhető el.');
}
$view->endDiv()->endDiv();

$view->newDiv('collapse multi-collapse', 'congress');
$view->newDiv('card card-body');
$view->putText('<h4>Gyűlés küldöttei<h4>');
$view->putElement('Küldöttek száma:');
$delegate_mod = getOptionValue(OPTIONS_NAME_DELEGATE_MOD);
$delegate_step = $options[OPTIONS_NAME_DELEGATE_STEP];
$view->newDiv('row');
$view->newDiv('col-md-2')->inputRadioButton('delegate_mod', 'Lineáris', DELEGATE_MODE_LINEAR)->endDiv();
$view->newDiv('col-md-5')->inputMemberId('delegate_step', 'Küldött', ' tagonként')->endDiv();
$view->endDiv();
$view->putElement('<hr class="my-3"/>');
$view->newDiv('row');
$view->newDiv('col-md-2')->inputRadioButton('delegate_mod', 'Sávos', DELEGATE_MODE_RANGE)->endDiv();
$view->newDiv('col-md-5');
$ranges = unserialize(getOptionValue(OPTIONS_NAME_DELEGATE_RANGE));
$del = 0;
$last_mem = 0;
$view->newElement('<ul class="list-group">');
if (sizeof($ranges) > 1) {

    while (isset($ranges[$del + 1])) {
        $view->newElement('<li class="list-group-item">');
        $view->putElement($del . ' küldött ' . $ranges[$del + 1] . ' taglétszámig');
        $view->submitDelete('delegate_remove_range', ($del + 1));
        $view->endElement('</li>');
        $del++;
        $last_mem = $ranges[$del];
    }
} else {
    $ranges[0] = 0;
    $view->putText('Nincs megadva taglétszám sáv.', ' ');
}
$next_mem = intval($last_mem, 10) + 10;
$view->newElement('<li class="list-group-item">');
$delegate_add_member = $ranges[$del] + 1;
$view->putElement($del . ' küldött '
        . '<input type="number" min="1" max="9999" size="4" onchange="saved=false;" class="mx-2" id="delegate_add_member" name="delegate_add_member" value="' . $next_mem . '"> taglétszámig. ');

$view->submitAdd('delegate_add_range', (intval($del, 10) + 1), (intval($del, 10) + 1) . ' küldött taglétszámig');
$view->endElement('</ul>');
$view->endDiv();
$view->endDiv();
$view->putElement('<hr class="my-3"/>');
$view->info('A fenti beállítások akkor aktívak, ha a mandátum számok nincsenek megadva az egyesületeknél.<br>Amennyiben egy egyesületnél a Mandátum 1 vagy Mandátum 2 mező nem nulla, akkor ezek közül a  Gyűlésnél választható mandátum száma lehet a küldöttnek. '
        . 'Minden egyesület csak egy küldöttet állíthat, de egy küldött képviselhet több egyesüleletet is.');
$view->endDiv();
$view->endDiv();



$view->newDiv('collapse multi-collapse', 'szamlazz');
$view->newDiv('card card-body');
$view->putText('<h4>Számlázz.hu<h4>');
$szamlazz_ena = (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) ? "on" : "off";
$szamlazz_user = getOptionValue(OPTIONS_NAME_SZAMLAZZ_USER);
$szamlazz_passwd = getOptionValue(OPTIONS_NAME_SZAMLAZZ_PASSWD);
$szamlazz_prefix = getOptionValue(OPTIONS_NAME_SZAMLAZZ_PREFIX);
$view->inputCheckbox('szamlazz_ena', 'Engedélyezve');
$view->inputText('szamlazz_user', 'Felhasználó');
$view->inputText('szamlazz_passwd', 'Jelszó');
$view->inputText('szamlazz_prefix', 'Számla előtag');
$view->endDiv()->endDiv();

$view->newDiv('collapse multi-collapse', 'fai');
$view->newDiv('card card-body');
$view->putText('<h4>FAI API<h4>');
$fai_ena = getOptionValue(OPTIONS_NAME_FAI_ENABLED) > 0 ? "on" : "off";
$fai_user = getOptionValue(OPTIONS_NAME_FAI_AUTH_USER);
$fai_passwd = getOptionValue(OPTIONS_NAME_FAI_AUTH_PASSWD);
$fai_url = getOptionValue(OPTIONS_NAME_FAI_URL);
$fai_view = getOptionValue(OPTIONS_NAME_FAI_VIEW);
$fai_print = getOptionValue(OPTIONS_NAME_FAI_PRINT);
$view->inputCheckbox('fai_ena', 'Engedélyezve');
$view->inputText('fai_user', 'Felhasználó');
$view->inputText('fai_passwd', 'Jelszó');
$view->inputText('fai_url', 'FAI URL');
$view->inputText('fai_view', 'FAI view');
$view->inputText('fai_print', 'FAI print');
$view->endDiv()->endDiv();

$view->endDiv();
$view->putText(date('Y-m-d H:i:s', getOptionValue(OPTIONS_NAME_LAST_CRONTAB)), 'Utolsó crontab');
$view->submitButton('save_options', 'Mentés');
$view->endKeret();

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
