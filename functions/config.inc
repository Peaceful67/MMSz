<?php

session_start();

foreach ($_POST as $key => $value) {
    $$key = $value;
}

date_default_timezone_set('Europe/Budapest');

$this_year = date('Y', time());
$beginingOfYear = $this_year . '-01-01';
$endOfYear = $this_year . '-12-31';
$neverEnding = '2099-12-31';
$today = date('Y-m-d');

define("YEAR_FROM", 2017);
$years = array();
for ($y = YEAR_FROM; $y <= $this_year; $y++) {
    $years[$y] = $y;
}


define('SECRET', '%/{)9A!#@');

define("CSV_DELIMITER", ";");
define("CSV_DECODE", "utf8_decode");

define("VERY_FIRST_DATE", '2017-01-01');

define("AGE_ADOLESENT", 14);
define("AGE_JUNIOR", 18);
define("AGE_RETIRED", 64);


define("REPLACE_NAME", "%#%");
define("REPLACE_ID", "%@%");
define("REPLACE_COMMAND_REGEXP", '/_#_\w{8}_#_/');
define("NEWLINE", "\n");

define("CHAPTCHA_METHOD_MUTLIPLE", "multiple");
define("CHAPTCHA_METHOD_GOOGLE", "google");
define("CHAPTCHA_METHOD_NOTHING", "nothing");
//$chaptcha = CHAPTCHA_METHOD_MUTLIPLE;
//$chaptcha = CHAPTCHA_METHOD_NOTHING;
define("GOOGLE_SITE_KEY", "6Le3hYEUAAAAAIISplLOnO3FVKedc8A3lnT1bmyM");
define("GOOGLE_SECRET_KEY", "6Le3hYEUAAAAAIBsY4AnTPN-YBd5x_z8KnEc11_9");

define("SESSION_DATA2EDIT", "data2edit");
define("SESSION_FEE2EDIT", "fee2edit");
define("SESSION_FEETYPE2EDIT", "feetype2edit");
define("SESSION_EDIT_MEMBER", "edit_member");
define("SESSION_FEE_TO_BILL", "fb_str");
define("SESSION_BILL_NUMBER", "fb_number");
define("SESSION_REQUIRED_MEMBER", "required");
define("SESSION_REQUIRED_NOROBOT", "norobot_required");
define("SESSION_FOUND_MEMBERS", "found_members");
define("SESSION_SELECTED_ITEM", "selected_item");
define("SESSION_SP_BACKREF", "simplepay_backref");
define("SESSION_SEARCH_VALUES", "search_values");
define("SESSION_VIEW_COLUMNS", "view_columns");
define("SESSION_MOD", "module");
define("SESSION_LAST_MOD", "last_mod");
define("SESSION_HISTORY", 'history');
define("SESSION_NEW_VALUES", 'new_values');
define("SESSION_SZAMLAZZ_RESULT", "szamlazz_result");
define("SESSION_FILTERED_NEWSLETTER", "filtered_newsletter");
define("SESSION_FILTERED_CONGRESS", "filtered_congress");
define("SESSION_ROLE_AS", "role_as");
define("SESSION_CONGRESS_ONLINE", 'congress_online');
define("SESSION_CONGRESS_ITEM", 'congress_item');
define("SESSION_CONGRESS_POLL", 'congress_poll');
define("SESSION_SELECTED_ROLE", 'selected_role');
define("SESSION_SELECTED_CLUB", 'selected_club');
define("SESSION_VIEW_DOCUMENT_ID", 'view_document');
define("SESSION_VIEW_DOCUMENT_ARRAY", 'view_document_array');
define("SESSION_LAST_CONGRESS_COMMENT", 'last_comment');

define("SEARCH_LISTED_MEMBER", 'listed_member');
define("SEARCH_SEARCH", 'search');
define("SEARCH_SELECTED_ACTIVE", 'active');
define("SEARCH_SELECTED_PAID", 'paid');
define("SEARCH_SELECTED_LICENCE", 'licence');
define("SEARCH_SEARCH_BY_ID", 'search_by_id');
define("SEARCH_ORDER_BY", 'order_by');
define("SEARCH_SELECTED_CLUB", 'club');

