<?php


$view->submitButton('submit_congress_close', 'Meghívók kiküldése', -1, 'A gyűlés létrehozása és a meghívók kiküldése', 'Biztosan lezárod a ' . $congressOnline->getCongressTitle() . ' című online gyűlést? '
        . '\\nEzt követően a szavazásban résztvevők megkapják a meghívókat és nincs lehetőség további módosításra');
$view->downloadPdf('save_sum', $selected_open_congress, 'Az összesítés letöltése');

(new CongressDelegates())->setMandateOfDelegates($selected_open_congress);
if (isset($save_sum) AND $save_sum == $selected_open_congress) {
    include_once FUNCTIONS . 'congress_show.inc';
    $congress_sum = getCongressSummerize($selected_open_congress);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójú gyűlés létrehozás előtt letöltve.');
    $filename_sum = CONGRESS_CREATE_SUM . $selected_open_congress . '.pdf';
    downloadPDF($filename_sum, $congress_sum, false);
}
if (isset($submit_congress_close)) {
    if ($congressOnline->isCongressAutoItem()) {
        $title = 'Napirendi pontok és a jegyzőkönyv hitelesítők megszavazása';
        $desc = 'Ez egy automatikusan létrehozott napirendi pont. A gyűlés napirendi pontjai:';
        $congressItems->setSelectedCongress($selected_open_congress);
        foreach ($congressItems->getSelectionOfCongress() as $item_title) {
            $desc .= '"' . $item_title . '", ';
        }
        $desc .= 'Kérem a napirendi pontok elfogadást!';
        if ($congressOnline->getCongressAuth1() > 0 OR $congressOnline->getCongressAuth2() > 0) {
            $desc .= 'A gyűlésről készülő jegyzőkönyv hitelesítői: '
                    . getMemberName($congressOnline->getCongressAuth1()) . ', '
                    . getMemberName($congressOnline->getCongressAuth2());
        }
        $autOptions = $congressOnline->getCongressOption() & CONGRESS_OPTIONS_INHERITED; // Ezeket vesszuk at a gyulesbol
        $auto_congress_item = $congressItems->addItem($autOptions + CONGRESS_OPTIONS_DECISION_50 + CONGRESS_OPTIONS_ABSTAIN_ENABLED + CONGRESS_OPTIONS_YES_NO); // Ezeket rakjuk hozza
        $congressItems->modifyItemTitle($title);
        $congressItems->modifyItemDescription($desc);
        $congressItems->insertMadatoryItemToCongress();
        logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $auto_congress_item . ' azonosítújú napirend automatikus létrehozása. ');
    }
    $congressOnline->createCongress();
    include_once FUNCTIONS . 'congress_show.inc';
    $congress_sum = getCongressSummerize($selected_open_congress);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójú gyűlés létrehozva.');
    $filename_sum = TMP_DIR . CONGRESS_CREATE_SUM . $selected_open_congress . '.pdf';
    downloadPDF($filename_sum, $congress_sum, true);

    $creatorEmail = new Email(EMAIL_CONTEXT_DUMMY, -1);
    $creatorEmail->saveEmail('Küldöttgyűlés létrehozva', 'Tisztelt '
            . getMemberName($member_id) . '!<br><br>'
            . 'Sikeresen létrehoztál megtárgyalásra kész küldöttgyűlést.<br>'
            . 'A meghívott tagok értesítést kaptak emailben.<br>'
            . 'A gyűlés összefoglalója mellékelve.<br>'
            . 'A tárgyalást figyelemmel kisérheted a "Online gyűlés"->"Gyűlések" menüpont alatt.');
    $creatorEmail->setFilename($filename_sum);
    $creatorEmail->sendEmail([$member_id => getMemberName($member_id)]);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójú gyűlés létrehozva.');
    $delegates = (new CongressDelegates())->getMembersOfCongress($selected_open_congress);
    $email = new Email(EMAIL_CONTEXT_CONGRESS, $selected_open_congress);
    $email->setFilename($filename_sum);
    $email->insertToBody(2, getMemberName($member_id));
    $email->sendEmail($delegates);
    $view->success('<h3>A gyűlés sikeresen létrehozva.</h3>');
} else {
    include_once FUNCTIONS . 'congress_show.inc';
    $view->put(getCongressSummerize($selected_open_congress));
}