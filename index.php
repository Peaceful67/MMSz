<?php

include_once 'params.inc';
include_once FUNCTIONS . 'init.inc';

$output = '<!DOCTYPE  html>
<head>        <meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1">
      <link type="text/css" rel="stylesheet" media="screen" href="design/horizontal.css">
        </script>';
if (CHAPTCHA_METHOD_GOOGLE == $chaptcha) {
    $output .= '<script src="https://www.google.com/recaptcha/api.js" async defer></script>';
}
$output .= '</head><body>

';

if (isset($_GET["sp_timeout"]) AND $_GET["sp_timeout"] == 1 AND isset($_GET["order_ref"])) {
    logger(0, -1, LOGGER_SIMPLEPAY, 'Visszatérés az OTP felületről a ' . $_GET["order_ref"]
            . ' azonosítójú számlák befizetés megszakítva.');
    stornoBillNumber($_GET["order_ref"]);
    redirect("sp_timeout");
    exit(0);
}
if (isset($_GET["init"]) AND $_GET["init"] == 'create') {
    /*
      include_once INCLUDES . 'create_tables.inc';
      $member_id = $edit_member = -1;
      $mod = 'new_member';
      $permission = ACCESS_ADMIN;
     */
} else {
    if (CHAPTCHA_METHOD_GOOGLE == $chaptcha) {
        include_once FUNCTIONS . 'rechaptchalib.php';
        $reCaptcha = new ReCaptcha(GOOGLE_SECRET_KEY);
        $resp = $reCaptcha->verifyResponse($_SERVER['REMOTE_ADDR'], $_POST['g-recaptcha-response']);
        $output .= " json_encode(array(
    'valid'   => $resp->success,
    'message' => $resp->success ? null  : 'Hey, the captcha is wrong!',   
    ))";
    }
    include_once INCLUDES . 'auth_member.inc';
    $permission = getPermission($member_id);
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
if (!isset($sel_group)) {
    $sel_group = -3;
}
push_mod($mod);

$output .= '<form method="post">';
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

$output .= '</form>';

$output .= '<div class="torzs">';
if (isset($_GET['simplepay'])) {
    include SIMPLEPAY . 'back.inc';
    redirect("card_backref");
    exit(0);
} elseif (isset($mod)) {
    if ($mod == $admin_menu["login"][SETUP_MOD_ID]) {
        $output .= '<h2>' . $admin_menu["login"][SETUP_MOD_TITLE] . '</h2>';
        include_once (MODULES . $admin_menu["login"][SETUP_MOD_MODULE]);
    } elseif ($mod == $admin_menu["gdpr"][SETUP_MOD_ID]) {
        $output .= '<h2>' . $admin_menu["gdpr"][SETUP_MOD_TITLE] . '</h2>';
        include_once (MODULES . $admin_menu["gdpr"][SETUP_MOD_MODULE]);
    } else {
        foreach ($admin_menu as $k => $v) {
            if (($mod == $v[SETUP_MOD_ID]) && isPermitted($v[SETUP_MOD_ACCESS])) {
                $output .= '<h2>' . $v[SETUP_MOD_TITLE] . '</h2>';
                include_once (MODULES . $v[SETUP_MOD_MODULE]);
                break;
            }
        }
    }
}
$output .= '</div >';



$output .= '</body>
            </html>';

theEnd($output);
?>

