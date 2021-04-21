<?php

$pidao_docus = $pidaoDocus->getPIDAODocusOfPidao($docu_pidao_edit);
$docuFile = new DocuFile();
if (empty($pidao_docus)) {
    warning('Nincsenek a képzéshezhez dokumentumok rendelve');
    return;
}
o('<div class="keret lists">');
output_button('pidao_docus_new', -1, 'add.jpg', 'Új képzési dokumentum hozzáadása');
o('<table>');
o('<tr><th>Neve</th><th>Feltöltve</th><th>Lejárat</th><th>Állapot</th><th>Művelet</th></tr>');
foreach ($pidao_docus as $docu) {
    if ($docu[DOCU_PIDAO_DOCUS_OWN] > 0) {
        $name = empty($docu[DOCU_PIDAO_DOCUS_OWN_NAME]) ? 'Nincs megadva' : $docu[DOCU_PIDAO_DOCUS_OWN_NAME];
    } else {
        $name = $docuFile->getNameByDocuID($docu[DOCU_PIDAO_DOCUS_KK_DOCU_ID]);
    }
    $expire_date = is_null($docu[DOCU_PIDAO_DOCUS_EXPIRE_DATE]) ? 'Nincs megadva' : $docu[DOCU_PIDAO_DOCUS_EXPIRE_DATE];
    o('<tr>');
    o('<td>' . $name . '</td>');
    o('<td>' . ($docu[DOCU_PIDAO_DOCUS_DOCU_ID] > 0 ? "Igen" : "Nem") . '</td>');
    o('<td>' . ($docu[DOCU_PIDAO_DOCUS_EXPIRE] > 0 ? $expire_date : "Nem lejáró") . '</td>');
    o('<td>' . ($docu[DOCU_PIDAO_DOCUS_APPROVED] > 0 ? date('Y-m-d', $docu[DOCU_PIDAO_DOCUS_APPROVED_TIME]) : "Nincs jóváhagyva") . '</td>');
    o('<td>');
    output_button('pidao_docus_edit', $docu[DOCU_PIDAO_DOCUS_ID], 'edit.jpg', 'Képzés dokumentumának szerkesztése');
    if ($docu[DOCU_PIDAO_DOCUS_OWN] > 0) {
        output_button('pidao_docus_delete', $docu[DOCU_PIDAO_DOCUS_ID], 'delete.jpg', 'Képzés dokumentum törlése', 'Biztosan törölni akarja a dokumentumot?');
    }
    o('</td>');
    o('</tr>');
}
o('</table>');
o('</div>');
