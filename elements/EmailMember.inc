<?php

define('EMAIL_MEMBER_TABLE', "email_member");
define('EMAIL_MEMBER_ID', "id");
define('EMAIL_MEMBER_EMAIL_ID', "email_id");
define('EMAIL_MEMBER_MEMBER_ID', "member_id");
define('EMAIL_MEMBER_SENT', "sent");

class EmailMember extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(EMAIL_MEMBER_TABLE);
        $this->setTableFields([
            EMAIL_MEMBER_ID,
            EMAIL_MEMBER_EMAIL_ID,
            EMAIL_MEMBER_MEMBER_ID,
            EMAIL_SENT,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . EMAIL_MEMBER_TABLE . '` (`'
                . EMAIL_MEMBER_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . EMAIL_MEMBER_EMAIL_ID . '` int(8) NOT NULL DEFAULT -1,`'
                . EMAIL_MEMBER_MEMBER_ID . '` int(8)  NOT NULL DEFAULT -1,`'
                . EMAIL_MEMBER_SENT . '` timestamp(1) NULL DEFAULT NULL,'
                . 'UNIQUE KEY `' . EMAIL_MEMBER_ID . '` (`' . EMAIL_MEMBER_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(EMAIL_MEMBER_ID);
    }

    function push($email_id, $mem_id) {
        return $this->insertElement([
                    EMAIL_MEMBER_EMAIL_ID => $email_id,
                    EMAIL_MEMBER_MEMBER_ID => $mem_id,
        ]);
    }

    function retriveUnsentEmail() {
        $unsent = $this->getElementsIsNull(EMAIL_SENT);
        if (empty($unsent)) {
            return false;
        }
        reset($unsent);
        $this->getItemById(key($unsent));
        return true;
    }

    function getEmailId() {
        return $this->getValue(EMAIL_MEMBER_EMAIL_ID);
    }

    function getMemberId() {
        return $this->getValue(EMAIL_MEMBER_MEMBER_ID);
    }

    function setSent() {
        $this->setTime(EMAIL_SENT);
    }

}
