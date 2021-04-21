<?php

define("CHECK_MOD_ACCESS", "mod_access_");
$modify_access = array(
    ACCESS_ADMIN, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT, ACCESS_CLUBLEADER, ACCESS_CLUB_DELEGATE, ACCESS_PAID, ACCESS_BRANCH_LEADER,
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
$view->putHidden('access_tab', 'on'); // Az első betöltődéskor nem fut le
$header = ['ID'];
foreach ($modify_access as $modify) {
    $header[] = $access[$modify];
}
$view->newTable('Modulok jogosultságai');
$view->putThead('', $header);
foreach ($admin_menu as $menu) {
    $view->newTableTr();
    $view->putElement('<td>' . $menu[SETUP_MOD_ID] . '</td>');
    foreach ($modify_access as $modify) {
        $view->putElement('<td><input type="checkbox" class="form-check-input mx-3" name="' . CHECK_MOD_ACCESS . $menu[SETUP_MOD_ID] . '_' . $modify . '" '
                . (($menu[SETUP_MOD_ACCESS] & $modify) > 0 ? 'checked' : '') . '  onChange="submit();"></td>');
    }
    $view->endTableTr();
}
$view->endTableTr();