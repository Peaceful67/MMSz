<?php

$db_tables = array(
    "categories" => "CREATE TABLE IF NOT EXISTS `categories` (
  `cat_id` int(8) NOT NULL AUTO_INCREMENT,
  `short_name` varchar(32) NOT NULL,
  `long_name` varchar(128) NOT NULL,
  `branch` int(8) NOT NULL DEFAULT '0',
  `org` int(8) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`cat_id`),
  UNIQUE KEY `id` (`cat_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "billing" => "CREATE TABLE IF NOT EXISTS `billing` (
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `fm_id` int(16) NOT NULL,
  `payment_id` int(16) NOT NULL,
  `creator_id` int(8) NOT NULL,
  `price` int(8) NOT NULL,
  `number` varchar(16) NOT NULL,
  `offline` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "category_member" => "CREATE TABLE IF NOT EXISTS `category_member` (
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `member_id` int(8) NOT NULL,
  `category_id` int(8) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `member_id` (`member_id`),
  KEY `member_id_2` (`member_id`),
  KEY `category_id` (`category_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "clubs" => "CREATE TABLE IF NOT EXISTS `clubs` (
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
  PRIMARY KEY (`IDK`),
  UNIQUE KEY `IDK_2` (`IDK`),
  KEY `IDK` (`IDK`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "congress" => "CREATE TABLE IF NOT EXISTS `congress` (
  `member_id` int(8) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;",
    "fee_member" => "CREATE TABLE IF NOT EXISTS `fee_member` (
  `payment_id` int(16) DEFAULT '0',
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `member_id` int(8) NOT NULL,
  `fee_id` int(8) NOT NULL,
  `fm_from` date NOT NULL,
  `fm_to` date NOT NULL,
  `licence` varchar(20) NOT NULL,
  `bill_id` int(16) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "fees" => "CREATE TABLE IF NOT EXISTS `fees` (
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `name` int(8) NOT NULL,
  `fee_from` date NOT NULL,
  `fee_to` date NOT NULL,
  `branch` int(4) NOT NULL DEFAULT '-1',
  `sum` int(8) NOT NULL DEFAULT '0',
  `type` int(4) NOT NULL DEFAULT '0',
  `club` int(8) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "fms2bill" => "CREATE TABLE IF NOT EXISTS `fms2bill` (
  `bill_number` varchar(16) NOT NULL,
  `mem_id` int(8) NOT NULL,
  `fm_id` int(8) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;",
    "group_table" => "CREATE TABLE IF NOT EXISTS `group_table` (
  `group_id` int(6) NOT NULL,
  `group_name` varchar(32) DEFAULT NULL,
  `group_weight` int(6) DEFAULT NULL,
  PRIMARY KEY (`group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;",
    "logs" => "CREATE TABLE IF NOT EXISTS `logs` (
  `id` int(20) NOT NULL AUTO_INCREMENT,
  `activity` varchar(256) NOT NULL,
  `member` int(8) NOT NULL,
  `time_stamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `related_id` int(8) NOT NULL,
  `log_category` int(8) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "members" => "CREATE TABLE IF NOT EXISTS `members` (
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
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "module_table" => "CREATE TABLE IF NOT EXISTS `module_table` (
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
) ENGINE=InnoDB DEFAULT CHARSET=utf8;",
    "payments" => "CREATE TABLE IF NOT EXISTS `payments` (
  `id` int(16) NOT NULL AUTO_INCREMENT,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `bill_number` varchar(16) NOT NULL,
  `cashier_id` int(16) NOT NULL,
  `bank_refno` varchar(16) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `bill_id` (`bill_number`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "prices" => "CREATE TABLE IF NOT EXISTS `prices` (
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `organ` int(4) NOT NULL,
  `insurance` int(4) NOT NULL,
  `type` int(4) NOT NULL,
  `year` int(8) NOT NULL,
  `sum` int(8) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "role_member" => "CREATE TABLE IF NOT EXISTS `role_member` (
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `permission` int(8) NOT NULL,
  `member_id` int(8) NOT NULL,
  `date_from` date NOT NULL,
  `date_to` date NOT NULL,
  `club_id` int(8) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;",
    "ALTER TABLE" => "ALTER TABLE `category_member`
  ADD CONSTRAINT `category_member_ibfk_1` FOREIGN KEY (`member_id`) REFERENCES `members` (`ID`),
  ADD CONSTRAINT `category_member_ibfk_2` FOREIGN KEY (`category_id`) REFERENCES `categories` (`cat_id`);",
    "organizations" => "CREATE TABLE IF NOT EXISTS `organizations` (
  `id` int(8) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
",
    "types" => "CREATE TABLE IF NOT EXISTS `types` (
  `fee_type` int(4) NOT NULL,
  `type_id` int(8) NOT NULL,
  `type_name` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
",
    "licence_member" => "CREATE TABLE IF NOT EXISTS `licence_member` (
  `licence_id` int(4) NOT NULL,
  `member_id` int(8) NOT NULL,
  `date_from` date NOT NULL,
  `date_to` date NOT NULL,
  `club_id` int(11) NOT NULL,
  PRIMARY KEY (`lm_id`),
  UNIQUE KEY `lm_id` (`lm_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8  ;",
    "CREATE TABLE `szamlazz` (
  `bill_number` varchar(16) NOT NULL,
  `name` varchar(64) NOT NULL,
  `addr` varchar(64) NOT NULL,
  `tax` varchar(16) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;",
    "szamlazz" => "
CREATE TABLE `szamlazz` (
  `bill_number` varchar(16) NOT NULL,
  `name` varchar(64) NOT NULL,
  `email` varchar(31) NOT NULL,
  `addr_post` varchar(4) NOT NULL,
  `addr_city` varchar(22) NOT NULL,
  `addr_street` varchar(32) NOT NULL,
  `tax` varchar(16) NOT NULL,
  `prepared` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1),
  `done` timestamp(1) NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
ALTER TABLE `szamlazz`
  ADD KEY `bill_number` (`bill_number`);
COMMIT;
",
    "options" => 'CREATE TABLE `options` (
  `id` int(11) NOT NULL,
  `name` varchar(32) NOT NULL,
  `value` varchar(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    "nl_member" => 'CREATE TABLE `nl_member` (
  `id` int(8) NOT NULL,
  `nl_id` int(8) NOT NULL,
  `member_id` int(8) NOT NULL,
  `sent` timestamp(1) NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
    "newletters" => 'CREATE TABLE `newsletter` (
  `id` int(8) NOT NULL,
  `creator_id` int(8) NOT NULL,
  `creator_time` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1),
  `subject` varchar(64) NOT NULL,
  `body` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;',
);
foreach ($db_tables as $key => $sql) {
    $mysqliLink->query($sql);
}
include_once INCLUDES . 'setup_init.inc';
include_once INCLUDES . 'setup_load.inc';

error_log("Created " . count($db_tables) . " tables");

