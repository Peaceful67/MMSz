<?php

$docuPD = new DocuTemplatePD();
$docuJOG = new DocuJOG();
$docuSDN = new DocuStandard();
$docuFile = new DocuFile();

o('<h3>Pilóta dokumentumok sablonjainak kezelése</h3>');
if (isset($docu_upload) AND isset($docu_kk_edit) AND $docu_kk_edit > 0) {
    $pd_docu_id = $docuPD->getTempPDById($docu_pd_edit)[DOCU_TEMP_PD_DOCU_ID];
    $docuFile->uploadDocument('docu_file', DOCU_FILE_TYPE_PDF, $pd_docu_id);
}
if (isset($docu_pc_cancel)) {
    unset($docu_pd_edit);
}
if (isset($docu_file_version_download) AND isset($selected_docu_file_version)) {
    if ($selected_docu_file_version > 0) {
        $docuFile->downloadFile($selected_docu_file_version);
    } else {
        warning('Válassz ki egy letöltendő verziót!');
    }
}
if (isset($docu_pd_new)) {
    $next = $docuFile->getNextDocuId();
    $docu_pd_edit = $docuTempPD->InsertTempPD($next);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_edit . ' azonosítójú pilóta sablon létrehozva');
}
if (isset($docu_pd_delete)) {
    $docuTempPD->deleteTempPD($docu_pd_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_delete . ' azonosítójú pilóta sablon  törölve');
    unset($docu_pd_edit);
}
if (isset($docu_pd_save)) {
    if (!isset($pd_docu_id)) {
        $pd_docu_id = -1;
    }
    $docu_file = $docuTempPD->getCurrentDocuFile($docu_pd_save);
    if(!empty($docu_file)) {
        $docu_file_id = $docu_file[DOCU_FILE_ID];
        $docuFile->setItemId($docu_file_id)->setDescription($docu_kk_desc);
    }
    $docuTempKK->updateTempKK($docu_kk_save, $kk_name, $selected_kk_dao_id, $selected_kk_jog_id, $selected_kk_sdn_id, $selected_kk_trainer_id, ((isset($kk_active) AND $kk_active == 'on') ? 1 : 0));
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_kk_save . ' azonosítójú pilóta sablon  módosítva');
    unset($docu_kk_edit);
}
if (isset($docu_kk_edit) AND $docu_kk_edit > 0) {
    $docu_kk = $docuTempKK->getTempKKById($docu_kk_edit);
    $selected_kk_dao_id = $docu_kk[DOCU_TEMP_KK_DAO_ID];
    $selected_kk_sdn_id = $docu_kk[DOCU_TEMP_KK_SDN_ID];
    $selected_kk_jog_id = $docu_kk[DOCU_TEMP_KK_JOG_ID];
    $kk_docu_id = $docu_kk[DOCU_TEMP_KK_DOCU_ID];
    $kk_active = $docu_kk[DOCU_TEMP_KK_ACTIVE] > 0 ? 'on' : 'off';
    if ($selected_kk_jog_id < 0 OR $selected_kk_sdn_id < 0) {
        $kk_active = 'disabled';
    }
    o('<div class="keret editor">');
    output_label('Neve', true);
    o('<input type="text" name="kk_name" value="' . $docu_kk[DOCU_TEMP_KK_NAME] . '"/>');
    output_ln();
    output_label('Képzés', true);
    output_selector($docuDAO->getNames(), 'kk_dao_id', false);
    output_ln();
    output_label('Képzettség', true);
    output_selector($docuJOG->getNames(), 'kk_jog_id', false);
    output_ln();
    output_label('Standard dokumentum', true);
    output_selector($docuSDN->getNames(), 'kk_sdn_id', false);
    output_ln();
    output_label('Oktatási képzettség', true);
    output_selector($docuJOG->getTrainerNames(), 'kk_trainer_id', false);
    output_ln();
    output_label('Aktív', true);
    output_switch_checkbox('kk_active', 'Igen', 'Nem');
    output_ln();
    output_label('Dokumentum leírása', true);
    $docu_files = $docuFile->getFilesByDocuId($kk_docu_id);
    if (empty($docu_files)) {
        o('Még nincs feltöltve dokumentum');
    } else {
        $docu_file = end($docu_files);
        o('<input type="text" name="docu_kk_desc" value="' . $docu_file[DOCU_FILE_DESCRIPTION] . '">');
        output_spaces(5);
        o('Feltöltve: ' . $docu_file[DOCU_FILE_UPLOADED]);
        output_ln();
        output_label('Letöltés');
        $docu_file_versions = array();
        foreach ($docu_files as $docu_file) {
            $docu_file_versions[$docu_file[DOCU_FILE_ID]] = '(' . $docu_file[DOCU_FILE_VERSION] . ') ' . $docu_file[DOCU_FILE_UPLOADED] . ' / ' . $docu_file[DOCU_FILE_DESCRIPTION];
        }
        output_selector($docu_file_versions, 'docu_file_version', false);
        output_spaces(3);
        output_button('docu_file_version_download', $selected_docu_file_version, 'download.png', 'A kiválasztott verzió letöltése');
        output_spaces(3);
 //       o('<button aname="docu_file_version_download" value="'.$selected_docu_file_version.'"><img src="'.IMAGES.'view.jpg"></button>');
    }
    output_ln();
    output_label('Feltöltendő fájl');
    o('<input name="docu_file" type="file"/>');
    o('<input type="submit" name="docu_upload" value="Feltöltés" />');
    output_ln(2);
    output_button('docu_kk_save', $docu_kk_edit, 'save.jpg', 'Módosítások mentése');
    output_spaces(5);
    output_button('docu_kk_cancel', -1, 'back.jpg', 'Szerkesztés elvetése');
    o('</div>');
    output_hidden('docu_kk_edit');
}
output_ln(2);
output_button('docu_kk_new', -1, 'add.jpg', 'Új képzési/képzettségi sablon létrehozása');
$kks = $docuTempKK->getItems();
if (empty($kks)) {
    return;
}
o('<div class="lists">');
o('<table><tr><th>Neve</th><th>Képzés</th><th>Képzettség</th><th>Standard dokumentum</th><th>Feltöltött dokumentum</th><th>Oktatói képzettség</th><th>Aktív</th><th>Művelet</th></tr>');
foreach ($kks as $kk_id => $kk) {
    $dao_id = $kk[DOCU_TEMP_KK_DAO_ID];
    $jog_id = $kk[DOCU_TEMP_KK_JOG_ID];
    $sdn_id = $kk[DOCU_TEMP_KK_SDN_ID];
    $trainer_id = $kk[DOCU_TEMP_KK_JOG_ID_O];
    $dao_name = ($dao_id > 0) ? $docuDAO->getDAOById($dao_id)[DOCU_ORG_NAME] : 'Nincs meghatározva';
    $jog_name = ($jog_id > 0) ? $docuJOG->getJOGById($jog_id)[DOCU_JOG_NAME] : 'Nincs meghatározva';
    $sdn_name = ($sdn_id > 0) ? $docuSDN->getSDNById($sdn_id)[DOCU_SDN_SHORT] : 'Nincs meghatározva';
    $trainer_name = ($trainer_id > 0) ? $docuJOG->getJOGById($trainer_id)[DOCU_JOG_NAME] : 'Nincs meghatározva';
    $docu_files = $docuFile->getFilesByDocuId($kk[DOCU_TEMP_KK_DOCU_ID]);
    if (empty($docu_files)) {
        $docu_name = 'Nincs feltöltve';
    } else {
        $docu_file = end($docu_files);
        $docu_name = empty($docu_file[DOCU_FILE_DESCRIPTION]) ? $docu_file[DOCU_FILE_ORIGIN] : $docu_file[DOCU_FILE_DESCRIPTION];
    }
    o('<tr><td>' . $kk[DOCU_TEMP_KK_NAME] . '</td>');
    o('<td>' . $dao_name . '</td>');
    o('<td>' . $jog_name . '</td>');
    o('<td>' . $sdn_name . '</td>');
    o('<td>' . $docu_name . '</td>');
    o('<td>' . $trainer_name . '</td>');
    o('<td>' . ($kk[DOCU_TEMP_KK_ACTIVE] > 0 ? 'Igen' : 'Nem') . '</td>');
    o('<td>');
    output_button('docu_kk_edit', $kk_id, 'edit.jpg', 'Képzési/képzettségi sablon szerkesztése');
    if ($docuDAO->isDeletable($kk_id)) {
        output_button('docu_kk_delete', $kk_id, 'delete.jpg', 'Képzési/képzettségi sablon törlése', 'Biztosan törölni akarja a sablont?');
    }
    o('</td></tr>');
}
o('</table></div>');


