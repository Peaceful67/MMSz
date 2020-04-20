<?php

include_once FUNCTIONS . 'vote_functions.inc';

if (!isset($selected_congress_online)) {
    $selected_congress_online = -1;
}
if (!isset($selected_congress_item)) {
    $selected_congress_item = -1;
}
if (!isset($selected_auth1)) {
    $selected_auth1 = -1;
}
if (!isset($selected_auth2)) {
    $selected_auth2 = -1;
}
if ($selected_congress_online > 0) {
    $congressOnline->setCongressEdit($selected_congress_online);
    $congressItems->setSelectedCongress($selected_congress_online);
}
if (!isset($expand_add_comment)) {
    $expand_add_comment = true;
} elseif ($expand_add_comment == "false") {
    $expand_add_comment = false;
} else {
    $expand_add_comment = true;
}
if (isset($close_poll)) {
    $congressItems->setItemClosed($selected_congress_item);
    redirect('congress_vote');
    return;
}
if (isset($edit_poll)) {
    $congressItems->setSelectedItem($selected_congress_item);
    include_once INCLUDES . 'congress_poll_edit.inc';
    if (isset($edit_poll)) {
        output_hidden('edit_poll');
        output_hidden('selected_congress_online');
        output_hidden('selected_congress_item');
        return;
    }
}
$congresses = $congressOnline->getCreatedCongressesOfDelegate() + $congressOnline->getCreatedCongressesByCreator($member_id);
if (empty($congresses)) {
    $output .= 'Nincs tárgyalandó szavazás !';
    return;
}
if (isset($warning_poll)) {
    $congressItems->setSelectedItem($selected_congress_item);
    $congressEmail = new Email(EMAIL_CONTEXT_WARNING, $selected_congress_item);
    $congressEmail->insertToBody(1, '"' . $congressItems->getCongressItemTitleById($selected_congress_item) . '"');
    $congressEmail->insertToBody(2, getMemberName($member_id));
    include_once INCLUDES . 'congress_item_warning.inc';
    if (isset($warning_poll)) {
        output_hidden('warning_poll');
        output_hidden('selected_congress_online');
        output_hidden('selected_congress_item');
        return;
    }
}
output_label('Szavazás');
if (count($congresses) == 1) {
    $selected_congress_online = key($congresses);
    $output .= $congresses[$selected_congress_online];
    output_hidden('selected_congress_online');
} else {
    output_selector($congresses, 'congress_online');
}
if ($selected_congress_online < 0) {
    return;
}
$congressOnline->setCongressEdit($selected_congress_online);
output_spaces(5);
if (isset($summerize_congress)) {
    $output .= '<button>Vissza</button>';
} else {
    $output .= '<button name="summerize_congress" title="A szavazás eredeti kiírása részletesen.">Összegzés</button>';
}
if (isset($save_item)) {
    include_once INCLUDES . 'congress_item_save.inc';
}

if (isset($add_item)) {
    $output .= '<div class="keret editor">';
    $congressItems->addItem($congressOnline->getCongressOption());
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_congress_item . ' azonosítújú napirend létrehozása. ');
    include_once INCLUDES . 'congress_item_edit.inc';
    $output .= '</div>';
}
if ($congressOnline->amICongressCreator() AND $congressOnline->isCongressAppendable()) {
    output_spaces(5);
    $output .= '<button title="Új napirend felvétele ehhez a szavazáshoz..." name="add_item">Új napirend</button>';
}

if (isset($summerize_congress)) {
    $output .= '<div class="keret">';
    $output .= getCongressSummerize($selected_congress_online);
    $output .= '</div>';
    return;
}
output_ln(2);
$congressItems->setSelectedCongress($selected_congress_online);
$items = $congressItems->getSelectionOfCongressOpen();
if (empty($items)) {
    return;
}
output_label('Napirend');
if (count($items) == 1) {
    $selected_congress_item = key($items);
    $output .= $congressItems->getCongressItemTitleById($selected_congress_item);
    output_hidden('selected_congress_item');
} else {
    output_selector($items, 'congress_item');
}

