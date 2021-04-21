<?php

$docuTempPD = new DocuTemplatePD();
$docuJOG = new DocuJOG();
$docuFile = new DocuFile();
$docuSDN = new DocuStandard();
output_prevent();
if (isset($docu_pd_save) AND isFileUploading('docu_file_upload') AND isset($docu_pd_edit) AND $docu_pd_edit > 0) {
    $pd_docu_id = $docuTempPD->getTempPDById($docu_pd_edit)[DOCU_TEMP_PD_DOCU_ID];
    $file_id = $docuFile->uploadDocument('docu_file_upload', DOCU_FILE_TYPE_PDF, $pd_docu_id);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_edit . ' azonosítójú képzettségi  sablonhoz ' . $file_id . ' azonosítójú fájl feltöltve.');
}
if (isset($docu_pd_cancel) AND isset($docu_pd_new) AND $docu_pd_cancel == $docu_pd_new) {
    $docuTempPD->deleteTempPD($docu_pd_cancel);
}
if (isset($docu_pd_cancel)) {
    unset($docu_pd_edit);
}
if (isset($docu_file_version_download)) {
    if ($docu_file_version_download > 0) {
        $docuFile->downloadFile($docu_file_version_download);
        logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, 'Képzettségi sablonhoz  tartozó' . $docu_file_version_download . ' azonosítójú fájl feltöltve.');
    } else {
        warning('Válassz ki egy letöltendő verziót!');
    }
}
if (isset($docu_pd_new) AND $docu_pd_new < 0) {
    $next = $docuFile->getNextDocuId();
    $docu_pd_edit = $docu_pd_new = $docuTempPD->InsertTempPD($next, $docu_jog_edit);
    output_hidden('docu_pd_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_edit . ' azonosítójú képzettségi sablon létrehozva');
}
if (isset($docu_pd_delete)) {
    $docuTempPD->deleteTempPD($docu_pd_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_delete . ' azonosítójú képzettségi sablon  törölve');
    unset($docu_pd_edit);
}
if (isset($docu_pd_save)) {
    if (!isset($selected_pd_docu_id)) {
        $selected_pd_docu_id = -1;
    }
    if (!isset($selected_pd_sdn_id)) {
        $selected_pd_sdn_id = -1;
    }
    $docu_file = $docuTempPD->getCurrentDocuFile($docu_pd_save);
    if (!empty($docu_file)) {
        $docu_file_id = $docu_file[DOCU_FILE_ID];
        $docuFile->setItemId($docu_file_id);
        $docuFile->setDescription('');
        $docuFile->setSdnId($selected_pd_sdn_id);
    }
    $docuTempPD->updateTempPD($docu_pd_save, $selected_pd_sdn_id, ((isset($pd_expire) AND $pd_expire == 'on') ? 1 : 0), ((isset($pd_public) AND $pd_public == 'on') ? 1 : 0));
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_save . ' azonosítójú képzettségi sablon  módosítva');
}
