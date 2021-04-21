<?php

$docu_kk = $docuTempKK->getTempKKById($docu_kk_edit);
$selected_kk_dao_id = $docu_kk[DOCU_TEMP_KK_DAO_ID];
$selected_kk_jog_id_o = $docu_kk[DOCU_TEMP_KK_JOG_ID_O];
$kk_docu_id = $docu_kk[DOCU_TEMP_KK_DOCU_ID];
$kk_pattern = $docu_kk[DOCU_TEMP_KK_PATTERN] > 0 ? 'on' : 'off';
o('<div class="keret editor">');
o('<h3>Képzés dokumentumának kezelése</h3>');
output_label('Dokumentum', true);
$docu_files = $docuFile->getFilesByDocuId($kk_docu_id);
if (empty($docu_files)) {
    o('Még nincs feltöltve dokumentum');
} else {
    if (isset($changed_docu_kk_edit) AND $changed_docu_kk_edit > 0 AND $changed_docu_kk_edit != $docu_kk_edit) {
        unset($selected_kk_file_id);
    }
    if (!isset($selected_kk_file_id)) {
        end($docu_files);
        $selected_kk_file_id = key($docu_files);
    }
    $docu_file_versions = array();
    foreach ($docu_files as $docu_file) {
        $docu_name = $docuFile->getNameByFileId($docu_file[DOCU_FILE_ID]);
        $docu_file_versions[$docu_file[DOCU_FILE_ID]] = $docu_name . ' (' . $docu_file[DOCU_FILE_UPLOADED] . ')';
    }
    output_chooser($docu_file_versions, 'kk_file_id', true);
}
output_ln();
output_label('Elnevezés kiválasztása');
if (!isset($selected_kk_sdn_id)) {
    if (isset($selected_kk_file_id) AND $docu_files[$selected_kk_file_id]) {
        $selected_kk_sdn_id = $docu_files[$selected_kk_file_id][DOCU_FILE_SDN_ID];
    } else {
        $selected_kk_sdn_id = -1;
    }
}
output_selector($docuSDN->getNamesOfType(DOCU_SDN_TYPE_KK), 'kk_sdn_id', false);
output_spaces(5);
output_button('kk_sdn_edit', -1, 'edit.jpg', 'Elnevezések hozzáadása, szerkesztése');
output_ln();
output_label('Oktatói képzettség', false);
output_selector($docuJOG->getTrainerNames(), 'kk_jog_id_o', false);
output_ln();
output_label('Típus', false);
output_switch_checkbox('kk_pattern', DOCU_PATTERN, DOCU_NO_PATTERN);
output_ln();
output_label('Lejárat', false);
output_switch_checkbox('kk_expire', DOCU_EXPIRES, DOCU_NOT_EXPIRES);
output_ln(2);
output_cancel('docu_kk_cancel', $docu_kk_edit);
output_spaces(5);
output_save('docu_kk_save', $docu_kk_edit);
output_spaces(5);
$_SESSION[SESSION_VIEW_DOCUMENT_ID] = "";
if (empty($docu_files)) {
    o('Feltöltés:');
    output_spaces(5);
} else {
    if ($selected_kk_file_id > 0) {
        o('<button name="docu_file_version_download" value="' . $selected_kk_file_id . '" onclick="saved=true;" title="Dokumentum letöltése"><img src="' . IMAGES . 'download.png"></button>');
        output_spaces(5);
        $_SESSION[SESSION_VIEW_DOCUMENT_ID] = $selected_kk_file_id;
        o('<button name="docu_file_version_view" value="-1" onclick="saved=true;window.open(\'?view_document\');" title="Dokumentum megtekintése"><img src="' . IMAGES . 'view.jpg"></button>');
    }

    output_spaces(5);
    o('Módosítás:');
    output_spaces(5);
}
o('<input name="docu_file_upload" id="docu_file_upload" type="file" />');
o('</div>');



