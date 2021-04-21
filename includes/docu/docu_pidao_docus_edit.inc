<?php

if (!isset($pidao_docus_edit)) {
    return;
}
$pidao_docu = $pidaoDocus->getPIDAODocusById($pidao_docus_edit);
o('<div class="keret editor">');
output_prevent();
$docus_arr = array();
$docu_index = 1;
if ($pidaoDocus->isOwn()) {
    output_label('Saját dokumentum neve', true);
    o('<input type="text" name="pidao_docus_own_name" value="' . $pidaoDocus->getOwnName() . '" onchange="saved=false;">');
    output_ln();
    output_label('Lejáró dokumentum', true);
    $pidao_docus_expire = $pidaoDocus->isExpire() ? 'on' : 'off';
    output_switch_checkbox('pidao_docus_expire', 'Igen', 'Nem');
    $kk_docu_id = -1;
} else {
    output_label('Sablon', false);
    $kk_docu_id = $pidao_docu[DOCU_PIDAO_DOCUS_KK_DOCU_ID];
    o($docuFile->getNameByDocuId($kk_docu_id));
    output_spaces(5);
    if ($kk_docu_id > 0) {
        $file_id = $docuFile->getFileIdByDocuId($kk_docu_id);
        if ($file_id > 0) {
            o('<button name="file_id_download" value="' . $file_id . '" onclick="saved=true;" title="Dokumentum letöltése"><img src="' . IMAGES . 'download.png"></button>');
            output_spaces(5);
            o('<button onclick="saved=true;window.open(\'?view_selected=' . $docu_index . '\');" title="Dokumentum megtekintése"><img src="' . IMAGES . 'view.jpg"></button>');
            $docus_arr[$docu_index] = $file_id;
            $docu_index++;
        } else {
            o('Nincs feltöltve');
        }
    } else {
        o('Nincs feltöltve sablon dockumentum');
    }
}
if ($pidaoDocus->isExpire()) {
    output_ln();
    output_label('Lejárat');
    $pidao_docus_expire_date = $pidaoDocus->getExpireDate();
    if (is_null($pidao_docus_expire_date)) {
        $pidao_docus_expire_date = $endOfYear;
    }
    o('<input type="date" name="pidao_docus_expire_date" size="8" value="' . $pidao_docus_expire_date . '">');
} 
output_ln();
output_label('Nyilvános');
$pidao_docus_public = $pidaoDocus->isPublic() ? 'on' : 'off';
output_switch_checkbox('pidao_docus_public', 'Igen', 'Nem');
output_ln();
output_label('Feltöltött', true);
$docu_id = $pidao_docu[DOCU_PIDAO_DOCUS_DOCU_ID];
$docu_files = $docuFile->getFilesByDocuId($docu_id);
if ($docu_id <= 0 OR empty($docu_files)) {
    o('Még nincs feltöltve dokumentum');
    $selected_docus_file_id = -1;
} else {
    if (isset($changed_docus_edit) AND $changed_docus_edit > 0 AND $changed_docus_edit != $pidao_docus_edit) {
        unset($selected_docus_file_id);
    }
    if (!isset($selected_docus_file_id)) {
        end($docu_files);
        $selected_docus_file_id = key($docu_files);
    }
    if (count($docu_files) > 1) {
        $docu_file_versions = array();
        foreach ($docu_files as $docu_file) {
            $docu_name = $docuFile->getNameByFileId($docu_file[DOCU_FILE_ID]);
            $docu_file_versions[$docu_file[DOCU_FILE_ID]] = $docu_name . ' (' . $docu_file[DOCU_FILE_UPLOADED] . ')';
        }
        output_chooser($docu_file_versions, 'docus_file_id', true);
    } else {
        $docu_file = $docu_files[$selected_docus_file_id];
        $docu_name = $docuFile->getNameByFileId($docu_file[DOCU_FILE_ID]);
        o($docu_name . ' (' . $docu_file[DOCU_FILE_UPLOADED] . ')');
    }
}

output_ln(2);
output_cancel('pidao_docus_cancel', $pidao_docus_edit);
output_spaces(5);
output_save('pidao_docus_save', $pidao_docus_edit, 'A változások mentése');
output_spaces(5);

if (empty($docu_files)) {
    o('Feltöltés:');
    output_spaces(5);
} else {
    if ($selected_docus_file_id > 0) {
        o('<button name="file_id_download" value="' . $selected_docus_file_id . '" onclick="saved=true;" title="Dokumentum letöltése"><img src="' . IMAGES . 'download.png"></button>');
        output_spaces(5);
        o('<button onclick="saved=true;window.open(\'?view_selected=' . $docu_index . '\');" title="Dokumentum megtekintése"><img src="' . IMAGES . 'view.jpg"></button>');
        $docus_arr[$docu_index++] = $selected_docus_file_id;
        output_spaces(5);
        o('Módosítás:');
        output_spaces(5);
    }
}
$kk_sdn_id = $docuFile->getSdnIdByDocuId($kk_docu_id);
output_button('pidao_docus_sdn_id', $kk_sdn_id, 'assign.jpg', 'A rendszerbe korábban feltöltött dokumentum hozzárendelése');
output_spaces(5);
o('<input name="docu_file_upload" id="docu_file_upload" type="file" />');
$_SESSION[SESSION_VIEW_DOCUMENT_ARRAY] = serialize($docus_arr);
$_SESSION[SESSION_VIEW_DOCUMENT_ID] = "";
output_hidden('pidao_docus_edit');
o('</div>');
