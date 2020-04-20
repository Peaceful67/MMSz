<?php

$output .= '<button name="submit_congress_close" title="A küldöttgyűlés létrehozása és a meghívók kiküldése" '
        . 'onClick="return confirm(\'Biztosan lezárod a  ' . $congressOnline->getCongressTitle() . ' című küldöttgyűlést? '
        . '\\nEzt már nem lehet visszavonni! \')"'
        . '>Kész</button>';


if (isset($submit_congress_close)) {
    if ($congressOnline->isCongressAutoItem()) {
        $title = 'Napirendi pontok és a jegyzőkönyv hitelesítők megszavazása';
        $desc = 'Ez egy automatikusan létrehozott napirendi pont. '
                . 'Kérem jelezze, aki a gyűlés többi napirendi pontjával nem ért egyett! '
                . 'A gyűlésről készülő jegyzőkönyv hitelesítői: '
                . getMemberName($congressOnline->getCongressAuth1()) . ', '
                . getMemberName($congressOnline->getCongressAuth2());
        $congressItems->addItem($congressOnline->getCongressOption());
        $congressItems->modifyItemTitle($title);
        $congressItems->modifyItemDescription($desc);
        $congressItems->insertMadatoryItemToCongress();
        logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $auto_congress_item . ' azonosítújú napirend automatikus létrehozása. ');
    }
    $congress_sum = getCongressSummerize($selected_open_congress);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójú szavzás létrehozva.');
    $filename_sum = CONGRESS_CREATE_SUM . $selected_open_congress . '.pdf';
    downloadPDF($filename_sum, $congress_sum, true);

    $creatorEmail = new Email(EMAIL_CONTEXT_DUMMY, -1);
    $creatorEmail->saveEmail('Küldöttgyűlés létrehozva', 'Tisztelt '
            . getMemberName($member_id) . '!<br><br>'
            . 'Sikeresen létrehoztál megtárgyalásra kész küldöttgyűlést.<br>'
            . 'A meghívott tagok értesítést kaptak emailben.<br>'
            . 'A gyűlés összefoglalója <a target="_blank" href="' . URL . $filename_sum . '">innen</a> letölthető.<br>'
            . 'Minden hozzászólásról email értesítést fogsz kapni !<br>'
            . 'A tárgyalást figyelemmel kisérheted a megfelelő menüpont alatt.');
    $creatorEmail->sendEmail([$member_id => '']);
    $congressOnline->closeCongress();
    $delegates = (new CongressDelegates())->getMembersOfCongress($selected_open_congress);
    $email = new Email(EMAIL_CONTEXT_CONGRESS, $selected_open_congress);
    $email->insertToBody(1, URL . $filename_sum);
    $email->insertToBody(2, getMemberName($member_id));
    $email->sendEmail($delegates);
    redirect('congress_online');
} else {
    $output .= getCongressSummerize($selected_open_congress);
}