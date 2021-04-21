<?php

$docuPidjog = new DocuPIDJOG();
$docuJog = new DocuJOG();

if (isset($docu_pidjog_new) AND $docu_pidjog_new < 0) {
    if ($selected_pidjog_jog_id < 0) {
        warning("Válassz ki egy képzést !");
    } else {
        $docuPidjog->createPidjog($member_pidjog, $selected_pidjog_jog_id);
        logger($member_id, $member_pidjog, LOGGER_DOCU_JOG, $selected_pidjog_jog_id . ' azonosítójú képzettség a felvétele');
    }
}
if (isset($docu_pidjog_cancel)) {
    unset($docu_pidjog_edit);
}
if (isset($docu_pidjog_delete)) {
    $docuPidjog->deletePIDJOG($docu_pidjog_delete);
    logger($member_id, $member_pidjog, LOGGER_DOCU_JOG, $docu_pidjog_delete . ' azonosítójú képzettség törlése');
}
if (isset($docu_pidjog_new) AND isset($docu_pidjog_cancel) AND $docu_pidjog_new == $docu_pidjog_cancel) {
    $docuPidjog->deletePIDJOG($docu_pidjog_cancel);
}
if (isset($docu_pidjog_save)) {
    $docuPidjog->updatePIDJOG($docu_pidjog_save, $docu_pidjog_licence);
    logger($member_id, $member_pidjog, LOGGER_DOCU_JOG, $docu_pidjog_save . ' azonosítójú képzettésg módosítása');
}
if (isset($docu_pidjog_edit) AND $docu_pidjog_edit > 0) {
    $pidjog = $docuPidjog->getPidjogById($docu_pidjog_edit);
    $selected_pidjog_jog_id = $pidjog[DOCU_PIDJOG_JOG_ID];
    output_prevent();
    o('<div class="editor keret">');
    if (($docuJog->isTrainer($selected_pidjog_jog_id))) {
        output_label('Oktató', true);
    } else {
        output_label('Pilóta', true);
    }
    o('(' . $pidjog[DOCU_PIDJOG_MEMBER_ID] . ') ' . getMemberName($pidjog[DOCU_PIDJOG_MEMBER_ID]));
    output_ln();
    output_label('Képzettség', true);
    o($docuJog->getName($pidjog[DOCU_PIDJOG_JOG_ID]));
    output_ln();
    output_label('Dokumentum azonosító', true);
    o('<input type="text" name="docu_pidjog_licence" onChange="saved=false;" value="' . $pidjog[DOCU_PIDJOG_LICENCE] . '">');
    output_ln(2);
    output_cancel('docu_pidjog_cancel', $docu_pidjog_edit);
    output_spaces(5);
    output_save('docu_pidjog_save', $docu_pidjog_edit, 'A képzettség mentése');
    output_hidden('docu_pidjog_edit');
    o('</div>');
    
    $pidjogDocus = new DocuPIDJOGDocus();
    include_once DOCU_INCLUDES . 'docu_pidjog_docus_save.inc';
    if (isset($pidjog_docus_edit) OR isset($pidjog_docus_new) OR isset($pidjog_docus_delete)) {
        include_once DOCU_INCLUDES . 'docu_pidjog_docus_edit.inc';
    }
    include_once DOCU_INCLUDES . 'docu_pidjog_docus_list.inc';
} else {
    include_once DOCU_INCLUDES . 'docu_list_pidjog.inc';
}
