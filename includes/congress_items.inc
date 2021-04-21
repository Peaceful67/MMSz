<?php

if (!isset($selected_open_congress) OR $selected_open_congress <= 0) {
    return;
}
if (!isset($selected_congress_item)) {
    $selected_congress_item = -1;
}
if (!isset($old_selected_congress_item)) {
    $old_selected_congress_item = $selected_congress_item;
}
$doc_file = CONGRESS_ITEM_DOC . $selected_congress_item . '.pdf';
if (isset($add_item)) {
    $selected_congress_item = $congressItems->addItem($congressOnline->getCongressOption() & CONGRESS_OPTIONS_INHERITED);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_congress_item . ' azonosítújú napirend létrehozása. ');
}
if (isset($delete_item_doc)) {
    unlink($doc_file);
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, "A " . $selected_congress_item . " azonosítójú napirendhez melléklet törlése.");
}
$view->putPrevent();
$congressItems->setSelectedItem($selected_congress_item);
if ($selected_congress_item > 0 AND $old_selected_congress_item == $selected_congress_item) {
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

$view->putElement('<h4>Napirendek szerkesztése</h4>');
$items = $congressItems->getSelectionOfCongress();
if (empty($items)) {
    $view->submitAdd('add_item', -1, 'Új napirendi pont létrehozása');
} elseif (count($items) == 1) {
    $selected_congress_item = key($items);
    $view->putText(empty($items[$selected_congress_item])?'Még nincs megadva cím !':$items[$selected_congress_item], 'Napirendi pont');
    if (strlen($items[$selected_congress_item]) >= 5) {
        $view->submitAdd('add_item', -1, 'Új napirendi pont létrehozása');
    }
    $view->putHidden('selected_congress_item');
} else {
    if ($selected_congress_item < 0 OR strlen($items[$selected_congress_item]) >= 5) {
        $view->submitDropDownChooser('Napirendi pontok', [-1=> 'Válassz!!!'] + $items, 'selected_congress_item', true);
        $view->submitAdd('add_item', -1, 'Új napirendi pont létrehozása');
    } else {
        $view->putHidden('selected_congress_item');
    }
}
if ($selected_congress_item > 0) {

    $congressItems->setSelectedItem($selected_congress_item);
    $view->submitDelete('delete_item', -1, 'Napirendi pont törlése', 'Biztosan törlöd a ' . $congressItems->getCongressItemTitle() . ' című napirendi pontot?');
    include_once INCLUDES . 'congress_item_edit.inc';
}
$old_selected_congress_item = $selected_congress_item;
$view->putHidden('old_selected_congress_item');