define("COOKIE_REMEMBER_ME", "remember_me");
define("COOKIE_MEMBER_ID", "member_id");
define("COOKIE_MEMBER_PASS", "member_pass");


define("TIME_REMEMBER_ME", 30 * 24 * 60 * 60);
define("TIME_ONE_DAY", 24 * 60 * 60);




define("FM_CHECKBOX", 'fm2b_chk');



define("AVATAR", "/avatars/");
define("PATH_IMAGES", "images/");
define("AVATAR_SIZED", AVATAR . 'sized/');
define("AVATAR_ORIGINAL", AVATAR . 'original/');
define("MODULES", "./modules/");
define("INCLUDES", "./includes/");
define("DOCUMENTS", "./documents/");
define("SIMPLEPAY", "./simplepay/");
define("CLASSES", "./classes/");
define("MPDF", CLASSES . "");
define("ELEMENTS", "./elements/");
define("POLLING", ELEMENTS . "polling/");
define("DOCU_MANAGEMENT", ELEMENTS . "docu_management/");

define("PROGI", URL . "/index.php");
define("CARD_LINK", URL . "/card?");
define("IMAGES", URL . "/" . PATH_IMAGES);
define("AVATAR_URL", URL . AVATAR);
define("RULES", DOCUMENTS . "rules_");
define("URL_RULES", URL . '/documents/rules_');
define("CONGRESS", 'congress');
define("CONGRESS_HELP_FILE", DOCUMENTS . CONGRESS . '_help.pdf');
define("CONGRESS_ITEM_DOC", DOCUMENTS . CONGRESS . "_item_");
define("CONGRESS_COMMENT_DOC", DOCUMENTS . CONGRESS . "_comment_");
define("CONGRESS_CREATE_SUM", CONGRESS . '_sum_');
define("CONGRESS_CREATE_RECORD", CONGRESS . '_record_');
define("CONGRESS_ITEM_SUM", CONGRESS . '_item_sum_');

define("MIME_ZIP", "application/x-gzip");
define("MIME_CSV", "application/csv");

define("AVATAR_WIDTH", 100);
define("AVATAR_HEIGHT", 130);
define("AVATAR_SIZED_WIDTH", 220);
define("AVATAR_SIZED_HEIGHT", 300);
define("AVATAR_TOLERANCE", 1.05);
define("CARD_AVATAR_WIDTH", 220);
define("CARD_AVATAR_HEIGHT", 300);
define("CARD_IMG_DPI", '327');

define("ICON_SIZE", 25);

define("ACCESS_PUBLIC", 0);
define("ACCESS_USER", 1);
define("ACCESS_CLUBLEADER", 1 << 1);
define("ACCESS_MMSZ", 1 << 2);
define("ACCESS_ADMIN", 1 << 3);
define("ACCESS_CLUB_DELEGATE", 1 << 4);
define("ACCESS_PAID", 1 << 5);
define("ACCESS_MMSZ_PRESIDENT", 1 << 6);
define("ACCESS_CLUB_PRESIDENT", 1 << 7);
define("ACCESS_BRANCH_LEADER", 1 << 8);
define("ACCESS_SUBCLUB_LEADER", 1 << 9);
define("ACCESS_NOBODY", 1 << 10);
define("ACCESS_MANAGE_NEW", 1 << 11);
define("ACCESS_MANAGE_EDIT", 1 << 12);
define("ACCESS_MANAGE_FAI", 1 << 13);
define("ACCESS_MANAGE_ALL", 1 << 14);
define("ACCESS_CLUB_MEMBER", ACCESS_USER + ACCESS_CLUBLEADER + ACCESS_CLUB_DELEGATE + ACCESS_CLUB_PRESIDENT);
define("ACCESS_MANAGE_BRANCH", ACCESS_MANAGE_ALL + ACCESS_MANAGE_NEW + ACCESS_MANAGE_EDIT + ACCESS_MANAGE_FAI + ACCESS_BRANCH_LEADER);
define("ACCESS_ANYBODY", 0b1111111111111111);


