<?php

$delegates = getHaventVoted($selected_online_poll);
if (isset($warning_cancel)) {
    unset($vote_warning);
    return;
}
if (isset($warning_send)) {
    $congressEmail->sendEmail($delegates);
    unset($vote_warning);
    $view->success('"' . $congressEmail->getEmailSubject() . '" tárgyú email a még nem szavazó résztvevőknek kiment.');
    return;
}
if (isset($warning_save)) {
    $congressEmail->saveEmail($email_subject, $email_body);
}
$view->putPrevent();
$view->newKeret();
$view->putElement('<h4>Szavazásra figyelmeztető email</h4>');
$view->submitSave('warning_save', $selected_online_poll, 'Az email szövegének mentése');
$view->submitBack('warning_cancel', 'Visszalépés a szerkesztésből');
$view->submitButton('warning_send', 'Küldés', -1, 'Levél kiküldése  ' . count($delegates) . ' tagnak', 'Biztosan ki akarod küldeni a levelet a ' . count($delegates) . ' tagnak ?');
$email_subject = $congressEmail->getEmailSubject();
$view->inputText('email_subject', 'Email tárgya');
$email_body = $congressEmail->getEmailBody();
$view->inputTextArea('email_body', 'Email törzse');
$view->endKeret();