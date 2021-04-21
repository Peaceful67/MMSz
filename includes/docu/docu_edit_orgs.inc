<?php

o('<h3>Szakágak számára szolgáltató szervezetek</h3>');
output_prevent();
$docuOrg = new DocuOrganizations();
if (isset($docu_org_new)AND $docu_org_new < 0) {
    $docu_org_new = $docu_org_edit = $docuOrg->InsertOrg();
    output_hidden('docu_org_new');
    output_not_saved();
    $docuOrg->updateOrg($docu_org_edit, '', DOCU_ORG_TYPE_EDU, -1);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_org_edit . ' azonosítójú  szervezet létrehozva');
}
if (isset($docu_org_delete)) {
    $docuOrg->deleteOrg($docu_org_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_org_delete . ' azonosítójú  szervezet törölve');
}
if (isset($docu_org_new) AND isset($docu_org_cancel) AND $docu_org_new == $docu_org_cancel) {
    $docuOrg->deleteOrg($docu_org_cancel);
}
if (isset($docu_org_save)) {
    $docuOrg->updateOrg($docu_org_save, $docu_org_name, $docu_org_type, $selected_docu_org_assoc);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_org_save . ' azonosítójú  szervezet módosítva');
}
if (isset($docu_org_edit) AND $docu_org_edit > 0) {
    $docu_org = $docuOrg->getOrgById($docu_org_edit);
    $selected_docu_org_assoc = $docu_org[DOCU_ORG_ASSOCIATION_ID];
    if ($docu_org[DOCU_ORG_TYPE] == DOCU_ORG_TYPE_EDU) {
        $docu_org_type = DOCU_ORG_TYPE_EDU;
    } else {
        $docu_org_type = DOCU_ORG_TYPE_FLF;
    }
    o('<div class="keret editor">');
    output_label('Szervezet neve', true);
    o('<input type="text" onchange="saved=false;" name="docu_org_name" value="' . $docu_org[DOCU_ORG_NAME] . '"/>');
    output_ln();
    output_label('Alszövetség', true);
    output_selector(getSubClubs(), 'docu_org_assoc', false);
    output_ln();
    output_label('Szervezet típusa', true);
    output_switch_radio('docu_org_type', DOCU_ORG_TYPE_FLF);
    o('FLF szervezet');
    output_ln();
    output_label();
    output_switch_radio('docu_org_type', DOCU_ORG_TYPE_EDU);
    o('Képzési szervezet');
    output_ln(2);
    output_save('docu_org_save', $docu_org_edit);
    output_spaces(5);
    output_cancel('docu_org_cancel', $docu_org_edit);
    o('</div>');
}
output_ln(2);
o('<div class="lists">');
output_button('docu_org_new', -1, 'add.jpg', 'Új szervezet létrehozása');
output_spaces(5);
output_back_history();
$orgs = $docuOrg->getItems();
if (empty($orgs)) {
    o('</div>');
    return;
}

o('<table><tr><th>Neve</th><th>Típus</th><th>Alszövetség</th><th>Művelet</th></tr>');
foreach ($orgs as $org_id => $org) {
    o('<tr><td>' . $org[DOCU_ORG_NAME] . '</td>');
    o('<td>' . $docu_org_types_name[$org[DOCU_ORG_TYPE]] . '</td>');
    o('<td>' . ($org[DOCU_ORG_ASSOCIATION_ID] > 0 ? getClubName($org[DOCU_ORG_ASSOCIATION_ID]) : 'Akármelyik szervezet') . '</td>');
    o('<td>');
    output_button('docu_org_edit', $org_id, 'edit.jpg', 'Szervezet szerkesztése');
    if ($docuOrg->isDeletable($org_id)) {
        output_button('docu_org_delete', $org_id, 'delete.jpg', 'Szervezet törlése', 'Biztosan törölni akarja a szervezetet?');
    }
    o('</td></tr>');
}
o('</table></div>');

