<?php

define("CONGRESS_OPTIONS_ABSTAIN_ENABLED", 1 << 0); // 1
define("CONGRESS_OPTIONS_YES_NO", 1 << 1); // 2
define("CONGRESS_OPTIONS_BY_NAME", 1 << 2); // 4
define("CONGRESS_OPTIONS_PUBLIC", 1 << 3); // 8
define("CONGRESS_OPTIONS_BY_EMAIL", 1 << 4); // 16
define("CONGRESS_OPTIONS_APPENDABLE", 1 << 5); // 32
define("CONGRESS_OPTIONS_MULTICHOICE", 1 << 6);  // 64
define("CONGRESS_OPTIONS_AUTO_ITEM", 1 << 7); // 128
define("CONGRESS_OPTIONS_AUTH_MANDATORY", 1 << 8); // 256

define("CONGRESS_OPTIONS_DECISION_DONE", 1 << 9); // 512
define("CONGRESS_OPTIONS_DECISION_50", 1 << 10); // 1024
define("CONGRESS_OPTIONS_DECISION_75", 1 << 11); // 2048
define("CONGRESS_OPTIONS_DECISION_100", 1 << 12); // 4096
define("CONGRESS_OPTIONS_DECISION_ETC", 1 << 13);  // 8192
define("CONGRESS_OPTIONS_POLL_BASE_VOTED", 1 << 14); // 16384
define("CONGRESS_OPTIONS_INHERITED", (CONGRESS_OPTIONS_POLL_BASE_VOTED + CONGRESS_OPTIONS_BY_EMAIL | CONGRESS_OPTIONS_PUBLIC | CONGRESS_OPTIONS_BY_NAME));

define("CONGRESS_MANDATE_TYPE_ONE", 0); // 1 küldöttnek egy szavazata van (MMSZ)
define("CONGRESS_MANDATE_TYPE_RANGE", 1); // 1 küldöttnek a sávonként megadott szavazata van
define("CONGRESS_MANDATE_TYPE_CLUB", 2); // 1 küldött az egyesületeknél megadott szavazata van
define("CONGRESS_MANDATE_TYPE_CLUB_2", 3); // 1 küldött az egyesületeknél megadott szavazata van

define("TOPIC_ID_ALL", -2);
define("TOPIC_GENREAL", 'Általános');

include_once POLLING . 'CongressOnline.inc';
include_once POLLING . 'CongressItems.inc';
include_once POLLING . 'CongressComments.inc';
include_once POLLING . 'CongressDelegates.inc';
include_once POLLING . 'CongressPoll.inc';
include_once POLLING . 'CongressPollOption.inc';
include_once POLLING . 'CongressPollVote.inc';
include_once POLLING . 'CongressAccess.inc';
include_once POLLING . 'CongressItemTopic.inc';
include_once POLLING . 'CongressInvited.inc';

$congressAccess = new CongressAccess($member_id);
$congressOnline = new CongressOnline();
$congressDelegates = new CongressDelegates();
$congressItems = new CongressItems();
$congressComment = new CongressComments();
$congressPoll = new CongressPoll();
$congressPollOption = new CongressPollOption();
$congressPollVote = new CongressPollVote();
$congressInvited = new CongressInvited();


$decision_options = [
    CONGRESS_OPTIONS_DECISION_50 => 'Egyszerű többség ',
    CONGRESS_OPTIONS_DECISION_75 => 'Minősített többség',
    CONGRESS_OPTIONS_DECISION_100 => 'Egyhangú',
    CONGRESS_OPTIONS_DECISION_ETC => 'Egyedileg megállapított',
];

$mandate_names = [
    CONGRESS_MANDATE_TYPE_ONE => [
        'short' => "Egy szavazat",
        'desc' => "Minden küldöttnek egy szavazata van.",
    ],
    CONGRESS_MANDATE_TYPE_RANGE => [
        'short' => "Sávos",
        'desc' => "A Szövetség beállításainál megadott taglétszám függő sávok szerint.",
    ],
    CONGRESS_MANDATE_TYPE_CLUB => [
        'short' => "1-es mandátum szerint",
        'desc' => "Küldöttenként az egyesületénél megadott 1-es számú mandátumszám szerinti szavazatszám.",
    ],
    CONGRESS_MANDATE_TYPE_CLUB_2 => [
        'short' => "2-es mandátum szerint",
        'desc' => "Küldöttenként az egyesületénél megadott 2-es számú mandátumszám szerinti szavazatszám.",
    ],
];

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

