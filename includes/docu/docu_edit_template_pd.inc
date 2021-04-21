<?php

if (isset($pd_sdn_edit)) {
    push_history(['template_select', 'docu_pd_edit']);
    put_new_values([
        'edu_base_select' => 4,
        'docu_sdn_new' => -1,
    ]);
    redirect('docu_edu_base');
    return;
}
$docuPD = new DocuTemplatePD();
$docuJOG = new DocuJOG();
$docuFile = new DocuFile();
$docuSDN = new DocuStandard();
output_prevent();
o('<h3>Pilóta dokumentumok sablonjainak kezelése</h3>');
$docu_pd = $docuPD->getTempPDById($docu_pd_edit);
$selected_pd_sdn_id = $docu_pd[DOCU_TEMP_PD_SDN_ID];
$pd_docu_id = $docu_pd[DOCU_TEMP_PD_DOCU_ID];
$pd_active = $docu_pd[DOCU_TEMP_PD_EXPIRE] > 0 ? 'on' : 'off';
$pd_public = $docu_pd[DOCU_TEMP_PD_PUBLIC] > 0 ? 'on' : 'off';

o('<div class="keret editor">');
output_label('Dokumentum', true);
$docu_files = $docuFile->getFilesByDocuId($pd_docu_id);
if (empty($docu_files)) {
    o('Még nincs feltöltve dokumentum');
    $selected_pd_file_id = -1;
} else {
    if(isset($changed_docu_pd_edit) AND $changed_docu_pd_edit > 0 AND $changed_docu_pd_edit != $docu_pd_edit) {
        unset($selected_pd_file_id);
    }
    if (!isset($selected_pd_file_id)) {
        end($docu_files);
        $selected_pd_file_id = key($docu_files);
    }
    $docu_file_versions = array();
    foreach ($docu_files as $docu_file) {
        $docu_name = $docuFile->getNameByFileId($docu_file[DOCU_FILE_ID]);
        $docu_file_versions[$docu_file[DOCU_FILE_ID]] = $docu_name . ' (' . $docu_file[DOCU_FILE_UPLOADED] . ')';
    }
    output_chooser($docu_file_versions, 'pd_file_id', true);
}
output_ln();
output_label('Dokumentum neve', true);
output_selector($docuSDN->getNamesOfType(DOCU_SDN_TYPE_PD), 'pd_sdn_id', false);
output_spaces(5);
output_button('pd_sdn_edit', -1, 'edit.jpg', 'Elnevezések hozzáadása, szerkesztése');
output_ln();
output_label('Lejárat', true);
output_switch_checkbox('pd_expire', DOCU_EXPIRES, DOCU_NOT_EXPIRES);
output_ln();
output_label('Nyilvános', true);
output_switch_checkbox('pd_public', 'Igen', 'Nem');
output_ln(2);
output_cancel('docu_pd_cancel', $docu_pd_edit);
output_spaces(5);
output_save('docu_pd_save', $docu_pd_edit);
output_spaces(5);
$_SESSION[SESSION_VIEW_DOCUMENT_ID] = "";
if (empty($docu_files)) {
    o('Feltöltés:');
    output_spaces(5);
} else {
    if ($selected_pd_file_id > 0) {
        o('<button name="docu_file_version_download" value="' . $selected_pd_file_id . '" onclick="saved=true;" title="Dokumentum letöltése"><img src="' . IMAGES . 'download.png"></button>');
        output_spaces(5);
        $_SESSION[SESSION_VIEW_DOCUMENT_ID] = $selected_pd_file_id;
        o('<button name="docu_file_version_view" value="-1" onclick="saved=true;window.open(\'?view_document\');" title="Dokumentum megtekintése"><img src="' . IMAGES . 'view.jpg"></button>');
        output_spaces(5);
        o('Módosítás:');
        output_spaces(5);
    }
}
o('<input name="docu_file_upload" id="docu_file_upload" type="file" />');
o('</div>');



o('</div>');


