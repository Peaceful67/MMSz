<?php

include_once FUNCTIONS . 'vote_functions.inc';
if (!isset($selected_vote_poll)) {
    if (isset($_SESSION[SESSION_CONGRESS_POLL])) {
        $selected_vote_poll = $_SESSION[SESSION_CONGRESS_POLL];
    } else {
        $selected_vote_poll = -1;
    }
} else {
    $_SESSION[SESSION_CONGRESS_POLL] = $selected_vote_poll;
}
if ($selected_vote_poll > 0) {
    $_SESSION[SESSION_CONGRESS_ONLINE] = $congressPoll->setSelectedPollId($selected_vote_poll)->getCongressId();
}
if (!isset($selected_created_poll)) {
    $selected_created_poll = -1;
}
if (!isset($radio_option)) {
    $radio_option = -1;
}
if (!isset($vote_options)) {
    $vote_options = array();
}
if (isset($save_vote)) {
    $congressPoll->setSelectedPollId($save_vote);
    $vote_err = false;
    if ($congressPoll->isPollByName()) {
        $isByName = true;
        $congressPollVote->resetVotes($member_id, $save_vote);
    } else { // Nem nevszerinti szavazas
        $isByName = false;
        if ($congressPollVote->isMemberAlreadyVoted($member_id, $save_vote)) {
            $view->warning('Titkos szavazás esetén nem lehet a szavaztod módosítani.');
            $vote_err = true;
        }
    }
    if (!$vote_err) {
        if ($congressPoll->isPollMultichoice()) {
            if (count($vote_options) > $congressPoll->getMaxOption() AND $congressPoll->getMaxOption() > 0) {
                $view->warning('Csak ' . $congressPoll->getMaxOption() . ' lehetőséget lehet bejelölni!');
            } elseif (isset($vote_options) AND ! empty($vote_options)) {
                if (in_array(CONGRESS_POLL_OPTION_ABSTAIN_ID, $vote_options)) {
                    $congressPollVote->setVote($save_vote, $member_id, CONGRESS_POLL_OPTION_ABSTAIN_ID, $isByName);
                    if ($isByName) {
                        logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $save_vote . ' azonosítójú szavazásra tartózkodó szavazat leadva');
                    } else {
                        logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $save_vote . ' azonosítójú szavazásra szavazat leadva');
                    }
                } else {
                    foreach ($vote_options as $option_id) {
                        $congressPollVote->setVote($save_vote, $member_id, $option_id, $isByName);
                        if ($isByName) {
                            logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $save_vote . ' azonosítójú szavazásra a ' . $option_id . ' azonosítójó szavazat leadva');
                        } else {
                            logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $save_vote . ' azonosítójú szavazásra szavazat leadva');
                        }
                    }
                }
                //     $selected_vote_poll = -1;
            } else {
                $view->warning('Nem jelölted be egyik lehetőséget sem.');
            }
        } else {
            if (isset($radio_option) AND $radio_option > 0) {
                $congressPollVote->setVote($save_vote, $member_id, $radio_option, $isByName);
                if ($isByName) {
                    logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $save_vote . ' azonosítójú szavazásra a ' . $radio_option . ' azonosítójó szavazat leadva');
                } else {
                    logger($member_id, -1, LOGGER_CONGRESS_VOTE, 'A ' . $save_vote . ' azonosítójú szavazásra szavazat leadva');
                }
            } else {
                $view->warning('Nem jelölted be egyik lehetőséget sem.');
            }
        }
    }
}

