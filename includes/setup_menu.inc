<?php

define("CHECKBOX_NAME", 'mod_ena_');
define("MENU_NAME", 'menu_name_');
define("MENU_TITLE", 'menu_title_');
define("MENU_WEIGHT", 'menu_weight_');
define("MENU_GROUP", 'menu_group_');

if (isset($_POST['chkboxes'])) {
    $chkboxes = $_POST['chkboxes'];
}
if (isset($menu_submit)) {
    foreach ($admin_menu as $menu) {
        $name = MENU_NAME . $menu[SETUP_MOD_ID];
        $title = MENU_TITLE . $menu[SETUP_MOD_ID];

        if (isset($$name) && $$name != $menu[SETUP_MOD_MENU]) {
            $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
                    . '`' . SETUP_MOD_MENU . '`="' . $$name . '" '
                    . ' WHERE `' . SETUP_MOD_ID . '`="' . $menu[SETUP_MOD_ID] . '";';
            logger($member_id, -1, LOGGER_SETUP, $menu[SETUP_MOD_ID] . ' azonosítójú menü neve módosítva.');
            $mysqliLink->query($sql);
        }
        if (isset($$title) && $$title != $menu[SETUP_MOD_TITLE]) {
            $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
                    . '`' . SETUP_MOD_TITLE . '`="' . $$title . '" '
                    . ' WHERE `' . SETUP_MOD_ID . '`="' . $menu[SETUP_MOD_ID] . '";';
            $mysqliLink->query($sql);
            logger($member_id, -1, LOGGER_SETUP, $menu[SETUP_MOD_ID] . ' azonosítójú menü címe módosítva.');
        }
    }
}
if (isset($menu_down)) {
    $md = 'menu_weight_' . $menu_down;
    $menu_weight = $$md;
    $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
            . ' `' . SETUP_MOD_WEIGHT . '`=' . $menu_weight
            . ' WHERE  `' . SETUP_MOD_WEIGHT . '`=' . ($menu_weight + 1) . ' ;';
    $mysqliLink->query($sql);
    $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
            . ' `' . SETUP_MOD_WEIGHT . '`=' . ($menu_weight + 1)
            . ' WHERE  `' . SETUP_MOD_ID . '`="' . $menu_down . '" ;';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, $menu_down . ' azonosítójú menü hátrébb helyezve.');
}
if (isset($menu_up)) {
    $md = 'menu_weight_' . $menu_up;
    $menu_weight = $$md;
    $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
            . ' `' . SETUP_MOD_WEIGHT . '`=' . $menu_weight
            . ' WHERE  `' . SETUP_MOD_WEIGHT . '`=' . ($menu_weight - 1) . ' ;';
    $mysqliLink->query($sql);
    $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
            . ' `' . SETUP_MOD_WEIGHT . '`=' . ($menu_weight - 1)
            . ' WHERE  `' . SETUP_MOD_ID . '`="' . $menu_up . '" ;';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, $menu_up . ' azonosítójú menu előrébb helyezve.');
}
if (isset($menu_delete)) {
    $sql = 'DELETE FROM `' . SETUP_MOD_TABLE . '` WHERE `' . SETUP_MOD_ID . '`="' . $menu_delete . '" ;';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_SETUP, $menu_delete . ' azonosítójú menu törlése.');
}
foreach ($admin_menu as $menu) {
    $group = MENU_GROUP . $menu[SETUP_MOD_ID];
    if (isset($$group) && $$group != $menu[SETUP_MOD_GROUP]) {
        $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
                . '`' . SETUP_MOD_GROUP . '`=' . $$group . ' '
                . ' WHERE `' . SETUP_MOD_ID . '`="' . $menu[SETUP_MOD_ID] . '";';
        $mysqliLink->query($sql);
    }
    if (isset($chkboxes)) {
        unset($ena);    // Toroljuk az elozo ciklusbol jovot
        if (in_array($menu[SETUP_MOD_ID], $chkboxes)) {
            if (1 != $menu[SETUP_MOD_ENABLED]) { // De nem volt
                $ena = 1;
            }
        } else {
            if (1 == $menu[SETUP_MOD_ENABLED]) { // De volt
                $ena = 0;
            }
        }

        if (isset($ena) && empty($menu[SETUP_MOD_DEPENDENCY])) { // Van valtoztatni valo
            $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
                    . '`' . SETUP_MOD_ENABLED . '`=' . $ena . ' '
                    . ' WHERE `' . SETUP_MOD_ID . '`="' . $menu[SETUP_MOD_ID] . '";';
            $mysqliLink->query($sql);
            logger($member_id, -1, LOGGER_SETUP, $menu[SETUP_MOD_ID] . ' azonosítójú menu ' .
                    ($ena == 1 ? 'engedélyezve' : 'tiltva'));
        }
    }
}
include INCLUDES . 'setup_load.inc';

