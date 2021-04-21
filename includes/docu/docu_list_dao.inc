<?php

o('<div class="lists">');
output_button('docu_dao_new', -1, 'add.jpg', 'Új képzés létrehozása');
output_spaces(5);
output_back_history();
output_label('Szűrés', true);
output_label('Képzés neve', false);
output_selector($docuSDN->getNamesOfType(DOCU_SDN_TYPE_DAO), 'search_dao_sdn_id', true, 'Összes');
$daos = $docuDAO->getElementsByArray([DOCU_DAO_SDN_ID => intval($selected_search_dao_sdn_id)]);
if (empty($daos)) {
    o('</div>');
    return;
}
o('<table><tr><th>Szervezet</th><th>Képzés neve</th><th>Oktatói minimum</th><th>Szerezhető</th><th>Művelet</th></tr>');
foreach ($daos as $dao_id => $dao) {
    $org_id = $dao[DOCU_DAO_ORG_ID];
    $org_name = ($org_id > 0) ? $docuOrg->getOrgById($org_id)[DOCU_ORG_NAME] : 'Összes szervezet';
    $sdn_id = $dao[DOCU_DAO_SDN_ID];
    $sdn_name = ($sdn_id > 0) ? $docuSDN->getSDNById($sdn_id)[DOCU_SDN_NAME] : 'Nincs meghatározva';
    $jog_id_o = $dao[DOCU_DAO_JOG_ID_O];
    $jog_id_t = $dao[DOCU_DAO_JOG_ID_T];
    o('<tr  style="color: ' . (($dao[DOCU_DAO_ACTIVE] == 1) ? 'green' : 'red') . ';" >');
    $jogo_name = ($jog_id_o > 0) ? $docuJOG->getJOGById($jog_id_o)[DOCU_JOG_NAME] : 'Nincs meghatározva';
    $jogt_name = ($jog_id_t > 0) ? $docuJOG->getJOGById($jog_id_t)[DOCU_JOG_NAME] : 'Nincs meghatározva';
    o('<td>' . $org_name . '</td>');
    o('<td>' . $sdn_name . '</td>');
    o('<td>' . $jogo_name . '</td>');
    o('<td>' . $jogt_name . '</td>');
    o('<td>');
    output_button('docu_dao_edit', $dao_id, 'edit.jpg', 'Képzés szerkesztése');
    if ($docuDAO->isDeletable($dao_id)) {
        output_button('docu_dao_delete', $dao_id, 'delete.jpg', 'Képzés törlése', 'Biztosan törölni akarja a képzést?');
    }
    o('</td></tr>');
}
o('</table></div>');

