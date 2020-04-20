<?php

if (!isset($selected_open_congress) OR $selected_open_congress <= 0) {
    return;
}
if (!isset($selected_congress_item)) {
    $selected_congress_item = -1;
}
$doc_file = CONGRESS_ITEM_DOC . $selected_congress_item . '.pdf';
if (isset($add_item)) {
    $selected_congress_item = $congressItems->addItem($congressOnline->getCongressOption());
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_congress_item . ' azonosítújú napirend létrehozása. ');
}
if (isset($delete_item_doc)) {
    unlink($doc_file);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, "A " . $selected_congress_item . " azonosítójú napirendhez melléklet törlése.");
}

$congressItems->setSelectedItem($selected_congress_item);
if (isset($save_item)) {
    include_once INCLUDES . 'congress_item_save.inc';
}
if (isset($delete_item)) {
    unlink(CONGRESS_ITEM_DOC . $selected_congress_item . '.pdf');
    $congressItems->deleteCongressItem();
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_congress_item . ' azonosítújú napirend törlése. ');
    $selected_congress_item = -1;
}
$item_edit = $congressItems->getCongressItem();
if (isset($item_edit[CONGRESS_ITEMS_TITLE])) {
    $item_title = $item_edit[CONGRESS_ITEMS_TITLE];
} else {
    $item_title = '';
}
if (isset($item_edit[CONGRESS_ITEMS_DESCRIPTION])) {
    $item_description = $item_edit[CONGRESS_ITEMS_DESCRIPTION];
} else {
    $item_description = '';
}

$output .= '<div class="editor">';
$output .= '<h3>Napirendi pontok szerkesztése</h3>';
output_label('Napirendi pontok');
output_selector($congressItems->getSelectionOfCongress(), 'congress_item');
output_spaces(5);
$output .= '<button title="Új napirendi pont létrehozása" name="add_item"><img src="' . IMAGES . 'add.jpg"></button>';
if ($selected_congress_item > 0) {
    include_once INCLUDES . 'congress_item_edit.inc';
    output_spaces(5);
    $output .= '<button title="Napirendi pont törlése" name="delete_item"'
            .'onClick="return confirm(\'Biztosan törlöd a napirendi pontot? \')"'
            . '><img src="' . IMAGES . 'delete.jpg"></button>';
}
$output .= '</div>';