function getUndecidedPollsOfCongress($congress_id) {
    $ret = array();
    $cItem = new CongressItems();
    $cPoll = new CongressPoll();
    $items = $cItem->setSelectedCongress($congress_id)->getSelectionOfCongress();
    foreach (array_keys($items) as $item_id) {
        if ($item_id > 0 AND $cPoll->getPollByItem($item_id)) {
            if ($cPoll->getPollId() > 0 AND ! $cPoll->isDecided()) {
                $ret[$cPoll->getPollId()] = $cPoll->getPollTitle();
            }
        }
    }
    return $ret;
}

function getNameDecisionOption($option) {
    global $decision_options;
    foreach ($decision_options as $option_mask => $decision_name) {
        if (($option & $option_mask) > 0) {
            return $decision_name;
        }
    }
    return '';
}

function getThresholdOfVote($poll_id, $option) {
    $congressPoll = new CongressPoll();
    $congressPoll->setSelectedPollId($poll_id);
    $congress_id = $congressPoll->getCongressIdByPollId($poll_id);
    if ((new CongressOnline())->setSelectedCongress($congress_id)->isCongressBaseVoted()) {
        $total = (new CongressPollVote())->getNumberOfVotes($congress_id, $poll_id);
        //       error_log('BaseVote:'. $total);
    } else {
        $congressDelegates = new CongressDelegates();
        $total = count($congressDelegates->getMembersOfCongress($congress_id));
        //       error_log('Total'.$total);
    }
    if ($total < 3) {
        return 'Nem meghatározható';
    }
    return getThreshold($total, $option);
}

function getThreshold($total, $option) { // A szavazat küszöbbel tér vissza
    $option &= CONGRESS_OPTIONS_DECISION_50 + CONGRESS_OPTIONS_DECISION_75 + CONGRESS_OPTIONS_DECISION_100 + CONGRESS_OPTIONS_DECISION_ETC;
    switch ($option) {
        case CONGRESS_OPTIONS_DECISION_100:
            $ret = $total;
            break;
        case CONGRESS_OPTIONS_DECISION_75:
            $ret = intval((($total) * 3 ) / 4) + 1;
            break;
        case CONGRESS_OPTIONS_DECISION_50:
            $ret = intval(($total) / 2) + 1;
            break;
        case CONGRESS_OPTIONS_DECISION_ETC:
            $ret = 'Nem meghatározható (egyedi)';
            break;
        default:
            $ret = 'Nem meghatározható (default)';
            break;
    }
    return $ret;
}

function showCommentsOfItem($item_id, $selected_topic_filter, $by_topic) {
    $ret = '';

    $comments = (new CongressComments())->getCommentsOfItem($item_id);
    if ($by_topic AND ! empty($comments)) {
        $cItemTopic = new CongressItemTopic();
        $topics = $cItemTopic->getTopicsOfItem($item_id);
        if (!empty($topics)) {
            $topics[-1] = TOPIC_GENREAL;
            ksort($topics);
            foreach ($topics as $topic_id => $topic_name) {
                $ret .= '<h3>Témakör: ' . $topic_name . '</h3>';
                foreach ($comments as $comment) {
                    if ($comment[CONGRESS_COMMENTS_TOPIC_ID] == $topic_id) {
                        $ret .= '<hr>';
                        $ret .= showComment($comment, false);
                    }
                }
            }
            return $ret;
        } // else Nincsenek is temakorok
    }
    if (!empty($comments)) {
        foreach ($comments as $comment) {
            if ($selected_topic_filter == TOPIC_ID_ALL OR $comment[CONGRESS_COMMENTS_TOPIC_ID] == $selected_topic_filter) {
                $ret .= '<hr>';
                $ret .= showComment($comment, false);
            }
        }
    } else {
        $ret .= 'Nincsnek hozzászólások';
    }
    return $ret;
}

