<?php

include_once FUNCTIONS . 'vote_functions.inc';

if (!isset($selected_congress_online)) {
    if (isset($_SESSION[SESSION_CONGRESS_ONLINE])) {
        $selected_congress_online = $_SESSION[SESSION_CONGRESS_ONLINE];
    } else {
        $selected_congress_online = -1;
    }
} else {
    $_SESSION[SESSION_CONGRESS_ONLINE] = $selected_congress_online;
}
if (!isset($selected_congress_online_old)) {
    $selected_congress_online_old = -1;
}
if ($selected_congress_online != $selected_congress_online_old) {
    $selected_congress_item = -1;
    $selected_congress_online_old = $selected_congress_online;
}
$view->putHidden('selected_congress_online_old');
if (!isset($selected_congress_item)) {
    if (isset($_SESSION[SESSION_CONGRESS_ITEM])) {
        $selected_congress_item = $_SESSION[SESSION_CONGRESS_ITEM];
    } else {
        $selected_congress_item = -1;
    }
} else {
    $_SESSION[SESSION_CONGRESS_ITEM] = $selected_congress_item;
}
if (!isset($selected_add_topic)) {
    $selected_add_topic = -1;
}
if (!isset($selected_auth1)) {
    $selected_auth1 = -1;
}
if (!isset($selected_auth2)) {
    $selected_auth2 = -1;
}
if ($selected_congress_online > 0) {
    $congressOnline->setSelectedCongress($selected_congress_online);
    $congressItems->setSelectedCongress($selected_congress_online);
}
if (!isset($expand_add_comment)) {
    $expand_add_comment = true;
} elseif ($expand_add_comment == "false") {
    $expand_add_comment = false;
} else {
    $expand_add_comment = true;
}
if (isset($forward_to_vote)) {
    $_SESSION[SESSION_CONGRESS_POLL] = $forward_to_vote;
    redirect('congress_vote');
    exit();
}
if (isset($close_item)) {
    $congressItems->setSelectedItem($selected_congress_item);
    $congressItems->setItemClosed($selected_congress_item);
    $view->success('A tárgyalás lezárva.');
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, 'A ' . $selected_congress_item . ' azonosítójú napirend lezárva');
}
if (isset($open_poll)) {
    $poll = new CongressPoll();
    if ($poll->openPollOfItem($selected_congress_item)) {
        $poll->emailOpenPoll();
        $view->success('Megnyitva a határozati javaslat szerinti szavazásra.');
        $view->success('A szavazás megnyitásáról a gyűlésen résztvevők értesítő email kapnak.');
        logger($member_id, -1, LOGGER_CONGRESS_ONLINE, 'A ' . $poll->getPollId() . ' azonosítójú szavás megnyitva');
    } else {
        $view->danger('Nem sikerült a szavazást megnyitni.');
    }
}
if (isset($edit_poll)) {
    $congressItems->setSelectedItem($selected_congress_item);
    include_once INCLUDES . 'congress_poll_edit.inc';
    if (isset($edit_poll)) {
        $view->putHidden('edit_poll');
        $view->putHidden('selected_congress_online');
        $view->putHidden('selected_congress_item');
        return;
    }
}
if (isset($edit_topic)) {
    include_once INCLUDES . 'congress_topic_edit.inc';
    $view->putHidden('selected_congress_online');
    $view->putHidden('selected_congress_item');
    if (isset($edit_topic)) {
        $view->putHidden('edit_topic');
        return;
    }
}
if (isset($edit_participants)) {
    include_once INCLUDES . 'congress_participants.inc';
    $view->putHidden('selected_congress_online');
    if (isset($edit_participants)) {
        $view->putHidden('edit_participants');
        return;
    }
}
$congresses = $congressOnline->getCreatedCongressesOfDelegate() + $congressOnline->getOnlineCongressesByCreator($member_id) + $congressOnline->getCreatedCongressesOfInvited();
if (empty($congresses)) {
    $view->warning('Nincs tárgyalandó szavazás !');
    return;
}
if (isset($warning_poll)) {
    $congressItems->setSelectedItem($selected_congress_item);
    $congressEmail = new Email(EMAIL_CONTEXT_ITEM_WARNING, $selected_congress_item);
    $congressEmail->insertToBody(1, '"' . $congressItems->getCongressItemTitleById($selected_congress_item) . '"');
    $congressEmail->insertToBody(2, getMemberName($member_id));
    include_once INCLUDES . 'congress_item_warning.inc';
    if (isset($warning_poll)) {
        $view->putHidden('warning_poll');
        $view->putHidden('selected_congress_online');
        $view->putHidden('selected_congress_item');
        return;
    }
}
$view->putPrevent();
if (isset($add_item)) {
    if ($add_item == -1) {
        $add_item = $congressItems->addItem($congressOnline->getCongressOption() & CONGRESS_OPTIONS_INHERITED);
        logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_congress_item . ' azonosítújú napirend létrehozása. ');
    }
    $selected_congress_item = $add_item;
    if ($add_item > 0) { // Ha befejeztük az add_item == -2     
        $congressItems->setSelectedItem($add_item);
        $doc_file = CONGRESS_ITEM_DOC . $selected_congress_item . '.pdf';
        if (isset($delete_item_doc)) {
            unlink($doc_file);
            logger($member_id, -1, LOGGER_CONGRESS_ONLINE, "A " . $selected_congress_item . " azonosítójú napirendhez melléklet törlése.");
        }
        $view->newKeret();
        include_once INCLUDES . 'congress_item_save.inc';
        include_once INCLUDES . 'congress_item_edit.inc';
        $view->endKeret();
        $view->putHidden('add_item');
        $old_selected_congress_item = $selected_congress_item;
        $view->putHidden('selected_congress_online');
        $view->putHidden('old_selected_congress_item');
        $view->submitButton('add_item', 'Szerkesztés vége', -2, 'Új napirend szerkesztésének befejezése');
        return;
    }
}
$view->newKeret();
$view->newDiv('row');
if (count($congresses) == 1) {
    $selected_congress_online = key($congresses);
    $view->putText($congresses[$selected_congress_online], 'Folyamatban lévő gyűlés');
    $view->putHidden('selected_congress_online');
} else {
    $view->submitDropDownChooser('Folyamatban lévő gyűlések', [-1 => 'Válassz!!!'] + $congresses, 'selected_congress_online', true);
}
if ($selected_congress_online < 0) {
    return;
}
$congressOnline->setSelectedCongress($selected_congress_online);
if (!isset($summerize_congress)) {
    $view->submitButton('summerize_congress', 'Összegzés', -1, 'A szavazás eredeti kiírása részletesen.');
}
if ($congressOnline->amICongressCreator()) {
    if ($congressOnline->isCongressAppendable()) {
        $view->submitButton('add_item', 'Új napirend', -1, 'Új napirend felvétele ehhez a gyűléshez...');
    }
    $view->submitButton('edit_participants', 'Résztvevők', $selected_congress_online, 'A gyűlés résztvevőinek módosítása');
}
$view->endDiv();

