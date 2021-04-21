<?php

if (isset($subclubs_cancel)) {
    unset($edit_subclubs);
    return;
}
$all_clubs = getClubNameList();
unset($all_clubs[$edit_subclubs]); // sajat magat ne lehessen betenni

if (isset($save_subclubs)) {
    $checked_subclubs = array();
    foreach ($all_clubs as $club_id => $club_name) {
        $var_name = 'chk_id_' . $club_id;
        if (isset($$var_name)) {
            $checked_subclubs[] = $club_id;
        }
    }
    $subclubs = serialize($checked_subclubs);
    $sql = 'UPDATE `' . CLUB_TABLE . '` SET '
            . ' `' . CLUB_SUBCLUBS . '`="' . $subclubs . '" '
            . ' WHERE `' . CLUB_ID . '`="' . $edit_club . '"';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_CLUB_EDIT, $edit_club . ' azonosítójú alszövetség tagegyesületei módosítva.');
    unset($edit_subclubs);
    return;
}
$view->putElement('<h4>Alszövetség egyesületeinek kijelölése</h4>');
$checked_subclubs = unserialize(getClubById($edit_subclubs)[CLUB_SUBCLUBS]);
if(!is_array($checked_subclubs)) {
    $checked_subclubs = array();
}
$view->submitSave('save_subclubs', 1, 'A kiválasztott egyesületek mentése');
$view->submitBack('subclubs_cancel', 'Visszatérés mentés nélkül');
$view->newKeret();
$view->newElement('<ul class="list-group">');

foreach ($all_clubs as $club_id => $club_name) {
    $checked = (in_array($club_id, $checked_subclubs)) ? 'checked' : '';
    $view->putElement('<li class="list-group-item"><label class="form-check-label" for="chk_id_' . $club_id . '">'
            . '<input id="chk_id_' . $club_id . '" type="checkbox" class="form-check-inpu" name="chk_id_' . $club_id . '"  ' . $checked . ' " value="' . $club_id . '">'
            . $club_name . '</label></li>');
}
$view->endElement('</ul>');
$view->endKeret();