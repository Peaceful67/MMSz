<?php

o('<div class="lists keret">');
o('<h3>Képzettség sablonok</h3>');
output_button('docu_pd_new', -1, 'add.jpg', 'Új képzettségi sablon létrehozása');

$pds = $docuTempPD->getPDsOfJog($docu_jog_edit);
if (empty($pds)) {
    o('</div>');
    return;
}
o('<table><tr><th>Feltöltött dokumentum</th><th>Publikus</th><th>Lejáró típusú</th><th>Művelet</th></tr>');
foreach ($pds as $pd_id => $pd) {
    $doc_id = $pd[DOCU_TEMP_PD_DOCU_ID];
    $public = $pd[DOCU_TEMP_PD_PUBLIC];
    $expire = $pd[DOCU_TEMP_PD_EXPIRE];
    $docu_files = $docuFile->getFilesByDocuId($pd[DOCU_TEMP_KK_DOCU_ID]);
    if (empty($docu_files)) {
        $docu_name = 'Nincs feltöltve';
    } else {
        $docu_file = end($docu_files);
        if ($docu_file[DOCU_FILE_SDN_ID] > 0) {
            $docu_name = $docuSDN->getName($docu_file[DOCU_FILE_SDN_ID]);
        } elseif (!empty($docu_file[DOCU_FILE_DESCRIPTION])) {
            $docu_name = $docu_file[DOCU_FILE_DESCRIPTION];
        } else {
            $docu_name = $docu_file[DOCU_FILE_ORIGIN];
        }
    }
    o('<tr>');
    o('<td>' . $docu_name . '</td>');
    o('<td>' . ($public > 0 ? "Igen" : "Nem") . '</td>');
    o('<td>' . ($expire > 0 ? DOCU_EXPIRES : DOCU_NOT_EXPIRES) . '</td>');
    o('<td>');
    o('<button name="docu_pd_edit" value="' . $pd_id . '" title="Képzettségi sablon szerkesztése" onClick="submitOnchange_docu_pd_edit();"><img src="' . IMAGES . '/edit.jpg"></button>');
    if ($docuTempPD->isDeletable($pd_id)) {
        output_button('docu_pd_delete', $pd_id, 'delete.jpg', 'Képzettségi sablon törlése', 'Biztosan törölni akarja a sablont?');
    }
    o('</td></tr>');
}
o('</table></div>');
