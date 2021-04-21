<?php

o('<h3>Elnevezések szerkesztése</h3>');

$docuSDN = new DocuStandard();
$docuOrg = new DocuOrganizations();

if (isset($docu_sdn_new) AND $docu_sdn_new < 0) {
    $docu_sdn_edit = $docu_sdn_new = $docuSDN->InsertSDN();
    output_hidden('docu_sdn_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_sdn_edit . ' azonosítójú  standard dokumentum létrehozva');
}
if (isset($docu_sdn_delete)) {
    $docuSDN->deleteSDN($docu_sdn_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_sdn_delete . ' azonosítójú standard dokumentum törölve');
}
if (isset($docu_sdn_cancel) AND isset($docu_sdn_new) AND $docu_sdn_new = $docu_sdn_cancel) {
    $docuSDN->deleteSDN($docu_sdn_cancel);
}
if (isset($docu_sdn_save)) {
    if (!$docuSDN->isUnique($docu_sdn_save, $docu_sdn_name, $docu_sdn_eng_short)) {
        warning('Ez a név vagy angol rövidítés nem egyedi');
        //       $docu_sdn_edit = $docu_sdn_save;
    }
    $docuSDN->updateSDN($docu_sdn_save, $docu_sdn_name, $docu_sdn_short, $docu_sdn_eng_name, $docu_sdn_eng_short, (isset($docu_sdn_expire) AND $docu_sdn_expire == 'on') ? 1 : 0, $selected_docu_sdn_org_id, $selected_docu_sdn_type);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_sdn_save . ' azonosítójú  standard dokumentum módosítva');
}
if (isset($docu_sdn_edit) AND $docu_sdn_edit > 0) {
    output_prevent();
    $docu_sdn = $docuSDN->getSDNById($docu_sdn_edit);
    $docu_sdn_expire = $docu_sdn[DOCU_SDN_EXPIRE] > 0 ? 'on' : 'off';
    $selected_docu_sdn_org_id = $docu_sdn[DOCU_SDN_ORG_ID];
    $selected_docu_sdn_type = $docu_sdn[DOCU_SDN_TYPE];
    $docu_sdn_name = $docu_sdn[DOCU_SDN_NAME];
    $docu_sdn_short = $docu_sdn[DOCU_SDN_SHORT];
    $docu_sdn_eng_name = $docu_sdn[DOCU_SDN_ENG_NAME];
    $docu_sdn_eng_short = $docu_sdn[DOCU_SDN_ENG_SHORT];
    o('<div class="keret editor">');
    output_label('Szervezet', true);
    output_selector($docuOrg->getNames(), 'docu_sdn_org_id', false, 'Összes szervezet');
    output_ln();
    output_label('Típus', true);
    output_selector($docuSDN->getTypes(), 'docu_sdn_type', false, 'Nincs meghatározva');
    output_ln();
    output_label('Név', true);
    o('<input type="text" name="docu_sdn_name" value="' . $docu_sdn_name . '"/>');
    output_ln();
    output_label('Rövid név', true);
    o('<input type="text" name="docu_sdn_short" value="' . $docu_sdn_short . '"/>');
    output_ln();
    output_label('Angol név', true);
    o('<input type="text" name="docu_sdn_eng_name" value="' . $docu_sdn_eng_name . '"/>');
    output_ln();
    output_label('Angol rövidítés', true);
    o('<input type="text" name="docu_sdn_eng_short" value="' . $docu_sdn_eng_short . '"/>');
    output_ln();
    output_label('Az ilyen dokuemntum', true);
    output_switch_checkbox('docu_sdn_expire', DOCU_EXPIRES, DOCU_NOT_EXPIRES);
    output_ln(2);
    output_save('docu_sdn_save', $docu_sdn_edit);
    output_spaces(5);
    output_cancel('docu_sdn_cancel', $docu_sdn_edit);
    o('</div>');
}
output_ln(2);
o('<div class="lists">');
output_button('docu_sdn_new', -1, 'add.jpg', 'Új dokumentum létrehozása');
output_spaces(5);
output_back_history();
if (!isset($search_sdn_name)) {
    $search_sdn_name = '';
}if (!isset($search_sdn_short)) {
    $search_sdn_short = '';
}
output_label('Szűrés', true);
output_label('Neve', false);
o('<input type="text" name="search_sdn_name" value="' . $search_sdn_name . '" onchange="formAutoSubmit();" />');
output_spaces(5);
output_label('Rövid név', false);
o('<input type="text" name="search_sdn_short" value="' . $search_sdn_short . '" onchange="formAutoSubmit();" />');
output_spaces(5);
output_label('Típus', true);
output_selector($docuSDN->getTypes(), 'search_sdn_type', false, 'Összes');
output_spaces(5);
output_button('search', -1, 'search.png', 'Szűrés');
$sdns = $docuSDN->getElementsByArray([
    DOCU_SDN_NAME => $search_sdn_name,
    DOCU_SDN_SHORT => $search_sdn_short,
    DOCU_SDN_TYPE =>  intval($selected_search_sdn_type),
        ]);
if (empty($sdns)) {
    output_ln();
    warning('Nincs találat');
    o('</div>');
    return;
}
o('<table><tr><th>Neve</th><th>Rövid</th><th>Angol</th><th>Angol rövid</th><th>Lejárat</th><th>Szervezet</th><th>Típus</th><th>Művelet</th></tr>');
foreach ($sdns as $sdn_id => $sdn) {
    $org_id = $sdn[DOCU_SDN_ORG_ID];
    $org_name = ($org_id > 0) ? $docuOrg->getOrgById($org_id)[DOCU_ORG_NAME] : 'Összes szervezet';
    o('<tr><td>' . $sdn[DOCU_SDN_NAME] . '</td>');
    o('<td>' . $sdn[DOCU_SDN_SHORT] . '</td>');
    o('<td>' . $sdn[DOCU_SDN_ENG_NAME] . '</td>');
    o('<td>' . $sdn[DOCU_SDN_ENG_SHORT] . '</td>');
    o('<td>' . ($sdn[DOCU_SDN_EXPIRE] > 0 ? DOCU_EXPIRES : DOCU_NOT_EXPIRES) . '</td>');
    o('<td>' . $org_name . '</td>');
    o('<td>' . $docuSDN->getTypeName($sdn[DOCU_SDN_TYPE]) . '</td>');
    o('<td>');
    output_button('docu_sdn_edit', $sdn_id, 'edit.jpg', 'Dokumentum szerkesztése');
    if ($docuSDN->isDeletable($sdn_id)) {
        output_button('docu_sdn_delete', $sdn_id, 'delete.jpg', 'Dokumentum törlése', 'Biztosan törölni akarja a dokumentumot?');
    }
    o('</td></tr>');
}
o('</table></div>');

