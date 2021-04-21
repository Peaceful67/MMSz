<?php


if (isset($add_name)) {
    $sql = 'INSERT INTO `' . BRANCHES_TABLE . '` (`' . BRANCHES_NAME . '`) VALUES ("Nevezd át !!!");';
    $mysqliLink->query($sql);
}
if (isset($save_name)) {
    $name_var = 'edit_name_' . $save_name;
    $sql = 'UPDATE `' . BRANCHES_TABLE . '` SET `' . BRANCHES_NAME . '`="' . $$name_var . '" WHERE `' . BRANCHES_ID . '`=' . $save_name . ';';
    $mysqliLink->query($sql);
}
if (isset($delete_name)) {
    $sql = 'DELETE FROM `' . BRANCHES_TABLE . '` WHERE `' . BRANCHES_ID . '`=' . $delete_name . ';';
    $mysqliLink->query($sql);
}
$branches = getBranches();
$view->newKeret();
$view->submitAdd('add_name', -1, 'Új szakág felvitele');
foreach ($branches as $branch_id => $branch_name) {
    $var_name = 'edit_name_' . $branch_id;
    $$var_name = $branch_name;
    $view->newDiv('row');
    $view->inputText($var_name);
    $view->submitSave('save_name', $branch_id, 'Szakág nevének mentése');
    if (!isBranchUsed($branch_id)) {
        $view->submitDelete('delete_name', $branch_id, 'Szakág törlése');
    }
    $view->endDiv();
}
$view->endKeret();
