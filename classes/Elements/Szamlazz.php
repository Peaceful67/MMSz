<?php

define('AGENT_URL', 'https://www.szamlazz.hu/szamla/');

/**
 * Description of Szamlazz
 *
 * @author Peaceful
 */
define("SZAMLAZZ_TABLE", "szamlazz");
define("SZAMLAZZ_ID", "id");
define("SZAMLAZZ_BILL_NUMBER", "bill_number");
define("SZAMLAZZ_NAME", "name");
define("SZAMLAZZ_EMAIL", "email");
define("SZAMLAZZ_ADDR_POST", "addr_post");
define("SZAMLAZZ_ADDR_CITY", "addr_city");
define("SZAMLAZZ_ADDR_STREET", "addr_street");
define("SZAMLAZZ_TAX", "tax");
define("SZAMLAZZ_PREPARED", "prepared");
define("SZAMLAZZ_DONE", "done");

define("SZAMLAZZ_DIR", './szamlazz/');

class Szamlazz extends BasicElement {

    protected $bill_number;
    protected $pdf_file;
    protected $xml_file;
    protected $cookie_file;

    function __construct() {
        parent::__construct();
        $this->setTableName(SZAMLAZZ_TABLE);
        $this->setTableFields([SZAMLAZZ_ID, SZAMLAZZ_BILL_NUMBER, SZAMLAZZ_NAME, SZAMLAZZ_EMAIL,
            SZAMLAZZ_ADDR_POST, SZAMLAZZ_ADDR_CITY, SZAMLAZZ_ADDR_STREET,
            SZAMLAZZ_TAX, SZAMLAZZ_PREPARED, SZAMLAZZ_DONE]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `szamlazz` (
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `bill_number` varchar(16) NOT NULL,
                    `name` varchar(64) NOT NULL,
                    `email` varchar(31) NOT NULL,
                    `addr_post` varchar(4) NOT NULL,
                    `addr_city` varchar(22) NOT NULL,
                    `addr_street` varchar(32) NOT NULL,
                    `tax` varchar(16) NOT NULL,
                    `prepared` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1),
                    `done` timestamp(1) NULL DEFAULT NULL,
                    PRIMARY KEY (`id`),
                    KEY `bill_number` (`bill_number`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8; ');
        $this->bill_number = '';
        $this->pdf_file = '';
        $this->xml_file = '';
        $this->cookie_file = '';
    }

    public function getElementByBillNumber($bn) {
        if ($this->getElementBy(SZAMLAZZ_BILL_NUMBER, $bn)) {
            $this->bill_number = $this->getValue(SZAMLAZZ_BILL_NUMBER);
            if (!empty($this->bill_number)) {
                return true;
            }
        }
        return false;
    }

    public function getPdfFileName($bn) {
        return SZAMLAZZ_DIR . 'szamla_' . $bn . '.pdf';
    }

    public function getPdfURL($bn) {
        return URL . $this->getPdfFileName($bn);
    }

    public function isPdfExists($bn) {
        return is_file($this->getPdfFileName($bn));
    }

    public function deleteSzamlaByBillNumber($bn) {
        $this->deleteElementBy(SZAMLAZZ_BILL_NUMBER, $bn);
    }

    private function getItem($key) {
        if (isset($this->tableRow)) {
            return $this->tableRow[$key];
        } else {
            return '';
        }
    }

    public function getName() {
        return $this->getItem(SZAMLAZZ_NAME);
    }

    public function getEmailAddr() {
        return $this->getItem(SZAMLAZZ_EMAIL);
    }

    public function insertSzamla($number, $email, $name, $post, $city, $street, $tax) {
        global $member_id;
        if (!$this->insertElement([
                    SZAMLAZZ_BILL_NUMBER => $number,
                    SZAMLAZZ_EMAIL => $email,
                    SZAMLAZZ_NAME => $name,
                    SZAMLAZZ_ADDR_POST => $post,
                    SZAMLAZZ_ADDR_CITY => $city,
                    SZAMLAZZ_ADDR_STREET => $street,
                    SZAMLAZZ_TAX => $tax
                ])) {
            error_log('Nem sikerült a szamlat elokesziteni');
        }
        logger($member_id, -1, LOGGER_SZAMLAZZ, $number . ' számla előkészítése');
    }

    private function setSzamlaDone() {
        global $mysqliLink, $member_id;
        $sql = 'UPDATE `' . SZAMLAZZ_TABLE . '` SET `' . SZAMLAZZ_DONE . '`=CURRENT_TIMESTAMP '
                . 'WHERE `' . SZAMLAZZ_BILL_NUMBER . '`="' . $this->bill_number . '";';
        $mysqliLink->query($sql);
        logger($member_id, -1, LOGGER_SZAMLAZZ, $this->bill_number . ' számla kiállítása sikeres');
    }

    private function createXml() {
        $today = date('Y-m-d');
        $xml = '<?xml version="1.0" encoding="UTF-8"?>
        <xmlszamla xmlns="http://www.szamlazz.hu/xmlszamla" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.szamlazz.hu/xmlszamla http://www.szamlazz.hu/docs/xsds/agent/xmlszamla.xsd">
        <beallitasok>
            <felhasznalo>' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_USER) . '</felhasznalo>
            <jelszo>' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_PASSWD) . '</jelszo>
            <eszamla>true</eszamla>
            <kulcstartojelszo></kulcstartojelszo>
            <szamlaLetoltes>true</szamlaLetoltes>
            <szamlaLetoltesPld>1</szamlaLetoltesPld>
            <valaszVerzio>1</valaszVerzio>
            <aggregator></aggregator>
        </beallitasok>
        <fejlec>
            <keltDatum>' . $today . '</keltDatum>
            <teljesitesDatum>' . $today . '</teljesitesDatum>
            <fizetesiHataridoDatum>' . $today . '</fizetesiHataridoDatum>
            <fizmod>Átutalás</fizmod>
            <penznem>Ft</penznem>
            <szamlaNyelve>hu</szamlaNyelve>
            <megjegyzes></megjegyzes>    
            <rendelesSzam>' . $this->bill_number . '</rendelesSzam>
            <elolegszamla>false</elolegszamla>
            <vegszamla>false</vegszamla>
            <helyesbitoszamla>false</helyesbitoszamla>
            <dijbekero>false</dijbekero>
            <szallitolevel>false</szallitolevel>
            <szamlaszamElotag>' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_PREFIX) . '</szamlaszamElotag>
            <fizetve>true</fizetve>
        </fejlec>
        <elado>
            <bank>' . getOptionValue(OPTIONS_NAME_COMPANY_BANK_NAME) . '</bank>
            <bankszamlaszam>' . getOptionValue(OPTIONS_NAME_COMPANY_BANK_ACCOUNT) . '</bankszamlaszam>
            <emailReplyto>' . getOptionValue(OPTIONS_NAME_MMSZ_EMAIL) . '</emailReplyto>
            <emailTargy>Számla értesítő</emailTargy>
            <emailSzoveg>Fizetését lekönyveltük, a számlát a mellékletben találja.</emailSzoveg>
            <alairoNeve>' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . '</alairoNeve>
        </elado>
        <vevo>
            <nev>' . $this->tableRow[SZAMLAZZ_NAME] . '</nev>
            <irsz>' . $this->tableRow[SZAMLAZZ_ADDR_POST] . '</irsz>
            <telepules>' . $this->tableRow[SZAMLAZZ_ADDR_CITY] . '</telepules>
            <cim>' . $this->tableRow[SZAMLAZZ_ADDR_STREET] . '</cim>
            <email>' . $this->tableRow[SZAMLAZZ_EMAIL] . '</email>
            <sendEmail>true</sendEmail>
            <adoszam>' . $this->tableRow[SZAMLAZZ_TAX] . '</adoszam>
        </vevo>
        <tetelek>';
        foreach (billItems($this->bill_number) as $bill) {
            $fm = getFmById($bill[BILLING_FM_ID]);
            $fee = getFeeById($fm[FM_FEE_ID]);
            $xml .= '
            <tetel>
                <megnevezes>' . get_fee_name($fee[FEE_TYPE], $fee[FEE_NAME]) . '</megnevezes>
                <mennyiseg>1</mennyiseg>
                <mennyisegiEgyseg>db</mennyisegiEgyseg>
                <nettoEgysegar>' . $bill[BILLING_PRICE] . '</nettoEgysegar>
                <afakulcs>TAM</afakulcs>
                <nettoErtek>' . $bill[BILLING_PRICE] . '</nettoErtek>
                <afaErtek>0</afaErtek>
                <bruttoErtek>' . $bill[BILLING_PRICE] . '</bruttoErtek>
                <megjegyzes>' . getMemberName($fm[FM_MEMEBER_ID]) . '</megjegyzes>
            </tetel>';
        }

        $xml .= '  </tetelek></xmlszamla>';
        return $xml;
    }

    private function sendXml($xml) {
        $this->xml_file = SZAMLAZZ_DIR . 'szamlazz_xml_' . $this->bill_number . '.xml';
        $this->cookie_file = SZAMLAZZ_DIR . 'szamlazz_cookie_' . $this->bill_number . '.txt';
        file_put_contents($this->xml_file, $xml);
        if (!file_exists($this->cookie_file)) {
            file_put_contents($this->cookie_file, '');
        }
        $curl = curl_init(AGENT_URL);
        curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($curl, CURLOPT_POST, true);
        curl_setopt($curl, CURLOPT_HEADER, true);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($curl, CURLOPT_POSTFIELDS, array('action-xmlagentxmlfile' => new CURLFile(realpath($this->xml_file))));
        curl_setopt($curl, CURLOPT_SAFE_UPLOAD, true);
        curl_setopt($curl, CURLOPT_TIMEOUT, 30);
        curl_setopt($curl, CURLOPT_COOKIEJAR, $this->cookie_file);
        if (file_exists($cookie_file) && filesize($this->cookie_file) > 0) {
            curl_setopt($curl, CURLOPT_COOKIEFILE, $this->cookie_file);
        }
        return $curl;
    }

    private function getSzamlazzResponse($ch) {
        $this->pdf_file = $this->getPdfFileName($this->bill_number);
        $agent_response = curl_exec($ch);
        $http_error = curl_error($ch);
        $agent_http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        $header_size = curl_getinfo($ch, CURLINFO_HEADER_SIZE);
        $agent_header = substr($agent_response, 0, $header_size);
        $agent_body = substr($agent_response, $header_size);
        curl_close($ch);
        $header_array = explode("\n", $agent_header);
        $volt_hiba = false;
        $agent_error = '';
        $agent_error_code = '';
        foreach ($header_array as $val) {
            if (substr($val, 0, strlen('szlahu')) === 'szlahu') {
                echo urldecode($val) . '<br>';
                if (substr($val, 0, strlen('szlahu_error:')) === 'szlahu_error:') {
                    $volt_hiba = true;
                    $agent_error = substr($val, strlen('szlahu_error:'));
                }
                if (substr($val, 0, strlen('szlahu_error_code:')) === 'szlahu_error_code:') {
                    $volt_hiba = true;
                    $agent_error_code = substr($val, strlen('szlahu_error_code:'));
                }
            }
        }

        if ($http_error != "") {
            error_log('Http hiba történt:' . $http_error);
            $volt_hiba = true;
        }

        if ($volt_hiba) {
            $err_mail_body = 'Agent hibakód: ' . $agent_error_code.'<br>';
            $err_mail_body .= 'Agent hibaüzenet: ' . urldecode($agent_error).'<br>';
            $err_mail_body .= 'Agent válasz: ' . urldecode($agent_body);
            mail_system_error($err_mail_body);
            error_log('Agent hibakód: ' . $agent_error_code);
            error_log('Agent hibaüzenet: ' . urldecode($agent_error));
            error_log('Agent válasz: ' . urldecode($agent_body));
            logger(-1, -1, LOGGER_SZAMLAZZ, 'Agent hibakód: ' . $agent_error_code . ', Agent hibaüzenet: ' . urldecode($agent_error));
            return false;
        } else {
            file_put_contents($this->pdf_file, $agent_body);
        }
        unlink($this->xml_file);
        unlink($this->cookie_file);
        return true;
    }

    public function makeSzamla() {
        $xml = $this->createXml();
        $curl = $this->sendXml($xml);
        $ret = $this->getSzamlazzResponse($curl);
        if ($ret) {
            $this->setSzamlaDone();
            setBillPrinted($this->bill_number);
        }
        return $ret;
    }

    public function szamlaToString() {
        $ar = ["szamla" => $this->tableRow,
            "pdf_file" => $this->pdf_file];
        return serialize($ar);
    }

}