define("FEE_TYPE_MEMBERSHIP", 0);
define("FEE_TYPE_INSURANCE", 1);
define("FEE_TYPE_ORGANIZATION", 2);
define("FEE_TYPE_CLUB", 3);
define("FEE_TYPE_ADMINISTRATION", 4);
define("FEE_TYPE_ASSOCIATION", 5);
define("FEE_TYPE_LICENCES", 6);
define("FEE_TYPE_OCCASION", 7);

define("FEE_TYPE_NAME", 0);
define("FEE_TYPE_PERIOD", 1);


define("PAGE_MEMBERS", 25);
define("PAGERS", 3);


define("FEE_PERIOD_YEAR", 0);
define("FEE_PERIOD_HALF", 1);
define("FEE_PERIOD_QUATER", 2);
define("FEE_PERIOD_MONTH", 3);
define("FEE_PERIOD_WEEK", 4);
define("FEE_PERIOD_DAY", 5);
define("FEE_PERIOD_2YEARS", 6);
define("FEE_PERIOD_3YEARS", 7);

define("FEE_NEXT_DAY", 10);

$uries = [
    'view_document' => 'uri_view_document',
    'view_selected' => 'uri_view_selected_document',
];


$controll_access = array(
    ACCESS_USER => array(ACCESS_ADMIN, ACCESS_CLUBLEADER, ACCESS_MMSZ),
    ACCESS_CLUBLEADER => array(ACCESS_ADMIN, ACCESS_CLUBLEADER, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT),
    ACCESS_MMSZ => array(ACCESS_ADMIN, ACCESS_MMSZ),
    ACCESS_CLUB_DELEGATE => array(ACCESS_ADMIN, ACCESS_CLUBLEADER, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT),
    ACCESS_MMSZ_PRESIDENT => array(ACCESS_ADMIN, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT),
    ACCESS_CLUB_PRESIDENT => array(ACCESS_ADMIN, ACCESS_CLUBLEADER, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT),
    ACCESS_BRANCH_LEADER => [ACCESS_ADMIN, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT],
);


define("MEMBER_TYPE_ADULT", 0);
define("MEMBER_TYPE_JUNIOR", 1);
define("MEMBER_TYPE_RETIRED", 2);
define("MEMBER_TYPE_INVALID", 3);
define("MEMBER_TYPE_ADOLESENT", 4);
define("MEMBER_TYPE_COLLABORATOR", 5);
define("MEMBER_TYPE_DIRECT", 6);


define("FEE_ORG_NAME_FAI", 0);


$default_organizations = array(
    FEE_ORG_NAME_FAI => "FAI",
);

$default_associations = array(
    0 => "MMSZ",
);

$default_club_categories = array(
    0 => "Felnőtt",
    1 => "Ifjúsági",
    2 => "Nyugdíjas",
    3 => "Pártoló",
);

$default_administrations = array(
    0 => "Adatkezelés"
);
$default_occasions = array();

$default_member_types = array(
    MEMBER_TYPE_ADULT => "Felnőtt",
    MEMBER_TYPE_JUNIOR => "Ifjúsági",
    MEMBER_TYPE_RETIRED => "Nyugdíjas",
    MEMBER_TYPE_INVALID => "Leszázalékolt",
    MEMBER_TYPE_ADOLESENT => "Serdülő",
    MEMBER_TYPE_COLLABORATOR => "Közreműködő",
    MEMBER_TYPE_DIRECT => "Közvetlen",
);

$default_member_insurances = array(
    0 => "Nem biztosított",
    1 => "MMSZ biztosított",
    2 => "Pártoló tag",
    3 => "Máshol biztosított",
);

$default_licences = array(
    0 => "Nevezd át !!!",
);

$fee_periods = array(
    FEE_PERIOD_YEAR => "éves",
    FEE_PERIOD_HALF => "féléves",
    FEE_PERIOD_QUATER => "negyedéves",
    FEE_PERIOD_MONTH => "havi",
    FEE_PERIOD_WEEK => "heti",
    FEE_PERIOD_DAY => "napi",
    FEE_PERIOD_2YEARS => "két éves",
    FEE_PERIOD_3YEARS => "három éves",
);



$roles = array(
    0 => "Tag",
    1 => "Klubvezető",
    2 => "Küldött",
    3 => "Elnök",
);

