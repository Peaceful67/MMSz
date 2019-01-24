<?php

define("CHECK_MOD_ACCESS", "mod_access_");
$modify_access = array(
    ACCESS_ADMIN, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT, ACCESS_CLUBLEADER, ACCESS_CLUB_DELEGATE, ACCESS_PAID, ACCESS_CATEGORY_LEADER,
);

if (isset($access_tab)) {
    foreach ($admin_menu as $menu) {
        $menu_access = $menu[SETUP_MOD_ACCESS];
        foreach ($modify_access as $modify) {
            $chk = CHECK_MOD_ACCESS . $menu[SETUP_MOD_ID] . '_' . $modify;
            if (isset($$chk) && $$chk == 'on') {
                $menu_access |= $modify;
            } else {
                $menu_access &= ~$modify;
            }
        }
        if ($menu[SETUP_MOD_ACCESS] != $menu_access) {
            $sql = 'UPDATE `' . SETUP_MOD_TABLE . '` SET '
                    . '`' . SETUP_MOD_ACCESS . '`=' . $menu_access . ' '
                    . ' WHERE `' . SETUP_MOD_ID . '`="' . $menu[SETUP_MOD_ID] . '";';
            $mysqliLink->query($sql);
            logger($member_id, -1, LOGGER_SETUP, $menu[SETUP_MOD_ID] . ' azonosítójú menu új jogosultságai: ' . $menu_access);
        }
    }
} 

include INCLUDES . 'setup_load.inc';
$output .= '<input type="hidden" name="access_tab" value="on">';
$output .= '<table>'
        . '<col width="16%"><col width="12%"><col width="12%"><col width="12%"><col width="12%"><col width="12%"><col width="12%"><col width="12%">'
        . '<tr><th>ID</th>';
foreach ($modify_access as $modify) {
    $output .= '<th>' . $access[$modify] . '</th>';
}
$output .= '</tr>';
foreach ($admin_menu as $menu) {
    $output .= '<tr>';
    $output .= '<td>' . $menu[SETUP_MOD_ID] . '</td>';
    foreach ($modify_access as $modify) {
        $output .= '<td><input type="checkbox" name="' . CHECK_MOD_ACCESS . $menu[SETUP_MOD_ID] . '_' . $modify . '" '
                . (($menu[SETUP_MOD_ACCESS] & $modify) > 0 ? 'checked' : '') . '  onChange="submit();"></td>';
    }
    $output .= '</tr>';
}
$output .= '</table>';
