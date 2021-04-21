<?php

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

