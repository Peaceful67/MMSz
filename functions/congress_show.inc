<?php

function getCongressSummerize($congressId) {
    global $mandate_names;
    if ($congressId < 0) {
        return;
    }
    $sumCongressOnline = new CongressOnline();
    $sumCongressOnline->setSelectedCongress($congressId);
    $sumCongressDelegates = new CongressDelegates();
    $auth1 = $sumCongressOnline->getCongressAuth1();
    $auth2 = $sumCongressOnline->getCongressAuth2();
    $congress_sum = '<h3>' . $sumCongressOnline->getCongressTitle() . '</h3>';
    $congress_sum .= $sumCongressOnline->getCongressDescription() . '<br>';
    $congress_sum .= '<b><label>Utólagos napirend:</label></b>' . ($sumCongressOnline->isCongressAppendable() ? 'A gyűlés tárgyalása során van lehetőség új napirend felvételére.' : 'Nem') . '<br>';
    $congress_sum .= '<b><label>Nyilvános:</label></b>' . ($sumCongressOnline->isCongressPublic() ? 'Igen' : 'Nem') . '<br>';
    $congress_sum .= '<b><label>Automatikus napirend:</label></b>' . ($sumCongressOnline->isCongressAutoItem() ? 'Automatikusan létrejön a gyűlés menetrendjét elfogadó első napirend.' : 'Nem') . '<br>';
    $congress_sum .= '<b><label>Jegyzőkönyv hitelesítők:</label></b>' . ($sumCongressOnline->isCongressAuthMandatory() ? 'Kötelező megadni' : 'Nem kötelező') . '<br>';
    $created = $sumCongressOnline->getCongressCreated();
    $congress_sum .= '<b><label>Létrehozva:</label></b>' . (is_null($created) ? 'Szerkesztés alatt' : output_time($created)) . '<br>';
    $closed = $sumCongressOnline->getCongressClosed();
    $congress_sum .= '<b><label>Lezárva:</label></b>' . (is_null($closed) ? 'Nincs lezárva' : output_time($closed)) . '<br>';
    $congress_sum .= '<b><label>Létrehozó és levezető elnök: </label></b>' . getMemberName($sumCongressOnline->getCongressCreator()) . '<br>';
    $congress_sum .= '<b><label>Jegyzőkönyv hitelesítők:</label></b>'
            . ($auth1 > 0 ? (getMemberName($auth1) . ', ') : 'Nincs megadva, ')
            . ($auth2 > 0 ? (getMemberName($auth2) ) : 'Nincs megadva')
            . '<br>';
    $mand_type = $sumCongressOnline->getMandateType();
    $congress_sum .= '<b><label>A szavazó számlálás: </label></b><a title="' . $mandate_names[$mand_type]['desc'] . '">' . $mandate_names[$mand_type]['short'] . '</a><br>';
    $congress_sum .= '<b><label>A számlálás alapja: </label></b>' . ($sumCongressOnline->isCongressBaseVoted() ? 'A szavazatot leadók száma' : 'A gyűlésre meghívottak száma') . '<br>';
    $congress_sum .= '<b><label>A szavazó tagok: </label></b>';
    $sumCongressDelegates->setSelectedCongress($congressId);
    $sumDelegates = $sumCongressDelegates->getMembersOfCongress($congressId);
    $congress_sum .= '(' . count($sumDelegates) . ' tag) :';
    foreach ($sumDelegates as $mem_id => $mem_name) {
        if (!empty($clubs = getDelegateClubNames($mem_id))) {
            $clubs = '(' . substr($clubs, 0, -2) . ')';
        } else {
            $clubs = '(Nincs egyesület megadva)';
        }
        $congress_sum .= '<a title="Azonosító:' . $mem_id . ' ' . $clubs . '">' . $mem_name;
        $mandate = $sumCongressDelegates->getMandateNumber($mem_id);
        if ($mandate > 0) {
            $congress_sum .= ' (' . $mandate . ')';
        }
        $congress_sum .= '</a>, ';
    }
    $congress_sum = substr($congress_sum, 0, -2);
    $sumInvited = (new CongressInvited())->getMembersOfCongress($congressId);
    if (!empty($sumInvited)) {
        $congress_sum .= '<br><b><label>Meghívott tagok </label></b>';
        foreach ($sumInvited as $mem_id => $mem_name) {
            $clubs = '(Nincs egyesület megadva)';
            if (!empty($clubs = getDelegateClubNames($mem_id))) {
                $clubs = '(' . substr($clubs, 0, -2) . ')';
            }
            $congress_sum .= '<a title="Azonosító:' . $mem_id . ' ' . $clubs . '">' . $mem_name . '</a>, ';
        }
    }
    $sumCongressItems = new CongressItems();
    $sumCongressItems->setSelectedCongress($congressId);
    $congress_sum .= '<hr><h3>Napirendi pontok:</h3>';
    foreach ($sumCongressItems->getSelectionOfCongress() as $itemId => $itemTitle) {
        $sumCongressItems->setSelectedItem($itemId);
        $congress_sum .= '<b><label>Címe:</label></b>' . $itemTitle . '<br>';
        $congress_sum .= $sumCongressItems->getCongressItemDescription() . '<br><br>';
        $congress_sum .= '<b><label>Név szerint:</label></b>' . ($sumCongressItems->isItemByName() ? 'Igen' : 'Nem') . '<br>';
        $congress_sum .= '<b><label>Email szavazás:</label></b>' . ($sumCongressItems->isItemByEmail() ? 'Igen' : 'Nem') . '<br>';
        $congress_sum .= '<b><label>Több választás:</label></b>' . ($sumCongressItems->isItemMultichoice() ? 'Több választ is be lehet jelölni' : 'Csak egyet lehet választani') . '<br>';
        $congress_sum .= '<b><label>Eldöntendő szavazás:</label></b>' . ($sumCongressItems->isItemYesNo() ? 'Igen' : 'Nem') . '<br>';
        $congress_sum .= '<b><label>Tartózkodás:</label></b>' . ($sumCongressItems->isItemAbstain() ? 'Megengedett' : 'Tiltott') . '<br>';
        $itemOption = $sumCongressItems->getItemOption();
        $congress_sum .= '<b><label>Szavazati küszöb:</label></b>' . getNameDecisionOption($itemOption) . '<br>';
        if (is_file(($doc_file = CONGRESS_ITEM_DOC . $itemId . '.pdf'))) {
            $congress_sum .= '<a target="_blank" href="' . URL . $doc_file . '">Melléklet</a><br>';
        }
        $dependencies = $sumCongressItems->getDependency();
        $congress_sum .= '<b><label>Ezek után zárható:</label></b>';
        if (!empty($dependencies)) {
            foreach ($dependencies as $depItemId) {
                $congress_sum .= '"' . $sumCongressItems->getCongressItemTitleById($depItemId) . '", ';
            }
        } else {
            $congress_sum .= 'Más napirendektől független!';
        }
        $congress_sum .= '<hr>';
    }
    $congress_sum .= '<label>Letöltve: </label>' . date('Y-m-d H:i');
    return $congress_sum;
}

