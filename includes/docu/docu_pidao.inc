<?php

$docuPidao = new DocuPIDAO();
$docuPidaoDocus = new DocuPIDAODocus();
$docuDao = new DocuDAO();
$docuFile = new DocuFile();
if (isset($docu_pidao_new) AND $docu_pidao_new < 0) {
    if ($selected_pidao_dao_id < 0) {
        warning("Válassz ki egy képzést !");
    } else {
        $docuPidao->createPidao($member_pidao, $selected_pidao_dao_id);
        logger($member_id, $member_pidao, LOGGER_DOCU_DAO, $selected_pidao_dao_id . ' azonosítójú képzés felvétele a tag képzései közé');
    }
}
if (isset($docu_pidao_cancel)) {
    unset($docu_pidao_edit);
}
if (isset($docu_pidao_delete)) {
    $docuPidao->deletePIDAO($docu_pidao_delete);
    logger($member_id, $member_pidao, LOGGER_DOCU_DAO, $docu_pidao_delete . ' azonosítójú képzés törlése a tag képzései közül');
}
if (isset($docu_pidao_new) AND isset($docu_pidao_cancel) AND $docu_pidao_new == $docu_pidao_cancel) {
    $docuPidao->deletePIDAO($docu_pidao_cancel);
}
if (isset($docu_pidao_save)) {
    $docuPidao->updatePIDAO($docu_pidao_save, $selected_pidao_dao_id);
    logger($member_id, $member_pidao, LOGGER_DOCU_DAO, $docu_pidao_save . ' azonosítójú képzés módosítása a tag képzései között');
}
if (isset($docu_pidao_edit) AND $docu_pidao_edit > 0) {
    $pidao = $docuPidao->getPidaoById($docu_pidao_edit);
    $selected_pidao_dao_id = $pidao[DOCU_PIDAO_DAO_ID];
    output_prevent();
    o('<div class="editor keret">');
    output_label('Növendék', true);
    o('(' . $pidao[DOCU_PIDAO_MEMBER_ID] . ') ' . getMemberName($pidao[DOCU_PIDAO_MEMBER_ID]));
    output_ln();
    output_label('Képzés', true);
    o($docuDao->getName($pidao[DOCU_PIDAO_DAO_ID]));
    output_ln();
    output_label('Vezető oktató', true);
    $dao_oas = array();
    output_selector($dao_oas, 'pidao_oa', false);
    output_ln(2);
    output_cancel('docu_pidao_cancel', $docu_pidao_edit);
    output_spaces(5);
    output_save('docu_pidao_save', $docu_pidao_edit, 'A képzés menétse');
    output_hidden('docu_pidao_edit');
    o('</div>');
    $pidaoDocus = new DocuPIDAODocus();
    include_once DOCU_INCLUDES . 'docu_pidao_docus_save.inc';
    if (isset($pidao_docus_edit) OR isset($pidao_docus_new) OR isset($pidao_docus_delete)) {
        include_once DOCU_INCLUDES . 'docu_pidao_docus_edit.inc';
    }
    include_once DOCU_INCLUDES . 'docu_pidao_docus_list.inc';
} else {
    include_once DOCU_INCLUDES . 'docu_list_pidao.inc';
}
