<?php

o('<h3>Képzettséggel vezethető légijármű kategóriák szerkesztése</h3>');
$docuLjk = new DocuLJK();
$docuJOG = new DocuJOG();
$docuJogLjk = new DocuJogLjk();
output_prevent();
if (isset($docu_jl_new) AND $docu_jl_new < 0) {
    $docu_jl_new = $docu_jl_edit = $docuJogLjk->InsertJogLjk();
    output_not_saved();
    output_hidden('docu_jl_new');
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_jl_edit . ' azonosítójú képzettséggel vezethető légijármű összefüggés létrehozva');
}
if (isset($docu_jl_delete)) {
    $docuJogLjk->deleteJogLjk($docu_jl_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_jl_delete . ' azonosítójú képzettséggel vezethető légijármű összefüggés törölve');
}
if (isset($docu_jl_save)) {
    $docuJogLjk->updateJogLjk($docu_jl_save, $docu_jl_name, $selected_docu_jl_jog_id, $selected_docu_jl_ljk_id);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_jl_save . ' azonosítójú képzettséggel vezethető légijármű összefüggés módosítva');
}
if (isset($docu_jl_new) AND isset($docu_jl_cancel) AND $docu_jl_new == $docu_jl_cancel) {
    $docuJogLjk->deleteJogLjk($docu_jl_cancel);
}
if (isset($docu_jl_edit) AND $docu_jl_edit > 0) {
    $docu_jl = $docuJogLjk->getJogLjkById($docu_jl_edit);
    $selected_docu_jl_jog_id = $docu_jl[DOCU_JOG_LJK_JOG_ID];
    $selected_docu_jl_ljk_id = $docu_jl[DOCU_JOG_LJK_LJK_ID];
    o('<div class="keret editor">');
    output_label('Légijármű kategória', true);
    output_selector($docuLjk->getNames(), 'docu_jl_ljk_id', false);
    output_ln();
    output_label('Képzettség', true);
    output_selector($docuJOG->getNames(), 'docu_jl_jog_id', false);
    output_ln(2);
    output_save('docu_jl_save', $docu_jl_edit);
    output_spaces(5);
    output_cancel('docu_jl_cancel', $docu_jl_edit);
    o('</div>');
}
output_ln(2);
o('<div class="lists">');
output_button('docu_jl_new', -1, 'add.jpg', 'Új képzettséggel vezethető légijármű összefüggés létrehozása');
output_spaces(5);
output_back_history();
$jls = $docuJogLjk->getItems();
if (empty($jls)) {
    o('</div>');
    return;
}
o('<table><tr><th>Képzettség</th><th>Légijármű kategória</th><th>Művelet</th></tr>');
foreach ($jls as $jl_id => $jl) {
    $jog_id = $jl[DOCU_JOG_LJK_JOG_ID];
    $jog_name = ($jog_id >= 0) ? $docuJOG->getJOGById($jog_id)[DOCU_JOG_NAME] : 'Nincs megadva';
    $ljk_id = $jl[DOCU_JOG_LJK_LJK_ID];
    $ljk_name = ($ljk_id >= 0) ? $docuLjk->getLJKById($ljk_id)[DOCU_LJK_NAME] : 'Nincs megadva';
    o('<tr>');
    o('<td>' . $jog_name . '</td>');
    o('<td>' . $ljk_name . '</td>');
    o('<td>');
    output_button('docu_jl_edit', $jl_id, 'edit.jpg', 'Képzettség-légijármű kategória összefüggés szerkesztése');
    if ($docuJogLjk->isDeletable($jl_id)) {
        output_button('docu_jl_delete', $jl_id, 'delete.jpg', 'Képzettség-légijármű kategória összefüggés törlése', 'Biztosan törölni akarja az összefüggést?');
    }
    o('</td></tr>');
}
o('</table></div>');