if (isset($summerize_congress)) {
    include_once FUNCTIONS . 'congress_show.inc';
    $view->newKeret();
    $view->submitReset('back');
    $view->put(getCongressSummerize($selected_congress_online));
    $view->endKeret();
    return;
}

$congressItems->setSelectedCongress($selected_congress_online);
$items = $congressItems->getSelectionOfCongress();
if (empty($items)) {
    return;
}
$view->newDiv('row');
if (count($items) == 1) {
    $selected_congress_item = key($items);
    $view->putText($congressItems->getCongressItemTitleById($selected_congress_item), 'Napirend');
    $view->putHidden('selected_congress_item');
} else {
    $view->submitDropDownChooser('Napirendek', [-1 => 'Válassz!!!'] + $items, 'selected_congress_item', true);
}
$view->endDiv()->newDiv('form-inline');
if ($selected_congress_item < 0) {
    return;
}
if (isset($save_comment) AND ( !isset($_SESSION[SESSION_LAST_CONGRESS_COMMENT]) OR (
        isset($_SESSION[SESSION_LAST_CONGRESS_COMMENT]) AND $_SESSION[SESSION_LAST_CONGRESS_COMMENT] != $save_comment))) {
    $_SESSION[SESSION_LAST_CONGRESS_COMMENT] = $save_comment;
    if (strlen($congress_comment) < 5) {
        $view->warning('Túl rövid hozzászólás');
    } else {
        if ($congressItems->isItemClosed($selected_congress_item)) {
            $view->danger('A tárgyalást időközben lezárták');
        } else {
            $comment_id = $congressComment->addComment($congress_comment, $selected_congress_item, $selected_add_topic);
            logger($member_id, -1, LOGGER_CONGRESS_COMMENT, "A " . $comment_id . " azonosítójú hozzászólás elküldve.");
        }
    }
}
$congressItems->setSelectedItem($selected_congress_item);
$isIamCreator = $congressOnline->amICongressCreator();
$onlinePoll = new CongressPoll();
$onlinePoll->getPollByItem($selected_congress_item);
if ($isIamCreator) {
    $view->submitButton('edit_poll', 'Határozat előkészítése');
    $errors = $congressItems->isReadyToVote();
    if (!$congressItems->isItemClosed($selected_congress_item)) {
        $view->submitButton('close_item', 'Lezárása', -1, 'Tárgyalás lezárása', 'Biztosan le akarod zárni a tárgyalást ?');
        $view->submitButton('warning_poll', 'Figyelmeztetés', -1, 'A lezárásra figyelmeztető email');
        $view->submitButton('edit_topic', 'Témakörök', $selected_congress_item, 'A napirendhez tartozó témakörök szerkesztése');
    } elseif (!$onlinePoll->isOpen()) {
        if (empty($errors)) {
            $view->submitButton('open_poll', 'Szavazás', -1, 'Szavazás megnyitása', 'Biztosan Meg akarod nyitni a szavazást ?');
        } else {
            $error_txt = 'Nem lehet a szavazás lezárni. Hiba:';
            foreach ($errors as $err) {
                $error_txt .= $err . '\n';
            }
            $view->submitAlert('Szabazás', 'A szavazást nem lehet megnyitni, még hiányoznak feltételek', $error_txt);
        }
    }
}
$view->endDiv();

