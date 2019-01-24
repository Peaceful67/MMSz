<?php


function getListAccess($access) {
    $ret = '<label for="one"><input type="checkbox" name="" >';

    $ret .= '';
    return $ret;
}

function getListCategories($id, $group) {
    global $main_menu_group;
    $ret = '<select name="'.MENU_GROUP . $id . '" id="selected_' . $id . '" onchange="submit();">';
    $ret .= '<option value=-2 '
            . ($group==-2 ? ' selected ': '')
            . '>Rejtett</option>';
    $ret .= '<option value=-1'
            . ($group==-1 ? ' selected ': '')
            . '>Kategória szint</option>';
    foreach ($main_menu_group as $menu_group) {
        $ret .= '<option value=' . $menu_group[SETUP_GROUP_ID] 
                . ($menu_group[SETUP_GROUP_ID]==$group ? ' selected ': '') 
                . '>' . $menu_group[SETUP_GROUP_NAME] . '</option>';
    }
    $ret .= '</select>';
    return $ret;
}
