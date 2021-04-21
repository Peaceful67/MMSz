<?php

include_once 'params.inc';
include_once FUNCTIONS . 'init.inc';
if (isset($mod)) {
    reset_history();
}

$head = '<!DOCTYPE html>
<html lang="hu">
<head>        
    <title>nyilvántartó</title>
    <meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
    <meta name="description" content="' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' Tagnyilvántartó, Verzió:' . VERSION . '"/>
    <meta name="author" content="Baksa Zsolt (peaceful.app@gmail.com)"/>
    <link type="text/css" rel="stylesheet"  href="design/documents.css"/>  
    <link type="text/css" rel="stylesheet" href="design/css/bootstrap.css"/>
    <link type="text/css" rel="stylesheet"  href="design/navbar.css"/>
    <link rel="stylesheet" href="design/font-awesome-4.7.0/css/font-awesome.min.css"/>
    <link href="design/css/bootstrap4-toggle.css" rel="stylesheet"/>
    <link type="text/css" rel="stylesheet"  href="design/horizontal.css"/>
    <link type="text/css" rel="stylesheet" href="design/general.css"/>
    <link rel="icon" href="images/favicon.ico" type="image/png">
    <script>
    function onPageLoaded() {
        $(\'#loading\').hide();
    }
    </script>
    <script src="design/js/functions.js"></script>
    <script src="design/js/jquery-3.6.0.slim.min.js" ></script>
    <script src="design/js/bootstrap.bundle.min.js" ></script>
    <script src="design/js/bootstrap4-toggle.js"></script>
    <script src="design/js/bootstrap.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous""></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.2/modernizr.js"></script>
</head>
<body>
';
// theHeader($head);
$view = new View();
$output = $head;

foreach ($uries as $param => $func) {  // Paramétertől függő függvényeket hívunk
    if (isset($_GET[$param])) {
        $func($_GET[$param]);
        break;
    }
}
if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0 AND ! is_writable(SZAMLAZZ_DIR)) {
    $view->danger("A számlák könyvtár nem írható");
}

if (isset($_GET["sp_timeout"]) AND $_GET["sp_timeout"] == 1 AND isset($_GET["order_ref"])) {
    logger(0, -1, LOGGER_SIMPLEPAY, 'Visszatérés az OTP felületről a ' . $_GET["order_ref"]
            . ' azonosítójú számlák befizetés megszakítva.');
    stornoBillNumber($_GET["order_ref"]);
    redirect("sp_timeout");
    exit(0);
}
if (filter_has_var(INPUT_GET, "c")) {
    $getCommand = new Command();
    $called_by_command = $getCommand->getItemOnce($_GET["c"]);
} else {
    $called_by_command = false;
}

if (isset($_GET['simplepay'])) {
    include SIMPLEPAY . 'back.inc';
    redirect("card_backref");
    exit(0);
}
if (!$called_by_command) {
    include_once INCLUDES . 'auth_member.inc';
} elseif (!isset($member_id)) {
    $member_id = 0;
}
if (!isset($member_pass)) {
    $member_pass = "";
}
$permission = getPermission($member_id);
if (isPermitted(ACCESS_ADMIN)) {
    if (isset($_SESSION[SESSION_ROLE_AS]) AND $_SESSION[SESSION_ROLE_AS] > 0) {
        $member_id = $_SESSION[SESSION_ROLE_AS];
        $permission = getPermission($member_id);
        error_log('Admin vagyok, de ez akarok lenni: ' . $member_id . ' jogok: ' . $permission);
    }
}

retriveSession();
if (isset($back_to_history)) {
    pop_history();
} else {
    get_new_values();
}
if (isset($mod)) {
    reset_mod();
}

if (!isset($mod) && isset($_SESSION) && isset($_SESSION[SESSION_MOD])) {
    $mod = $_SESSION[SESSION_MOD];
}

if (!isGdpr($member_id) AND ! $called_by_command) {
    if (isset($mod)) {
        push_mod($mod);
    }
    $mod = 'gdpr';
}

if ($member_id == 0) {
    $mod = $admin_menu["login"][SETUP_MOD_ID];
}

if (isset($mod)) {
    $_SESSION[SESSION_MOD] = $mod;
    push_mod($mod);
} else {
    $mod = $admin_menu["login"][SETUP_MOD_ID];
    $login_normal = "Belépés";
}

if ($mod == $admin_menu["gdpr"][SETUP_MOD_ID]) {
    $permission = ACCESS_NOBODY;
}