function getCongressDetailed($congress_id, $comments = false, $by_topic = false) {
    if ($congress_id < 0) {
        return 'Nincs kiválasztva gyűlés';
    }
    $cItems = new CongressItems();
    $congress_sum = '';
    $cOnline = new CongressOnline();
    $cOnline->setSelectedCongress($congress_id);
    $congress_sum .= '<h3>' . $cOnline->getCongressTitle() . '</h3>';
    $congress_sum .= $cOnline->getCongressDescription() . '<br>';
    $cPoll = new CongressPoll();
    $polls = $cPoll->getIdsOfCongress($congress_id);
    if (empty($polls)) {
        $congress_sum .= 'A gyűléshez még nem tartoznak határozatok';
    } else {
        $congress_sum .= '<hr>';
        foreach ($polls as $item_id => $poll_id) {
            if ($poll_id < 0) {
                $congress_sum .= '<br><b><label>Napirend címe:</label></b>' . $cItems->getCongressItemTitleById($item_id);
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Tárgyalás alatt, még nincs határozati javaslat.';
//                $congress_sum .= '<br><b><label>Napirendi pont:</label></b>' ;
                $congress_sum .= '<hr>';
                continue;
            }
            $cPoll->setSelectedPollId($poll_id);
            if ($cPoll->isEdited()) {
                $congress_sum .= '<br><b><label>Címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                if ($cItems->isItemClosed($cPoll->getCongressItemId())) {
                    $congress_sum .= 'Szavazás előtt';
                } else {
                    $congress_sum .= 'Tárgyalás alatt';
                }
                $congress_sum .= '<br><label>Határozati javaslat:</label>' . $cPoll->getPollDescription();
                $congress_sum .= '<br><br><label>Szavazás lezárul:</label>' . output_time($cPoll->getPollToBeClosed());
            } elseif ($cPoll->isOpen()) {
                $congress_sum .= '<br><b><label>Határozat címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Szavazás alatt';
                $congress_sum .= '<br><br><label>Határozati javaslat:</label>' . $cPoll->getPollDescription();
                $congress_sum .= '<br><label>Szavazás lezárul:</label>' . output_time($cPoll->getPollToBeClosed());
                $numVotes = $cPoll->countOfVotes();
                $totalVotes = (new CongressDelegates)->numOfTotalVotes($congress_id);
                $congress_sum .= '<br><label>Szavazat számok:</label>' . $numVotes . '/' . $totalVotes;
                if ($cPoll->isClosable()) {
                    $congress_sum .= ' : a szavazás lezárható';
                }
                if ($cPoll->isPollByName()) {
                    $congress_sum .= '<br><b><label>Eddigi szavazatok:</label></b>' . showVoteNumbers($cPoll->getVotes());
                    $congress_sum .= '<br><label>Név szerint:</label>' . showVotesByName($poll_id);
                }
                $havenotVoted = showHaventVoted($poll_id);
                if (!empty($havenotVoted)) {
                    $congress_sum .= '<br><label>Még nem szavaztak:</label>' . $havenotVoted;
                }
//                $congress_sum .= '<br><label>Várható eredmény:</label>' . showVoteResult($poll_id);
            } elseif ($cPoll->isDecided()) {
                $congress_sum .= '<br><b><label>Címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Határozat elkészült';
                $congress_sum .= '<br><label>Határozat:</label>' . $cPoll->getPollDecision() . '<br>';
                if ($cPoll->getPollClosed() >= $cPoll->getPollToBeClosed()) {
                    $congress_sum .= '<br><label>Automatikusan lezárult:</label>' . output_time($cPoll->getPollClosed());
                } else {
                    $congress_sum .= '<br><label>Levezető elnök lezárta:</label>' . output_time($cPoll->getPollClosed());
                }
                $congress_sum .= '<br><b><label>Szavazat számok:</label></b>' . $cPoll->countOfVotes() . '/' . (new CongressDelegates)->numOfTotalVotes($congress_id);
                $congress_sum .= '<br><label>Szavazatok:</label>' . showVoteNumbers($cPoll->getVotes());
                if (!$cPoll->isPollMultiChoice() AND ! $cPoll->isPollDecisionETC()) {
                    $congress_sum .= '<br><label>Szavazás eredménye:</label>' . showVoteResult($poll_id);
                }
                if ($cPoll->isPollByName()) {
                    $congress_sum .= '<br><label>Név szerint:</label>' . showVotesByName($poll_id);
                }
            } elseif ($cPoll->isClosed()) {
                $congress_sum .= '<br><label><b>Címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Szavazás lezárva';
                $congress_sum .= '<br><label>Határozati javaslat:</label>' . $cPoll->getPollDescription() . '<br>';
                if ($cPoll->getPollClosed() >= $cPoll->getPollToBeClosed()) {
                    $congress_sum .= '<br><label>Automatikusan lezárult:</label>' . output_time($cPoll->getPollClosed());
                } else {
                    $congress_sum .= '<br><label>Levezető elnök lezárta:</label>' . output_time($cPoll->getPollClosed());
                }
                $congress_sum .= '<br><b><label>Szavazat számok:</label></b>' . $cPoll->countOfVotes() . '/' . (new CongressDelegates)->numOfTotalVotes($congress_id);
                $congress_sum .= '<br><label>Szavazatok:</label>' . showVoteNumbers($cPoll->getVotes());
                if (!$cPoll->isPollMultiChoice() AND ! $cPoll->isPollDecisionETC()) {
                    $congress_sum .= '<br><label>Szavazás eredménye:</label>' . showVoteResult($poll_id);
                } if ($cPoll->isPollByName()) {
                    $congress_sum .= '<br><label>Név szerint:</label>' . showVotesByName($poll_id);
                }
            }
            $congress_sum .= '<hr>';
        }
    }
    if ($comments) {
        $congress_sum .= '<h3>A gyűlés tárgyalása:</h3>';
        $congress_sum .= showCommentsOfCongress($congress_id, $by_topic);
    }
    return $congress_sum;
}

