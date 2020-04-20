<?php

if (isset($poll_edit_exit)) {
    unset($edit_poll);
    return;
}
$congressPoll = $congressPoll->createFromItemIfNotExists($selected_congress_item);
$congressPollOption->setSelectedPoll($congressPoll->getPollId());
if (isset($poll_save)) {
    if (isset($poll_title)) {
        $congressPoll->setPollTitle($poll_title);
    }
    if (isset($poll_description)) {
        $congressPoll->setPollDescription($poll_description);
    }
} else {
    $poll_title = $congressPoll->getPollTitle();
    $poll_description = $congressPoll->getPollDescription();
}
if (isset($poll_option_add)) {
    $congressPollOption->addPollOption();
}
if (isset($poll_option_del)) {
    $congressPollOption->delPollOption($poll_option_del);
}
o('<div class="keret editor">');
output_label('<b>Gyűlés címe</b>');
o($congressOnline->getCongressTitle() . '<br>');
output_label('<b>Napirendi pont</b>');
$output .= $congressItems->getCongressItemTitle();
output_ln();
o($congressItems->getCongressItemDescription());
o('</div>');

o('<div class="keret editor">');
o('<h3>Szavazás szerkesztése</h3>');
output_label('Cím');
o('<input type="text" name="poll_title" value="' . $poll_title . '">');
output_ln();
output_label('Szavazás');
o('<input type="text" size="80" maxlenght="250" name="poll_description" value="' . $poll_description . '">');
output_ln();
output_label('Lehetőségek');
output_add('poll_option_add');
output_spaces(5);
o($congressPoll->isPollMultiChoice() ? 'Több válasz is kiválasztható' : 'Csak egy válasz lehetséges');
output_ln();
$pollOptions = $congressPollOption->getPollOptions();
if (empty($pollOptions)) {
    o('Még nincs megadva szavazati lehetőség');
    output_ln();
} else {
    foreach ($pollOptions as $optionId => $option) {
        $congressPollOption->setSelectedPollOption($optionId);
        if (isset($poll_save)) {
            $var_name = 'poll_option_title_' . $optionId;
            if (isset($$var_name) AND ( $$var_name != $congressPollOption->getPollOptionTitle())) {
                $congressPollOption->setPollOptionTitle($$var_name);
            }
            $var_name = 'poll_option_desc_' . $optionId;
            if (isset($$var_name) AND ( $$var_name != $congressPollOption->getPollOptionDescription())) {
                $congressPollOption->setPollOptionDescription($$var_name);
            }
        }
        output_button('poll_option_del', $optionId, 'delete.jpg');
        output_spaces(5);
        o('Szavazat:');
        o('<input type="text" name="poll_option_title_' . $optionId . '" value="' . $congressPollOption->getPollOptionTitle() . '">');
        output_spaces(5);
        o('Megjegyzés:');
        o('<input type="text" name="poll_option_desc_' . $optionId . '" value="' . $congressPollOption->getPollOptionDescription() . '">');
        output_ln();
    }
}

o('<button name="poll_edit_exit">Vissza</button>');
output_spaces(5);
output_save('poll_save');
output_spaces(5);
o('</div>');