define("LOGGER_MEMBER_MOD", 10);
define("LOGGER_CATEGORY_MOD", 11);
define("LOGGER_MEMBER_LOGIN", 12);
define("LOGGER_FEE_EDIT", 13);
define("LOGGER_CLUB_EDIT", 14);
define("LOGGER_BILL", 15);
define("LOGGER_PAY", 16);
define("LOGGER_DATABASE", 17);
define("LOGGER_FM_EDIT", 18);
define("LOGGER_SYSTEM_ERROR", 19);
define("LOGGER_EMAIL_SENT", 20);
define("LOGGER_QUERY", 21);
define("LOGGER_SIMPLEPAY", 22);
define("LOGGER_STATISTIC", 23);
define("LOGGER_CONGRESS", 24);
define("LOGGER_GPDR", 25);
define("LOGGER_SETUP", 26);
define("LOGGER_ORGANIZATION", 27);
define("LOGGER_ASSOCCIATION", 28);
define("LOGGER_LICENCES", 29);
define("LOGGER_SZAMLAZZ", 30);
define("LOGGER_OPTIONS", 31);
define("LOGGER_CRONTAB", 32);
define("LOGGER_FAI_API", 33);
define("LOGGER_TYPES", 34);
define("LOGGER_CONGRESS_ONLINE", 35);
define("LOGGER_CONGRESS_COMMENT", 36);
define("LOGGER_CONGRESS_VOTE", 37);
define("LOGGER_HELP", 38);
define("LOGGER_DOCU_MANAGEMENT_BASE", 39);
define("LOGGER_DOCU_DAO", 40);
define("LOGGER_DOCU_JOG", 41);
define("LOGGER_DOCU_FILES", 42);


$logger_categories = array(
    LOGGER_MEMBER_MOD => "Tagadatok módosítása",
    LOGGER_CATEGORY_MOD => "Versenykategória módosítás",
    LOGGER_MEMBER_LOGIN => "Belépés a felületre",
    LOGGER_FEE_EDIT => "Díjtételek szerkesztés",
    LOGGER_CLUB_EDIT => "Egyesület szerkesztés",
    LOGGER_BILL => "Utalási tranzakció",
    LOGGER_PAY => "Fizetési tranzakció",
    LOGGER_DATABASE => "Adatbázis karbantartás",
    LOGGER_FM_EDIT => "Tagsági díjtételeinek választása",
    LOGGER_SYSTEM_ERROR => "Rendszerüzenet",
    LOGGER_EMAIL_SENT => "Email küldés",
    LOGGER_QUERY => "Lekérdezés",
    LOGGER_SIMPLEPAY => "Bankkártyás fizetés",
    LOGGER_STATISTIC => "Adatok lekérdezése",
    LOGGER_CONGRESS => "Küldött gyűlés kezelés",
    LOGGER_GPDR => "Adatkezelési szabályzat elfogadás",
    LOGGER_SETUP => "Menü beállítások",
    LOGGER_ORGANIZATION => "Szervezeti tagság szerkesztése",
    LOGGER_ASSOCCIATION => "Szövetségi tagság szerkesztése",
    LOGGER_LICENCES => "Jogosítások szerkesztése",
    LOGGER_SZAMLAZZ => "Online számlakezelés",
    LOGGER_OPTIONS => "Beállítások módosítása",
    LOGGER_CRONTAB => "Crontab",
    LOGGER_FAI_API => "FAI API",
    LOGGER_TYPES => "Típuslisták szerkesztése",
    LOGGER_CONGRESS_ONLINE => "Online szavazás szerkesztése",
    LOGGER_CONGRESS_COMMENT => "Online szavazás hozzászólás",
    LOGGER_CONGRESS_VOTE => "Online szavazás szavazás",
    LOGGER_HELP => "Leírás szerkesztése",
    LOGGER_DOCU_MANAGEMENT_BASE => "Dokumentumtár törzsadat kezelés",
    LOGGER_DOCU_DAO => "Képzések kezelése",
    LOGGER_DOCU_JOG => "Képzettségek kezelése",
    LOGGER_DOCU_FILES => "Dokumentumok kezelése",
);
?>
