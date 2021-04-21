<?php

if (isset($kk_sdn_edit)) {
    push_history(['edu_base_select', 'docu_dao_edit', 'docu_kk_edit', 'docu_temp_kk_dao_id']);
    put_new_values([
        'edu_base_select' => 4,
        'docu_sdn_new' => -1,
    ]);
    redirect('docu_edu_base');
    return;
}
$docuTempKK = new DocuTemplateKK();
$docuFile = new DocuFile();
$docuSDN = new DocuStandard();
output_prevent();
if (isset($docu_kk_save) AND isFileUploading('docu_file_upload') AND isset($docu_kk_edit) AND $docu_kk_edit > 0) {
    $kk_docu_id = $docuTempKK->getTempKKById($docu_kk_edit)[DOCU_TEMP_KK_DOCU_ID];
    $file_id = $docuFile->uploadDocument('docu_file_upload', DOCU_FILE_TYPE_PDF, $kk_docu_id);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_kk_edit . ' azonosítójú képzési sablonhoz ' . $file_id . ' azonosítójú fájl feltöltve.');
}
if (isset($docu_kk_cancel) AND isset($docu_kk_new) AND $docu_kk_cancel == $docu_kk_new) {
    $docuTempKK->deleteTempKK($docu_kk_cancel);
}
if (isset($docu_kk_cancel)) {
    unset($docu_kk_edit);
}
if (isset($docu_file_version_download)) {
    if ($docu_file_version_download > 0) {
        $docuFile->downloadFile($docu_file_version_download);
        logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, 'Képzési sablonhoz  tartozó' . $docu_file_version_download . ' azonosítójú fájl feltöltve.');
    } else {
        warning('Válassz ki egy letöltendő verziót!');
    }
}
if (isset($docu_kk_new) AND $docu_kk_new < 0) {
    $next = $docuFile->getNextDocuId();
    $docu_kk_edit = $docu_kk_new = $docuTempKK->InsertTempKK($next, $docu_dao_edit);
    output_hidden('docu_kk_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_kk_edit . ' azonosítójú képzési sablon létrehozva');
}
if (isset($docu_kk_delete)) {
    $docuTempKK->deleteTempKK($docu_kk_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_kk_delete . ' azonosítójú képzési sablon  törölve');
    unset($docu_kk_edit);
}
if (isset($docu_kk_save)) {
    if (!isset($selected_kk_sdn_id)) {
        $selected_kk_sdn_id = -1;
    }
    $docu_file = $docuTempKK->getCurrentDocuFile($docu_kk_save);
    if (!empty($docu_file)) {
        $docu_file_id = $docu_file[DOCU_FILE_ID];
        $docuFile->setItemId($docu_file_id);
        $docuFile->setDescription('');
        $docuFile->setSdnId($selected_kk_sdn_id);
    }
    $docuTempKK->updateTempKK($docu_kk_save, $selected_kk_jog_id_o, 
            ((isset($kk_pattern) AND $kk_pattern == 'on') ? 1 : 0), ((isset($kk_expire) AND $kk_expire == 'on') ? 1 : 0));
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_kk_save . ' azonosítójú képzési sablon  módosítva');
}
