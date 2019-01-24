<?php
logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Kilépés");
session_unset();
setcookie(COOKIE_REMEMBER_ME, "false");
setcookie(COOKIE_MEMBER_ID, "");
setcookie(COOKIE_MEMBER_PASS, "");
unset($member_id);
unset($member_pass);
header("Location: ".PROGI);
 
?>
