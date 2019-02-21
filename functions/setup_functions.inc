<?php

function getListAccess($access) {
    $ret = '<label for="one"><input type="checkbox" name="" >';

    $ret .= '';
    return $ret;
}

function insert_menu_item($menu_item) {
    global $mysqliLink;
    $sql = 'INSERT INTO `' . SETUP_MOD_TABLE . '` ('
            . '`' . SETUP_MOD_ID . '`, `' . SETUP_MOD_TITLE . '`, `' . SETUP_MOD_MENU . '`, '
            . '`' . SETUP_MOD_MODULE . '`, `' . SETUP_MOD_DEPENDENCY . '`, `' . SETUP_MOD_GROUP . '`,'
            . '`' . SETUP_MOD_ACCESS . '`, `' . SETUP_MOD_WEIGHT . '`, `' . SETUP_MOD_ENABLED . '`'
            . ' ) VALUES ('
            . '"' . $menu_item[SETUP_MOD_ID] . '", "' . $menu_item[SETUP_MOD_TITLE] . '", "' . $menu_item[SETUP_MOD_MENU] . '", '
            . '"' . $menu_item[SETUP_MOD_MODULE] . '", "' . $menu_item[SETUP_MOD_DEPENDENCY] . '", "' . $menu_item[SETUP_MOD_GROUP] . '", '
            . '"' . $menu_item[SETUP_MOD_ACCESS] . '", "' . $menu_item[SETUP_MOD_WEIGHT] . '", "' . $menu_item[SETUP_MOD_ENABLED] . '" '
            . ');';
    $mysqliLink->query($sql);
}

function getListCategories($id, $group) {
    global $main_menu_group;
    $ret = '<select name="' . MENU_GROUP . $id . '" id="selected_' . $id . '" onchange="submit();">';
    $ret .= '<option value=-2 '
            . ($group == -2 ? ' selected ' : '')
            . '>Rejtett</option>';
    $ret .= '<option value=-1'
            . ($group == -1 ? ' selected ' : '')
            . '>Kategória szint</option>';
    foreach ($main_menu_group as $menu_group) {
        $ret .= '<option value=' . $menu_group[SETUP_GROUP_ID]
                . ($menu_group[SETUP_GROUP_ID] == $group ? ' selected ' : '')
                . '>' . $menu_group[SETUP_GROUP_NAME] . '</option>';
    }
    $ret .= '</select>';
    return $ret;
}