function getCongressShort($congress_id) {
    if ($congress_id < 0) {
        return 'Nincs kiválasztva gyűlés';
    }
    $cItems = new CongressItems();
    $congress_sum = '';
    $cOnline = new CongressOnline();
    $cOnline->setSelectedCongress($congress_id);
    $congress_sum .= '<h3>' . $cOnline->getCongressTitle() . '</h3>';
    $congress_sum .= $cOnline->getCongressDescription() . '<br>';
    $cPoll = new CongressPoll();
    $polls = $cPoll->getIdsOfCongress($congress_id);
    if (empty($polls)) {
        $congress_sum .= 'A gyűléshez még nem tartoznak határozatok';
    } else {
        $congress_sum .= '<hr>';
        foreach ($polls as $item_id => $poll_id) {
            if ($poll_id < 0) {
                $congress_sum .= '<br><b><label>Napirend címe:</label></b>' . $cItems->getCongressItemTitleById($item_id);
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Tárgyalás alatt, még nincs határozati javaslat.<hr>';
                continue;
            }
            $cPoll->setSelectedPollId($poll_id);
            if ($cPoll->isEdited()) {
                $congress_sum .= '<br><b><label>Címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                if ($cItems->isItemClosed($cPoll->getCongressItemId())) {
                    $congress_sum .= 'Szavazás előtt';
                } else {
                    $congress_sum .= 'Tárgyalás alatt';
                }
                $congress_sum .= '<br><label>Határozati javaslat:</label>' . $cPoll->getPollDescription() . '<br>';
                $congress_sum .= '<br><label>Szavazás tart:</label>' . output_time($cPoll->getPollToBeClosed());
            } elseif ($cPoll->isOpen()) {
                $congress_sum .= '<br><b><label>Határozat címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Szavazás alatt';
                $congress_sum .= '<br><label>Határozati javaslat:</label>' . $cPoll->getPollDescription() . '<br>';
                $congress_sum .= '<br><label>Szavazás lezárul:</label>' . output_time($cPoll->getPollToBeClosed());
                $numVotes = $cPoll->countOfVotes();
                $totalVotes = (new CongressDelegates)->numOfTotalVotes($congress_id);
                $congress_sum .= '<br><b><label>Szavazat számok:</label></b>' . $numVotes . '/' . $totalVotes;
                if ($cPoll->isClosable()) {
                    $congress_sum .= ' : a szavazás lezárható';
                }
                if ($cPoll->isPollByName()) {
                    $congress_sum .= '<br><label>Eddigi szavazatok:</label>' . showVoteNumbers($cPoll->getVotes());
                }
            } elseif ($cPoll->isDecided()) {
                $congress_sum .= '<br><b><label>Címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Határozat elkészült';
                $congress_sum .= '<br><label>Határozat:</label>' . $cPoll->getPollDecision() . '<br>';
                if ($cPoll->getPollClosed() >= $cPoll->getPollToBeClosed()) {
                    $congress_sum .= '<br><label>Automatikusan lezárult:</label>' . output_time($cPoll->getPollClosed());
                } else {
                    $congress_sum .= '<br><label>Levezető elnök lezárta:</label>' . output_time($cPoll->getPollClosed());
                }
                $congress_sum .= '<br><b><label>Szavazat számok:</label></b>' . $cPoll->countOfVotes() . '/' . (new CongressDelegates)->numOfTotalVotes($congress_id);
                $congress_sum .= '<br><label>Szavazatok:</label>' . showVoteNumbers($cPoll->getVotes());
                if (!$cPoll->isPollMultiChoice() AND ! $cPoll->isPollDecisionETC()) {
                    $congress_sum .= '<br><label>Szavazás eredménye:</label>' . showVoteResult($poll_id);
                }
            } elseif ($cPoll->isClosed()) {
                $congress_sum .= '<br><b><label>Címe:</label></b>' . $cPoll->getPollTitle();
                $congress_sum .= '<br><b><label>Állapot:</label></b>';
                $congress_sum .= 'Szavazás lezárva';
                $congress_sum .= '<br><label>Határozati javaslat:</label>' . $cPoll->getPollDescription() . '<br>';
                if ($cPoll->getPollClosed() >= $cPoll->getPollToBeClosed()) {
                    $congress_sum .= '<br><label>Automatikusan lezárult:</label>' . output_time($cPoll->getPollClosed());
                } else {
                    $congress_sum .= '<br><label>Levezető elnök lezárta:</label>' . output_time($cPoll->getPollClosed());
                }
                $congress_sum .= '<br><b><label>Szavazat számok:</label></b>' . $cPoll->countOfVotes() . '/' . (new CongressDelegates)->numOfTotalVotes($congress_id);
                $congress_sum .= '<br><label>Szavazatok:</label>' . showVoteNumbers($cPoll->getVotes());
                if (!$cPoll->isPollMultiChoice() AND ! $cPoll->isPollDecisionETC()) {
                    $congress_sum .= '<br><label>Szavazás eredménye:</label>' . showVoteResult($poll_id);
                }
            }
            $congress_sum .= '<hr>';
        }
    }
    return $congress_sum;
}

