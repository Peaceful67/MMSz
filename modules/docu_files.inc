<?php

include_once FUNCTIONS . 'docu_functions.inc';
$docuFile = new DocuFile();
$docuSDN = new DocuStandard();

if (!isset($selected_member_id)) {
    $selected_member_id = $member_id;
}
if (isPermitted(ACCESS_ADMIN)) {
    $members = $docuFile->getOwners();
    output_label('Feltöltő', true);
    output_chooser($members, 'member_id', true);
    output_ln(2);
}
if (isset($assign_field)) {
    $docuClass = new $change_class();
    $docuClass->getItemById($change_id);
    $docuClass->updateValue($change_field, $assign_field);
}
output_prevent();
if (isset($change_class) AND isset($change_field)) {
    $select_field = true;
    if (!isset($change_sdn_id)) {
        $change_sdn_id = -1;
    }
    output_hidden('change_class');
    output_hidden('change_field');
    output_hidden('change_id');
    output_hidden('change_sdn_id');
} else {
    $select_field = false;
}
if (isset($docu_upload_docu_id)) {   // Egy új verzió feltöltése
    if (empty($_FILES['docu_file']['name'])) {
        warning('Nincs kiválasztva fájl feltöltésre!');
    } else {
        $prev_file = $docuFile->getFileByDocuId($docu_upload_docu_id);
        $file_id = $docuFile->uploadDocument('docu_file', DOCU_FILE_TYPE_DOCUMENTS, $docu_upload_docu_id);
        if (!empty($prev_file)) {
            $docuFile->setSdnDescOfFile($file_id, $prev_file[DOCU_FILE_SDN_ID], $prev_file[DOCU_FILE_DESCRIPTION]);
        }
    }
}

if (isset($docu_upload)) {
    if (empty($_FILES['docu_file']['name'])) {
        warning('Nincs kiválasztva fájl feltöltésre!');
    } else {
        $docu_id = $docuFile->getNextDocuId();
        $docuFile->uploadDocument('docu_file', DOCU_FILE_TYPE_DOCUMENTS, $docu_id);
    }
}

if (isset($docu_desc_save)) {
    $docuFile->getFileById($docu_desc_save);
    $var_name = 'docu_desc_' . $docu_desc_save;
    $docuFile->setDescription($$var_name);
    $sdn_var_name = 'selected_docu_file_sdn_' . $docu_desc_save;
    $docuFile->setSDN($$sdn_var_name);
    logger($member_id, $selected_member_id, LOGGER_DOCU_FILES, $docu_desc_save . ' azonosítójú fájl nevének módosítása');
}
if (isset($docu_download)) {
    logger($member_id, $selected_member_id, LOGGER_DOCU_FILES, $docu_download . ' azonosítójú fájl letöltve');
    $docuFile->downloadFile($docu_download);
}
if (isset($docu_delete)) {
    logger($member_id, $selected_member_id, LOGGER_DOCU_FILES, $docu_delete . ' azonosítójú fájl törölve');
    $docuFile->deleteFile($docu_delete);
}
$docu_file_sdns = $docuSDN->getNames();
$ownDocus = $docuFile->getDocuIdsOfOwner($selected_member_id);
o('<div class="lists">');
if (!$select_field) {
    output_label('Feltöltendő fájl');
    o('<input name="docu_file" type="file"/>');
    o('<input type="submit" name="docu_upload" value="Új dokumentum feltöltés" />');
    output_spaces(5);
}
output_back_history();
output_ln(2);
if (empty($ownDocus)) {
    warning('Nincsenek felvitt dokumentumok');
    o('</div>');
    return;
}
o('<table><tr><th>Feltöltve</th><th>Leírás</th><th>Rendszer név</th><th>Művelet</th></tr>');
$docu_index = 1;
$docus_arr = array();
foreach ($ownDocus as $own_docu_id => $files) {
    if (empty($files)) {
        continue;
    }
    if ($select_field) {
        if ($change_sdn_id > 0 AND $change_sdn_id != $ownDocu[DOCU_FILE_SDN_ID]) {
            continue;
        }
    }
    o('<tr>');
    o('<td>');
    if (count($files) > 1) {
        $var_name = 'selected_file_id_' . $own_docu_id;
        $file_arr = array();
        foreach ($files as $file_id) {
            $file_arr[$file_id] = $docuFile->getUploadedById($file_id);
        }

        if (!isset($$var_name)) {
            reset($file_arr);
            $$var_name = key($file_arr);
        }
        $file_id = $$var_name;
        $file = $docuFile->getFileById($file_id);

        output_chooser($file_arr, 'file_id_' . $own_docu_id, true);
    } else {
        $file_id = $files[0];
        $file = $docuFile->getFileById($file_id);
        o($docuFile->getUploadedById($file_id));
    }
    o('</td>');
    o('<td>');
    o('<input name="docu_desc_' . $file_id . '" type="text" value="' . $file[DOCU_FILE_DESCRIPTION] . '" onchange="saved=false;">');
    o('</td>');
    o('<td>');
    $var_name = 'docu_file_sdn_' . $file_id;
    $selected_var_name = 'selected_' . $var_name;
    if (!isset($$selected_var_name)) {
        $$selected_var_name = $file[DOCU_FILE_SDN_ID];
    }
    output_selector($docu_file_sdns, $var_name, false, 'Nincs megadva');
    o('</td>');
    o('<td>');
    $docus_arr[$docu_index] = $file_id;
    o('<button name="docu_file_view" value="-1" onclick="saved=true;window.open(\'?view_selected=' . $docu_index . '\');" title="Dokumentum megtekintése:' . $docu_index . '/' . $file_id . '"><img src="' . IMAGES . 'view.jpg"></button>');
    $docu_index++;
    output_spaces(3);
    output_button('docu_download', $file_id, 'download.png', 'Dokumentum letöltés');
    output_spaces(3);
    output_button('docu_upload_docu_id', $own_docu_id, 'upload.jpg', 'A dokumentum egy új verziójának a feltöltése', 'Biztosan egy új verziót akarsz feltölteni?');
    output_spaces(3);
    if ($docuFile->isDeletable($file_id)) {
        $disable = '';
        $title = 'Dokumentum törlése';
    } else {
        $title = "A dokumentum már használatban van, nem lehet törölni.";
        $disable = ' disabled ';
    }
    o('<button name="docu_delete" value="' . $file_id . '" title="' . $title . '" onclick="return confirm(\'Biztosan törölni akarod a dokumentumot ?\');" ' . $disable . '>'
            . '<img src="' . IMAGES . 'delete.jpg"></button>');
    output_spaces(3);
    output_save('docu_desc_save', $file_id, 'Leírás mentése');
    if ($select_field) {
        output_spaces(3);
        output_button('assign_field', $own_docu_id, 'assign.jpg', 'A dokumentum hozzárendelése', 'Biztosan hozzárendeli a dokumentumot és visszatér a korábbi ablakba?');
    }
    o('</td>');
    o('<tr>');
}

o('</table>');
$_SESSION[SESSION_VIEW_DOCUMENT_ARRAY] = serialize($docus_arr);
$output .= '</div>';
