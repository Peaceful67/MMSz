<?php

include_once FUNCTIONS . 'vote_functions.inc';
include_once FUNCTIONS . 'congress_show.inc';

define("CONGRESS_SUM_ANNOUNCEMENT", 'announcement');
define("CONGRESS_SUM_SHORT", 'short');
define("CONGRESS_SUM_DETAILED", 'detailed');
define("CONGRESS_SUM_RECORD", 'record');

if (!isset($selected_congress)) {
    if (isset($_SESSION[SESSION_CONGRESS_ONLINE])) {
        $selected_congress = $_SESSION[SESSION_CONGRESS_ONLINE];
    } else {
        $selected_congress = -1;
    }
} else {
    $_SESSION[SESSION_CONGRESS_ONLINE] = $selected_congress;
}
if (!isset($selected_online_poll)) {
    if (isset($_SESSION[SESSION_CONGRESS_POLL])) {
        $selected_online_poll = $_SESSION[SESSION_CONGRESS_POLL];
    } else {
        $selected_online_poll = -1;
    }
} else {
    $_SESSION[SESSION_CONGRESS_POLL] = $selected_online_poll;
}

set_default_value('radio_overview', CONGRESS_SUM_SHORT);
if (!isset($show_comments)) {
    $show_comments = '';
    $show_comments_by_topic = '';
}
if (!isset($show_comments_by_topic)) {
    $show_comments_by_topic = '';
}
$download_button = 'download_' . CONGRESS_SUM_ANNOUNCEMENT;
if (isset($$download_button)) {
    $congress_sum = getCongressSummerize($$download_button);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $$download_button . ' azonosítójú gyűlés kiírása letöltve.');
    $filename = 'congress_sum_' . $$download_button . '.pdf';
    downloadPDF($filename, $congress_sum, false);
}
$download_button = 'download_' . CONGRESS_SUM_SHORT;
if (isset($$download_button)) {
    $congress_short = getCongressShort($$download_button);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $$download_button . ' azonosítójú gyűlés rövid összefoglalója letöltve.');
    $filename = 'congress_short_' . $$download_button . '.pdf';
    downloadPDF($filename, $congress_short, false);
}
$download_button = 'download_' . CONGRESS_SUM_DETAILED;
if (isset($$download_button)) {
    $congress_detailed = getCongressDetailed($$download_button, $show_comments == 'on', $show_comments_by_topic == 'on');
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $$download_button . ' azonosítójú gyűlés részletes összefoglalója letöltve.');
    $filename = 'congress_detailed_' . $$download_button . '.pdf';
    downloadPDF($filename, $congress_detailed, false);
}
$download_button = 'download_' . CONGRESS_SUM_RECORD;
if (isset($$download_button)) {
    $congress_record = getCongressRecord($$download_button, $show_comments == 'on', $show_comments_by_topic == 'on');
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $$download_button . ' azonosítójú gyűlés jegyzőkönyve letöltve.');
    $filename = 'congress_record_' . $$download_button . '.pdf';
    downloadPDF($filename, $congress_record, false);
}
if (isset($close_poll)) {
    (new CongressPoll())->setPollClosed($close_poll);
}

