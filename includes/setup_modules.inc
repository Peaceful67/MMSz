<?php

if (isset($reset_menu)) {
    $sql = 'DELETE FROM `' . SETUP_GROUP_TABLE . '` WHERE 1;';
    $mysqliLink->query($sql);
    $sql = 'DELETE FROM `' . SETUP_MOD_TABLE . '` WHERE 1;';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, 'A menürendszer alaphelyzetbe állítva.');
    include_once INCLUDES . 'setup_init.inc';
    include INCLUDES . 'setup_load.inc';
}

if (isset($new_menu)) {
    include_once INCLUDES.'create_tables.inc';
    if (array_key_exists($new_menu, $admin_menu_default)) {
        insert_menu_item($admin_menu_default[$new_menu]);
        $sql = 'SELECT * FROM `' . SETUP_MOD_TABLE . '`     WHERE 1 ORDER BY `' . SETUP_MOD_WEIGHT . '` ;';
        $admin_menu = array();
        $result = $mysqliLink->query($sql);
        if ($result) {
            while ($row = mysqli_fetch_assoc($result)) {
                $admin_menu[$row[SETUP_MOD_ID]] = $row;
            }
        }
    }
}
$view->submitButton('reset_menu', 'Alaphelyzet', 0, 'A menük alaphelyzetbe állítása. Minden beállítás elveszik.', 'Biztosan vissza akarod állítani ez eredeti kinézetet ?');
$view->newDiv('form-inline');
$view->putLabel('Bekapcsolható modulok');
foreach ($admin_menu_default as $key => $menu) {// Megnézzük, van-e új menü
    if (!array_key_exists($key, $admin_menu)) {
        $view->submitButton('new_menu', $key, $menu[SETUP_MOD_MENU], $menu[SETUP_MOD_TITLE]);    
    }
}
$view->endDiv();
