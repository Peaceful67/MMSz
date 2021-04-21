<?php

if($permission == ACCESS_PUBLIC || !isset($member_id)) {
    $output .= "Üdvözöllek az adminisztrációs oldalon !"
            . "<br>Kérlek lépje be !<br>";
} else {
    $r = retriveMember($member_id);
    $output .= "Üdvözöllek ".$r[MEMBER_VEZNEV]." ".$r[MEMBER_KERNEV]." !"
            . "<br>";
    $output .= '<br>'.printRoles($member_id);
    $output .= '<br><br>Verzió szám: '.VERSION;
}
