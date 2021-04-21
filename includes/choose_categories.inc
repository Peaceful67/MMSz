<?php

define("CAT_CHECKNAME", 'cat_chk_');
if (!isset($selected_branch) OR isset($cat_reset)) {
    $selected_branch = -1;
}
if (!isset($prior_branch)) {
    $prior_branch = $selected_branch;
}

if ($selected_branch >= 0) {
    if ($prior_branch == $selected_branch) {
        $old_categories = getListMemberOldCategories($category_member_id);
        foreach (getCategories($selected_branch) as $cat_id => $cat_name) {
            $chk_name = CAT_CHECKNAME . $cat_id;
            $mysqliLink->query('DELETE FROM `' . CM_TABLE . '` WHERE `' . CM_MEMBER_ID . '`=' . $category_member_id . ' '
                    . ' AND `' . CM_CATEGORY . '`=' . $cat_id);
            if (isset(${$chk_name}) AND $ {$chk_name} == 'on') {
                $sql = 'INSERT INTO ' . CM_TABLE . ' (' . CM_MEMBER_ID . ',' . CM_CATEGORY . ') '
                        . 'VALUES (' . $category_member_id . ', ' . $cat_id . ')';
                $mysqliLink->query($sql);
            }
        }
    } else {
        $prior_branch = $selected_branch;
    }
    $new_categories = getListMemberOldCategories($category_member_id);
    $old_categories = !empty($old_categories) ? $old_categories : 'Üres';
    $new_categories = !empty($new_categories) ? $new_categories : 'Üres';
    if ($old_categories != $new_categories) {
        logger($member_id, -1, LOGGER_CATEGORY_MOD, $category_member_id . " azonosítójú kategória módosítása: régi:" . $old_categories . ', új:' . $new_categories);
    }
}

$view->putPrevent();
$view->newKeret('kategoriak');

$view->putText(getListMemberCategories($category_member_id), 'Kategóriák');
$view->inputSelector('Szakág', $branches, 'branch', true);

if ($selected_branch >= 0) {
    $categories = getCategories($selected_branch);
    $member_categories = getMemberCategories($category_member_id);
    if (count($categories) > 0) {
        $view->newElement('<ul class="list-group list-group-striped">');
        foreach ($categories as $cat_id => $cat_name) {
            $cat_long = getCategoryById($cat_id)[CATEGORIES_LONG];
            $chk_name = CAT_CHECKNAME . $cat_id;
            $$chk_name = (in_array($cat_id, $member_categories)) ? 'on' : 'off';
            $view->newElement('<li class="list-group-item">');
            $view->inputCheckbox($chk_name, $cat_name, true);
            $view->endElement('</li>');
        }
        $view->endElement('</ul>');
    } else {
        $view->warning($branches[$selected_branch]. ' szakágnak nincsenek megadva kategóriák!');
    }
}
$view->putHidden('prior_branch');
$view->putHidden('category_member_id');
$view->putHidden('edit_member');
$view->endKeret();

