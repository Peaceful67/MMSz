<?php

define('EMAIL_TABLE', "emails");
define('EMAIL_ID', "id");
define('EMAIL_ELEMENT_ID', "element_id");
define('EMAIL_SUBJECT', "subject");
define('EMAIL_BODY', "body");
define('EMAIL_OPTIONS', 'options');
define('EMAIL_SENT', "sent");
define("EMAIL_CONTEXT", 'context');

define("EMAIL_OPTION_FILENAME", 'filename');

define("EMAIL_CONTEXT_DUMMY", 'dummy');
define("EMAIL_CONTEXT_CONGRESS", 'congress');
define("EMAIL_CONTEXT_ITEM_WARNING", 'item_warning');
define("EMAIL_CONTEXT_POLL_WARNING", 'poll_warning');
define("EMAIL_CONTEXT_VOTE", 'vote');
define("EMAIL_CONTEXT_VOTE_BY_EMAIL", 'vote_by_email');
define("EMAIL_CONTEXT_NEWSLETTER", 'newsletter');
define("EMAIL_CONTEXT_POLL_CLOSED_CREATOR", 'poll_creator');
define("EMAIL_CONTEXT_POLL_CLOSED_DELEGATE", 'poll_delegate');
define("EMAIL_CONTEXT_CONGRESS_CLOSED_CREATOR", 'congress_creator');
define("EMAIL_CONTEXT_CONGRESS_CLOSED_DELEGATE", 'congress_delegate');
define("EMAIL_CONTEXT_ONLINE_INVITE", 'online_invite');
define("EMAIL_CONTEXT_INFORM_NEW_MEMBER", 'new_member');

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
            EMAIL_OPTIONS,
            EMAIL_SENT,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . EMAIL_TABLE . '` (`'
                . EMAIL_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . EMAIL_CONTEXT . '` varchar(16) NOT NULL DEFAULT "",`'
                . EMAIL_SUBJECT . '` varchar(256) NOT NULL DEFAULT "",`'
                . EMAIL_BODY . '` text NOT NULL DEFAULT "" ,`'
                . EMAIL_OPTIONS . '` varchar(256) NOT NULL DEFAULT "",`'
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
        } else {
            $this->itemId = $this->insertElement([
                EMAIL_CONTEXT => $context,
                EMAIL_ELEMENT_ID => $element_id,
                EMAIL_SUBJECT => self::$premade_emails[$context][EMAIL_SUBJECT],
                EMAIL_BODY => self::$premade_emails[$context][EMAIL_BODY],
            ]);
        }
        $this->getItemById($this->itemId);
    }

    function getEmailById($email_id) {
        $this->getItemById($email_id);
        return $this;
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

    protected function getEmailOption() {
        return $this->getValue(EMAIL_OPTIONS);
    }

    protected function setEmailOption($option) {
        $this->updateValue(EMAIL_OPTIONS, $option);
    }

    function setFilename($filename) {
        $option_arr = unserialize($this->getEmailOption());
        $option_arr[EMAIL_OPTION_FILENAME] = $filename;
        $this->setEmailOption(serialize($option_arr));
    }

    function getFilename() {
        $option_arr = unserialize($this->getEmailOption());
        if (isset($option_arr[EMAIL_OPTION_FILENAME])) {
            return $option_arr[EMAIL_OPTION_FILENAME];
        } else {
            return "";
        }
    }

    function getEmailBody() {
        return $this->getValue(EMAIL_BODY);
    }

    function changeNewLines($body) {
        return str_replace('&#10', '<br>', $body);
    }

    function sendEmail($members) {
        $this->setTimeCurrent(EMAIL_SENT);
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

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
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
            . 'A napirendi pontok megtárgyalásában és a szavazásban kérem szíves közreműködését.<br>'
            . 'A napirendi pontok megtárgyalásához a <a taget="_blank" href="' . URL . '">tagnyilvántartó</a> rendszerbe be kell lépnie. Belépéshez az azonosítója: %@%<br>'
            . 'A főmenüben "Online gyűlés" -> "Tárgyalás" menüpontja alatt van lehetősége hozzászólni a napirendi pontokhoz.<br>'
            . 'A gyűlés összefoglalóját a mellékletben találja, és a folyamatosan figyelemmel kisérheti a tagnyilvántartó "Online gyűlés" -> "Gyűlések" menüpontja alatt <br>'
            . 'A szavazás menetének részletes leírása <a title="Leírás a szavazás menetéről" target="_blank" href="http://modellsport.hu/dokumentumok/online-szavazasi-rendszer-hasznalati-utasitas-kuldottek-szamara">innen</a> tölthető le.<br><br>'
            . 'Üdvözlettel:<br>%2%',
        ],
        EMAIL_CONTEXT_ONLINE_INVITE => [
            EMAIL_SUBJECT => 'Meghívó elektronikus gyűlés tárgyalására',
            EMAIL_BODY => 'Tisztelt %#% !<br><br>'
            . 'A tagnyilvántartóban folyamatban van egy elektronikus gyűlés.<br>'
            . 'A gyűlés címe: %1%<br>'
            . 'Ön nem szerepel a gyűlésre meghívott küldöttek között. Az egyik napirendi pont tárgyalása közben azonban érintett lett, ezért felvettem Önt a tárgyaláson résztvevők közé.<br>'
            . 'Szavazati jog ezzel nem jár, viszont örömmel vennénk, ha tárgyaláson hozzászólásával kifejtené véleményét.<br>'
            . 'A napirendi pontok megtárgyalásához a <a taget="_blank" href="' . URL . '">tagnyilvántartó</a> rendszerbe be kell lépnie. Belépéshez az azonosítója: %@%<br>'
            . 'A főmenüben "Online gyűlés" -> "Tárgyalás" menüpontja alatt van lehetősége megtekinteni és hozzászólni a napirendi pontokhoz.<br>'
            . 'A gyűlés összefoglalóját folyamatosan figyelemmel kisérheti a tagnyilvántartó "Online gyűlés" -> "Gyűlések" menüpontja alatt <br>'
            . 'A szavazás menetének részletes leírása <a title="Leírás a szavazás menetéről" target="_blank" href="http://modellsport.hu/dokumentumok/online-szavazasi-rendszer-hasznalati-utasitas-kuldottek-szamara">innen</a> tölthető le.<br><br>'
            . 'Üdvözlettel:<br>%2%',
        ],
        EMAIL_CONTEXT_POLL_WARNING => [
            EMAIL_SUBJECT => 'Felhívás szavazásra',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'Még nem szavazott a %1% című határozati javaslatról.&#10A szavazás automatikusan lezárul:%2% &#10'
            . 'Ezután már nem lehet majd a határozati javaslatról szavazni.&#10'
            . 'Kérem, minél hamarabb lépjen be a tagnyilvántartó felületére és az "Online gyűlés" => "Szavazás" menüpont alatt küldje el szavazatát.&#10&#10'
            . 'Üdvözlettel:&#10'
            . '%3%',
        ],
        EMAIL_CONTEXT_ITEM_WARNING => [
            EMAIL_SUBJECT => 'Napirend tárgyalás közelgő lezárása',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'Rövidesen lefogjuk zárni a %1% című napirendi pont megtárgyalását és azt szavazásra bocsátom&#10'
            . 'Kérem, minél hamarabb küldje meg hozzászólását.&#10&#10'
            . 'Üdvözlettel:&#10'
            . '%2%',
        ],
        EMAIL_CONTEXT_VOTE => [
            EMAIL_SUBJECT => 'Szavazás megkezdése',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'Lezártam a "%1%" című napirendi pont megtárgyalását és azt szavazásra bocsátottam&#10&#10'
            . '</b>A határozati javaslat címe:</b> %2% &#10'
            . '</b>A határozati javaslat szövege:</b> %3% &#10'
            . 'Szavazni a tagnyilvántartó "Online gyűlés" => "Szavazás" menüpontja alatt lehet.&#10'
            . 'Szavazás állapotát  "Online gyűlés" => "Gyűlések" menüpontja alatt lehet figyelemmel kísérni.&#10'
            . 'A szavazás lezárásig van lehetőség a szavazat módosítására. Ennek legkésőbbi időpontja: %4%.&#10&#10'
            . 'Üdvözlettel:&#10'
            . '%5%',
        ],
        EMAIL_CONTEXT_VOTE_BY_EMAIL => [
            EMAIL_SUBJECT => 'Szavazás megkezdése',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'Lezártam a "%1%" című napirendi pont megtárgyalását és azt szavazásra bocsátottam&#10&#10'
            . '<b>A határozati javaslat címe:</b> %2% &#10'
            . '<b>A határozati javaslat szövege:</b> %3% &#10&#10'
            . 'Erről a napirendről lehetőség van a tagnyilvántartóba belépés nélkül, az alábbi linkekre kattintva is szavazni. '
            . 'A linkeket egyszer lehet csak használni, illetve a szavazás lezárásáig élnek.&#10'
            . '&#10 %6%'
            . '&#10Ezen kivül lehetőség van szavazni vagy szavazotot módosítani a tagnyilvántartó "Online gyűlés" => "Szavazás" menüpontja alatt.&#10'
            . 'Szavazás állapotát  "Online gyűlés" => "Gyűlések" menüpontja alatt lehet figyelemmel kísérni.&#10'
            . 'A szavazás lezárásig van lehetőség a szavazat módosítására. Ennek legkésőbbi időpontja: %4%.&#10&#10'
            . 'Üdvözlettel:&#10'
            . '%5%',
        ],
        EMAIL_CONTEXT_POLL_CLOSED_CREATOR => [
            EMAIL_SUBJECT => 'Napirend megszavazása',
            EMAIL_BODY => 'Tisztelt %#% ! Üdvözlettel:',
        ],
        EMAIL_CONTEXT_POLL_CLOSED_DELEGATE => [
            EMAIL_SUBJECT => 'Napirend megszavazása',
            EMAIL_BODY => 'Tisztelt %#% ! Üdvözlettel:',
        ],
        EMAIL_CONTEXT_CONGRESS_CLOSED_CREATOR => [
            EMAIL_SUBJECT => 'Gyűlés lezárása',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'A "%1%" című gyűlés összes napirendjében döntés született, a gyűlésnek vége.&#10'
            . 'Az összes résztvevő értesítést kap, mellékelve az elkészült jegyzőkönyvet.&#10'
            . 'A jegyzőkönyv később is elérhető a tagnyilvántartó  "Online gyűlés" => "Gyűlések" menüpontja alatt.&#10&#10'
            . 'Üdvözlettel:&#10',
        ],
        EMAIL_CONTEXT_CONGRESS_CLOSED_DELEGATE => [
            EMAIL_SUBJECT => 'Gyűlés lezárása',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'A "%1%" című gyűlés összes napirendjében döntés született, a gyűlésnek vége.&#10'
            . 'Köszönöm a gyűlésen való részvételét és az estleges hozzászólásait.&#10'
            . 'Az elkészült jegyzőkönyvet a mellékletben találja, de az később is elérhető a tagnyilvántartó  "Online gyűlés" => "Gyűlések" menüpontja alatt.&#10&#10'
            . 'Üdvözlettel:&#10'
            . '%2%',
        ],
        EMAIL_CONTEXT_INFORM_NEW_MEMBER => [
            EMAIL_SUBJECT => 'Regisztráció a tagnyilvántartóba',
            EMAIL_BODY => 'Tisztelt %#% ! &#10&#10'
            . 'A "%1%" tagságod okán az imént regisztráltalak a tagnyilvántartóban.&#10'
            . 'Az adataid kezeléséhez be is tudsz lépni a <a href="http://%2%" title="%1%">%2%</a> oldalon a tagnyilvántartóba az "Első belépés" rész szerint.&#10'
            . 'Ehhez szükséges azonosítód, amire a későbbiekben is szükséged lehet:<b>%@%</b>.&#10&#10'
            . 'Üdvözlettel:&#10'
            . '%3%',
        ],
    ];

}
 