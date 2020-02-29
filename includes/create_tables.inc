<?php

$db_tables = array(
    CATEGORIES_TABLE => 'CREATE TABLE IF NOT EXISTS `' . CATEGORIES_TABLE . '` (
                    `cat_id` int(8) NOT NULL AUTO_INCREMENT,
                    `short_name` varchar(32) NOT NULL,
                    `long_name` varchar(128) NOT NULL,
                    `branch` int(8) NOT NULL DEFAULT "0",
                    `org` int(8) NOT NULL DEFAULT "-1",
                    PRIMARY KEY (`cat_id`),
                    UNIQUE KEY `id` (`cat_id`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    BILLING_TABLE => 'CREATE TABLE IF NOT EXISTS `' . BILLING_TABLE . '` (
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
                    `fm_id` int(16) NOT NULL,
                    `payment_id` int(16) NOT NULL,
                    `creator_id` int(8) NOT NULL,
                    `price` int(8) NOT NULL,
                    `number` varchar(16) NOT NULL,
                    `offline` timestamp NOT NULL DEFAULT "0000-00-00 00:00:00",
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    CM_TABLE => 'CREATE TABLE IF NOT EXISTS `' . CM_TABLE . '` (
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `member_id` int(8) NOT NULL,
                    `category_id` int(8) NOT NULL,
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`),
                    KEY `member_id` (`member_id`),
                    KEY `member_id_2` (`member_id`),
                    KEY `category_id` (`category_id`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    CLUB_TABLE => 'CREATE TABLE IF NOT EXISTS `' . CLUB_TABLE . '` (
                    `IDK` int(11) NOT NULL AUTO_INCREMENT,
                    `KlubNev` varchar(32) NOT NULL,
                    `email` varchar(64) CHARACTER SET ucs2 NOT NULL,
                    `IrSzK` int(11) DEFAULT NULL,
                    `VarosK` varchar(19) DEFAULT NULL,
                    `CimK` varchar(23) DEFAULT NULL,
                    `VezK` int(11) NOT NULL,
                    `Akt` varchar(5) NOT NULL,
                    `act` tinyint(1) NOT NULL,
                    `prezident` varchar(32) NOT NULL,
                    `register` varchar(32) NOT NULL,
                    `comment` varchar(61) DEFAULT NULL,
                    `Mező1` varchar(24) DEFAULT NULL,
                    `Mező2` varchar(18) DEFAULT NULL,
                    `homepage` varchar(64) NOT NULL,
                    `bank` varchar(32) NOT NULL,
                    `tax` VARCHAR(16) NOT NULL,
                    `mandat` INT(4) NOT NULL DEFAULT "0",
                    PRIMARY KEY (`IDK`),
                    UNIQUE KEY `IDK_2` (`IDK`),
                    KEY `IDK` (`IDK`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    COMMAND_TABLE => 'CREATE TABLE IF NOT EXISTS `'.COMMAND_TABLE.'` (
                    `mem_id` int(8) NOT NULL,
                    `code` varchar(8) NOT NULL,
                    `expire` timestamp NOT NULL,
                    `name` varchar(8) NOT NULL,
                    `value` varchar(32) NOT NULL,
                    UNIQUE KEY `code` (`code`)
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    CONGRESS_TABLE => 'CREATE TABLE IF NOT EXISTS `' . CONGRESS_TABLE . '` (
                    `member_id` int(8) NOT NULL,
                    `enter` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1),
                    `quit` timestamp(1) NOT NULL DEFAULT "0000-00-00 00:00:00.0",
                    `id` int(11) NOT NULL AUTO_INCREMENT,
                    PRIMARY KEY (`id`)
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    FM_TABLE => 'CREATE TABLE IF NOT EXISTS `' . FM_TABLE . '` (
                    `payment_id` int(16) DEFAULT "0",
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `member_id` int(8) NOT NULL,
                    `fee_id` int(8) NOT NULL,
                    `fm_from` date NOT NULL,
                    `fm_to` date NOT NULL,
                    `licence` varchar(20) NOT NULL,
                    `bill_id` int(16) DEFAULT "0",
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    FEE_TABLE => 'CREATE TABLE IF NOT EXISTS `' . FEE_TABLE . '` (
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `name` int(8) NOT NULL,
                    `fee_from` date NOT NULL,
                    `fee_to` date NOT NULL,
                    `branch` int(4) NOT NULL DEFAULT "-1",
                    `sum` int(8) NOT NULL DEFAULT "0",
                    `type` int(4) NOT NULL DEFAULT "0",
                    `club` int(8) NOT NULL DEFAULT "-1",
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    FMS2BILL_TABLE => 'CREATE TABLE IF NOT EXISTS `' . FMS2BILL_TABLE . '` (
                    `bill_number` varchar(16) NOT NULL,
                    `mem_id` int(8) NOT NULL,
                    `fm_id` int(8) NOT NULL,
                    `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    SETUP_GROUP_TABLE => 'CREATE TABLE IF NOT EXISTS `' . SETUP_GROUP_TABLE . '` (
                    `group_id` int(6) NOT NULL,
                    `group_name` varchar(32) DEFAULT NULL,
                    `group_weight` int(6) DEFAULT NULL,
                    PRIMARY KEY (`group_id`)
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    'logs' => "CREATE TABLE IF NOT EXISTS `logs` (
                    `id` int(20) NOT NULL AUTO_INCREMENT,
                    `activity` varchar(256) NOT NULL,
                    `member` int(8) NOT NULL,
                    `time_stamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
                    `related_id` int(8) NOT NULL,
                    `log_category` int(8) NOT NULL,
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    MEMBER_TABLE => 'CREATE TABLE IF NOT EXISTS `' . MEMBER_TABLE . '` (
                    `ID` int(8) NOT NULL AUTO_INCREMENT,
                    `VezNev` varchar(19) DEFAULT NULL,
                    `KerNev` varchar(18) DEFAULT NULL,
                    `Szul` varchar(4) DEFAULT NULL,
                    `HoNap` varchar(5) DEFAULT NULL,
                    `Nem` varchar(1) DEFAULT NULL,
                    `Fiz` varchar(1) DEFAULT NULL,
                    `Díj` varchar(1) DEFAULT NULL,
                    `IrSz` varchar(4) DEFAULT NULL,
                    `Varos` varchar(22) DEFAULT NULL,
                    `Lakcim` varchar(32) DEFAULT NULL,
                    `Klub` varchar(3) DEFAULT NULL,
                    `Min` varchar(1) DEFAULT NULL,
                    `Szak` varchar(3) DEFAULT NULL,
                    `Kateg` varchar(17) DEFAULT NULL,
                    `Spec` varchar(2) DEFAULT NULL,
                    `Jel` varchar(1) DEFAULT NULL,
                    `FAI` varchar(6) DEFAULT NULL,
                    `Be` int(4) DEFAULT NULL,
                    `Ki` varchar(5) DEFAULT NULL,
                    `E-mail` varchar(31) DEFAULT NULL,
                    `Megjegyzés` varchar(38) DEFAULT NULL,
                    `SzülHely` varchar(21) DEFAULT NULL,
                    `Anyja` varchar(31) DEFAULT NULL,
                    `TAJ` varchar(9) DEFAULT NULL,
                    `telefon` varchar(32) NOT NULL,
                    `password` varchar(40) NOT NULL,
                    `born` date NOT NULL,
                    `homepage` varchar(64) NOT NULL,
                    `nickname` varchar(32) NOT NULL,
                    `licence` varchar(20) NOT NULL,
                    `birth_city` varchar(32) NOT NULL,
                    `mother` varchar(32) NOT NULL,
                    `gdpr` timestamp NULL DEFAULT NULL,
                    PRIMARY KEY (`ID`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    SETUP_MOD_TABLE => 'CREATE TABLE IF NOT EXISTS `' . SETUP_MOD_TABLE . '` (
                    `module_id` varchar(16) NOT NULL,
                    `module_title` varchar(64) DEFAULT NULL,
                    `module_menu` varchar(32) DEFAULT NULL,
                    `module_module` varchar(32) DEFAULT NULL,
                    `module_dependency` varchar(32) DEFAULT NULL,
                    `module_group` int(6) DEFAULT NULL,
                    `module_access` int(11) DEFAULT NULL,
                    `module_weight` int(6) DEFAULT NULL,
                    `module_enabled` int(1) DEFAULT NULL,
                    PRIMARY KEY (`module_id`)
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    PAYMENT_TABLE => 'CREATE TABLE IF NOT EXISTS `' . PAYMENT_TABLE . '` (
                    `id` int(16) NOT NULL AUTO_INCREMENT,
                    `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
                    `bill_number` varchar(16) NOT NULL,
                    `cashier_id` int(16) NOT NULL,
                    `bank_refno` varchar(16) NOT NULL,
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`),
                    KEY `bill_id` (`bill_number`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    ROLE_TABLE => 'CREATE TABLE IF NOT EXISTS `' . ROLE_TABLE . '` (
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `permission` int(8) NOT NULL,
                    `member_id` int(8) NOT NULL,
                    `date_from` date NOT NULL,
                    `date_to` date NOT NULL,
                    `club_id` int(8) NOT NULL,
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`)
                    ) ENGINE=InnoDB  DEFAULT CHARSET=utf8;',
    TYPES_TABLE => 'CREATE TABLE IF NOT EXISTS `' . TYPES_TABLE . '` (
                    `fee_type` int(4) NOT NULL,
                    `type_id` int(8) NOT NULL,
                    `type_name` varchar(32) NOT NULL
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    LM_TABLE => 'CREATE TABLE IF NOT EXISTS `' . LM_TABLE . '` (
                    `lm_id` int(4) NOT NULL AUTO_INCREMENT,
                    `member_id` int(8) NOT NULL,
                    `lm_type_id` int(8) NOT NULL,
                    `date_from` date NOT NULL,
                    `date_to` date NOT NULL,
                    `club_id` int(11) NOT NULL,
                    PRIMARY KEY (`lm_id`),
                    UNIQUE KEY `lm_id` (`lm_id`)
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8  ;',
    SZAMLAZZ_TABLE => 'CREATE TABLE IF NOT EXISTS `' . SZAMLAZZ_TABLE . '` (
                    `bill_number` varchar(16) NOT NULL,
                    `name` varchar(64) NOT NULL,
                    `addr` varchar(64) NOT NULL,
                    `tax` varchar(16) NOT NULL
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    OPTIONS_TABLE => 'CREATE TABLE IF NOT EXISTS `' . OPTIONS_TABLE . '` (
                    `id` int(11) NOT NULL,
                    `name` varchar(32) NOT NULL,
                    `value` varchar(128) NOT NULL
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    NLM_TABLE => 'CREATE TABLE IF NOT EXISTS `' . NLM_TABLE . '` (
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `nl_id` int(8) NOT NULL,
                    `member_id` int(8) NOT NULL,
                    `sent` timestamp(1) NULL DEFAULT NULL,
                    PRIMARY KEY (`id`),
                    UNIQUE KEY `id` (`id`)
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    NEWSLETTER_TABLE => 'CREATE TABLE IF NOT EXISTS `' . NEWSLETTER_TABLE . '` (
                `id` int(8) NOT NULL AUTO_INCREMENT,
                `creator_id` int(8) NOT NULL,
                `creator_time` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1),
                `subject` varchar(64) NOT NULL,
                `body` text NOT NULL,
                PRIMARY KEY (`id`),
                UNIQUE KEY `id` (`id`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    FAI_TABLE => 'CREATE TABLE IF NOT EXISTS `' . FAI_TABLE . '` (
                `id` int(11) NOT NULL AUTO_INCREMENT,
                `idlicence` int(8) NOT NULL,
                `idlicencee_lic` int(8) NOT NULL,
                `hun_id` int(8) NOT NULL,
                PRIMARY KEY (`id`),
                KEY `hun_id` (`hun_id`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    BRANCHES_TABLE => 'CREATE TABLE IF NOT EXISTS `' . BRANCHES_TABLE . '` (
                `'.BRANCHES_ID.'` int(8) NOT NULL AUTO_INCREMENT,
                `'.BRANCHES_NAME.'` varchar(32) NOT NULL,
                 PRIMARY KEY (`'.BRANCHES_ID.'`),
                UNIQUE KEY `'.BRANCHES_ID.'` (`id`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
);
$num_success = $num_unsucess = 0;
foreach ($db_tables as $key => $sql) {
    if ($mysqliLink->query($sql) === true) {
        error_log($key . 'tabla sikeres');
        $num_success++;
    } else {
        error_log($key . 'tabla sikertelen');
        $num_unsucess++;
    }
}
error_log(count($db_tables) . ' tablabol ' . $num_success . ' db. tabla letrejott es ' . $num_unsucess . ' db tabla nem jott letre');

// include_once INCLUDES . 'setup_init.inc';
include_once INCLUDES . 'setup_load.inc';



