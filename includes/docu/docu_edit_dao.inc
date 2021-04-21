<?php

if (isset($docu_dao_temp_kk_edit) AND $docu_dao_temp_kk_edit > 0) {
    $docu_dao_edit = $docu_dao_temp_kk_edit;
    push_history(['edu_base_select', 'docu_dao_edit']);
    put_new_values([
        'edu_base_select' => 20,
        'docu_temp_kk_dao_id' => $docu_dao_temp_kk_edit,
    ]);
    redirect('docu_edu_base');
}
o('<h3>Képzések szerkesztése</h3>');
$docuDAO = new DocuDAO();
$docuOrg = new DocuOrganizations();
$docuSDN = new DocuStandard();
$docuJOG = new DocuJOG();
$docuFile = new DocuFile();
$docuTempKK = new DocuTemplateKK();
output_prevent();
if (isset($docu_dao_new) AND $docu_dao_new < 0) {
    $docu_dao_edit = $docu_dao_new = $docuDAO->InsertDAO();
    output_hidden('docu_dao_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_dao_edit . ' azonosítójú képzés létrehozva');
}
if (isset($docu_dao_new) AND isset($docu_dao_cancel) AND $docu_dao_new == $docu_dao_cancel) {
    $docuDAO->deleteDAO($docu_dao_cancel);
}
if (isset($docu_dao_cancel)) {
    unset($docu_dao_edit);
}
if (isset($docu_dao_delete)) {
    $docuDAO->deleteDAO($docu_dao_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_dao_delete . ' azonosítójú képzés törölve');
}
if (isset($docu_dao_save)) {
    $docuDAO->updateDAO($docu_dao_save, $selected_docu_dao_org_id, $selected_docu_dao_sdn_id, $selected_docu_dao_jog_id_o, $selected_docu_dao_jog_id_t, ((isset($docu_dao_active) AND $docu_dao_active == 'on') ? 1 : 0));
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_dao_save . ' azonosítójú képzés módosítva');
}
if (isset($docu_dao_edit) AND $docu_dao_edit > 0) {
    $docu_dao = $docuDAO->getDAOById($docu_dao_edit);
    $selected_docu_dao_org_id = $docu_dao[DOCU_DAO_ORG_ID];
    $selected_docu_dao_sdn_id = $docu_dao[DOCU_DAO_SDN_ID];
    $selected_docu_dao_jog_id_o = $docu_dao[DOCU_DAO_JOG_ID_O];
    $selected_docu_dao_jog_id_t = $docu_dao[DOCU_DAO_JOG_ID_T];
    $docu_dao_active = $docu_dao[DOCU_DAO_ACTIVE] > 0 ? 'on' : 'off';
    o('<div class="keret editor">');
    $label_width = 200;
    output_label('Szervezet', true, false, $label_width);
    output_selector($docuOrg->getEduNames(), 'docu_dao_org_id', false);
    output_ln();
    output_label('Szerezhető képzettség', true, false, $label_width);
    output_selector($docuJOG->getNames(), 'docu_dao_jog_id_t', false);
    output_ln();
    output_label('Képzés neve', true, false, $label_width);
    output_selector($docuSDN->getNamesOfType(DOCU_SDN_TYPE_DAO), 'docu_dao_sdn_id', false);
    output_ln();
    output_label('Szükséges oktatói képzettség', true, false, $label_width);
    output_selector($docuJOG->getTrainerNames(), 'docu_dao_jog_id_o', false);
    output_ln();
    output_label('Aktív', true, false, $label_width);
    output_switch_checkbox('docu_dao_active', 'Igen', 'Nem');
    output_ln(2);
    output_cancel('docu_dao_cancel', $docu_dao_edit);
    output_spaces(5);
    output_save('docu_dao_save', $docu_dao_edit);
    o('</div>');
    output_post_onchange('docu_kk_edit');
    include_once DOCU_INCLUDES . 'docu_save_template_kk.inc';
    if (isset($docu_kk_edit) AND $docu_kk_edit > 0) {
        include_once DOCU_INCLUDES . 'docu_edit_template_kk.inc';
        output_hidden('docu_kk_edit');
    }
    include_once DOCU_INCLUDES . 'docu_list_template_kk.inc';
    output_hidden('docu_dao_edit');
} else {
    include_once DOCU_INCLUDES . 'docu_list_dao.inc';
}

