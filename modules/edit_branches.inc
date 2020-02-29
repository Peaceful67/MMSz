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
$output .= '<form method="post"><div class="keret lists"><br>';
$output .= '<button title="Új szakág felvitele" name="add_name"><img src="' . IMAGES . 'add.jpg"></button><br>';
foreach ($branches as $branch_id => $branch_name) {
    $output .= '<input type="text" name="edit_name_' . $branch_id . '" value="' . $branch_name . '">&nbsp;&nbsp;&nbsp;&nbsp;';
    $output .= '<button  title="Szakág mentése" name="save_name" value="' . $branch_id . '"><img src="' . IMAGES . 'save.jpg"></button>&nbsp;&nbsp;&nbsp;&nbsp;';
    if (!isBranchUsed($branch_id)) {
        $output .= '<button  title="Szakág törlése"  name="delete_name" value="' . $branch_id . '"><img src="' . IMAGES . 'delete.jpg"></button>';
    } else {
        $output .= 'használatban.';
    }
    $output .= '</br>';
}
$output .= '</div></form>';