$output .= '<br><br>';

$sql = 'SELECT MIN(`' . SETUP_MOD_WEIGHT . '`) AS `min`,'
        . ' MAX(`' . SETUP_MOD_WEIGHT . '`) AS `max` FROM `' . SETUP_MOD_TABLE . '`;';
$row = mysqli_fetch_assoc($mysqliLink->query($sql));
$min_weight = $row['min'];
$max_weight = $row['max'];
$output .= '<table>'
        . '<col width="5%"><col width="15%"><col width="40%"><col width="25%"><col width="25%">'
        . '<tr><th>Eng.</th><th>ID</th><th>Név/Cím <input type="submit" name="menu_submit" value="Mentés"></th><th>Elhelyezkedés</th><th>Műveletek</th></tr>';
foreach ($admin_menu as $menu) {
    $deleteable = true;
    $output .= '<input type="hidden" name="' . MENU_WEIGHT . $menu[SETUP_MOD_ID] . '" value="' . $menu[SETUP_MOD_WEIGHT] . '">';
    $output .= '<tr>';
    $output .= '<td><input type="checkbox" name="chkboxes[]" value="' . $menu[SETUP_MOD_ID] . '" onChange="submit();" ';
    if ($menu[SETUP_MOD_DEPENDENCY] != "") {
        $deleteable = false;
        $dependecy = $menu[SETUP_MOD_DEPENDENCY];
        $output .= ' disabled title="' . $dependecy . '" ';
        if (array_key_exists($dependecy, $admin_menu)) {
            $output .= ($admin_menu[$dependecy][SETUP_MOD_ENABLED] == 1) ? ' checked  ' : '  ';
        } else {
            $output .= ($menu[SETUP_MOD_ENABLED] == 1) ? ' checked  ' : ' ';
        }
    } else {
        $output .= ($menu[SETUP_MOD_ENABLED] == 1) ? ' checked ' : '  ';
    }
    $output .= '></td>';
    $output .= '<td>' . $menu[SETUP_MOD_ID] . '</td>';
    $output .= '<td><input type="text" size="16" maxlength="32" name="' . MENU_NAME . $menu[SETUP_MOD_ID] . '" value="' . $menu[SETUP_MOD_MENU] . '">'
            . '<input type="text" size="32" maxlength="64" name="' . MENU_TITLE . $menu[SETUP_MOD_ID] . '" value="' . $menu[SETUP_MOD_TITLE] . '"></td>';
    $output .= '<td align="center">' . getListCategories($menu[SETUP_MOD_ID], $menu[SETUP_MOD_GROUP]) . '</td>';
    $output .= '<td>';
    if ($menu[SETUP_MOD_WEIGHT] > $min_weight) {
        $output .= '<button  type="submit" name="menu_up" value="' . $menu[SETUP_MOD_ID] . '" >'
                . '<img src="' . IMAGES . 'up.jpg" title="Fel"></button>';
    }

    if ($menu[SETUP_MOD_WEIGHT] < $max_weight) {
        $output .= '<button  type="submit" name="menu_down" value="' . $menu[SETUP_MOD_ID] . '" >'
                . '<img src="' . IMAGES . 'down.jpg" title="Le"></button>';
    }
    if ($deleteable) {
        $output .= '<button  type="submit" name="menu_delete" value="' . $menu[SETUP_MOD_ID] . '" >'
                . '<img src="' . IMAGES . 'delete.jpg" title="Törlés"></button>';
    }
    $output .= '</td>';
    $output .= '</tr>';
}
$output .= '</table>';

