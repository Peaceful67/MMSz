<?php

$docuFile = new DocuFile();
if (isset($file_id_download)) {
    if ($file_id_download > 0) {
        $docuFile->downloadFile($file_id_download);
        logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, 'Képzettség dokumentumhoz  tartozó' . $file_id_download . ' azonosítójú fájl feltöltve.');
    }
}
if (isset($pidjog_docus_save) && isFileUploading('docu_file_upload') && isset($pidjog_docus_edit) && $pidjog_docus_edit > 0) {
    $pidjogDocus->getPIDJOGDocusById($pidjog_docus_save);
    $docu_id = $pidjogDocus->getDocuId();
    if ($docu_id < 0) {
        $docu_id = $docuFile->getNextDocuId();
        $pidjogDocus->setDocuId($docu_id);
    }
    $file_id = $docuFile->uploadDocument('docu_file_upload', DOCU_FILE_TYPE_DOCUMENTS, $docu_id);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $pidjog_docus_save . ' azonosítójú képzettség dokumentumhoz ' . $file_id . ' azonosítójú fájl feltöltve.');
}
if (isset($pidjog_docus_cancel) AND isset($pidjog_docus_new) AND $pidjog_docus_cancel == $pidjog_docus_new) {
    $pidjogDocus->deletePIDJOGDocus($pidjog_docus_cancel);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $pidjog_docus_cancel . ' azonosítójú képzettségi dokumentum törölve');
}
if (isset($pidjog_docus_cancel)) {
    unset($pidjog_docus_edit);
    return;
}
if (isset($pidjog_docus_new) AND $pidjog_docus_new < 0) {
    $pidjog_docus_edit = $pidjog_docus_new = $pidjogDocus->createPidjogDocu($docu_pidjog_edit, -1, -1, -1, false, true);
    $pidjogDocus->getPIDJOGDocusById($pidjog_docus_new);
    output_hidden('pidjog_docus_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pidjog_edit . ' azonosítójú képzettséghez ' . $pidjog_docus_edit . ' azonosítójú dokumentum hozzáadva');
}
if (isset($pidjog_docus_delete)) {
    $pidjogDocus->deletePIDJOGDocus($pidjog_docus_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $pidjog_docus_delete . ' azonosítójú képzettségi dokumentum törölve');
    unset($pidjog_docus_edit);
}

if (isset($pidjog_docus_save)) {
    if (!isset($pidjog_docus_expire_date)) {
        $pidjog_docus_expire_date = date('Y-m-d', strtotime($today. ' +1 year'));
    }
    $pidjogDocus->getPIDJOGDocusById($pidjog_docus_save);
    if ($pidjogDocus->isOwn()) {
        $own_name = $pidjog_docus_own_name;
        $is_expire = (isset($pidjog_docus_expire) && $pidjog_docus_expire == 'on');
    } else {
        $own_name = '';
        $is_expire = $pidjogDocus->isExpire();
    }
    $is_public = (isset($pidjog_docus_public) && $pidjog_docus_public == 'on');
    if ($is_expire) {
        $expire_date = ($pidjog_docus_expire_date > 0) ? $pidjog_docus_expire_date : date('Y-m-d', strtotime($today . ' +1 year'));
    } else {
        $expire_date = 0;
    }

    $pidjogDocus->updateDocu($pidjog_docus_save, $is_expire, $expire_date, $is_public, $own_name);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pidjog_edit . ' azonosítójú képzettséghez ' . $pidjog_docus_save . ' azonosítójú dokumentum módosítva');
}