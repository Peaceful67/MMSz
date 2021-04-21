<?php

$docuSDN = new DocuStandard();
if (!isset($pidjog_docus_edit)) {
    return;
}
$pidjogDocus->getPIDJOGDocusById($pidjog_docus_edit);



o('<div class="keret editor">');
output_prevent();
$docus_arr = array();
$docu_index = 1;
if ($pidjogDocus->isOwn()) {
    output_label('Saját dokumentum neve', true);
    o('<input type="text" name="pidjog_docus_own_name" value="' . $pidjogDocus->getOwnName() . '" onchange="saved=false;">');
    output_ln();
    output_label('Lejáró dokumentum', true);
    $pidjog_docus_expire = $pidjogDocus->isExpire() ? 'on' : 'off';
    output_switch_checkbox('pidjog_docus_expire', 'Igen', 'Nem');
} else {
    output_label('Sablon', false);
    o($docuSDN->getName($pidjogDocus->getSDNId()));
    output_spaces(5);
    $pd_docu_id = $pidjogDocus->getPDDocuId();
    if ($pd_docu_id > 0) {
        $file_id = $docuFile->getFileIdByDocuId($pd_docu_id);
        if ($file_id > 0) {
            o('<button name="file_id_download" value="' . $file_id . '" onclick="saved=true;" title="Dokumentum letöltése"><img src="' . IMAGES . 'download.png"></button>');
            output_spaces(5);

            o('<button name="docu_file_version_view" value="-1" onclick="saved=true;window.open(\'?view_selected=' . $docu_index . '\');" title="Dokumentum megtekintése"><img src="' . IMAGES . 'view.jpg"></button>');
            $docus_arr[$docu_index++] = $file_id;
        } else {
            o('Nincs feltöltve');
        }
    } else {
        o('Nincs feltöltve');
    }
}
if ($pidjogDocus->isExpire()) {
    output_ln();
    output_label('Lejárat', true);
    $pidjog_docus_expire_date = $pidjogDocus->getExpireDate();
    if (is_null($pidjog_docus_expire_date)) {
        $pidjog_docus_expire_date = $endOfYear;
    }
    o('<input type="date" name="pidjog_docus_expire_date" size="8" value="' . $pidjog_docus_expire_date . '">');
} else {
    $pidjog_docus_expire_date = NULL;
}
output_ln();
output_label('Nyilvános dokumentum', true);
$pidjog_docus_public = $pidjogDocus->isPublic() ? 'on' : 'off';
output_switch_checkbox('pidjog_docus_public', 'Igen', 'Nem');
output_ln();
output_label('Feltöltött', true);
$docu_id = $pidjogDocus->getDocuId();
$docu_files = $docuFile->getFilesByDocuId($docu_id);
if ($docu_id <= 0 OR empty($docu_files)) {
    o('Még nincs feltöltve dokumentum');
    $selected_docus_file_id = -1;
} else {
    if (isset($changed_docus_edit) AND $changed_docus_edit > 0 AND $changed_docus_edit != $pidjog_docus_edit) {
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
output_cancel('pidjog_docus_cancel', $pidjog_docus_edit);
output_spaces(5);
output_save('pidjog_docus_save', $pidjog_docus_edit, 'A változások mentése');
output_spaces(5);
$_SESSION[SESSION_VIEW_DOCUMENT_ID] = "";
if (empty($docu_files)) {
    o('Feltöltés:');
    output_spaces(5);
} else {
    if ($selected_docus_file_id > 0) {
        o('<button name="file_id_download" value="' . $selected_docus_file_id . '" onclick="saved=true;" title="Dokumentum letöltése"><img src="' . IMAGES . 'download.png"></button>');
        output_spaces(5);
        o('<button name="docu_file_version_view" value="-1" onclick="saved=true;window.open(\'?view_selected=' . $docu_index . '\');" title="Dokumentum megtekintése"><img src="' . IMAGES . 'view.jpg"></button>');
        $docus_arr[$docu_index++] = $selected_docus_file_id;
        output_spaces(5);
        o('Módosítás:');
        output_spaces(5);
    }
}
o('<input name="docu_file_upload" id="docu_file_upload" type="file" />');
$_SESSION[SESSION_VIEW_DOCUMENT_ARRAY] = serialize($docus_arr);
output_hidden('pidjog_docus_edit');
o('</div>');
