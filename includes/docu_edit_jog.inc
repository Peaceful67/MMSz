<?php

o('<h3>Képzettségek szerkesztése</h3>');
$docuJOG = new DocuJOG();
output_prevent();
if (isset($docu_jog_new) AND $docu_jog_new < 0) {
    $docu_jog_edit = $docu_jog_new = $docuJOG->InsertJOG();
    output_not_saved();
    output_hidden('docu_jog_new');
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_jog_edit . ' azonosítójú képzettség létrehozva');
}
if (isset($docu_jog_delete)) {
    $docuJOG->deleteJOG($docu_jog_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_jog_delete . ' azonosítójú képzettség törölve');
}
if (isset($docu_jog_new) AND isset($docu_jog_cancel) AND $docu_jog_new == $docu_jog_cancel) {
    $docuJOG->deleteJOG($docu_jog_cancel);
}
if (isset($docu_jog_save)) {
    $docuJOG->updateJOG($docu_jog_save, $docu_jog_name, ((isset($docu_jog_is_trainer) AND $docu_jog_is_trainer == 'on') ? 1 : 0), $selected_docu_jog_lower_id);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_jog_save . ' azonosítójú képzettség módosítva');
}
if (isset($docu_jog_edit) AND $docu_jog_edit > 0) {
    $docu_jog = $docuJOG->getJOGById($docu_jog_edit);
    $selected_docu_jog_lower_id = $docu_jog[DOCU_JOG_LOWER];
    $docu_jog_is_trainer = $docu_jog[DOCU_JOG_IS_TRAINER] > 0 ? 'on' : 'off';
    o('<div class="keret editor">');
    output_label('Kézettség neve', true);
    o('<input type="text"  onchange="saved=false;" name="docu_jog_name" value="' . $docu_jog[DOCU_JOG_NAME] . '"/>');
    output_ln();
    output_label('Oktatói képzettség', true);
    output_switch_checkbox('docu_jog_is_trainer', 'Igen', 'Nem');
    output_ln();
    output_label('Előző szint', true);
    $docu_jog_names = $docuJOG->getNames();
    unset($docu_jog_names[$docu_jog_edit]);
    output_selector($docu_jog_names, 'docu_jog_lower_id', false, 'Legalsó');
    output_ln(2);
    output_save('docu_jog_save', $docu_jog_edit);
    output_spaces(5);
    output_cancel('docu_jog_cancel', $docu_jog_edit);
    o('</div>');
}
output_ln(2);
o('<div class="lists">');
output_button('docu_jog_new', -1, 'add.jpg', 'Új képzettség létrehozása');
output_spaces(5);
output_back_history();
$jogs = $docuJOG->getItems();
if (empty($jogs)) {
    o('</div>');
    return;
}
o('<table><tr><th>Neve</th><th>Oktatói képzettség</th><th>Előző képzettség</th><th>Művelet</th></tr>');
foreach ($jogs as $jog_id => $jog) {
    $lower_id = $jog[DOCU_JOG_LOWER];
    $lower_name = ($lower_id > 0) ? $docuJOG->getJOGById($lower_id)[DOCU_JOG_NAME] : 'Legalsó';
    o('<tr><td>' . $jog[DOCU_JOG_NAME] . '</td>');
    o('<td>' . ($jog[DOCU_JOG_IS_TRAINER] > 0 ? 'Igen' : 'Nem') . '</td>');
    o('<td>' . $lower_name . '</td>');
    o('<td>');
    output_button('docu_jog_edit', $jog_id, 'edit.jpg', 'Képzettség szerkesztése');
    if ($docuJOG->isDeletable($jog_id)) {
        output_button('docu_jog_delete', $jog_id, 'delete.jpg', 'Képzettség törlése', 'Biztosan törölni akarja a képzettséget?');
    }
    o('</td></tr>');
}
o('</table></div>');

