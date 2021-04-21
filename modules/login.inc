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
if(!isset($login_email)) {
    $login_email = '';
}
if(!isset($member_email)) {
    $member_email = '';
}
if(!isset($remember_me)) {
    $remember_me = 'off';
}
if (isset($login_normal)) {
    if ($member_id == 0) {
        $view->danger("Jelszó hiba!");
        session_unset();
        unset($login_normal);
    } else {
        logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Sikeres belépés");
        unset($login_normal);
        if (isGdpr($member_id)) {
            redirect("profile");
            exit;
        } else {
            redirect("gdpr");
            exit;
        }
    }
}

if (isset($password_renew)) {
    if (isset($passwd_id) AND $passwd_id > 0) {
        $member_id = $passwd_id;
        $password = randomPassword();
        $record = retriveMember($passwd_id);
        if (isset($record[MEMBER_EMAIL]) AND $record[MEMBER_EMAIL] == '') {
            $view->danger("A megadott azonosítóhoz nem tartozik email cím!");
        } elseif (0 == strcasecmp(trim($record[MEMBER_EMAIL]), $member_email)) {
            if (getOptionValue(OPTIONS_NAME_PASSWORD_LINK) > 0) {
                $command = new Command(Command::PASSWORD);
                $code = $command->putItem($passwd_id, $password);
                $body = 'Az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' adminisztrációs felületen jelszó generálást kértél.<br>';
                $body .= 'Ezen a <b><a href="' . URL . '?c=' . $code . '">linken</a></b> keresztül 24 órán belül automatikusan beléphetsz<br>';
                $body .= "Add meg az új jelszavad !<br>";
            } else {
                $body = 'Az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' adminisztrációs felületen generált jelszavad: ' . $password . '<br>';
                $body .= 'Az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosítód: ' . $passwd_id . '<br>';
                $body .= "Ezzel belépve, a profilod beállításainál a jelszavad megváltoztathatod.<br>";
            }
            $subject = "Generált jelszó";
            send_mail($subject, $body);
            setPassword($passwd_id, $password);
            logger($passwd_id, $passwd_id, LOGGER_MEMBER_MOD, "Sikeres jelszó módosítás, emailben kiküldve");
            $password_changed = true;
//            redirect('password_renewed');
        } else {

            $view->danger("A megadott email cím hibás!");
        }
    } else {
        $view->danger("Az azonosító mezőt ki kell tölteni !");
    }
}

if (isset($password_changed) AND $password_changed) {
    $view->newKeret();
    $view->success('A jelszó módosításhoz szükséges emailt elküldtük a megadott email címre.');
    $view->info('Néhény percen belül meg kell érkezzen a postafiókba. Esetleg érdemes a SPAM-ek között is keresni.');
    $view->info('Javasoljuk a SPAM szürő olyan belállítását, hogy a <b>' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . '</b> tagnyilvántartójából érkező emaileket soha ne tekintse spam-nek!');
    $view->endKeret();
    return;
}

$view->newKeret();
$view->putElement('<h4>Regisztrált felhasználóknak</h4>');
$view->newDiv('row align-items-center');
$view->newDiv('col')->inputEmail('login_email', 'E-mail')->endDiv();
$view->putText('vagy');
$view->newDiv('col')->inputMemberId('member_id',   getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosító')->endDiv();   

$view->endDiv()->newDiv('row align-items-center');
$view->newDiv('col')->inputPassword('member_pass', 'Jelszó')->endDiv();
$view->newDiv('col')->inputCheckbox('remember_me', 'Emlékezz rám')->endDiv();
$view->endDiv();
$view->submitButton('login_normal', 'Belépés');
$view->endKeret(); 

$view->newKeret();
$view->putText('<h4>Első belépés, elfelejtett jelszó</h4>');
$view->info('Amennyiben még nincs jelszavad vagy elfelejtetted, add meg az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosítódat és az emailcímedet');
$view->info('Az email címednek szerepelnie kell az adatbázisban, amire a belépéshez szükséges linket elküldjük.');

$view->newDiv('row form-inline mt-2 mx-2');
$view->inputMemberId('passwd_id',   getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' azonosító');
$view->inputEmail('member_email', 'E-mail');
$view->submitButton('password_renew', 'Jelszó generálás');
$view->endDiv()->endKeret();

$view->newKeret();
$view->putElement('A tagnyilvántartó rendszert a <b>' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . '</b> számára '
        . 'a fejlesztők és üzemeltetők a szabadidejükben, társadalmi munkában fejlesztik, működtetik.<br>'
        . 'Nagyon szívesen veszünk bármilyen jellegű ötletet, észrevételt, esetleg hiba jelzést a működéssel, funkciókkal kapcsolatban.<br><br>'
        . 'Üdvözlettel:<br>' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' csapata</div>');
$view->endKeret();



