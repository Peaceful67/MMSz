<?php

define("CONGRESS_OPTIONS_BY_NAME", 2);
define("CONGRESS_OPTIONS_PUBLIC", 4);
define("CONGRESS_OPTIONS_BY_EMAIL", 8);
define("CONGRESS_OPTIONS_APPENDABLE", 16);
define("CONGRESS_OPTIONS_MULTICHOICE", 32);
define("CONGRESS_OPTIONS_AUTO_ITEM", 64);
define("CONGRESS_OPTIONS_AUTH_MANDATORY", 128);


include_once POLLING . 'CongressOnline.inc';
include_once POLLING . 'CongressItems.inc';
include_once POLLING . 'CongressComments.inc';
include_once POLLING . 'CongressDelegates.inc';
include_once POLLING . 'CongressPoll.inc';
include_once POLLING . 'CongressPollOption.inc';
include_once POLLING . 'CongressPollVote.inc';

$congressOnline = new CongressOnline();
$congressDelegates = new CongressDelegates();
$congressItems = new CongressItems();
$congressComment = new CongressComments();
$congressPoll = new CongressPoll();
$congressPollOption = new CongressPollOption();
$congressPollVote = new CongressPollVote();

function getCongressSummerize($congressId) {
    if ($congressId < 0) {
        return;
    }
    $sumCongressOnline = new CongressOnline();
    $sumCongressOnline->setCongressEdit($congressId);
    $sumCongressItems = new CongressItems();
    $sumCongressDelegates = new CongressDelegates();
    $sumCongressItems->setSelectedCongress($congressId);
    $auth1 = $sumCongressOnline->getCongressAuth1();
    $auth2 = $sumCongressOnline->getCongressAuth2();
    $congress_sum = '<h3>' . $sumCongressOnline->getCongressTitle() . '</h3>';
    $congress_sum .= $sumCongressOnline->getCongressDescription() . '<br>';
    $congress_sum .= '<b><label>Utólagos napirend:</label></b>' . ($sumCongressOnline->isCongressAppendable() ? 'Igen' : 'Nem') . '<br>';
    $created = $sumCongressOnline->getCongressCreated();
    $congress_sum .= '<b><label>Gyűlés létrehozva:</label></b>' . (is_null($created) ? 'Nincs létrehozva' : $created) . '<br>';
    $closed = $sumCongressOnline->getCongressClosed();
    $congress_sum .= '<b><label>Gyűlés lezárva:</label></b>' . (is_null($closed) ? 'Nincs lezárva' : $closed) . '<br>';
    $congress_sum .= '<b><label>Gyűlés létrehozója és levezető elnöke:</label></b>' . getMemberName($sumCongressOnline->getCongressCreator()) . '<br>';
    $congress_sum .= '<b><label>Jegyzőkönyv hitelesítők:</label></b>'
            . ($auth1 > 0 ? (getMemberName($auth1) . ', ') : 'Nincs megadva, ')
            . ($auth2 > 0 ? (getMemberName($auth2) ) : 'Nincs megadva')
            . '<br>';
    $congress_sum .= '<b><label>A szavazó tagok: </label></b>';
    foreach ($sumCongressDelegates->getMembersOfCongress($congressId) as $mem_id => $mem_name) {
        $congress_sum .= $mem_name . ', ';
    }
    $congress_sum .= '<hr><h3>Napirendi pontok:</h3>';
    foreach ($sumCongressItems->getSelectionOfCongress() as $itemId => $itemTitle) {
        $sumCongressItems->setSelectedItem($itemId);
        $congress_sum .= '<b><label>Címe:</label></b>' . $itemTitle . '<br>';
        $congress_sum .= $sumCongressItems->getCongressItemDescription() . '<br>';
        $congress_sum .= '<b><label>Név szerint:</label></b>' . ($sumCongressItems->isItemByName() ? 'Igen' : 'Nem') . '<br>';
        $congress_sum .= '<b><label>Publikus:</label></b>' . ($sumCongressItems->isItemPublic() ? 'Igen' : 'Nem') . '<br>';
        $congress_sum .= '<b><label>Email szavazás:</label></b>' . ($sumCongressItems->isItemByEmail() ? 'Igen' : 'Nem') . '<br>';
        $congress_sum .= '<b><label>Több választás:</label></b>' . ($sumCongressItems->isItemMultichoice() ? 'Igen' : 'Nem') . '<br>';
        if (is_file(($doc_file = CONGRESS_ITEM_DOC . $itemId . '.pdf'))) {
            $congress_sum .= '<a target="_blank" href="' . URL . $doc_file . '">Melléklet</a><br>';
        }
        $dependencies = $sumCongressItems->getDependency();
        if (!empty($dependencies)) {
            $congress_sum .= '<b><label>Ezek után zárható:</label></b>';
            foreach ($dependencies as $depItemId) {
                $congress_sum .= '"' . $sumCongressItems->getCongressItemTitleById($depItemId) . '", ';
            }
        } else {
            $congress_sum .= '<b><label>Más napirendektől független!</label></b>';
        }
        $congress_sum .= '<br><b><label>Állapot:</label></b>';
        $closed = $sumCongressItems->getClose();
        if (!$sumCongressItems->isItemClosed($itemId)) {
            $congress_sum .= 'Tárgyalás alatt';
        } else {
            $congress_sum .= 'Lezárva: ' . $closed;
        }
        $congress_sum .= '<hr>';
    }
    return $congress_sum;
}

function getCommentedItemSummerize($itemId) {
    if ($itemId < 0) {
        return;
    }
    $sumCongressItem = new CongressItems();
    $sumComments = new CongressComments();
    $sumCongressItem->setSelectedItem($itemId);
    $ret = '<h3>' . $sumCongressItem->getCongressItemTitle() . '</h3>';
    $ret .= 'Leírás:' . $sumCongressItem->getCongressItemDescription() . '<br>';
    $comments = $sumComments->getCommentsOfItem($itemId);
    if (!empty($comments)) {
        foreach ($comments as $comment) {
            $ret .= '<hr>';
            $mem_id = $comment[CONGRESS_COMMENTS_MEMBER_ID];
            $ret .= '<b>' . getMemberName($mem_id) . ' : ' . date('Y-m-d H:i:s', strtotime($comment[CONGRESS_COMMENTS_TIME])) . '</b>';
            $ret .= $comment[CONGRESS_COMMENTS_COMMENT];
            $doc_file = CONGRESS_COMMENT_DOC . $comment[CONGRESS_COMMENTS_ID] . '.pdf';
            if (is_file($doc_file)) {
                $ret .= '<br><a target="_blank" href="' . URL . $doc_file . '">Melléklet</a>';
            }
        }
    } else {
        $ret .= 'Nincs hozzászólás.';
    }

    return $ret;
}