if (isset($decide_poll)) {
    $decidePoll = new CongressPoll();
    $decidePoll->setSelectedPollId($decide_poll)->setDecided();
    $congress_id = $decidePoll->getCongressId();
    $decideCongress = new CongressOnline();
    if ($decideCongress->setSelectedCongress($congress_id)->setCongressClosed()) { // Sikerült lezárni, értessítünk mindenkit.
        $congress_sum = getCongressRecord($congress_id, false);
        $email = new Email(EMAIL_CONTEXT_CONGRESS_CLOSED_CREATOR, $congress_id);
        $filename_sum = TMP_DIR . CONGRESS_CREATE_RECORD . $congress_id . '.pdf';
        downloadPDF($filename_sum, $congress_sum, true);
        $email->setFilename($filename_sum);
        $email->insertToBody(1, $decideCongress->getCongressTitle());
        $email->sendEmail([$member_id => getMemberName($member_id)]);
        logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $congress_id . ' azonosítójú gyűlés lezárva.');

        $delegates = (new CongressDelegates())->getMembersOfCongress($congress_id);
        $email = new Email(EMAIL_CONTEXT_CONGRESS_CLOSED_DELEGATE, $congress_id);
        downloadPDF($filename_sum, $congress_sum, true);
        $email->setFilename($filename_sum);
        $email->insertToBody(1, $decideCongress->getCongressTitle());
        $email->insertToBody(2, getMemberName($member_id));
        $email->sendEmail($delegates);
    }
}
$view->newKeret();
$congresses_created = $congressOnline->getCreatedCongressesByCreator($member_id);
$congresses_delegated = $congressOnline->getCongressesOfDelegate($member_id);
$congresses_public = $congressOnline->getPublicCongresses();
$congress_invited = $congressOnline->getCongressesOfInvited();
$congresses = $congresses_created + $congresses_delegated + $congresses_public;
krsort($congresses);
$view->newDiv('row form-group');
if (empty($congresses)) {
    $view->warning('Nincsenek gyűlések még');
    $selected_congress = -1;
} elseif (count($congresses) == 1) {
    $selected_congress = key($congresses);
    $view->putHidden('selected_congress');
    $view->putText($congresses[$selected_congress], 'Gyűlés', true);
} else {
    $view->submitDropDownChooser('Gyűlések', [-1 => 'Válassz!!!'] + $congresses, 'selected_congress');
}
if ($selected_congress > 0) {
    $congressOnline->setSelectedCongress($selected_congress);
 //   $view->putText(substr($congressOnline->getCongressDescription(), 0, 50) . '...');
    if ($congressOnline->amICongressCreator() AND ! $congressOnline->isCongressClosed()) {
        $view->submitButton('change_auth', 'Hitelesítők', $selected_congress, 'Hitelesítők megváltoztatása');
    }
    $view->endDiv();
    $view->newDiv('row from-group');
    $view->newElement('<ul class="nav nav-tabs">');
    $view->newElement('<li class="nav-item">');
    $view->downloadPdf('download_' . $radio_overview, $selected_congress, 'A kiválasztott nézet letöltése PDF formátumban');
    $view->endElement('</li>');
    $view->newElement('<li class="nav-item">');
    $view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_overview == CONGRESS_SUM_ANNOUNCEMENT ? 'active' : '') . '" name="radio_overview" title="A gyűlés kiírása: beállítások, napirendi pontok" value="' . CONGRESS_SUM_ANNOUNCEMENT . '">Kiírás</button>');
    $view->endElement('</li>');
    $view->newElement('<li class="nav-item">');
    $view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_overview == CONGRESS_SUM_SHORT ? 'active' : '') . '" name="radio_overview" title="A gyűlés rövid összefoglalója napirendi pontonként" value="' . CONGRESS_SUM_SHORT . '">Rövid</button>');
    $view->endElement('</li>');
    $view->newElement('<li class="nav-item">');
    $view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_overview == CONGRESS_SUM_DETAILED ? 'active' : '') . '" name="radio_overview" title="A gyűlés részletes bemutatása napirendi pontonként, a tárgyalás listázásának lehetőségével" value="' . CONGRESS_SUM_DETAILED . '">Részletes</button>');
    $view->endElement('</li>');
    $view->newElement('<li class="nav-item">');
    $view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_overview == CONGRESS_SUM_RECORD ? 'active' : '') . '" name="radio_overview" title="A lezárt gyűlés jegyzőkönyvének megtekintése" value="' . CONGRESS_SUM_RECORD . '">Jegyzőkönyv</button>');
    $view->endElement('</li>');
    $view->endElement('</ul>');

    if ($radio_overview == CONGRESS_SUM_DETAILED OR $radio_overview == CONGRESS_SUM_RECORD) {
        $view->inputCheckbox('show_comments', 'Tárgyalás folyamata', true);
        if ($show_comments == 'on') {
            $view->inputCheckbox('show_comments_by_topic', 'Részletes bontásban', true);
        }
    }
    $view->endDiv();
    if (isset($congresses_created[$selected_congress])) { // Ha creator vagyok
        $online_polls = getUndecidedPollsOfCongress($selected_congress);
        if (!empty($online_polls)) {
            if (!isset($selected_online_poll)) {
                $selected_online_poll = -1;
            }
            if (count($online_polls) == 1) {
                $selected_online_poll = key($online_polls);
                $view->putText($online_polls[$selected_online_poll], 'Napirendi pont', true);
                $view->putHidden('selected_online_poll');
            } else {
                $view->submitDropDownChooser('Napirendi pontok', [-1 => 'Válassz!!!'] + $online_polls, 'selected_online_poll');
            }
            if ($selected_online_poll > 0) {
                $view->submitButton('edit_decision', 'Határozat', $selected_online_poll, 'Határozat szerkesztés, előkészítése');
                if ($congressPoll->setSelectedPollId($selected_online_poll)->isClosable()) {
                    $view->submitButton('close_poll', 'Lezárás', $selected_online_poll, 'A szavazás lezárása, ezután már senki módosíthatja a szavaztatát.', 'Biztosan le akarja zárni a szavazást ? Bár beérkezett az összes szavazat, '
                            . 'a megadott zárási időpont még hátra van, ezután már nem lehet szavaztot módosítania senkinek.');
                }
                if ($congressPoll->isOpen() AND ! $congressPoll->isClosed()) { // Tárgyalás alatt.
                    $view->submitButton('vote_warning', 'Szavazásra felszólítás', $selected_online_poll, 'Ismételt szavazásra felhívó email a még nem szavazóknak');
                }
                if ($congressPoll->isClosed() AND ! empty($congressPoll->getPollDecision())) {
                    $view->submitButton('decide_poll', 'Döntés', $selected_online_poll, 'Határozat hozatal', 'Biztosan határozatot akar hozni ?');
                }
            }
        }
    }
} else {
    $view->endDiv();
}
$view->endKeret();
if (isset($change_auth_back)) {
    unset($change_auth);
}
if (isset($change_auth) AND $change_auth == $selected_congress AND $selected_congress > 0) {
    $view->newKeret();
    $delegates = $congressDelegates->getMembersOfCongress($change_auth);
    if (!isset($selected_auth1)) {
        $selected_auth1 = $congressOnline->getCongressAuth1();
    }
    if (!isset($selected_auth1_old)) {
        $selected_auth1_old = $selected_auth1;
    }
    if (!isset($selected_auth2)) {
        $selected_auth2 = $congressOnline->getCongressAuth2();
    }
    if (!isset($selected_auth2_old)) {
        $selected_auth2_old = $selected_auth2;
    }
    if ($selected_auth1 == $selected_auth2) {
        $view->danger('A két hitelesítőnek különbözőnek kell lennie!');
    } else {
        if ($selected_auth1 != $selected_auth1_old) {
            $selected_auth1_old = $selected_auth1;
            $congressOnline->setCongressAuth1($selected_auth1);
        }
        if ($selected_auth2 != $selected_auth2_old) {
            $selected_auth2_old = $selected_auth2;
            $congressOnline->setCongressAuth2($selected_auth2);
        }
    }
    $view->submitBack('change_auth_back', 'Visszatérés a szerkesztésből.');
    $view->newDiv('row');
    $view->submitDropDownChooser('Hitelesítő 1', $delegates, 'selected_auth1');
    $view->submitDropDownChooser('Hitelesítő 2', $delegates, 'selected_auth2');
    $view->endDiv();
    $view->putHidden('selected_auth1_old');
    $view->putHidden('selected_auth2_old');
    $view->putHidden('change_auth');
    $view->endKeret();
    return;
}
if (isset($save_decision)) {
    $cPoll = (new CongressPoll())->setSelectedPollId($edit_decision);
    $cPoll->setPollDecision($poll_decision);
    $cPoll->setPollDecisionNumber($poll_decision_number);
    unset($edit_decision);
}
if (isset($edit_decision) AND $edit_decision == $selected_online_poll AND $selected_online_poll > 0 AND ! isset($edit_decision_back)) {
    $view->newKeret();
    $cPoll = (new CongressPoll())->setSelectedPollId($edit_decision);
    if (!isset($poll_decision_number)) {
        $poll_decision_number = $cPoll->getPollDecisionNumber();
    }

    if (empty($cPoll->getPollDecision())) {
        $cPoll->setPollDecision($cPoll->getPollDescription());
    }
    $view->putPrevent();
    $view->submitSave('save_decision', 0, 'A határozat szövegének mentése.');
    $view->submitBack('edit_decision_back', 'Visszatérés a szerkesztésből.');
    $view->putText(showVoteNumbers($cPoll->getVotes()), 'Szavazatok');
    $item_id = $cPoll->getCongressItemId();
    $cItem = (new CongressItems())->setSelectedItem($item_id);
    $view->putText(getNameDecisionOption($cPoll->getPollOptions())
            . ' (' . getThresholdOfVote($cPoll->getPollId(), $cPoll->getPollOptions()) . ' tag szavazata)', 'Szavazati küszöb', true);
    $view->putText(showVoteResult($edit_decision), 'Szavazás eredménye');
    $poll_decision_number = $cPoll->getPollDecisionNumber();
    $view->inputText('poll_decision_number', 'Határozat száma');
    $poll_decision = $cPoll->getPollDecision();
    $view->inputTextArea('poll_decision', 'Végső határozat');
    $view->endKeret();
    $view->putHidden('edit_decision');
    return;
}

