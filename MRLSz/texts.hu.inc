<?php
// Szervezet függő adatok
define("BANK_ACCOUNT", "x-x-x");

define("ADMIN_EMAIL", "peaceful.app@gmail.com");
if (DEVELOPMENT) {
    define("FAI_NOTIF_EMAIL", "postafiok@emg.hu");
    define("MMSZ_EMAIL", "postafiok@emg.hu");
    define("MMSZ_ELNOK_EMAIL", "postafiok@emg.hu");
} else {
    define("FAI_NOTIF_EMAIL", "darojanos@freemail.hu");
    define("MMSZ_EMAIL", "darojanos@freemail.hu");
    define("MMSZ_ELNOK_EMAIL", "gergely.tuzson@digitalis.hu");
}
define("MMSZ_TEL", "+36309357993");


$access = array(
    ACCESS_PUBLIC => 'Bárki',
    ACCESS_USER => 'Tag',
    ACCESS_CLUBLEADER => 'Vezető',
    ACCESS_MMSZ => "MRLSz adminisztrátor",
    ACCESS_ADMIN => 'Rendszer adminisztrátor',
    ACCESS_PAID => 'Aktív',
    ACCESS_CLUB_DELEGATE => 'Küldött',
    ACCESS_MMSZ_PRESIDENT => 'Elnökségi tag',
    ACCESS_CLUB_PRESIDENT => 'Elnök',
    ACCESS_CATEGORY_LEADER => 'Szakágvezető',
    ACCESS_NOBODY => 'Senki',
);

$fee_type = array(
    FEE_TYPE_MEMBERSHIP => array("MRLSz tagság", FEE_PERIOD_YEAR),
    FEE_TYPE_INSURANCE => array("Biztosítási forma", FEE_PERIOD_HALF),
    FEE_TYPE_ORGANIZATION => array("Nezetközi Szervezet", FEE_PERIOD_YEAR),
    FEE_TYPE_CLUB => array("Egyesületi tagdíj", FEE_PERIOD_YEAR),
    FEE_TYPE_ADMINISTRATION => array("Egyéb", FEE_PERIOD_YEAR),
    FEE_TYPE_ASSOCIATION => array("Szövetségi tagság", FEE_PERIOD_YEAR),
    FEE_TYPE_LICENCES => array("Jogosítások", FEE_PERIOD_3YEARS),
    FEE_TYPE_OCCASION => array("Alkalom", FEE_PERIOD_DAY),
);
$member_types = array(
    MEMBER_TYPE_ADULT => "Felnőtt",
    MEMBER_TYPE_JUNIOR => "Ifjúsági",
    MEMBER_TYPE_RETIRED => "Tag tipus 3",
    MEMBER_TYPE_INVALID => "Tag tipus 4",
    MEMBER_TYPE_ADOLESENT => "Tag tipus 5",
    MEMBER_TYPE_COLLABORATOR => "Közreműködő",
    MEMBER_TYPE_DIRECT => "Közvetlen",
);
$club_categories = array(
    0 => "Felnőtt",
    1 => "Ifjúsági",
    2 => "Saját 0",
    3 => "Pártoló",
    4 => "Saját 1",
    5 => "Saját 2",
    6 => "Saját 3",
);

$member_insurances = array(
    0 => "Nem biztosított",
    1 => "NVSZ biztosított",
    2 => "Pártoló tag",
    3 => "Máshol biztosított",
);

$branches = array(
    0 => "Általános Repülő",
    1 => "Ballonrepülő",
    2 => "Drónrepülő",
    3 => "Ejtőernyős",
    4 => "Motoros Műrepülő",
    5 => "Modellező",
    6 => "Sárkányrepülő és Siklóernyős",
    7 => "Ultrakönnyű és Paramotoros",
    8 => "Vitorlázó Műrepülő",
    9 => "Vitorlázórepülő",
);


// Emailezéskor használt szövegek

define("EMAIL_SENDER", 'noreply@MRLSZ.hu');
define("EMAIL_SIGNATURE", 'Az MRLSz csapata');
