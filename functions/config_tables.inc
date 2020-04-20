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
define("CLUB_TAX", 'tax');
define("CLUB_MANDATUM", 'mandat');

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
define("CONGRESS_ID", 'id');
define("CONGRESS_MEMBER_ID", "member_id");
define("CONGRESS_ENTER", "enter");
define("CONGRESS_QUIT", "quit");


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
define("LM_LICENCE", "lm_id");
define("LM_TYPE_ID", "lm_type_id");
define("LM_MEMBER", "member_id");
define("LM_FROM", "date_from");
define("LM_TO", "date_to");
define("LM_CLUB", "club_id");



define("OPTIONS_TABLE", 'options');
define("OPTIONS_ID", 'id');
define("OPTIONS_NAME", 'name');
define("OPTIONS_VALUE", 'value');

define("NEWSLETTER_TABLE", "newsletter");
define("NEWSLETTER_ID", 'id');
define("NEWSLETTER_SUBJECT", 'subject');
define("NEWSLETTER_BODY", 'body');
define("NEWSLETTER_CREATOR", 'creator_id');
define("NEWSLETTER_TIME", 'creator_time');

define("NLM_TABLE", 'nl_member');
define("NLM_ID", 'id');
define("NLM_MEMBER", 'member_id');
define("NLM_NEWSLETTER_ID", 'nl_id');
define("NLM_SENT", 'sent');

define("FAI_TABLE", 'fai');
define("FAI_ID", 'id');
define("FAI_IDLICENCE", 'idlicence');
define("FAI_LICENCEE", 'idlicencee_lic');
define("FAI_HUNID", 'hun_id');

define("BRANCHES_TABLE", 'branches');
define("BRANCHES_ID", 'id');
define("BRANCHES_NAME", 'name');
