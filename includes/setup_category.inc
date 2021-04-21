<?php


if (isset($group_down)) {
    $gd = 'group_weight_' . $group_down;
    $gd_weight = $$gd;
    $sql = 'UPDATE `' . SETUP_GROUP_TABLE . '` SET '
            . ' `' . SETUP_GROUP_WEIGHT . '`=' . $gd_weight
            . ' WHERE  `' . SETUP_GROUP_WEIGHT . '`=' . ($gd_weight + 1) . ' ;';
    $mysqliLink->query($sql);
    $sql = 'UPDATE `' . SETUP_GROUP_TABLE . '` SET '
            . ' `' . SETUP_GROUP_WEIGHT . '`=' . ($gd_weight + 1)
            . ' WHERE  `' . SETUP_GROUP_ID . '`=' . $group_down . ' ;';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, $group_down . '. azonosítójú kategória hátrébb helyezve.');
}
if (isset($group_up)) {
    $gd = 'group_weight_' . $group_up;
    $gd_weight = $$gd;
    $sql = 'UPDATE `' . SETUP_GROUP_TABLE . '` SET '
            . ' `' . SETUP_GROUP_WEIGHT . '`=' . $gd_weight
            . ' WHERE  `' . SETUP_GROUP_WEIGHT . '`=' . ($gd_weight - 1) . ' ;';
    $mysqliLink->query($sql);
    $sql = 'UPDATE `' . SETUP_GROUP_TABLE . '` SET '
            . ' `' . SETUP_GROUP_WEIGHT . '`=' . ($gd_weight - 1)
            . ' WHERE  `' . SETUP_GROUP_ID . '`=' . $group_up . ' ;';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, $group_up . '. azonosítójú kategória előrébb helyezve.');
}
if (isset($delete_group)) {
    $sql = 'DELETE FROM `' . SETUP_GROUP_TABLE . '` WHERE `' . SETUP_GROUP_ID . '`=' . $delete_group . ';';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, $delete_group . '. azonosítójú kategória kitörölve.');
}
if (isset($menu_group_new)) {
    $sql = 'SELECT MAX(`' . SETUP_GROUP_ID . '`) AS max FROM `' . SETUP_GROUP_TABLE . '`;';
    $res = $mysqliLink->query($sql);
    $new_id = (mysqli_fetch_assoc($res)['max']) + 1;
    $sql = 'INSERT INTO `' . SETUP_GROUP_TABLE . '` ('
            . '`' . SETUP_GROUP_ID . '`, `' . SETUP_GROUP_NAME . '`, `' . SETUP_GROUP_WEIGHT . '` '
            . ' ) VALUES ('
            . '"' . $new_id . '", "", "0" '
            . ');';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, $new_id . '. azonosítójú kategória létrehozva.');
}
if (isset($menu_group_submit)) {
    foreach ($main_menu_group as $menu_group) {
        $group_name = "group_name_" . $menu_group[SETUP_GROUP_ID];
        $group_weight = "group_weight_" . $menu_group[SETUP_GROUP_ID];
        $sql = 'UPDATE `' . SETUP_GROUP_TABLE . '` SET '
                . ' `' . SETUP_GROUP_NAME . '`="' . $$group_name . '" '
                . ' WHERE `' . SETUP_GROUP_ID . '`=' . $menu_group[SETUP_GROUP_ID] . ';';
        $mysqliLink->query($sql);
    }
    logger($member_id, -1, LOGGER_SETUP, 'Kategóriák megnevezésének módosítása.');
}

include INCLUDES . 'setup_load.inc';
$view->submitAdd('menu_group_new', 0, 'Új főmenü létrehozása');
$view->submitSave('menu_group_submit', 0, 'Módosítások mentése');
$sql = 'SELECT MIN(`' . SETUP_GROUP_WEIGHT . '`) AS `min`,'
        . ' MAX(`' . SETUP_GROUP_WEIGHT . '`) AS `max` FROM `' . SETUP_GROUP_TABLE . '`;';
$row = mysqli_fetch_assoc($mysqliLink->query($sql));
$min_weight = $row['min'];
$max_weight = $row['max'];


foreach ($main_menu_group as $menu_group) {
    $view->newDiv('row');
    $view->putHidden('group_weight_' . $menu_group[SETUP_GROUP_ID], $menu_group[SETUP_GROUP_WEIGHT]);
    $var_name = 'group_name_' . $menu_group[SETUP_GROUP_ID];
    $$var_name = $menu_group[SETUP_GROUP_NAME];
    $view->inputText($var_name, $menu_group[SETUP_GROUP_ID]);
    if ($menu_group[SETUP_GROUP_WEIGHT] > $min_weight) {
        $view->submitUp('group_up', $menu_group[SETUP_GROUP_ID]);
    }
    if ($menu_group[SETUP_GROUP_WEIGHT] < $max_weight) {
        $view->submitDown('group_down', $menu_group[SETUP_GROUP_ID]);
    }
    $deletable = true;
    foreach ($admin_menu as $menu_item) {
        if ($menu_item[SETUP_MOD_GROUP] == $menu_group[SETUP_GROUP_ID]) {
            $deletable = false;
        }
    }
    if ($deletable) {
        $view->submitDelete('delete_group', $menu_group[SETUP_GROUP_ID], 'Törlés', 'Biztosan tölni akarod a kategóriát ?');
    }
    $view->endDiv();
}
