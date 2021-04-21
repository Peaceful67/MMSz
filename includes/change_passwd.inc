<?php

$view->newKeret('editor');

if (!isset($old_pass)) {
    $old_pass = '';
}
if (isset($changed_passwd)) {
    if ($new_pass1 == $new_pass2) {
        if (strlen($new_pass1) > 4) {
            if (strlen($new_pass1) < 16) {
                setPassword($member_id, $new_pass1);
                logger($member_id, $member_id, LOGGER_MEMBER_MOD, " Jelszóváltoztatás");
                setcookie(COOKIE_REMEMBER_ME, "true", $now + TIME_REMEMBER_ME);
                setcookie(COOKIE_MEMBER_ID, $member_id, $now + TIME_REMEMBER_ME);
                setcookie(COOKIE_MEMBER_PASS, $new_pass1, $now + TIME_REMEMBER_ME);
                unset($_SESSION[SESSION_DATA2EDIT]);
                $view->success('<h3>A jelszó módosítás sikeresen megtörtént</h3>');
                return;
            } else {
                $view->danger("A jelszónak 16 karakternél rövidebbnek kell lennie !");
            }
        } else {
            $view->danger("A jelszó 4 karakternél hosszabbnak kell lennie!");
        }
    } else {
        $view->danger("A két új jelszó nem egyezik meg !");
    }
}
if (isset($login_by_email) AND $login_by_email) {
    logger($member_id, $member_id, LOGGER_MEMBER_LOGIN, "Emailen kapott linken keresztül beléptetve");
    setcookie(COOKIE_MEMBER_ID, $member_id);
    setcookie(COOKIE_MEMBER_PASS, $member_pass);
//    redirect('profile');
}
if(!isset($new_pass1)) {
    $new_pass1='';
}
if(!isset($new_pass2)) {
    $new_pass2='';
}

$view->putElement('<h4>Jelszó modosítás</h4>');
$view->putHidden('member_id')->putHidden('mod');
$view->inputPassword('new_pass1', 'Új jelszó');
$view->inputPassword('new_pass2', 'Új jelszó ismét');
$view->submitButton('changed_passwd', 'Módosítás');
/*
$output .= '
       <input type="hidden" value="' . $member_id . '" name="member_id">
                  <input type="hidden" value="' . $mod . '" name="mod">';
$output .= '<label for="new_pass1">Új jelszó: </label>
<input type="password" name="new_pass1"  id="new_pass1" value=""> <br>
<label for="new_pass2">Új jelszó ismét: </label><input type="password" name="new_pass2"  id="new_pass2" value=""><br>
<input type="submit" name="changed_passwd" value="Módosítás"><br>
<br>
';
 * 
 */
$view->endKeret();