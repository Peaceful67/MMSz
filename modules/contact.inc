<?php

define('CONTACT_TEXT', 'text');
define('CONTACT_EMAIL_TO', 'email_to');
if (!isset($selected_subject)) {
    $selected_subject = -1;
}
if (!isset($message)) {
    $message = '';
}
$contact_types = [
    [
        CONTACT_TEXT => 'Adminisztratív jellegű',
        CONTACT_EMAIL_TO => [OPTIONS_NAME_MMSZ_EMAIL, OPTIONS_NAME_ADMIN_EMAIL],
    ],
    [
        CONTACT_TEXT => 'Hiba jelzés',
        CONTACT_EMAIL_TO => [OPTIONS_NAME_ADMIN_EMAIL],
    ],
    [
        CONTACT_TEXT => 'Fejlesztési javaslat',
        CONTACT_EMAIL_TO => [OPTIONS_NAME_ADMIN_EMAIL],
    ],
];
if (isset($send_message)) {
    if ($selected_subject >= 0) {
        if (strlen($message) > 20) {
            $body = getMemberName($member_id) . ' (' . $member_id . ') felhasználó a ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' tagnyilvántartójából az alábbi üzenetet küldte:<br>';
            $body .= str_replace(NEWLINE, '<br>', $message);
            $addresses = $contact_types[$selected_subject][CONTACT_EMAIL_TO];
            $subject = $contact_types[$selected_subject][CONTACT_TEXT];
            foreach ($addresses as $opt_addr) {
                $addr = getOptionValue($opt_addr);
                send_mail_to($addr, $subject, $body);
            }
            $body = 'Az alábbi üzenetet küldted a ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' tagnyilvántartójából. A címzettek a levelet megkapták.<br>';
            $body .= str_replace(NEWLINE, '<br>', $message);
            $addr = getMemberEmail($member_id);
            send_mail_to($addr, $subject, $body);
            $message = '';
            $selected_subject = -1;
            $view->success('Az üzenetet elküldtük');
        } else {
            $view->warning('Túl rövid az üzenetn (minimum 20 karakter)');
        }
    } else {
        $view->warning('Válaszd ki az üzenet jellegét. Ettől függ, ki kapja meg az üzenetet!');
    }
}
$view->newKeret();
$subjects = [];
foreach ($contact_types as $id => $type) {
    $subjects[$id] = $type[CONTACT_TEXT];
}
$view->submitDropDownChooser('Üzenet jellege', [-1 => 'Válassz !!!']+ $subjects, 'selected_subject');
$view->inputTextArea('message', '', 'Az üzenet szövege');
$view->submitButton('send_message', 'Elküld', -1, 'Az üzenet elküldése', 'Biztosan el akarod küldeni az üzenetet ?');
$view->endKeret();