function showComment($comment, $avatar = false) {
    $ret = '<div class="show_avatar">';
    $mem_id = $comment[CONGRESS_COMMENTS_MEMBER_ID];
    if ($avatar) {
        $ret .= outputAvatarOfMember($mem_id);
    }
    $ret .= '<b>' . getMemberName($mem_id) . '</b> : ' . output_time($comment[CONGRESS_COMMENTS_TIME]);
    $ret .= '</div>';
    $ret .= $comment[CONGRESS_COMMENTS_COMMENT];
    $doc_file = CONGRESS_COMMENT_DOC . $comment[CONGRESS_COMMENTS_ID] . '.pdf';
    if (is_file($doc_file)) {
        $ret .= '<br><a target="_blank" href="' . URL . $doc_file . '">Melléklet</a>';
    }
    return $ret;
}

function countVotes($votes) {
    $options = array();
    foreach ($votes as $vote) {
        if ($vote[CONGRESS_POLL_VOTE_DELEGATE] < 0) {
            $options[$vote[CONGRESS_POLL_VOTE_OPTION_ID]] = $vote[CONGRESS_POLL_VOTE_ANONIM_COUNTER];
        } elseif (isset($options[$vote[CONGRESS_POLL_VOTE_OPTION_ID]])) {
            $options[$vote[CONGRESS_POLL_VOTE_OPTION_ID]] ++;
        } else {
            $options[$vote[CONGRESS_POLL_VOTE_OPTION_ID]] = 1;
        }
    }
    return $options;
}

function showVoteNumbers($votes) {
    $ret = '';
    $cPollOption = new CongressPollOption();
//    error_log('showVoteNumbers votes:' . print_r($votes, true));
    $options = countVotes($votes);
//    error_log('showVoteNumbers options:' . print_r($options, true));
    if (!empty($options)) {
        $first_vote = true;
        foreach ($options as $option_id => $num) {
            if ($first_vote) {
                $first_vote = false;
            } else {
                $ret .= '<br><label> </label>';
            }
            $ret .= '<a title="' . $cPollOption->getDescription($option_id) . '">' . $cPollOption->getTitle($option_id) . ': </a> ' . $num . ' szavazat';
        }
    }
    return $ret;
}

function showVoteResult($poll_id) {
    $poll = new CongressPoll();
    $poll->setSelectedPollId($poll_id);
    $results = $poll->getResult();
    $ret = '';
    if ($results AND ! empty($results)) {
        if (!in_array(true, $results)) {
            return 'Egyik sem érte el a szavazati küszöböt';
        }
        $cPollOption = new CongressPollOption();
        $first_option = true;
        foreach ($results as $option_id => $isWon) {
            if ($first_option) {
                $first_option = false;
            } else {
                $ret .= '<label> </label>';
            }
            $ret .= '<a title="' . $cPollOption->getDescription($option_id) . '">' . $cPollOption->getTitle($option_id) . ': </a>  ' . ($isWon ? 'Megszavazva' : 'Elutasítva') . '<br>';
        }
    } else {
        $ret .= 'A szavazás eredménytelen';
    }
    return $ret;
}

function showCommentsOfCongress($congress_id, $by_topic) {
    $ret = '';
    if ($by_topic) {
        $items = (new CongressItems())->setSelectedCongress($congress_id)->getSelectionOfCongress();
        foreach ($items as $item_id => $item_name) {
            $ret .= '<div class="keret"><h3>Napirend:' . $item_name . '</h3>';
            $ret .= showCommentsOfItem($item_id, TOPIC_ID_ALL, $by_topic);
            $ret .= '</div>';
        }
    } else {
        $cComments = new CongressComments();
        $comments = $cComments->getCommentsOfCongress($congress_id);
        foreach ($comments as $comment) {
            $ret .= '<hr>';
            $ret .= '<b><label>Napirend:</label></b>' . $comment[CONGRESS_ITEMS_TITLE] . '<br>';
            $ret .= showComment($comment, false);
        }
    }
    return $ret;
}

