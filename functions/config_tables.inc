<?php

define("MEMBER_TABLE", "members");
define("MEMBER_ID", "ID");
define("MEMBER_VEZNEV", "VezNev");
define("MEMBER_KERNEV", "KerNev");
define("MEMBER_EMAIL", "E-mail");
define("MEMBER_BORN", "born");
define("MEMBER_SEX", "Nem");
define("MEMBER_ADDR_POST", "IrSz");
define("MEMBER_ADDR_CITY", "Varos");
define("MEMBER_ADDR_STREET", "Lakcim");
define("MEMBER_PASSWORD", "password");
define("MEMBER_KATEG", "Kateg");
define("MEMBER_TEL", "telefon");
define("MEMBER_HOMEPAGE", "homepage");
define("MEMBER_NICKNAME", "nickname");
define("MEMBER_LICENCE", "licence");
define("MEMBER_BIRTH_CITY", "birth_city");
define("MEMBER_MOTHER", "mother");
define("MEMBER_GDPR", "gdpr");


define("CLUB_TABLE", "clubs");
define("CLUB_ID", "IDK");
define("CLUB_NAME", "KlubNev");
define("CLUB_EMAIL", "email");
define("CLUB_ADDR_POST", "IrSzK");
define("CLUB_ADDR_CITY", "VarosK");
define("CLUB_ADDR_STREET", "CimK");
define("CLUB_ACT", "act");
define("CLUB_COMMENT", "comment");
define("CLUB_HOMEPAGE", "homepage");
define("CLUB_BANK", "bank");

define("CATEGORIES_TABLE", 'categories');
define("CATEGORIES_ID", "cat_id");
define("CATEGORIES_SHORT", 'short_name');
define("CATEGORIES_LONG", 'long_name');
define("CATEGORIES_BRANCH", 'branch');
define("CATEGORIES_ORG", 'org');

define("CM_TABLE", "category_member");
define("CM_ID", "id");
define("CM_MEMBER_ID", "member_id");
define("CM_CATEGORY", "category_id");

define("FEE_TABLE", "fees");
define("FEE_ID", "id");
define("FEE_NAME", "name");
define("FEE_FROM", "fee_from");
define("FEE_TO", "fee_to");
define("FEE_TYPE", "type");
define("FEE_CLUB", "club");
define("FEE_BRANCH", "branch");
define("FEE_SUM", "sum");

define("FM_TABLE", "fee_member");
define("FM_ID", "id");
define("FM_MEMEBER_ID", "member_id");
define("FM_FEE_ID", "fee_id");
define("FM_FROM", "fm_from");
define("FM_TO", "fm_to");
define("FM_LICENCE", "licence");
define("FM_BILL_ID", "bill_id");
define("FM_PAY_ID", "payment_id");

define("PAYMENT_TABLE", "payments");
define("PAYMENT_ID", "id");
define("PAYMENT_BILL_NUMBER", "bill_number");
define("PAYMENT_TIME", "time");
define("PAYMENT_CASHIER", "cashier_id");
define("PAYMENT_BANK_REFNO", "bank_refno");


define("BILLING_TABLE", "billing");
define("BILLING_ID", "id");
define("BILLING_TIME", "time");
define("BILLING_CREATOR", "creator_id");
define("BILLING_FM_ID", "fm_id");
define("BILLING_PAYMENT_ID", "payment_id");
define("BILLING_PRICE", "price");
define("BILLING_NUMBER", "number");
define("BILLING_OFFLINE", "offline");

define("ROLE_TABLE", "role_member");
define("ROLE_ID", "id");
define("ROLE_PERMISSION", "permission");
define("ROLE_MEMBER", "member_id");
define("ROLE_FROM", "date_from");
define("ROLE_TO", "date_to");
define("ROLE_CLUB", "club_id");
define("ROLE_CATEGORY_LEADER", ROLE_CLUB);

define("CONGRESS_TABLE", "congress");
define("CONGRESS_MEMBER_ID", "member_id");

define("FMS2BILL_TABLE", "fms2bill");
define("FMS2BILL_BILL_NUMBER", "bill_number");
define("FMS2BILL_MEM_ID", "mem_id");
define("FMS2BILL_FM_ID", "fm_id");
define("FMS2BILL_TIME", "time");

define("SETUP_MOD_TABLE", "module_table");
define("SETUP_MOD_TITLE", "module_title");
define("SETUP_MOD_MENU", "module_menu");
define("SETUP_MOD_GROUP", "module_group");
define("SETUP_MOD_ID", "module_id");
define("SETUP_MOD_MODULE", "module_module");
define("SETUP_MOD_ACCESS", "module_access");
define("SETUP_MOD_WEIGHT", "module_weight");
define("SETUP_MOD_DEPENDENCY", "module_dependency");
define("SETUP_MOD_ENABLED", "module_enabled");

define("SETUP_GROUP_ID", "group_id");
define("SETUP_GROUP_WEIGHT", "group_weight");
define("SETUP_GROUP_NAME", "group_name");
define("SETUP_GROUP_TABLE", "group_table");

define("TYPES_TABLE", "types");
define("TYPES_TYPE", "fee_type");
define("TYPES_TYPE_ID", "type_id");
define("TYPES_TYPE_NAME", "type_name");

define("LM_TABLE", "licence_member");
define("LM_LICENCE", "licence_id");
define("LM_MEMBER", "member_id");
define("LM_FROM", "date_from");
define("LM_TO", "date_to");
define("LM_CLUB", "club_id");

define("SZAMLAZZ_TABLE", "szamlazz");
define("SZAMLAZZ_BILL_NUMBER", "bill_number");
define("SZAMLAZZ_NAME", "name");
define("SZAMLAZZ_EMAIL", "email");
define("SZAMLAZZ_ADDR_POST", "addr_post");
define("SZAMLAZZ_ADDR_CIYT", "addr_city");
define("SZAMLAZZ_ADDR_STREET", "addr_street");
define("SZAMLAZZ_TAX", "tax");