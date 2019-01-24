<?php
$output .= '<div class="keret">';
if (isset($changed_passwd)) {
    if (md5($old_pass . SECRET) == getPassword($member_id)) { // 
        if ($new_pass1 == $new_pass2) {
            if (strlen($new_pass1) > 4) {
                if (strlen($new_pass1) < 16) {
                    setPassword($member_id, $new_pass1);
                    logger($member_id, $member_id, LOGGER_MEMBER_MOD, " Jelszóváltoztatás");
                    redirect("login");
                } else {
                    warning("A jelszónak 16 karakternél rövidebbnek kell lennie !");
                }
            } else {
                warning("A jelszó 4 karakternél hosszabbnak kell lennie!");
            }
        } else {
            warning("A két új jelszó nem egyezik meg !");
        }
    } else {
        warning("A régi jelszó hibás !");
    }
}
$output .= '<H1>Jelszó modosítás</H1>
       <form method="post">
       <input type="hidden" value="' . $member_id . '" name="member_id">
                  <input type="hidden" value="' . $mod . '" name="mod">
<label for="oldpass">Régi jelszó: </label>
<input type="password"  name="old_pass" value="" id="old_pass"> <br> 
<label for="new_pass1">Új jelszó: </label>
<input type="password" name="new_pass1"  id="new_pass1" value=""> <br>
<label for="new_pass2">Új jelszó ismét: </label><input type="password" name="new_pass2"  id="new_pass2" value=""><br>
<input type="submit" name="changed_passwd" value="Módosítás"><br>
<br>
</form></div>';