function showVotesByName($poll_id) {
    $ret = '';
    $cPollOption = new CongressPollOption();
    $congressDelegates = new CongressDelegates();
    $congressDelegates->setCongressIdByPollId($poll_id);
    $votes = (new CongressPollVote())->getVotesOfPoll($poll_id);
    $first_name = true;
    $last_mem_id = -1;
    foreach ($votes as $vote) {
        $mem_id = $vote[CONGRESS_POLL_VOTE_DELEGATE];
        $option = $cPollOption->getTitle($vote[CONGRESS_POLL_VOTE_OPTION_ID]);
        if ($mem_id != $last_mem_id) {
            if ($first_name) {
                $first_name = false;
            } else {
                $ret .= '<br><label></label>';
            }
            if (!empty($clubs = getDelegateClubNames($mem_id))) {
                $clubs = '(' . substr($clubs, 0, -2) . ')';
            }
            $mandat = $congressDelegates->getMandateNumber($mem_id);
            $ret .= '<a title="Azonosító:' . $mem_id . '">' . getMemberName($mem_id) . ' ' . $clubs
                    . ($mandat > 0 ? (' (' . $mandat . ' mandátum)') : '') . '</a>: ';
            $last_mem_id = $mem_id;
        }
        $ret .= $option . ', ';
    }
    return $ret;
}

function showHaventVoted($poll_id) {
    $ret = '';
    $delegates = getHaventVoted($poll_id);
    if (!empty($delegates)) {
        foreach ($delegates as $mem_id => $mem_name) {
            $ret .= '<a title="Azonosító:' . $mem_id . '">' . $mem_name . '</a>, ';
        }
    }
    return $ret;
}

function getHaventVoted($poll_id) {
    $cPoll = (new CongressPoll())->setSelectedPollId($poll_id);
    $congress_id = $cPoll->getCongressId();
    $delegates = (new CongressDelegates())->getMembersOfCongress($congress_id);
    $cPollOption = new CongressPollOption();
    $votes = (new CongressPollVote())->getVotesOfPoll($poll_id);
    foreach ($votes as $vote) {
        unset($delegates[$vote[CONGRESS_POLL_VOTE_DELEGATE]]);
    }
    return $delegates;
}

/* Ez már másképp lett
  function getMandatOfDelegate($mem_id) {
  global $mysqliLink;
  $mandat = 0;
  $sql = 'SELECT `' . CLUB_MANDATUM . '` FROM `' . CLUB_TABLE . '` '
  . ' INNER JOIN `' . ROLE_TABLE . '` AS `delegated_club` ON ('
  . ' `delegated_club`.`' . ROLE_MEMBER . '`=' . $mem_id . ' AND '
  . ' `delegated_club`.`' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_DELEGATE . ' AND '
  . '(CURDATE() BETWEEN `delegated_club`.`' . ROLE_FROM . '` AND `delegated_club`.`' . ROLE_TO . '`)'
  . ')'
  . ' WHERE `' . CLUB_TABLE . '`.`' . CLUB_ID . '`=`delegated_club`.' . '`' . ROLE_CLUB . '`;';
  $res = $mysqliLink->query($sql);
  while ($res AND $row = $res->fetch_assoc()) {
  $mandat += ($row[CLUB_MANDATUM] == 0) ? 1 : $row[CLUB_MANDATUM];
  }
  return $mandat;
  }
 */

function getMandatedClubsofDelegate($congress_id) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `clubs`
INNER JOIN `congress_delegates` ON (`congress_delegates`.`congress_id`=' . $congress_id . ')
INNER JOIN `role_member` AS `delegated_club`
ON (`delegated_club`.`member_id`=`congress_delegates`.`member_id` AND
        `delegated_club`.`permission`=' . ACCESS_CLUB_DELEGATE . ' AND
     (CURDATE() BETWEEN `delegated_club`.`date_from` AND
`delegated_club`.`date_to`)
    )
WHERE `clubs`.`IDK`=`delegated_club`.`club_id` AND `clubs`.`mandat`>0
';
    $ret = array();
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row;
    }
    return $ret;
}