function getCongressRecord($congress_id, $comments = false, $by_topic = false) {
    if ($congress_id < 0) {
        return 'Nincs kiválasztva gyűlés';
    }
    $cOnline = new CongressOnline();
    $cDelegates = new CongressDelegates();
    $closed = $cOnline->setSelectedCongress($congress_id)->getCongressClosed();
    if (is_null($closed)) {
        return 'A gyűlés még nincs lezárva, a jegyzőkönyv nem elérhető';
    }
    $congress_sum = '<h3>Jegyzőkönyv</h3>';
    $congress_sum .= '<b><label>Gyűlés címe: </label></b>' . $cOnline->getCongressTitle() . '<br>';
    $congress_sum .= '<b><label>Szövege: </label></b>' . $cOnline->getCongressDescription() . '<br>';
    $congress_sum .= '<b><label>Nyivános: </label></b>' . ($cOnline->isCongressPublic() ? 'Igen' : 'Nem') . '<br>';
    $created = $cOnline->getCongressCreated();
    $congress_sum .= '<b><label>Létrehozva: </label></b>' . (is_null($created) ? 'Szerkesztés alatt' : output_time($created)) . '<br>';
    $closed = $cOnline->getCongressClosed();
    $congress_sum .= '<b><label>Lezárva: </label></b>' . (is_null($closed) ? 'Nincs lezárva' : output_time($closed)) . '<br>';
    $congress_sum .= '<b><label>Létrehozó és levezető elnök: </label></b>' . getMemberName($cOnline->getCongressCreator()) . '<br>';
    $congress_sum .= '<b><label>Résztvevő tagok: </label></b>';
    $sumDelegates = $cDelegates->getMembersOfCongress($congress_id);
    $congress_sum .= '( ' . count($sumDelegates) . ' tag ) :';
    foreach ($sumDelegates as $mem_id => $mem_name) {
        $congress_sum .= $mem_name . ', ';
    }
    $sumInvited = (new CongressInvited())->getMembersOfCongress($congress_id);
    if (!empty($sumInvited)) {
        $congress_sum .= '<br><b><label>Meghívott tagok </label></b>';
        foreach ($sumInvited as $mem_id => $mem_name) {
            $congress_sum .= $mem_name . ', ';
        }
    }
    $cPoll = new CongressPoll();
    $polls = $cPoll->getIdsOfCongressByDecisionNum($congress_id);
    foreach ($polls as $poll_id) {
        $cPoll->setSelectedPollId($poll_id);
        $congress_sum .= '<hr>';
        $congress_sum .= '<br><b><label>Címe: </label></b>' . $cPoll->getPollTitle();
        $congress_sum .= '<br><b><label>határozat száma: </label></b>' . ((empty($cPoll->getPollDecisionNumber())) ? 'Nincs megadva' : $cPoll->getPollDecisionNumber());
        $congress_sum .= '<br><b><label>Határozat szövege: </label></b><b style="font-size:18px;">' . $cPoll->getPollDecision() . '</b><br>';
        $congress_sum .= '<br><label>Szavazás meghatározott vége: </label>' . output_time($cPoll->getPollToBeClosed());
        $congress_sum .= '<br><label>Szavazás lezárult: </label>' . output_time($cPoll->getPollClosed());
        $congress_sum .= '<br><b><label>Szavazat számok: </label></b>' . $cPoll->countOfVotes() . '/' . (new CongressDelegates)->numOfTotalVotes($congress_id);
        $pollOption = $cPoll->getPollOptions();
        $congress_sum .= '<br><label>Szavazati küszöb: </label>' . getNameDecisionOption($pollOption) . ' (' . getThresholdOfVote($poll_id, $pollOption) . ' tag szavazata)';
        $congress_sum .= '<br><label>Szavazatok: </label>' . showVoteNumbers($cPoll->getVotes());
        if (!$cPoll->isPollMultiChoice() AND ! $cPoll->isPollDecisionETC()) {
            $congress_sum .= '<br><label>Szavazás eredménye: </label>' . showVoteResult($poll_id);
        } if ($cPoll->isPollByName()) {
            $congress_sum .= '<br><label>Név szerint: </label>' . showVotesByName($poll_id);
        }
    }
    if ($comments) {
        $congress_sum .= '<h3>A gyűlés tárgyalása:</h3>';
        $congress_sum .= showCommentsOfCongress($congress_id, $by_topic);
    }
    $congress_sum .= '<hr><br><br>';
    $congress_sum .= '<label>Levezető elnök :</label>';
    $congress_sum .= '...........................   (' . getMemberName($cOnline->getCongressCreator()) . ')<br><br><br>';

    if ($cOnline->getCongressAuth1() > 0) {
        $congress_sum .= '<label>Hitelesítő :</label>';
        $congress_sum .= '...........................   (' . getMemberName($cOnline->getCongressAuth1()) . ')<br><br><br>';
    }
    if ($cOnline->getCongressAuth2() > 0) {
        $congress_sum .= '<label>Hitelesítő :</label>';
        $congress_sum .= '..........................   (' . getMemberName($cOnline->getCongressAuth2()) . ')<br><br><br>';
    }
    return $congress_sum;
}

