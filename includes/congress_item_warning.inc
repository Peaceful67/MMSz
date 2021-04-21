<?php
$delegates = (new CongressDelegates())->getMembersOfCongress($selected_congress_online);
if (isset($warning_cancel)) {
    unset($warning_poll);
    return;
}
if (isset($warning_send)) {
    $congressEmail->sendEmail($delegates);
    unset($warning_poll);
    $save_comment = 0;
    $congress_comment = '"'.$congressEmail->getEmailSubject() .'" tárgyú email a szavazásban résztvevőknek kiment.';
    return;
}
if (isset($warning_save)) {
    $congressEmail->saveEmail($email_subject, $email_body);
}
$view->putPrevent();
$view->putElement('<h4>Napirend lezárására figyelmeztető email szerkesztése.</h4>');
$view->newKeret();
$view->putLabel('Lezárásra figyelmeztető email');
$view->submitButton('warning_send', 'Küldés', -1, 'Levél kiküldése', 'Biztosan ki akarod küldeni a levelet a '. count($delegates).' tagnak ?');
$view->endKeret()->newKeret();
$view->submitBack('warning_cancel', 'Visszatérés a figyelmeztető email szerkesztésből');
$view->submitSave('warning_save', 0, 'Figyelmeztető email szövegének mentése');
$email_subject = $congressEmail->getEmailSubject();
$view->inputText('email_subject', 'Email tárgya');
$email_body = $congressEmail->getEmailBody();
$view->inputTextArea('email_body', 'Email törzse');
$view->endKeret();
