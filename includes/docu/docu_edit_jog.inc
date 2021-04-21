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
if (isset($docu_jog_cancel)) {
    unset($docu_jog_edit);
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
    output_label('Képzettség neve', true);
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
    output_post_onchange('docu_pd_edit');
    include_once DOCU_INCLUDES . 'docu_save_template_pd.inc';
    if (isset($docu_pd_edit) AND $docu_pd_edit > 0) {
        include_once DOCU_INCLUDES . 'docu_edit_template_pd.inc';
        output_hidden('docu_pd_edit');
    }
    include_once DOCU_INCLUDES . 'docu_list_template_pd.inc';
    output_hidden('docu_jog_edit');
} else {
    include_once DOCU_INCLUDES . 'docu_list_jog.inc';
}