function getCongressProgress($congress_id) {
    $progress = [
        'created' => [10, 'grey', 'Létrehozva'],
        'item' => [20, 'yellow', 'Tárgyalás alatt'],
        'item_poll' => [30, 'orange', 'Tárgyalás alatt, határozati javaslattal'],
        'poll' => [40, 'blue', 'Szavazás alatt'],
        'pool_vote' => [50, 'blue', 'Szavazás alatt, érkezett szavazokkal'],
        'voted' => [80, 'blue', 'Szavazás lezárva'],
        'decision' => [90, 'darkblue', 'Szavazás lezárva, határozat szerkesztés alatt'],
        'decided' => [100, 'green', 'Határozat elkészült, napirend lezárva'],
    ];
    if ($congress_id < 0) {
        return 'Nincs kiválasztva gyűlés';
    }
    $cItems = new CongressItems();
    $ret = '';
    $cOnline = new CongressOnline();
    $cOnline->setSelectedCongress($congress_id);
    $cPoll = new CongressPoll();
    $polls = $cPoll->getIdsOfCongress($congress_id);
    foreach ($polls as $item_id => $poll_id) {
        $ret .= '<br><<label>Napirend:</label>' . substr($cItems->getCongressItemTitleById($item_id), 0, 30) . '...';
        $ret .= '<progress value="' . $value . '" max="100" background-color="' . $background . '">' . $status . '</progress>';
    }
    return $ret;
}
