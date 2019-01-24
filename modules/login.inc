<?php

if (!isset($chaptha_entered)) {
    $chaptha_entered = "";
}
if (!isset($chaptha_expected)) {
    $chaptha_expected = "";
}
if (isset($login_normal)) {
    if ($member_id == 0) {
        warning("Jelszó hiba!");
        session_unset();
        unset($login_normal);
    } elseif ($chaptha_entered == $chaptha_expected) {
        logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Sikeres belépés");
        unset($login_normal);
        if (isGdpr($member_id)) {
            redirect("profile");
            exit;
        } else {
            redirect("gdpr");
            exit;
        }
    } elseif (CHAPTCHA_METHOD_GOOGLE == $chaptcha) {
        logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Sikeres belépés");
        unset($login_normal);
        if (isGdpr($member_id)) {
            redirect("profile");
            exit;
        } else {
            redirect("gdpr");
            exit;
        }
    } else {
        logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Sikertelen belépés, robotellenőrzési hiba");
        warning("Robot ellenőrzési hiba !");
        session_unset();
        unset($login_normal);
    }
}
if (isset($password_renew)) {
    if (isset($member_id) AND $member_id > 0) {
        $password = randomPassword();
        $record = retriveMember($member_id);
        if ($record[MEMBER_EMAIL] == '') {
            warning("A megadott azonosítóhoz nem tartozik email cím!");
        } elseif (0 == strcasecmp(trim($record[MEMBER_EMAIL]), $member_email)) {
            $body = 'Az ' . COMPANY_NAME . ' adminisztrációs felületen generált jelszavad: ' . $password . '<br>';
            $body .= 'Az ' . COMPANY_NAME . ' azonosítód: ' . $member_id . '<br>';
            $body .= "Ezzel belépve, a profilod beállításainál a jelszavad megváltoztathatod.<br>";
            $subject = "Generált jelszó";
            send_mail($subject, $body);
            setPassword($member_id, $password);
            logger($member_id, $member_id, LOGGER_MEMBER_MOD, "Sikeres jelszó módosítás, emailben kiküldve");
            redirect("login");
        } else {
            warning("A megadott email cím hibás!");
        }
    } else {
        warning("Az azonosító mezőt ki kell tölteni !");
    }
}


$output .= '<form method="post" autocomplete="off"><div class="member_id">
    <label for="id">' . COMPANY_NAME . ' azonosító: </label>
    <input type="hidden" name="mod" value="' . $mod . '">
    <input type="number" max="9999" name="member_id" value="' . $member_id . '" >&nbsp;&nbsp;vagy&nbsp;&nbsp;';
$output .= '<label>Email :</label><input type="text"  name="login_email" ><br><br>
    <label>Jelszó: </label>
    <input type="password" name="member_pass" value="' . $member_pass . '"></div><br>';
if (CHAPTCHA_METHOD_GOOGLE == $chaptcha) {
    $output .= '<div class="g-recaptcha" data-sitekey="6Le3hYEUAAAAAIISplLOnO3FVKedc8A3lnT1bmyM"></div><br>';
} else {
    $output .= '<input type="hidden" name="chaptha_expected" value="' . $chaptha_number . '">
<label for="chaptha_entered">Írd ide, mennyi ' . $chaptha_text . ': </label><input type="text" size="4" name="chaptha_entered" value="">';
}

$output .= '    <input type="submit" name="login_normal" value="Belépés">
&nbsp;&nbsp;&nbsp;&nbsp;
Emlékezz rám: <input type="checkbox" name="remember_me" value="true" ' .
        ((isset($remember_me) AND $remember_me == "true") ? "checked" : "") . '><hr><br>
Amennyiben még nincs jelszavad vagy elfelejtetted, <br>add meg az ' . COMPANY_NAME . ' azonosítódat és az emailcímedet<br>
Az email címednek szerepelnie kell az adatbázisban !<br>
<label for="email">Email cím: </label>
<input type="text" name="member_email" value="" id="email">  
   <input type="submit" name="password_renew" value="Jelszó generálás">
</form> ';
?>