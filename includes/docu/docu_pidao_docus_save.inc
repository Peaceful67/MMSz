<?php
if (isset($pidao_docus_sdn_id)) {
    push_history(['member_pidao', 'docu_pidao_edit', 'pidao_docus_edit']);
    put_new_values([
        'change_sdn_id' => $pidao_docus_sdn_id,
        'change_class' => 'DocuPIDAODocus',
        'change_field' => DOCU_PIDAO_DOCUS_DOCU_ID,
        'change_id' => $pidao_docus_edit,
    ]);
    redirect('docu_files');
    return;
}
$docuFile = new DocuFile();
if (isset($file_id_download)) {
    if ($file_id_download > 0) {
        $docuFile->downloadFile($file_id_download);
        logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, 'Képzés dokumentumhoz  tartozó' . $file_id_download . ' azonosítójú fájl feltöltve.');
    }
}
if (isset($pidao_docus_save) && isFileUploading('docu_file_upload') && isset($pidao_docus_edit) && $pidao_docus_edit > 0) {
    $docu = $pidaoDocus->getPidaoDocusById($pidao_docus_save);
    $docu_id = $docu[DOCU_PIDAO_DOCUS_DOCU_ID];
    if ($docu_id < 0) {
        $docu_id = $docuFile->getNextDocuId();
        $pidaoDocus->setDocuId($docu_id);
    }
    $file_id = $docuFile->uploadDocument('docu_file_upload', DOCU_FILE_TYPE_DOCUMENTS, $docu_id);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $pidao_docus_save . ' azonosítójú képzettség dokumentumhoz ' . $file_id . ' azonosítójú fájl feltöltve.');
}
if (isset($pidao_docus_cancel) AND isset($pidao_docus_new) AND $pidao_docus_cancel == $pidao_docus_new) {
    $pidaoDocus->deletePIDAODocus($pidao_docus_cancel);
}
if (isset($pidao_docus_cancel)) {
    unset($pidao_docus_edit);
    return;
}
if (isset($pidao_docus_new) AND $pidao_docus_new < 0) {
    $pidao_docus_edit = $pidao_docus_new = $pidaoDocus->createPidaoDocu($docu_pidao_edit, -1, -1, false, true);
    $pidaoDocus->getPIDAODocusById($pidao_docus_edit);
    output_hidden('pidao_docus_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pidao_edit . ' azonosítójú képzéshez ' . $pidao_docus_edit . ' azonosítójú dokumentum hozzáadva');
}
if (isset($pidao_docus_delete)) {
    $pidaoDocus->deletePIDAODocus($pidao_docus_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $pidao_docus_delete . ' azonosítójú képzési dokumentum törölve');
    unset($pidao_docus_edit);
}
if (isset($pidao_docus_save)) {
     if (!isset($pidao_docus_expire_date)) {
        $pidao_docus_expire_date = date('Y-m-d', strtotime($today. ' +1 year'));
    }
    $pidaoDocus->getPIDAODocusById($pidao_docus_save);
    if ($pidaoDocus->isOwn()) {
        $own_name = $pidao_docus_own_name;
        $is_expire = (isset($pidao_docus_expire) && $pidao_docus_expire == 'on');
    } else {
        $own_name = '';
        $is_expire = $pidaoDocus->isExpire();
    }
    $is_public = (isset($pidao_docus_public) && $pidao_docus_public == 'on');
    if ($is_expire) {
        $expire_date = ($pidao_docus_expire_date > 0) ? $pidao_docus_expire_date : date('Y-m-d', strtotime($today . ' +1 year'));
    } else {
        $expire_date = 0;
    }
    $pidaoDocus->updatePidaoDocu($is_expire, $expire_date, $is_public, $pidao_docus_own_name);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pidao_edit . ' azonosítójú képzéshez ' . $pidao_docus_save . ' azonosítójú dokumentum módosítva');
}
