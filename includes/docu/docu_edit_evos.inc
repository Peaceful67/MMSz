<?php

o('<h3>Egyesületek választott szolgáltató szervezetei</h3>');
$docuLJK = new DocuLJK();
$docuEVO = new DocuEVO();
output_prevent();
if (isset($docu_evo_new) AND $docu_evo_new < 0) {
    $docu_evo_new = $docu_evo_edit = $docuEVO->InsertEVO();
    output_hidden('docu_evo_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_evo_edit . ' azonosítójú szervezetkiválasztás létrehozva');
}
if (isset($docu_evo_delete)) {
    $docuEVO->deleteEVO($docu_evo_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_evo_delete . ' azonosítójú  szervezetkiválasztás törölve');
}
if (isset($docu_evo_save)) {
    $docuEVO->updateEVO($docu_evo_save, $selected_docu_evo_club, $selected_docu_evo_org, $selected_docu_evo_craft);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_evo_save . ' azonosítójú  szervezetkiválasztás módosítva');
}
if (isset($docu_evo_new) AND isset($docu_evo_cancel) AND $docu_evo_new == $docu_evo_cancel) {
    $docuEVO->deleteEVO($docu_evo_cancel);
}
if (isset($docu_evo_edit) AND $docu_evo_edit > 0) {
    $docu_evo = $docuEVO->getEVOById($docu_evo_edit);
    $selected_docu_evo_club = $docu_evo[DOCU_EVO_CLUB];
    $selected_docu_evo_org = $docu_evo[DOCU_EVO_ORG_ID];
    $selected_docu_evo_craft = $docu_evo[DOCU_EVO_LJK];
    o('<div class="keret editor">');
    output_label('Egyesület', true);
    output_selector(getClubNameList(), 'docu_evo_club', false);
    output_ln();
    output_label('Szervezet', true);
    output_selector((new DocuOrganizations())->getNames(), 'docu_evo_org', false);
    output_ln();
    output_label('Légijármű kategória', true);
    output_selector($docuLJK->getNames(), 'docu_evo_craft', false, 'Nincs megadva');
    output_ln(2);
    output_save('docu_evo_save', $docu_evo_edit);
    output_spaces(5);
    output_cancel('docu_evo_cancel', $docu_evo_edit);
}
output_ln(2);
o('<div class="lists">');
output_button('docu_evo_new', -1, 'add.jpg', 'Új szervezet felvétele');
output_spaces(5);
output_back_history();
output_label('Szűrés', true);
output_label('Egyesület', false);
output_selector(getClubNameList(), 'search_evo_club', true, 'Összes');
$evos = $docuEVO->getElementsByArray([DOCU_EVO_CLUB => intval($selected_search_evo_club)]);
if (empty($evos)) {
    o('</div>');
    return;
}
o('<table><tr><th>Egyesület</th><th>Szervezet</th><th>Légijármű kategória</th><th>Művelet</th></tr>');
foreach ($evos as $evo_id => $evo) {
    $club = ($evo[DOCU_EVO_CLUB] > 0) ? getClubName($evo[DOCU_EVO_CLUB]) : 'Nincs megadva';
    $org = ($evo[DOCU_EVO_ORG_ID] > 0) ? (new DocuOrganizations())->getOrgById($evo[DOCU_EVO_ORG_ID])[DOCU_ORG_NAME] : 'Nincs megadva';
    $craft = ($evo[DOCU_EVO_LJK] > 0) ? $docuLJK->getLJKById($evo[DOCU_EVO_LJK])[DOCU_LJK_NAME] : 'Nincs megadva';
    o('<tr><td>' . $club . '</td>');
    o('<td>' . $org . '</td>');
    o('<td>' . $craft . '</td>');
    o('<td>');
    output_button('docu_evo_edit', $evo_id, 'edit.jpg', 'Szervezet kiválasztása');
    if ($docuEVO->isDeletable($evo_id)) {
        output_button('docu_evo_delete', $evo_id, 'delete.jpg', 'Kiválasztás törlése');
    }
    o('</td></tr>');
}
o('</table></div>');

