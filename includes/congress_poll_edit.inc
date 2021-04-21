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
    if (isset($selected_max_option)) {
        $congressPoll->setMaxOption($selected_max_option);
    } else {
        $selected_max_option = $congressPoll->getMaxOption();
    }
    $tbc = $to_be_closed_date . ' ' . $to_be_closed_time;
    $congressPoll->setPollToBeClosed($tbc);
} else {
    $poll_title = $congressPoll->getPollTitle();
    $poll_description = $congressPoll->getPollDescription();
    $selected_max_option = $congressPoll->getMaxOption();
}
if (isset($poll_option_add)) {
    $congressPollOption->addPollOption();
}
if (isset($poll_option_del)) {
    $congressPollOption->delPollOption($poll_option_del);
}
$view->newKeret();
$view->putText($congressOnline->getCongressTitle(), 'Gyűlés címe', true);
$view->putText($congressItems->getCongressItemTitle(), 'Napirendi pont', true);
$view->putText($congressItems->getCongressItemDescription());
$view->endKeret();

$view->newKeret();
$view->putElement('<h4>Határozat előkészítése</h4>');
$view->putPrevent();
$view->inputText('poll_title', 'Határozati javaslat címe');
$view->inputTextArea('poll_description', 'Határozati javaslat szövege');

$to_be_closed = strtotime($congressPoll->getPollToBeClosed());
if (empty($to_be_closed)) {
    $to_be_closed = strtotime('+7 days', $now);
}
if ($to_be_closed < $now) {
    $view->warning('A lezárás nem lehet a múltban, javasolt zárás egy hét múlva.');
    $to_be_closed = strtotime('+7 days', $now);
}
$to_be_closed_date = date('Y-m-d', $to_be_closed);
$to_be_closed_time = date('H:i', $to_be_closed);
$view->newDiv('row');
$view->inputDate('to_be_closed_date', 'Lezárás legkésőbbi időpontja');
$view->inputText('to_be_closed_time');
$view->endDiv();

$pollOptions = $congressPollOption->getPollOptions();
if ($congressPoll->isPollMultiChoice()) {
    $pollOptionsMax = [0 => 'Korlátlan'];
    $max_i = 1;
    foreach ($pollOptions as $opt) {
        $pollOptionsMax[$max_i] = $max_i;
        $max_i++;
    }
    $view->inputDropDownChooser('Legtöbb kiválasztható', $pollOptionsMax, 'selected_max_option');
    $poll_type_text = 'Több válasz is kiválasztható,';
} else {
    $poll_type_text = 'Csak egy válasz lehetséges, ';
}
$poll_type_text .= ($congressPoll->isPollAbstain() ? 'tartózkodás engedélyezett' : 'tartózkodás nem engedélyezett');
if ($congressPoll->isPollYesNo()) {
    $poll_type_text .= ', eldöntendő szavazás (Igen/Nem)';
    $view->putText($poll_type_text, 'Szavazás típusa');
} else {
    $view->putText($poll_type_text, 'Szavazás típusa');
    $view->submitAdd('poll_option_add', -1, 'Szavazati lehetőség hozzáadása');
    if (empty($pollOptions)) {
        $view->warning('Még nincs megadva szavazati lehetőség');
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
            $view->newDiv('row');
            $view->submitDelete('poll_option_del', $optionId);
            $var_name = 'poll_option_title_' . $optionId;
            $$var_name = $congressPollOption->getPollOptionTitle();
            $view->inputText($var_name, 'Szavazat');
            $var_name = 'poll_option_desc_' . $optionId;
            $$var_name = $congressPollOption->getPollOptionDescription();
            $view->inputText($var_name, 'Megjegyzés');
            $view->endDiv();
        }
    }
}
$view->submitBack('poll_edit_exit', 'Visszatérés a határozati javaslat szerkesztésből');
$view->submitSave('poll_save', 0, 'A szerkesztett határozati javaslat adatainak menétése');
$view->endKeret();
