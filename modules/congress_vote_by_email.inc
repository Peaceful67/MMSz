<?php

if (!isset($getCommand) OR ! isset($member_id) OR ! isset($poll_id) OR ! isset($option_id) OR $member_id <= 0 OR $poll_id <= 0 OR $option_id <= 0) {
    warning('Jogosulatlan hozzáférés');
    return;
}
include_once FUNCTIONS . 'vote_functions.inc';

$congressPoll->setSelectedPollId($poll_id);
o('<div class="keret editor">');
o('Tisztelt <b>' . getMemberName($member_id) . '</b> !');
output_ln(2);
o('Köszönjük szavazatodat.');
output_ln();
output_label('A napirend címe', true);
o($congressPoll->getPollTitle());
output_ln();
output_label('A napirend szövege', true);
o($congressPoll->getPollDescription());
output_ln();
output_label('A napirend legkésőbb lezárul', true);
o($congressPoll->getPollToBeClosed());
output_ln(2);
output_label('Leadott szavazatod', true);
o($congressPollOption->getTitle($option_id));
$errors = array();
output_ln();
$votes = $congressPollVote->getVoteOptionsOfMember($member_id, $poll_id);
if (!empty($votes)) {
    output_label('Korábbi szavazatid:');
    foreach ($votes as $vote) {
        o($congressPollOption->getTitle($vote));
        output_ln();
        output_label();
    }
}
if ($congressPoll->isClosed()) {
    $errors[] = "A szavazás lezárult";
}
if ($congressPollVote->isVoteAlready($member_id, $poll_id, $option_id)) {
    $errors[] = 'Ez a szavazat már le volt adva a rendszerben';
}
if (!$congressPoll->isPollByName() AND $congressPollVote->isMemberAlreadyVoted($member_id, $poll_id)) {
    $errors[] = 'Titkos szavazás esetén nem lehet a szavaztod módosítani.';
}
if ($congressPoll->isPollMultiChoice()) {
    if ($congressPoll->getMaxOption() > 0 AND $congressPoll->getMaxOption() <= count($votes)) {
        $errors[] = 'Csak ' . $congressPoll->getMaxOption() . ' lehetőséget lehet bejelölni!';
    }
}
if (empty($errors)) {
    if (!$congressPoll->isPollMultiChoice()) {
        $congressPollVote->resetVotes($member_id, $poll_id);
    }
    o('<h3>Szavazatodat a rendszer rögzítette.</h3>');
    if ($congressPoll->isPollByName()) {
        logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $poll_id . ' azonosítójú szavazásra a ' . $option_id . ' azonosítójó szavazat emailben leadva');
    } else {
        logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $poll_id . ' azonosítójú szavazásra szavazat emailben leadva');
    }
    $congressPollVote->setVote($poll_id, $member_id, $option_id, $congressPoll->isPollByName());
} else {
    warning('Szavazatodat a rendszer nem rögzítette');
    foreach ($errors as $error) {
        warning($error);
    }
}
session_unset();
o('<button title="Tovább a ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' tagnyilvántartó oldalára" >Tovább</button>');
o('</div>');
