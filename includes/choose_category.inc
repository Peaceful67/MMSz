<?php

if (isset($category_change)) {    
    $categories = getCategories();
    $old_categories = getListMemberOldCategories($category_member_id);
    $sql = 'DELETE FROM ' . CM_TABLE . ' WHERE ' . CM_MEMBER_ID . "=" . $category_member_id;
    $mysqliLink->query($sql);
    $new_categories = '';
    foreach ($branches as $br_index => $br) {
        $check_cat = 'cat_check_' . $br_index;
        if (isset(${$check_cat}) AND $ {$check_cat} == 'on') {
            $br_cat = 'category_' . $br_index;
            foreach ($categories as $cat_index => $cat_name) {
                if (isset(${$br_cat}) AND in_array($cat_index, ${$br_cat})) {
                    $new_categories .= $cat_name . ', ';
                    $sql = 'INSERT INTO ' . CM_TABLE . ' (' . CM_MEMBER_ID . ',' . CM_CATEGORY . ') VALUES (' . $category_member_id . ', ' . $cat_index . ')';
                    $mysqliLink->query($sql);
                }
            }
        }
    }

    $old_categories = !empty($old_categories) ? $old_categories : 'Üres';
    $new_categories = !empty($new_categories) ? $new_categories : 'Üres';
    logger($member_id, -1, LOGGER_CATEGORY_MOD, $category_member_id . " azonosítójú kategória módosítása: régi:" . $old_categories . ', új:' . $new_categories);
    reloadEditMember();
}

$output .= '<div class="keret">';
$output .= '<lablel>Rögzített kategóriák:</label>';
$output .= getListMemberCategories($category_member_id) . '<br>';

$output .= '<form method="post">';
$output .= '<div class="kategoriak">';
$member_categories = getMemberCategories($category_member_id);
$output .= '<table><tr>';
foreach ($branches as $br_index => $br) {
    $categories = getCategories($br_index);
    $output .= '<td>';
    $output .= $br . ':';
    $ch = false;
    foreach ($categories as $cat_index => $cat_name) {
        if (in_array($cat_index, $member_categories)) {
            $ch = true;
        }
    }
    $output .= '<input type="checkbox"  name="cat_check_' . $br_index . '" ' . ($ch ? 'checked' : '') . '>';
    $output .= '</td>';
}
$output .= '</tr><tr>';

foreach ($branches as $br_index => $br) {
    $categories = getCategories($br_index);
    $output .= '<td>';
    $output .= '<select name="category_' . $br_index . '[]" multiple>';
    foreach ($categories as $cat_index => $cat_name) {
        $output .= '<option value="' . $cat_index . '"' . (in_array($cat_index, $member_categories) ? 'selected' : '' ) . '>' . $cat_name . '</option>';
    }
    $output .= '</select>';
    $output .= '</td>';
}
$output .= '</tr></table>';

$output .= '<input type="submit" name="category_change" value="Rögzítés">'
        . '<input type="hidden" name="category_member_id" value="' . $category_member_id . '">';
$output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
$output .= '</div>';
$output .= '</form>';
//$output .= 'Átvett kategória: ' . retriveMember($category_member_id)[MEMBER_KATEG] . '<br>';
$output .= '</div>';

