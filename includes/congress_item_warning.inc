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
    $congress_comment = '"'.$congressEmail->getEmailSubject() .'" tárgyú email a szavazásban résztveknek kiment.';
    return;
}
if (isset($warning_save)) {
    $congressEmail->saveEmail($email_subject, $email_body);
}
o('<h4>Napirend lezárására figyelmeztető email szerkesztése.</h4>');
o('<div class="keret">');
output_label('Lezárásra figyelmeztető email', true);
output_spaces(5);
o('<button name="warning_cancel">Vissza</button>');
output_spaces(5);
o('<button title="Levél kiküldése" name="warning_send" onClick="return confirm(\'Biztosan ki akarod küldeni a levelet a '. count($delegates).' tagnak ?\')">Küldés</button>');
o('</div>');
o('<div class="keret editor">');
output_button('warning_save', 0, 'save.jpg');
output_ln();
output_label('Email tárgya', false);
o('<input maxlenght="100" size="40" type="text" name="email_subject" value="'.$congressEmail->getEmailSubject().'">');
output_ln();
output_label('Email törzse', false);
o('<textarea cols="150" rows="16" name="email_body">'.$congressEmail->getEmailBody().'</textarea>');
o('</div>');