$polls2Vote = $congressPoll->getPollToVote($member_id);
$view->newKeret();
if (empty($polls2Vote)) {
    $view->warning('Nincs megnyitott szavazás számodra');
    $selected_vote_poll = -1;
} elseif (count($polls2Vote) == 1) {
    $selected_vote_poll = key($polls2Vote);
    $view->putHidden('selected_vote_poll');
} else {
    $view->submitDropDownChooser('Szavazások', [-1 => 'Válassz!!!'] + $polls2Vote, 'selected_vote_poll');
}
if ($selected_vote_poll > 0) {
    $congressPoll->setSelectedPollId($selected_vote_poll);
    $vote_congress_id = $congressPoll->getCongressId();
    $view->putText($congressOnline->setSelectedCongress($vote_congress_id)->getCongressTitle(), 'Gyűlés címe', true);
    $itemTitle = (new CongressItems())->getCongressItemTitleById($congressPoll->getCongressItemId());
    $view->putText($itemTitle, 'Napirend', true);
    if ($itemTitle != $congressPoll->getPollTitle()) {
        $view->putText($congressPoll->getPollTitle(), 'Határozat címe', true);
    }
    $view->putText($congressPoll->getPollDescription(), 'Határozat szövege', true);
    $view->putText(output_time($congressPoll->getPollToBeClosed()), 'Szavazás lezárul:', true);
    $voted = $congressPollVote->getVoteOptionsOfMember($member_id, $selected_vote_poll);
    if (!empty($voted)) {
        $view->success('Köszönjük szavazatodat.');
        $vote_txt = '';
        foreach ($voted as $option_id) {
            $vote_txt .= $congressPollOption->getTitle($option_id) . ', ';
        }
        $view->putText($vote_txt, 'Korábbi szavazatod', true);
        $view->info('A szavazás menetét figyelemmel kísérheted az "Online gyűlés" => "Gyűlések" oldalon a "Rövid" vagy "Részletes" összefoglaló alatt.'
                . '<hr><br>A szavazás lezárásig módosíthatod a szavazatod. ');
    }
    $congressPollOption->setSelectedPoll($selected_vote_poll);
    $congressVoteOptions = $congressPollOption->getPollOptions();
    if ($congressPoll->isPollMultichoice()) {
        if ($congressPoll->isPollAbstain()) {
            $view->inputMultiCheckbox('vote_options', CONGRESS_POLL_OPTION_ABSTAIN_ID, $congressPollOption->getTitle(CONGRESS_POLL_OPTION_ABSTAIN_ID), false, $congressPollOption->getDescription(CONGRESS_POLL_OPTION_ABSTAIN_ID));
        }
        if (!in_array(CONGRESS_POLL_OPTION_ABSTAIN_ID, $vote_options)) {
            foreach ($congressVoteOptions as $option_id => $option) {
                $view->inputMultiCheckbox('vote_options', $option_id, $congressPollOption->getTitle($option_id), false, $congressPollOption->getDescription($option_id));
            }
        }
    } else {
        if (isset($voted[0])) {
            $radio_option = $voted[0];
        }
        if ($congressPoll->isPollYesNo()) {
            $view->inputRadioButton('radio_option', $congressPollOption->getTitle(CONGRESS_POLL_OPTION_YES_ID), CONGRESS_POLL_OPTION_YES_ID, $congressPollOption->getDescription(CONGRESS_POLL_OPTION_YES_ID));
            $view->inputRadioButton('radio_option', $congressPollOption->getTitle(CONGRESS_POLL_OPTION_NO_ID), CONGRESS_POLL_OPTION_NO_ID, $congressPollOption->getDescription(CONGRESS_POLL_OPTION_NO_ID));
        } else {
            foreach ($congressVoteOptions as $option_id => $option) {
                $view->inputRadioButton('radio_option', $congressPollOption->getTitle($option_id), $option_id, $congressPollOption->getDescription($option_id));
            }
        }
        if ($congressPoll->isPollAbstain()) {
            $view->inputRadioButton('radio_option', $congressPollOption->getTitle(CONGRESS_POLL_OPTION_ABSTAIN_ID), CONGRESS_POLL_OPTION_ABSTAIN_ID, $congressPollOption->getDescription(CONGRESS_POLL_OPTION_ABSTAIN_ID));
        }
    }
    $view->submitButton('save_vote', 'Szavazás', $selected_vote_poll, 'Szavazat beküldése', 'Biztosan el akarja küldeni a szavazatát ?');
    $view->putElement('<hr><h4>A határozathoz kapcsolódó tárgyalás:</h4>');
    $view->putElement(showCommentsOfItem($congressPoll->getCongressItemId(), TOPIC_ID_ALL, true));
    $view->endKeret();
}
    