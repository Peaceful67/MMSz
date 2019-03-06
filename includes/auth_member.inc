<?php

$now = time();
if (isset($_GET["c"])) {
    $command = new Command();
    $command->getItemOnce($_GET["c"]);
    if ($command->isCommandPassword()) {
        $member_id = $command->mem_id;
        $member_pass = $command->value;
        $login_normal = 'coded';
        $mod = 'profile';
        $radio_edit_member = 'passwd';
        $old_pass = $member_pass;
        $remember_me = "true";
    }
}
if (!isset($member_id)) {
    $member_id = 0;
}
if (!isset($member_pass)) {
    $member_pass = "";
}
if (!isset($chaptha_entered)) {
    $chaptha_entered = "";
}
if (!isset($chaptha_expected)) {
    $chaptha_expected = "";
}
if (isset($password_renew)) {
    return;
}
if (isset($login_normal)) {
    if ($member_id == 0 AND isset($login_email) AND ! empty($login_email)) {
        $member_id = getMemberIdByEmail($login_email);
    }

    if (isset($remember_me) AND $remember_me == "true") {
        setcookie(COOKIE_REMEMBER_ME, "true", $now + TIME_REMEMBER_ME);
        setcookie(COOKIE_MEMBER_ID, $member_id, $now + TIME_REMEMBER_ME);
        setcookie(COOKIE_MEMBER_PASS, $member_pass, $now + TIME_REMEMBER_ME);
    } else {
        setcookie(COOKIE_REMEMBER_ME, "false");
        setcookie(COOKIE_MEMBER_ID, $member_id);
        setcookie(COOKIE_MEMBER_PASS, $member_pass);
    }
} elseif (isset($_COOKIE[COOKIE_MEMBER_ID])) {
    $member_id = $_COOKIE[COOKIE_MEMBER_ID];
    $member_pass = $_COOKIE[COOKIE_MEMBER_PASS];
    if (isset($_COOKIE[COOKIE_REMEMBER_ME]) && $_COOKIE[COOKIE_REMEMBER_ME] == "true") {
        setcookie(COOKIE_REMEMBER_ME, "true", TIME_REMEMBER_ME);
    }
} else {
    $member_id = 0;
    $member_pass = "";
}
$pass_expected = getPassword($member_id);
$pass_given = md5($member_pass . SECRET);
if (($member_id != 0) AND ( $pass_expected != $pass_given)) { // Nem jo a jelszo
    logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Sikertelen belépés, hibás jelszó");
    $member_id = 0;
}
?>