if (isset($vote_warning) AND $vote_warning == $selected_online_poll AND $vote_warning > 0) {
    $cWarningPoll = (new CongressPoll)->setSelectedPollId($vote_warning);
    $congressEmail = new Email(EMAIL_CONTEXT_POLL_WARNING, $vote_warning);
    $congressEmail->insertToBody(1, '"' . $cWarningPoll->getPollTitle() . '"');
    $congressEmail->insertToBody(2, output_time($cWarningPoll->getPollToBeClosed()));
    $congressEmail->insertToBody(3, getMemberName($member_id));
    include_once INCLUDES . 'congress_poll_warning.inc';
    if (isset($vote_warning)) {
        $view->putHidden('vote_warning');
        $view->putHidden('selected_congress');
        $view->putHidden('selected_online_poll');
        return;
    }
}
if ($selected_congress > 0) {
    $view->newKeret();
    switch ($radio_overview) {
        case CONGRESS_SUM_RECORD:
            $output .= getCongressRecord($selected_congress, $show_comments == 'on', $show_comments_by_topic == 'on');
            break;
        case CONGRESS_SUM_DETAILED:
            $output .= getCongressDetailed($selected_congress, $show_comments == 'on', $show_comments_by_topic == 'on');
            break;
        case CONGRESS_SUM_SHORT:
            $output .= getCongressShort($selected_congress);
            break;
        case CONGRESS_SUM_ANNOUNCEMENT:
        default:
            $output .= getCongressSummerize($selected_congress);
            break;
    }
    $view->endKeret();
}

