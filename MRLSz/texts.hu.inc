<?php

// Szervezet függő adatok
define("BANK_ACCOUNT", "x-x-x");
define("FAI_NOTIF_EMAIL", "palharsfalvi@gmail.com");
define("MMSZ_EMAIL", "palharsfalvi@gmail.com");
define("MMSZ_ELNOK_EMAIL", "gergely.tuzson@digitalis.hu");

define("ADMIN_EMAIL", "peaceful.app@gmail.com");
define("MMSZ_TEL", "70-7731303");
define("EMAIL_SENDER", 'rlsztitkar@gmail.com');
define("EMAIL_SIGNATURE", 'Az RLSZ Vezetése');


$access = array(
    ACCESS_PUBLIC => 'Bárki',
    ACCESS_USER => 'Tag',
    ACCESS_CLUBLEADER => 'Vezető',
    ACCESS_MMSZ => "MMSz adminisztrátor",
    ACCESS_ADMIN => 'Rendszer adminisztrátor',
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
    FEE_TYPE_MEMBERSHIP => array("RLSZ tagság", FEE_PERIOD_YEAR),
    FEE_TYPE_INSURANCE => array("Biztosítási forma", FEE_PERIOD_HALF),
    FEE_TYPE_ORGANIZATION => array("Szervezet", FEE_PERIOD_YEAR),
    FEE_TYPE_CLUB => array("Egyesületi tagdíj", FEE_PERIOD_YEAR),
    FEE_TYPE_ADMINISTRATION => array("Egyéb", FEE_PERIOD_YEAR),
    FEE_TYPE_ASSOCIATION => array("Szövetségi tagság", FEE_PERIOD_YEAR),
    FEE_TYPE_LICENCES => array("Jogosítások", FEE_PERIOD_3YEARS),
    FEE_TYPE_OCCASION => array("Alkalom", FEE_PERIOD_DAY),
);

$default_branches = array(
    0 => "Általános repülő",
    1 => "Ballon repülő",
    2 => "Drón repülő",
    3 => "Ejtőernyős",
    4 => "Motoros műrepülő",
);