$view->newDiv('row')->putText($congressItems->getCongressItemDescription(), 'Leírás', true)->endDiv();
$pendings = $congressItems->getPendingDependencies();
if (strlen($pendings) > 0) {
    $view->info('<b>Ez előtt lezárandó szavazások:  </b>' . $pendings);
}
$cTopic = new CongressItemTopic();
$topics = $cTopic->getTopicsOfItem($selected_congress_item);
if (empty($topics)) {
    $selected_topic_filter = -2;
} else {
    set_default_value('selected_topic_filter', TOPIC_ID_ALL);
    $filter_topics = $topics;
    $filter_topics[TOPIC_ID_ALL] = 'Összes';
    ksort($filter_topics);
    $view->submitDropDownChooser('Hozzászólás szűrő', [-1 => 'Általános'] + $filter_topics, 'selected_topic_filter', true);
}
$view->endKeret();

if ($onlinePoll->getPollIdByItem($selected_congress_item) > 0) {
    $view->newKeret();
    if ($congressItems->isItemClosed($selected_congress_item)) {
        $view->info('Szavazás a napirendi pontról lezárul: ' . output_time($onlinePoll->getPollToBeClosed()));
    }
    $view->putText($onlinePoll->getPollDescription(), 'Határozat javaslat szövege', true);
    $view->endKeret();
}
$view->newKeret();
if ($congressItems->isItemClosed($selected_congress_item)) {
    $view->warning('A napirendi pont tárgyalása befejeződött');
    if ($onlinePoll->isOpen()) {
        $view->warning('A szavazás már folyik');
        $poll_id = $onlinePoll->getPollId();
        $view->submitButton('forward_to_vote', 'Szavazás', $poll_id, 'Tovább a napirend szavazásához');
    }
    $view->putElement(showCommentsOfItem($selected_congress_item, $selected_topic_filter, $selected_topic_filter != TOPIC_ID_ALL));
    $view->endKeret();
    return;
}
$view->submitRefresh('reload_page', 0, 'Az oldal frissítése');
$view->putElement('<button type="button" class="btn btn-primary mx-2  px-4" data-toggle="modal" data-target="#new_comment" >Új hozzászólás</button>');
$view->newElement('<div class="modal fade" id="new_comment" tabindex="-1" aria-labelledby="new_comment_label" aria-hidden="true" role="dialog">');
$view->newDiv('modal-dialog modal-lg')->newDiv('modal-content');
$view->newDiv('modal-header');
$view->putElement('<h5 class="modal-title" id="new_comment_label">Új hozzászólás</h5>');
$view->newElement('<button class="close" data-dismiss="modal" aria-label="Bezár">');
$view->putElement('<span aria-hidden="true">&times;</span>');
$view->endElement('</button>');
$view->endDiv()->newDiv('modal-body');
$congress_comment = '';
$view->inputTextArea('congress_comment', '', 'Hozzászólás...');
if (!empty($topics)) {
    $view->inputDropDownChooser('Témakör', [-1 => 'Általános'] + $topics, 'selected_add_topic');
}
$view->submitButton('save_comment', 'Küld', time(), 'Hozzászólás beküldése', 'Biztosan beküldöd a hozzászólásod? Azt már sem törölni, sem módosítani nem lehet később.');
$view->endDiv()->endDiv()->endDiv();
$view->endElement('</div>');

