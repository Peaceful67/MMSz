<?php

if (!isset($chaptha_entered)) {
    $chaptha_entered = "";
}
if (!isset($chaptha_expected)) {
    $chaptha_expected = "";
}
if (!isset($passwd_id)) {
    $passwd_id = 0;
}
if (isset($login_normal)) {
    if ($member_id == 0) {
        warning("Jelszó hiba!");
        session_unset();
        unset($login_normal);
    } else {
        switch ($chaptcha) {
            case CHAPTCHA_METHOD_GOOGLE:
                logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Sikeres belépés");
                unset($login_normal);
                if (isGdpr($member_id)) {
                    redirect("profile");
                    exit;
                } else {
                    redirect("gdpr");
                    exit;
                }
                break;
            case CHAPTCHA_METHOD_MUTLIPLE:
                if ($chaptha_entered == $chaptha_expected) {
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
                break;
            case CHAPTCHA_METHOD_NOTHING:
                logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Sikeres belépés");
                unset($login_normal);
                if (isGdpr($member_id)) {
                    redirect("profile");
                    exit;
                } else {
                    redirect("gdpr");
                    exit;
                }

                berak;
        }
    }
}
if (isset($password_renew)) {
    if (isset($passwd_id) AND $passwd_id > 0) {
        $member_id = $passwd_id;
        $password = randomPassword();
        $record = retriveMember($passwd_id);
        if ($record[MEMBER_EMAIL] == '') {
            warning("A megadott azonosítóhoz nem tartozik email cím!");
        } elseif (0 == strcasecmp(trim($record[MEMBER_EMAIL]), $member_email)) {
            $body = 'Az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' adminisztrációs felületen generált jelszavad: ' . $password . '<br>';
            $body .= 'Az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosítód: ' . $passwd_id . '<br>';
            $body .= "Ezzel belépve, a profilod beállításainál a jelszavad megváltoztathatod.<br>";
            if (getOptionValue(OPTIONS_NAME_PASSWORD_LINK) > 0) {
                $command = new Command;
                $code = $command->putPassword($passwd_id, $password);
                $body .= 'Ezen a <a href="' . URL . '?c=' . $code . '">linken</a> keresztül 24 órán belül automatikusan beléphetsz a fenti jelszóval<br>';
            }
            $subject = "Generált jelszó";
            send_mail($subject, $body);
            setPassword($passwd_id, $password);
            logger($passwd_id, $passwd_id, LOGGER_MEMBER_MOD, "Sikeres jelszó módosítás, emailben kiküldve");
            redirect("login");
        } else {
            warning("A megadott email cím hibás!");
        }
    } else {
        warning("Az azonosító mezőt ki kell tölteni !");
    }
}


$output .= '<form method="post" autocomplete="off"><div class="member_id"><div class="keret">
    <label for="id">' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosító: </label>
    <input type="hidden" name="mod" value="' . $mod . '">
    <input type="number" max="9999" name="member_id" value="' . $member_id . '" >&nbsp;&nbsp;vagy&nbsp;&nbsp;';
$output .= '<label>Email :</label><input type="text"  name="login_email" ><br><br>
    <label>Jelszó: </label>
    <input type="password" name="member_pass" value="' . $member_pass . '"><br><br>';
switch ($chaptcha) {
    case CHAPTCHA_METHOD_GOOGLE:
        $output .= '<div class="g-recaptcha" data-sitekey="6Le3hYEUAAAAAIISplLOnO3FVKedc8A3lnT1bmyM"></div><br>';
        break;
    case CHAPTCHA_METHOD_MUTLIPLE:
        $output .= '<input type="hidden" name="chaptha_expected" value="' . $chaptha_number . '">
                    <label for="chaptha_entered">Írd ide, mennyi ' . $chaptha_text . ': </label><input type="text" size="4" name="chaptha_entered" value="">';
        break;
    default:
        break;
}
$output .= 'Emlékezz rám: <input type="checkbox" name="remember_me" value="true" ' .
        ((isset($remember_me) AND $remember_me == "true") ? "checked" : "") . ' >&nbsp;&nbsp;&nbsp;&nbsp;
             <input type="submit" name="login_normal" value="Belépés">
            </div><br><div class="keret">
Amennyiben még nincs jelszavad vagy elfelejtetted, <br>add meg az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosítódat és az emailcímedet<br>
Az email címednek szerepelnie kell az adatbázisban !<br>
A belépéshez szükséges generált jelszót elküldjük az email címedre.<br><br>
<label>' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosító: </label>
    <input type="number" max="9999" name="passwd_id" value="' . $passwd_id . '" ><br><br>
<label for="email">Email cím: </label>
<input type="text" name="member_email" value="" id="email">  
   <input type="submit" name="password_renew" value="Jelszó generálás" >
</form> ';
?>