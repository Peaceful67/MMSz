<?php

o('<h3>Szakágak légijármű kategóriái</h3>');

$docuLJK = new DocuLJK();
if (isset($docu_ljk_new) AND $docu_ljk_new < 0) {
    $docu_ljk_edit = $docu_ljk_new = $docuLJK->InsertLJK();
    output_hidden('docu_ljk_new');
    output_not_saved();
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_ljk_edit . ' azonosítójú  légijármű kategória létrehozva');
}
if (isset($docu_ljk_new) AND isset($docu_ljk_cancel) AND $docu_ljk_new == $docu_ljk_cancel) {
    $docuLJK->deleteLJK($docu_ljk_cancel);
}
if (isset($docu_ljk_delete)) {
    $docuLJK->deleteLJK($docu_ljk_delete);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_ljk_delete . ' azonosítójú  légijármű kategória törölve');
}
if (isset($docu_ljk_save)) {
    $docuLJK->updateLJK($docu_ljk_save, $docu_ljk_name, $selected_docu_ljk_branch);
    logger($member_id, -1, LOGGER_DOCU_MANAGEMENT_BASE, $docu_ljk_save . ' azonosítójú  légijármű kategória módosítva');
}
if (isset($docu_ljk_edit) AND $docu_ljk_edit > 0) {
    $docu_ljk = $docuLJK->getLJKById($docu_ljk_edit);
    $selected_docu_ljk_branch = $docu_ljk[DOCU_LJK_BRANCH];
    o('<div class="keret editor">');
    output_label('Neve', true);
    o('<input type="text" name="docu_ljk_name" value="' . $docu_ljk[DOCU_LJK_NAME] . '"/>');
    output_ln();
    output_label('Szakág', true);
    output_selector($branches, 'docu_ljk_branch', false);
    output_ln(2);
    output_button('docu_ljk_save', $docu_ljk_edit, 'save.jpg', 'Módosítások mentése');
    output_spaces(5);
    output_button('docu_ljk_cancel', $docu_ljk_edit, 'back.jpg', 'Szerkesztés elvetése');
    o('</div>');
}
output_ln(2);
o('<div class="lists">');
output_button('docu_ljk_new', -1, 'add.jpg', 'Új jármű kategória létrehozása');
output_spaces(5);
output_back_history();
$ljks = $docuLJK->getLJKs();
if (empty($ljks)) {
        o('</div>');
    return;
}
o('<table><tr><th>Neve</th><th>Szakág</th><th>Művelet</th></tr>');
foreach ($ljks as $ljk_id => $ljk) {
    $ljk_branch_name = (isset($branches[$ljk[DOCU_LJK_BRANCH]])) ? $branches[$ljk[DOCU_LJK_BRANCH]] : 'Nincs megadva';
    o('<tr><td>' . $ljk[DOCU_LJK_NAME] . '</td>');
    o('<td>' . $ljk_branch_name . '</td>');
    o('<td>');
    output_button('docu_ljk_edit', $ljk_id, 'edit.jpg', 'Légijármű kategória szerkesztése');
    if ($docuLJK->isDeletable($ljk_id)) {
        output_button('docu_ljk_delete', $ljk_id, 'delete.jpg', 'Légijármű kategória törlése', 'Biztosan törölni akarja a kategóriát?');
    }
    o('</td></tr>');
}
o('</table></div>');

