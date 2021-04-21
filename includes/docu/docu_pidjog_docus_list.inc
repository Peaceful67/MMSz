<?php


$pidjog_docus = $pidjogDocus->getPIDJOGDocusOfPidjog($docu_pidjog_edit);
$docuSdn = new DocuStandard();
if (empty($pidjog_docus)) {
    warning('Nincsenek a képzettséghez dokumentumok rendelve');
    return;
}
o('<div class="keret lists">');
output_button('pidjog_docus_new', -1, 'add.jpg', 'Új képzettségi dokumentum hozzáadása');
o('<table>');
o('<tr><th>Neve</th><th>Feltöltve</th><th>Lejárat</th><th>Művelet</th></tr>');
foreach ($pidjog_docus as $docu) {
    if($docu[DOCU_PIDJOG_DOCUS_OWN]>0) {
        $name = $docu[DOCU_PIDJOG_DOCUS_OWN_NAME];
    } else {
        $name = $docuSdn->getName($docu[DOCU_PIDJOG_DOCUS_SDN_ID]); 
    }
    o('<tr>');
    o('<td>' .$name . '</td>');
    o('<td>' . ($docu[DOCU_PIDJOG_DOCUS_DOCU_ID] > 0 ? "Igen" : "Nem") . '</td>');
    o('<td>' . ($docu[DOCU_PIDJOG_DOCUS_EXPIRE] > 0 ? $docu[DOCU_PIDJOG_DOCUS_EXPIRE_DATE] : "Nem lejáró") . '</td>');
    o('<td>');
    output_button('pidjog_docus_edit', $docu[DOCU_PIDJOG_DOCUS_ID], 'edit.jpg', 'Képzettség dokumentumának szerkesztése');
    if($docu[DOCU_PIDJOG_DOCUS_OWN]>0 AND $pidjogDocus->isDeletable($docu[DOCU_PIDJOG_DOCUS_ID])) {
         output_button('pidjog_docus_delete', $docu[DOCU_PIDJOG_DOCUS_ID], 'delete.jpg', 'Képzettségi dokumentum törlése', 'Biztosan törölni akarja a dokumentumot?');
    }
    o('</td>');
    o('</tr>');
}
o('</table>');
o('</div>');