if (!$called_by_command AND $mod != $admin_menu["gdpr"][SETUP_MOD_ID]) {
    $view->newElement('<form action="/" method="post">');
    $view->newElement('<nav class="navbar navbar-expand-md navbar-light fixed-top bg-info" role="navigation">');
    $view->newDiv('container');
    $view->putElement('<a class="navbar-brand" href="" title="Tagnyilvántartó">' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . '</a>');
    $view->newElement('<button class="navbar-toggler navbar-toggler-right border-0" type="button" data-toggle="collapse" data-target="#navbar">');
    $view->putElement('<span class="navbar-toggler-icon"></span>');
    $view->endElement('</button>');
    $view->newDiv('collapse navbar-collapse', 'navbar');
    $view->newElement('<ul class="navbar-nav">');
    foreach ($main_menu_group as $group) {

        $selectable = false; // Van-e bármi megjeleníthető    
        foreach ($admin_menu as $k => $v) {
            if (($v[SETUP_MOD_GROUP] == $group[SETUP_GROUP_ID]) && isPermitted($v[SETUP_MOD_ACCESS])) {
                $selectable = true;
            }
        }
        if ($selectable) {  // Van !!!
            $view->newElement('<li class="nav-item dropdown">');
            $view->putElement('<button class="dropbtn bg-info" id="group_' . $group[SETUP_GROUP_ID] . '" >' . $group[SETUP_GROUP_NAME] . '</button>');
            $view->newDiv('dropdown-content');
            foreach ($admin_menu as $k => $v) { // Kirakjuk az ehhez a csoporthoz tartozó funkió gombokat
                if (($v[SETUP_MOD_GROUP] == $group[SETUP_GROUP_ID]) && isPermitted($v[SETUP_MOD_ACCESS])) {
                    $view->putElement('<button class="btn btn-default" name="mod" value="' . $v[SETUP_MOD_ID] . '" title="' . $v[SETUP_MOD_TITLE] . '">' . $v[SETUP_MOD_MENU] . '</button>');
                }
            }
            $view->endDiv();
            $view->endElement('</li>');
        }
    }
    foreach ($admin_menu as $k => $v) { // Fomenuben lévő funkciók
        if (($v[SETUP_MOD_GROUP] == -1) && isPermitted($v[SETUP_MOD_ACCESS]) && $v[SETUP_MOD_ENABLED] == 1) {
            $onclick = $v[SETUP_MOD_ID] == 'logout' ? ' onClick="return confirm(\'Biztosan ki akarsz lépni ?\');"' : '';
            $view->newElement('<li class="nav-item bg-info">');
            $view->putElement('<button class="btn btn-info"  name="mod" value="' . $v[SETUP_MOD_ID] . '" ' . $onclick . ' title="' . $v[SETUP_MOD_TITLE] . '">' . $v[SETUP_MOD_MENU] . '</button>');
            $view->endElement('</li>');
        }
    }

    $view->endElement('</ul>');
    $view->endDiv();
    $view->endDiv();
    $view->endElement('</nav>');
    $view->endElement('</form>');
}


$view->newDiv('container');
$view->newDiv('torzs');
if (isset($mod)) {
    if (!isPermitted($admin_menu[$mod][SETUP_MOD_ACCESS])) { // Mégsincs joga hozzá, valahogy meghackeltek
        $mod = 'login';
    }
    $view->newElement('<form action="/" method="post" id="MainForm" role="main" enctype="multipart/form-data" autocomplete="on">');
    if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
        $view->warning("Fejlesztői állapot (" . $member_id . ")");
    }
    if ($mod == $admin_menu["login"][SETUP_MOD_ID]) {
        $view->putElement('<h3>' . $admin_menu["login"][SETUP_MOD_TITLE] . '</h3>');
        include_once (MODULES . $admin_menu["login"][SETUP_MOD_MODULE]);
    } elseif ($mod == $admin_menu["gdpr"][SETUP_MOD_ID]) {
        $view->putElement('<h3>' . $admin_menu["gdpr"][SETUP_MOD_TITLE] . '</h3>');
        include_once (MODULES . $admin_menu["gdpr"][SETUP_MOD_MODULE]);
    } else {
        foreach ($admin_menu as $k => $v) {
            if (($mod == $v[SETUP_MOD_ID]) && isPermitted($v[SETUP_MOD_ACCESS])) {
                $view->putElement('<h3>' . $v[SETUP_MOD_TITLE] . '</h3>');
                include_once (MODULES . $v[SETUP_MOD_MODULE]);
                break;
            }
        }
    }
    $view->endElement('</form>');
}
$view->endDiv(); // törzs
$view->endDiv(); // container
$view->putElement('</body></html>');

theEnd($output);

include_once INCLUDES . 'crontab.inc';