$comments = $congressComment->getCommentsOfItem($selected_congress_item);
if ($isIamCreator AND isset($change_comment_topic)) {
    foreach ($comments as $comment) {
        $comment_id = $comment[CONGRESS_COMMENTS_ID];
        $var_name = 'selected_comment_topic_' . $comment_id;
        if ($$var_name != $comment[CONGRESS_COMMENTS_TOPIC_ID]) {
            $congressComment->setSelectedCommment($comment_id)->setTopicOfComment($$var_name);
            logger($member_id, $comment[CONGRESS_COMMENTS_MEMBER_ID], LOGGER_CONGRESS_COMMENT, $comment_id . ' azonosítójú hozzászólás témaköre ' . $$var_name . '-ra módosítva.');
        }
    }
    $comments = $congressComment->getCommentsOfItem($selected_congress_item);
}
if (!empty($comments)) {
    foreach ($comments as $comment) {
        if ($selected_topic_filter == TOPIC_ID_ALL OR $comment[CONGRESS_COMMENTS_TOPIC_ID] == $selected_topic_filter) {
            $view->putElement('<hr>');
            $view->newDiv('row show_avatar', 'comment_id_' . $comment[CONGRESS_COMMENTS_ID]);
            $mem_id = $comment[CONGRESS_COMMENTS_MEMBER_ID];
            $view->putElement(outputAvatarOfMember($mem_id));
            $view->putElement('<b>' . getMemberName($mem_id) . '</b> : ' . output_time($comment[CONGRESS_COMMENTS_TIME]));
            if ($isIamCreator AND ! empty($topics)) {
                $var_name = 'selected_comment_topic_' . $comment[CONGRESS_COMMENTS_ID];
                $$var_name = $comment[CONGRESS_COMMENTS_TOPIC_ID];
                $view->inputDropDownChooser('', [-1 => 'Általános'] + $topics, $var_name);
                $view->submitSave('change_comment_topic', $comment[CONGRESS_COMMENTS_ID], 'A hozzászólás témakörének mentése');
            }
            $view->EndDiv();
            $view->putElement($comment[CONGRESS_COMMENTS_COMMENT]);
        }
    }
    if (isset($change_comment_topic) AND $change_comment_topic > 0) {
        $view->putScript('var alteredElement = document.getElementById("comment_id_' . $change_comment_topic . '");alteredElement.scrollIntoView({behavior: "auto", block:"center"});');
    }
}
$view->endKeret();
