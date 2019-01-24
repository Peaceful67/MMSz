<?php

foreach ($main_menu_group_default as $group_item) {
    $sql = 'INSERT INTO `' . SETUP_GROUP_TABLE . '` ('
            . '`' . SETUP_GROUP_ID . '`, `' . SETUP_GROUP_NAME . '`, `' . SETUP_GROUP_WEIGHT . '` '
            . ' ) VALUES ('
            . '"' . $group_item[SETUP_GROUP_ID] . '", "' . $group_item[SETUP_GROUP_NAME] . '", "' . $group_item[SETUP_GROUP_WEIGHT] . '" '
            . ');';
    $mysqliLink->query($sql);
}

foreach ($admin_menu_default as $menu_item) {
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
