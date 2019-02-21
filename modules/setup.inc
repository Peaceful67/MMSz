<?php

include_once FUNCTIONS . 'setup_functions.inc';

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

if (!isset($radio_setup)) {
    $radio_setup = "category";
}

$output .= '<form method="post">';
foreach ($admin_menu_default as $key => $menu) {// Megnézzük, van-e új menü
    if (!array_key_exists($key, $admin_menu)) {
        $output .= '<label>Új menü pont:</label>'
                . '<input type="submit" name="new_menu" value="' . $key . '"><br><br>';
    }
}
$output .= '<input type="submit" name="reset_menu" value="Alaphelyzet" onClick="return confirm(\'Biztosan vissza akarod állítani ez eredeti kinézetet ?\')">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
$output .= 'Kategóriák:<input type="radio" name="radio_setup" value="category"  onChange="submit();" ' . (($radio_setup == "category") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Menük:<input type="radio" name="radio_setup" value="menu" onChange="submit();" ' . (($radio_setup == "menu") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Jogosultságok:<input type="radio" name="radio_setup" value="access" onChange="submit();" ' . (($radio_setup == "access") ? " checked " : "") . '>';
$output .= '</form>';
include_once INCLUDES . 'setup_load.inc';
$output .= '<form method="post"><br><br><div class="lists">';
$output .= '<input type="hidden" name="radio_setup" value="' . $radio_setup . '"><hr>';
switch ($radio_setup) {
    case "category" :
        include_once INCLUDES . 'setup_category.inc';
        break;
    case "menu" :
        include_once INCLUDES . 'setup_menu.inc';
        break;
    case "access" :
        include_once INCLUDES . 'setup_access.inc';
        break;
}

$output .= '</div></form>';
