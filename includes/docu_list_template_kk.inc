<?php


o('<div class="lists keret">');
o('<h3>Képzés dokumentumai</h3>');
output_button('docu_kk_new', -1, 'add.jpg', 'Új képzési/képzettségi sablon létrehozása');

$kks = $docuTempKK->getTempsOfDao($docu_dao_edit);
if (empty($kks)) {
    o('</div>');
    return;
}
o('<table><tr><th>Feltöltött dokumentum</th><th>Oktatói képzettség</th><th>Művelet</th></tr>');
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
    o('<td>' . $jog_name . '</td>');
    o('<td>');
    output_button('docu_kk_edit', $kk_id, 'edit.jpg', 'Képzési/képzettségi sablon szerkesztése');
    if ($docuTempKK->isDeletable($kk_id)) {
        output_button('docu_kk_delete', $kk_id, 'delete.jpg', 'Képzési/képzettségi sablon törlése', 'Biztosan törölni akarja a sablont?');
    }
    o('</td></tr>');
}
o('</table></div>');
