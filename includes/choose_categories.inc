<?php

define("CAT_CHECKNAME", 'cat_chk_');
if (!isset($selected_branch)) {
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
//    reloadEditMember();
}



$output .= '<div class="keret">';
$output .= '<lablel>Rögzített kategóriák: </label>';
$output .= getListMemberCategories($category_member_id) . '<br>';

$output .= '<form method="post">';
$output .= '<div class="kategoriak">';
$member_categories = getMemberCategories($category_member_id);
$output .= '<label>Szakág:</label><select name="selected_branch" onChange="submit();">';
$output .= '<option value ="-1" ' . (($selected_branch == -1) ? "selected" : "") . '>Válassz !</option>';
foreach ($branches as $br_index => $br) {
    $output .= '<option value="' . $br_index . '" ' . (($selected_branch == $br_index) ? "selected" : "") . '>' . $br . '</option>';
}
$output .= '</select>';


if ($selected_branch >= 0) {
    $categories = getCategories($selected_branch);
    $output .= '<ul>';
    foreach ($categories as $cat_id => $cat_name) {
        $cat_long = getCategoryById($cat_id)[CATEGORIES_LONG];
        $chk_name = CAT_CHECKNAME . $cat_id;
        $checked = (in_array($cat_id, $member_categories)) ? 'checked' : '';
        $output .= '<li><input type="checkbox" name="' . $chk_name . '" ' . $checked . ' onChange="submit();" ><a title="' . $cat_long . '">' . $cat_name . '</a></li>';
    }
    $output .= '</ul>';
}
$output .= '<input type="hidden" name="prior_branch" value="' . $prior_branch. '">';
$output .= '<input type="hidden" name="category_member_id" value="' . $category_member_id . '">';
$output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
$output .= '</div>';
$output .= '</form>';
//$output .= 'Átvett kategória: ' . retriveMember($category_member_id)[MEMBER_KATEG] . '<br>';
$output .= '</div>';