if ($selected_congress_item < 0) {
    return;
}
output_spaces(5);
if (isset($save_comment)) {
    if (strlen($congress_comment) < 5) {
        warning('Túl rövid hozzászólás');
    } else {
        $comment_id = $congressComment->addComment($congress_comment, $selected_congress_item);
        logger($member_id, -1, LOGGER_CONGRESS_COMMENT, "A " . $comment_id . " azonosítójú hozzászólás elküldve.");
        $doc_file = CONGRESS_COMMENT_DOC . $comment_id . '.pdf';
        if (isset($_FILES["congress_comment_file"]) AND ! empty($_FILES["congress_comment_file"]["tmp_name"])) {
            move_uploaded_file($_FILES["congress_comment_file"]["tmp_name"], $doc_file);
            logger($member_id, -1, LOGGER_CONGRESS_COMMENT, "A " . $comment_id . " azonosítójú hozzászóláshoz melléklet feltöltve.");
        }
    }
}
$congressItems->setSelectedItem($selected_congress_item);
if ($congressOnline->amICongressCreator()) {
    $output .= '<button name="edit_poll">Szavazás előkészítése</button>';
    $readyToVote = $congressItems->isReadyToVote();
    output_spaces(5);
    $output .= '<button name="close_poll" title="Tárgyalás lezárása és a szavazás megkezdése" ' . ($readyToVote ? "" : " disabled ") . ''
            . ' onClick="return confirm(\'Biztosan le akarod zárni a tárgyalást és megkezdeni a szavazást ?\')">'
            . 'Lezásársa</button>';
    output_spaces(5);
    $output .= '<button name="warning_poll" title="A lezárásra figyelmeztető email" ' . ($readyToVote ? "" : " disabled ") . '>Figyelmeztetés</button>';
}
$output .= '<div class="keret">';
$output .= '<b>Leírás:</b><br>' . $congressItems->getCongressItemDescription();
$pendings = $congressItems->getPendingDependencies();
if (strlen($pendings) > 0) {
    output_ln();
    output_label('<b>Ez előtt lezárandó napirendek</b>');
    o($pendings);
}
$output .= '</div><div class="keret editor">';
if ($congressItems->isItemClosed($selected_congress_item)) {
    warning('A napirendi pontot időközben lezárták');
    $output .= '</div>';
    return;
}
output_button('reload_page', 0, 'reload.jpg', 'Oldal frissítése');
output_spaces(5);
if ($expand_add_comment) {
    o('<button name="expand_add_comment" value="false" title="Csak olvasás">'
            . '<img src="' . IMAGES . 'collapse.jpg"></button>');
    output_spaces(3);
    $output .= '<b>';
    output_label('Hozzászólás');
    $output .= '</b>';
    output_ln();
    $output .= '<textarea rows="5" cols="150" maxlength="1000" name="congress_comment"></textarea>';
    output_ln();
    $output .= '<button '
            . 'onClick="return confirm(\'Biztosan beküldöd a hozzászólásod? Azt már sem törölni, sem módosítani nem lehet később.\')"'
            . 'title="Hozzászólás beküldése" name="save_comment">Küld</button>';
    output_spaces(5);
    $output .= 'Melléklet: <input type="file" name="congress_comment_file"  id="congress_comment_file"/>';
} else {
    o('<button name="expand_add_comment" value="true" title="Hozzászólás szerkesztő megnyitása"'
            . '><img src="' . IMAGES . 'expand.jpg"></button>');
}
output_ln();
$comments = $congressComment->getCommentsOfItem($selected_congress_item);
if (!empty($comments)) {
    foreach ($comments as $comment) {
        $output .= '<hr>';
        $output .= '<div class="show_avatar">';
        $mem_id = $comment[CONGRESS_COMMENTS_MEMBER_ID];
        $output .= outputAvatarOfMember($mem_id);
        $output .= '<b>' . getMemberName($mem_id) . ' : ' . date('Y-m-d H:i:s', strtotime($comment[CONGRESS_COMMENTS_TIME])) . '</b>';
        $output .= '</div>';
        $output .= $comment[CONGRESS_COMMENTS_COMMENT];
        $doc_file = CONGRESS_COMMENT_DOC . $comment[CONGRESS_COMMENTS_ID] . '.pdf';
        if (is_file($doc_file)) {
            output_ln();
            $output .= '<a target="_blank" href="' . URL . $doc_file . '">Melléklet</a>';
        }
    }
}
$output .= '</div>';
