<?php

include_once FUNCTIONS . 'setup_functions.inc';

if (!isset($radio_setup)) {
    $radio_setup = "category";
}
$view->putHidden('radio_setup');
$view->newTab();
$view->putTabItem('radio_setup', 'modules', 'Modulok', '');
$view->putTabItem('radio_setup', 'category', 'Főmenük', '');
$view->putTabItem('radio_setup', 'menu', 'Almenük', '');
$view->putTabItem('radio_setup', 'access', 'Jogosultságok', '');
$view->endTab();
include_once INCLUDES . 'setup_load.inc';
$view->newKeret();
switch ($radio_setup) {
    case "modules":
        include_once INCLUDES . 'setup_modules.inc';
        break;        
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
$view->endKeret();
