<?php

include_once FUNCTIONS.'setup_functions.inc';

foreach ($main_menu_group_default as $group_item) {
    $sql = 'INSERT INTO `' . SETUP_GROUP_TABLE . '` ('
            . '`' . SETUP_GROUP_ID . '`, `' . SETUP_GROUP_NAME . '`, `' . SETUP_GROUP_WEIGHT . '` '
            . ' ) VALUES ('
            . '"' . $group_item[SETUP_GROUP_ID] . '", "' . $group_item[SETUP_GROUP_NAME] . '", "' . $group_item[SETUP_GROUP_WEIGHT] . '" '
            . ');';
    $mysqliLink->query($sql);
}
/*
foreach ($admin_menu_default as $menu_item) {
    insert_menu_item($menu_item);
}
*/
/*
foreach ($default_associations as $key => $value) {
    $sql = 'INSERT INTO `' . ASSOCIATIONS_TABLE . '` '
            . '(`' . ASSOCIOATIONS_NAME . '`) VALUES ("' . $value . '")'
            . '(`' . ASSOCIOATIONS_ID . '`) VALUES ("' . $key . '");';
    $mysqliLink->query($sql);
}


foreach ($default_organizations as $key => $value) {
    $sql = 'INSERT INTO `' . ORGANIZATIONS_TABLE . '` '
            . '(`' . ORGANIZATIONS_NAME . '`) VALUES ("' . $value . '")'
            . '(`' . ORGANIZATIONS_ID . '`) VALUES ("' . $key . '");';
    $mysqliLink->query($sql);
}
*/