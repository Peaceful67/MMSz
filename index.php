<?php
include_once 'params.inc';
include_once FUNCTIONS . 'init.inc';


$output = '<!DOCTYPE  html>
<head>        
    <meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' Tagnyilvántartó, Verzió:' . VERSION . '">
    <meta name="author" content="Baksa Zsolt (peaceful.app@gmail.com)">';

$output .= '<link type="text/css" rel="stylesheet" media="screen" href="design/horizontal.css">';

$output .= '</head><body>';
if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0 AND ! is_writable(SZAMLAZZ_DIR)) {
    warning("<br><br>A számlák könyvtár nem írható");
}

if (isset($_GET["sp_timeout"]) AND $_GET["sp_timeout"] == 1 AND isset($_GET["order_ref"])) {
    logger(0, -1, LOGGER_SIMPLEPAY, 'Visszatérés az OTP felületről a ' . $_GET["order_ref"]
            . ' azonosítójú számlák befizetés megszakítva.');
    stornoBillNumber($_GET["order_ref"]);
    redirect("sp_timeout");
    exit(0);
}

if (isset($_GET) AND isset($_GET["c"])) {
    $command = new Command('');
    $command->getItemOnce($_GET["c"]);
}
include_once INCLUDES . 'auth_member.inc';

$permission = getPermission($member_id);
if (isPermitted(ACCESS_ADMIN)) {
    if (isset($_SESSION[SESSION_ROLE_AS]) AND $_SESSION[SESSION_ROLE_AS] > 0) {
        $member_id = $_SESSION[SESSION_ROLE_AS];
        $permission = getPermission($member_id);
        error_log('Admin vagyok, de ez akarok lenni: ' . $member_id . ' jogok: ' . $permission);
    }
}

retriveSession();

if (isset($mod)) {
    reset_mod();
}

if (!isset($mod) && isset($_SESSION) && isset($_SESSION[SESSION_MOD])) {
    $mod = $_SESSION[SESSION_MOD];
}

if ($member_id == 0) {
    $mod = $admin_menu["login"][SETUP_MOD_ID];
}
if (isset($mod)) {
    $_SESSION[SESSION_MOD] = $mod;
} else {
    $mod = $admin_menu["login"][SETUP_MOD_ID];
    $login_normal = "Belépés";
}
if ($mod == $admin_menu["gdpr"][SETUP_MOD_ID]) {
    $permission = ACCESS_PUBLIC;
}
push_mod($mod);
if (isset($sel_group)) {
    $mod = "";
} else {
    $sel_group = -3;
}

$output .= '<form method="post" enctype="multipart/form-data">';
$output .= '<div class="control">';
foreach ($main_menu_group as $group) {

    $selectable = false; // Van-e bármi megjeleníthető    
    foreach ($admin_menu as $k => $v) {
        if (($v[SETUP_MOD_GROUP] == $group[SETUP_GROUP_ID]) && isPermitted($v[SETUP_MOD_ACCESS])) {
            $selectable = true;
        }
    }
    if ($selectable) {  // Van !!!
        $output .= '';
        $output .= '<button name="sel_group" value="' . $group[SETUP_GROUP_ID] . '">' . $group[SETUP_GROUP_NAME] . '</button>';
        $output .= '';
    }
}

foreach ($admin_menu as $k => $v) {
    if (($v[SETUP_MOD_GROUP] == -1) && isPermitted($v[SETUP_MOD_ACCESS]) && $v[SETUP_MOD_ENABLED] == 1) {
        $output .= '<button name="mod" value="' . $v[SETUP_MOD_ID] . '" >' . $v[SETUP_MOD_MENU] . '</button>';
    }
}

$output .= '';
$output .= '</div >';
$output .= '<div class="sub_control">';
if ($mod != $admin_menu["gdpr"][SETUP_MOD_ID]) {
    foreach ($admin_menu as $k => $v) {
        if (($sel_group == $v[SETUP_MOD_GROUP]) && isPermitted($v[SETUP_MOD_ACCESS]) && $v[SETUP_MOD_ENABLED] == 1) {
            $output .= '<button name="mod" value="' . $v[SETUP_MOD_ID] . '" >' . $v[SETUP_MOD_MENU] . '</button>';
        }
    }
}
$output .= '</div>';


$output .= '<div class="torzs">';
if (isset($_GET['simplepay'])) {
    include SIMPLEPAY . 'back.inc';
    redirect("card_backref");
    exit(0);
} elseif (isset($mod)) {
    if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
        warning("Fejlesztő állapot (" . $member_id . ")");
    }
    if ($mod == $admin_menu["login"][SETUP_MOD_ID]) {
        $output .= '<h2>' . $admin_menu["login"][SETUP_MOD_TITLE] . '</h2>';
        include_once (MODULES . $admin_menu["login"][SETUP_MOD_MODULE]);
    } elseif ($mod == $admin_menu["gdpr"][SETUP_MOD_ID]) {
        $output .= '<h2>' . $admin_menu["gdpr"][SETUP_MOD_TITLE] . '</h2>';
        include_once (MODULES . $admin_menu["gdpr"][SETUP_MOD_MODULE]);
    } else {
        foreach ($admin_menu as $k => $v) {
            if (($mod == $v[SETUP_MOD_ID]) && isPermitted($v[SETUP_MOD_ACCESS])) {
                o('<h2>' . $v[SETUP_MOD_TITLE]);
                if (getOptionValue(OPTIONS_NAME_HELP_ENABLED) > 0) {
                    $help = new Help($mod);
                    if ($help->isHelpExiting()) {
                        output_spaces(5);
                        o('<div class="tooltip">');
                        o('<img style="height: 15px; width:15px; padding: 0px;" src="' . IMAGES . 'help.png">');
                        o('<span style="width: 450px;" class="tooltiptext">');
                        o($help->getTitle());
                        output_ln();
                        o($help->getDescription());
                        o('</span></div>');
                    }
                }
                o('</h2>');
                include_once (MODULES . $v[SETUP_MOD_MODULE]);
                break;
            }
        }
    }
}
$output .= '</div >';
$output .= '</form>';


$output .= '</body>
            </html>';

theEnd($output);

include_once INCLUDES . 'crontab.inc';

