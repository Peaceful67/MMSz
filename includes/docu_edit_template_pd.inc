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
if (isset($docu_upload) AND isset($docu_pd_edit) AND $docu_pd_edit > 0) {
    $pd_docu_id = $docuPD->getTempPDById($docu_pd_edit)[DOCU_TEMP_PD_DOCU_ID];
    $docuFile->uploadDocument('docu_file', DOCU_FILE_TYPE_PDF, $pd_docu_id);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_edit . ' azonosítójú pilóta dokumentum sablonhoz '.$pd_docu_id.' azonosítójú fájl feltöltve.');
    unset($selected_pd_docu_id);
}
if (isset($docu_pd_cancel) AND isset($docu_pd_new) AND $docu_pd_cancel == $docu_pd_new) {
    $docuPD->deleteTempPD($docu_pd_cancel);
}
if (isset($docu_pd_cancel)) {
    unset($docu_pd_edit);
}
if (isset($docu_file_version_download)) {
    if ($docu_file_version_download > 0) {
        $docuFile->downloadFile($docu_file_version_download);
    } else {
        warning('Válassz ki egy letöltendő verziót!');
    }
}
if (isset($docu_pd_new) AND $docu_pd_new < 0) {
    $next = $docuFile->getNextDocuId();
    $docu_pd_edit = $docu_pd_new = $docuPD->InsertTempKK($next);
    output_hidden('docu_pd_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_pd_edit . ' azonosítójú pilóta dokumentum sablon létrehozva');
}
if (isset($docu_pd_delete)) {
    $docuPD->deleteTempPD($docu_pd_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_kk_delete . ' azonosítójú képzési/képzettségi sablon  törölve');
    unset($docu_kk_edit);
}
if (isset($docu_kk_save)) {
    if (!isset($selected_kk_docu_id)) {
        $selected_kk_docu_id = -1;
    }
    if (!isset($selected_kk_sdn_id)) {
        $selected_kk_sdn_id = -1;
    }
    $docu_file = $docuTempKK->getCurrentDocuFile($docu_kk_save);
    if (!empty($docu_file)) {
        $docu_file_id = $docu_file[DOCU_FILE_ID];
        $docuFile->setItemId($docu_file_id)->setDescription($docu_kk_desc)->setSdnId($selected_kk_sdn_id);
    }
    $docuTempKK->updateTempKK($docu_kk_save, $selected_kk_dao_id, $selected_kk_jog_id_o, ((isset($kk_active) AND $kk_active == 'on') ? 1 : 0));
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_kk_save . ' azonosítójú képzési/képzettségi sablon  módosítva');
    unset($docu_kk_edit);
}
if (isset($docu_kk_edit) AND $docu_kk_edit > 0) {
    $docu_kk = $docuTempKK->getTempKKById($docu_kk_edit);
    $selected_kk_dao_id = $docu_kk[DOCU_TEMP_KK_DAO_ID];
    $selected_kk_jog_id_o = $docu_kk[DOCU_TEMP_KK_JOG_ID_O];
    $kk_docu_id = $docu_kk[DOCU_TEMP_KK_DOCU_ID];
    $kk_active = $docu_kk[DOCU_TEMP_KK_ACTIVE] > 0 ? 'on' : 'off';
    if (false) { // A kötelező mezők nem lehetnek negatívok
        $kk_active = 'disabled';
    }
    o('<div class="keret editor">');
    output_label('Képzés', true);
    output_selector($docuDAO->getNames(), 'kk_dao_id', false);
    output_ln();
    output_label('Oktatói képzettség', true);
    output_selector($docuJOG->getTrainerNames(), 'kk_jog_id_o', false);
    output_ln();
    output_label('Aktív', true);
    output_switch_checkbox('kk_active', 'Igen', 'Nem');
    output_ln();
    output_label('Dokumentum', true);
    $docu_files = $docuFile->getFilesByDocuId($kk_docu_id);
    if (empty($docu_files)) {
        o('Még nincs feltöltve dokumentum');
    } else {
        if (!isset($selected_kk_docu_id)) {
            end($docu_files);
//            $selected_kk_docu_id = $last_docu_file[DOCU_FILE_ID];
            $selected_kk_docu_id = key($docu_files);
        }
        $docu_file_versions = array();
        foreach ($docu_files as $docu_file) {
            $docu_name = $docuFile->getNameByFileId($docu_file[DOCU_FILE_ID]);
            $docu_file_versions[$docu_file[DOCU_FILE_ID]] = $docu_name . ' (' . $docu_file[DOCU_FILE_UPLOADED] . ')';
        }
        output_chooser($docu_file_versions, 'kk_docu_id', true);
        output_ln();
        output_label('Elnevezés');
        if (!isset($selected_kk_sdn_id)) {
            $selected_kk_sdn_id = $docu_files[$selected_kk_docu_id][DOCU_FILE_SDN_ID];
        }
        output_selector($docuSDN->getNamesOfType(DOCU_SDN_TYPE_KK), 'kk_sdn_id', false);
        output_spaces(5);
        output_button('kk_sdn_edit', -1, 'edit.jpg', 'Elnevezések szerkesztése');
        output_ln();
        output_label('Egyedi név');
        o('<input type="text" name="docu_kk_desc" value="' . $docu_files[$selected_kk_docu_id][DOCU_FILE_DESCRIPTION] . '">');
        output_ln();
        output_label('Fájl');
        o('<button name="docu_file_version_download" value="' . $selected_kk_docu_id . '" onclick="saved=true;" title="Dokumentum letöltése"><img src="' . IMAGES . 'download.png"></button>');
        output_spaces(3);
        $_SESSION[SESSION_VIEW_DOCUMENT_ID] = $docu_files[$selected_kk_docu_id][DOCU_FILE_ID];
        o('<button name="docu_file_version_view" value="-1" onclick="saved=true;window.open(\'?view_document\');" title="Dokumentum megtekintése"><img src="' . IMAGES . 'view.jpg"></button>');
    }
    output_ln();
    output_label('Új dokumentum');
    o('<input name="docu_file" id="docu_file" type="file" />');
    o('<input type="submit" name="docu_upload" value="Feltöltés"/>');
    output_ln(2);
    output_save('docu_kk_save', $docu_kk_edit);
    output_spaces(5);
    output_cancel('docu_kk_cancel', $docu_kk_edit);
    o('</div>');
    output_hidden('docu_kk_edit');
}
output_ln(2);
o('<div class="lists">');
output_button('docu_kk_new', -1, 'add.jpg', 'Új képzési/képzettségi sablon létrehozása');
$kks = $docuTempKK->getItems();
if (empty($kks)) {
    o('</div>');
    return;
}
o('<table><tr><th>Képzés</th><th>Feltöltött dokumentum</th><th>Oktatói képzettség</th><th>Aktív</th><th>Művelet</th></tr>');
foreach ($kks as $kk_id => $kk) {
    $dao_id = $kk[DOCU_TEMP_KK_DAO_ID];
    $jog_id = $kk[DOCU_TEMP_KK_JOG_ID_O];
    $doc_id = $kk[DOCU_TEMP_KK_DOCU_ID];
    $dao_name = ($dao_id > 0) ? $docuDAO->getName($dao_id) : 'Nincs meghatározva';
    $jog_name = ($jog_id > 0) ? $docuJOG->getName($jog_id) : 'Nincs meghatározva';
    $docu_files = $docuFile->getFilesByDocuId($kk[DOCU_TEMP_KK_DOCU_ID]);
    if (empty($docu_files)) {
        $docu_name = 'Nincs feltöltve';
    } else {
        $docu_file = end($docu_files);
        if (!empty($docu_file[DOCU_FILE_DESCRIPTION])) {
            $docu_name = $docu_file[DOCU_FILE_DESCRIPTION];
        } elseif ($docu_file[DOCU_FILE_SDN_ID] > 0) {
            $docu_name = $docuSDN->getName($docu_file[DOCU_FILE_SDN_ID]);
        } else {
            $docu_name = $docu_file[DOCU_FILE_ORIGIN];
        }
    }
    o('<tr>');
    o('<td>' . $dao_name . '</td>');
    o('<td>' . $docu_name . '</td>');
    o('<td>' . $jog_name . '</td>');
    o('<td>' . ($kk[DOCU_TEMP_KK_ACTIVE] > 0 ? 'Igen' : 'Nem') . '</td>');
    o('<td>');
    output_button('docu_kk_edit', $kk_id, 'edit.jpg', 'Képzési/képzettségi sablon szerkesztése');
    if ($docuTempKK->isDeletable($kk_id)) {
        output_button('docu_kk_delete', $kk_id, 'delete.jpg', 'Képzési/képzettségi sablon törlése', 'Biztosan törölni akarja a sablont?');
    }
    o('</td></tr>');
}
o('</table></div>');


