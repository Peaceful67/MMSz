<?php

define("DOCU_INCLUDES", INCLUDES.'docu/');

include_once DOCU_MANAGEMENT . 'DocuFile.inc';
include_once DOCU_MANAGEMENT . 'DocuOrganizations.inc';
include_once DOCU_MANAGEMENT . 'DocuEVO.inc';
include_once DOCU_MANAGEMENT . 'DocuLJK.inc';
include_once DOCU_MANAGEMENT . 'DocuStandard.inc';
include_once DOCU_MANAGEMENT . 'DocuDAO.inc';
include_once DOCU_MANAGEMENT . 'DocuJOG.inc';
include_once DOCU_MANAGEMENT . 'DocuJogLjk.inc';
include_once DOCU_MANAGEMENT . 'DocuTemplateKK.inc';
include_once DOCU_MANAGEMENT . 'DocuTemplatePD.inc';
include_once DOCU_MANAGEMENT . 'DocuPIDAO.inc';
include_once DOCU_MANAGEMENT . 'DocuPIDJOG.inc';
include_once DOCU_MANAGEMENT . 'DocuPIDAODocus.inc';
include_once DOCU_MANAGEMENT . 'DocuPIDJOGDocus.inc';
include_once DOCU_MANAGEMENT . 'DocuRoleMember.inc';



define("DOCU_ACCESS_ADMIN", 1); // Rendszer adminisztrátor
define("DOCU_ACCESS_OPERATOR", 2); // Szövetségi titkár 
define("DOCU_ACCESS_MAINTAINER", 3);  // Karbantartó, hangármester
define("DOCU_ACCESS_DHT", 4); // Deputy head of Training / Képzési szervezet helyi megbízott
define("DOCU_ACCESS_EDU_LEADER", 5); // Képzési szervezet vezetőség
define("DOCU_ACCESS_EDU_OPERATOR", 6); // Képzési szervezet ügyintéző
define("DOCU_ACCESS_FLF_LEADER", 7);  // FLF szervezet vezető
define("DOCU_ACCESS_FLF_OPERATOR", 8); // FLF szervezet ügyintéző
define("DOCU_ACCESS_EDU_AUTHORITY", 9); // Képzési szervezet ellenőr (hatóság)
define("DOCU_ACCESS_FLF_AUTHORITY", 10); // FLF szervezet ellenőr (hatóság)
define("DOCU_ACCESS_JOG_OA", 11); // Oktatói képzettség

if (!is_dir(getcwd() . LOCALFILE_PATH) OR ! is_writable(getcwd() . LOCALFILE_PATH)) {
    $output .= 'Dir:' . getcwd() . LOCALFILE_PATH . output_ln();
    warning('A local fájlok könyvtár nem létezik vagy nem írható');
}

define('DOCU_EXPIRES', 'Lejár');
define('DOCU_NOT_EXPIRES', 'Nem jár le');
define('DOCU_PATTERN', 'Minta');
define('DOCU_NO_PATTERN', 'Kitöltendő');

$docu_access = [
    DOCU_ACCESS_ADMIN => 'Rendszer adminisztrátor',
    DOCU_ACCESS_OPERATOR => 'Szövetségi titkár',
    DOCU_ACCESS_MAINTAINER => 'Karbantartó, hangármester',
    DOCU_ACCESS_DHT => 'Deputy head of Training / Képzési szervezet helyi megbízott',
    DOCU_ACCESS_EDU_LEADER => 'Képzési szervezet vezetőség',
    DOCU_ACCESS_EDU_OPERATOR => 'Képzési szervezet ügyintéző',
    DOCU_ACCESS_FLF_LEADER => 'FLF szervezet vezető',
    DOCU_ACCESS_FLF_OPERATOR => 'FLF szervezet ügyintéző',
    DOCU_ACCESS_EDU_AUTHORITY => 'Képzési szervezet ellenőr (hatóság)',
    DOCU_ACCESS_FLF_AUTHORITY => 'FLF szervezet ellenőr (hatóság)',
];


