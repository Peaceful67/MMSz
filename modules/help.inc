<?php

if (isset($save_help) AND isset($selected_module_help) AND isset($help_title) AND isset($help_description)) {
    $help = new Help($selected_module_help);
    if ($help->getTitle() != $help_title) {
        $help->setTitle($help_title);
        logger($member_id, -1, LOGGER_HELP, $selected_module_help . 'modul leírás címének módosítása');
    }
    if ($help->getDescription() != $help_description) {
        $help->setDescription($help_description);
        logger($member_id, -1, LOGGER_HELP, $selected_module_help . 'modul leírásának módosítása');
    }
}
o('<form method="post">');
output_label('Modul neve');
$menu_arr = array();
foreach ($admin_menu as $key => $menu) {
    if ($menu[SETUP_MOD_ENABLED] == 1 AND strlen($menu[SETUP_MOD_MENU]) > 0
            AND $menu[SETUP_MOD_ACCESS] > 0 AND $menu[SETUP_MOD_GROUP] > -2) {
        $menu_arr[$key] = $menu[SETUP_MOD_MENU];
    }
}

output_selector($menu_arr, 'module_help', true);
if ($selected_module_help == -1) {
    o('</form>');
    return;
}
$help = new Help($selected_module_help);

o('<div class="editor keret">');
output_ln();
output_label('Modul címe');
o($admin_menu[$selected_module_help][SETUP_MOD_TITLE]);
output_ln();
output_label('Utoljára módosítva');
o($help->getUpdated());
output_ln();
output_label('Leírás címe');
o('<input type="text" name="help_title" value="' . $help->getTitle() . '">');
output_ln();
output_label('Leírás');
o('<textarea rows="15" cols="100" maxlength="10000" name="help_description">' . $help->getDescription() . '</textarea>');
output_ln();
output_save('save_help');
o('</div>');
o('</form>');
