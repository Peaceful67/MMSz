<?php

// Szervezet függő adatok
define("BANK_ACCOUNT", "10200823-22214380-00000000");
define("FAI_NOTIF_EMAIL", "postafiok@emg.hu");
define("MMSZ_EMAIL", "postafiok@emg.hu");
define("MMSZ_ELNOK_EMAIL", "postafiok@emg.hu");

define("ADMIN_EMAIL", "peaceful.app@gmail.com");
define("MMSZ_TEL", "70-7731303");
define("EMAIL_SENDER", 'noreply@modellsport.hu');
define("EMAIL_SIGNATURE", 'Az MMSz csapata');


$access = array(
    ACCESS_PUBLIC => 'Bárki',
    ACCESS_USER => 'Tag',
    ACCESS_CLUBLEADER => 'Vezető',
    ACCESS_MMSZ => "Adminisztrátor",
    ACCESS_ADMIN => 'Admin',
    ACCESS_PAID => 'Aktív',
    ACCESS_CLUB_DELEGATE => 'Küldött',
    ACCESS_MMSZ_PRESIDENT => 'Elnökségi tag',
    ACCESS_CLUB_PRESIDENT => 'Elnök',
    ACCESS_BRANCH_LEADER => 'Szakágvezető',
    ACCESS_NOBODY => 'Senki',
    ACCESS_MANAGE_NEW => 'Új tag felvétel',
    ACCESS_MANAGE_EDIT => 'Tag szerkesztése',
    ACCESS_MANAGE_FAI => 'FAI jóváhagyás',
    ACCESS_MANAGE_ALL => 'Minden tag',
);

$fee_type = array(
    FEE_TYPE_MEMBERSHIP => array("MMSz tagság", FEE_PERIOD_YEAR),
    FEE_TYPE_INSURANCE => array("Biztosítási forma", FEE_PERIOD_HALF),
    FEE_TYPE_ORGANIZATION => array("Szervezet", FEE_PERIOD_YEAR),
    FEE_TYPE_CLUB => array("Egyesületi tagdíj", FEE_PERIOD_YEAR),
    FEE_TYPE_ADMINISTRATION => array("Egyéb", FEE_PERIOD_YEAR),
    FEE_TYPE_ASSOCIATION => array("Szövetségi tagság", FEE_PERIOD_YEAR),
    FEE_TYPE_LICENCES => array("Jogosítások", FEE_PERIOD_3YEARS),
    FEE_TYPE_OCCASION => array("Alkalom", FEE_PERIOD_DAY),
);

$default_branches = array(
    0 => "Autós",
    1 => "Hajós",
    2 => "Repülős",
    3 => "Drón",
    4 => "Egyéb",
);


// Emailezéskor használt szövegek

