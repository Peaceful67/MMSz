<?php

define('EMAIL_TABLE', "emails");
define('EMAIL_ID', "id");
define('EMAIL_ELEMENT_ID', "element_id");
define('EMAIL_SUBJECT', "subject");
define('EMAIL_BODY', "body");
define('EMAIL_SENT', "sent");
define("EMAIL_CONTEXT", 'context');



define("EMAIL_CONTEXT_DUMMY", 'dummy');
define("EMAIL_CONTEXT_CONGRESS", 'congress');
define("EMAIL_CONTEXT_WARNING", 'warning');
define("EMAIL_CONTEXT_VOTE", 'vote');
define("EMAIL_CONTEXT_NEWSLETTER", 'newsletter');

class Email extends BasicElement {

    private $context;

    function __construct($context, $element_id) { // Küldéshez: EMAIL_CONTEXT_DUMMY, -1
        global $mysqliLink;
        parent::__construct();
        $this->setTableName(EMAIL_TABLE);
        $this->setTableFields([
            EMAIL_ID,
            EMAIL_CONTEXT,
            EMAIL_ELEMENT_ID,
            EMAIL_SUBJECT,
            EMAIL_BODY,
            EMAIL_SENT,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . EMAIL_TABLE . '` (`'
                . EMAIL_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . EMAIL_CONTEXT . '` varchar(16) NOT NULL DEFAULT "",`'
                . EMAIL_SUBJECT . '` varchar(256) NOT NULL DEFAULT "",`'
                . EMAIL_BODY . '` text NOT NULL DEFAULT "" ,`'
                . EMAIL_ELEMENT_ID . '` int(8)  NOT NULL DEFAULT -1,`'
                . EMAIL_SENT . '` timestamp(1) NULL DEFAULT NULL,'
                . 'UNIQUE KEY `' . EMAIL_ID . '` (`' . EMAIL_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(EMAIL_ID);
        if (!isset(self::$premade_emails[$context])) {
            error_log('Email: undefined context');
            $context = EMAIL_CONTEXT_DUMMY;
        }
        $this->context = $context;
        $sql = 'SELECT * FROM `' . EMAIL_TABLE . '` WHERE `' . EMAIL_CONTEXT . '`="' . $context . '" AND `' . EMAIL_ELEMENT_ID . '`=' . $element_id . ' LIMIT 1;';
        $res = $mysqliLink->query($sql);
        if ($res AND $row = $res->fetch_assoc()) {
            $this->itemId = $row[EMAIL_ID];
            error_log('Email already exists: ' . $this->itemId);
        } else {
            $this->itemId = $this->insertElement([
                EMAIL_CONTEXT => $context,
                EMAIL_ELEMENT_ID => $element_id,
                EMAIL_SUBJECT => self::$premade_emails[$context][EMAIL_SUBJECT],
                EMAIL_BODY => self::$premade_emails[$context][EMAIL_BODY],
            ]);
            error_log('Email: created:' . $this->itemId);
        }
        $this->getItemById($this->itemId);
    }

    function getEmailById($email_id) {
        return $this->getItemById($email_id);
    }

    function getEmailId($element_id) {
        global $mysqliLink;
        if (!$this->isInitalized(EMAIL_ELEMENT_ID)) {
            return -1;
        }
        $sql = 'SELECT * FROM `' . $this->tableName . '` WHERE `'
                . EMAIL_ELEMENT_ID . '`=' . $element_id .
                ' AND `' . EMAIL_CONTEXT . '`="' . $this->context . '" LIMIT 1;';
        $res = $mysqliLink->query($sql);
        if ($res AND $res->num_rows > 0) {
            $this->tableRow = $res->fetch_assoc();
            return $this->itemId = $this->tableRow[$this->key];
        } else {
            return -1;
        }
    }

    function saveEmail($subject, $body) {
        $this->updateValue(EMAIL_BODY, $body);
        $this->updateValue(EMAIL_SUBJECT, $subject);
    }

    function getEmailSubject() {
        return $this->getValue(EMAIL_SUBJECT);
    }

    function getEmailBody() {
        return $this->getValue(EMAIL_BODY);
    }

    function changeNewLines($body) {
        return str_replace('&#10', '<br>', $body);
    }

    function sendEmail($members) {
        $this->setTime(EMAIL_SENT);
        $emailMember = new EmailMember();
        foreach ($members as $mem_id => $mem_name) {
            $emailMember->push($this->itemId, $mem_id);
        }
    }

    function insertToBody($pos, $tag) {
        $repleace = '%' . $pos . '%';
        $body = str_replace($repleace, $tag, $this->getEmailBody());
        $this->updateValue(EMAIL_BODY, $body);
    }

    private static $premade_emails = [
        EMAIL_CONTEXT_DUMMY => [
            EMAIL_SUBJECT => 'Tárgy',
            EMAIL_BODY => 'Tisztelt %#% ! Üdvözlettel:',
        ],
        EMAIL_CONTEXT_CONGRESS => [
            EMAIL_SUBJECT => 'Online gyűlés meghívó',
            EMAIL_BODY => 'Tisztelt %#% !<br><br>'
            . 'A tagnyilvántartóban létrehoztam egy online gyűlést.<br>'
            . 'A napirendi pontok megtárgyalásában és a szavazásban kérem szíves közreműködésedet.<br>'
            . 'A napirendi pontok megtárgyalásához a tagnyilvántartó "Online gyűlés" -> "Tárgyalás" menüpontja alatt van lehetőség.<br>'
            . 'A gyűlés összefoglalója <a title="Összefoglaló" href="%1%" target="_blank">innen</a> letölthető <br>'
            . 'A szavazás menetének részletes leírása <a title="Leírás a szavazás menetéről" target="_blank" href="' . URL . CONGRESS_HELP_FILE . '">innen</a> tölthető le.<br><br>'
            . 'Üdvözlettel:<br>%2%',
        ],
        EMAIL_CONTEXT_WARNING => [
            EMAIL_SUBJECT => 'Napirend tárgyalás közelgő lezárása',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'Rövidesen lefogjuk zárni a %1% című napirendi pont megtárgyalását és azt szavazásra bocsátom&#10'
            . 'Kérem a mai nap végéig küldje meg hozzászólását.&#10&#10'
            . 'Üdvözlettel:&#10'
            . '%2%',
        ],
        EMAIL_CONTEXT_VOTE => [
            EMAIL_SUBJECT => 'Napirend megszavazása',
            EMAIL_BODY => 'Tisztelt %#% ! Üdvözlettel:',
        ],
    ];

}
