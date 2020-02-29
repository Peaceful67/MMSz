<?php

define('AGENT_URL', 'https://www.szamlazz.hu/szamla/');
$cookie_file = TMP_DIR . 'szamlazz_cookie_' . $szamla[SZAMLAZZ_BILL_NUMBER] . '.txt';
$xml_file = TMP_DIR . 'szamlazz_xml_' . $szamla[SZAMLAZZ_BILL_NUMBER] . '.xml';
$pdf_file = SZAMLAK . 'szamla_' . $szamla[SZAMLAZZ_BILL_NUMBER] . '.pdf';

$xml = '<?xml version="1.0" encoding="UTF-8"?>
<xmlszamla xmlns="http://www.szamlazz.hu/xmlszamla" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.szamlazz.hu/xmlszamla http://www.szamlazz.hu/docs/xsds/agent/xmlszamla.xsd">
  <beallitasok>
    <felhasznalo>' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_USER) . '</felhasznalo>
    <jelszo>' . getOptionValue(OPTIONS_NAME_SZAMLAZZ_PASSWD) . '</jelszo>
    <eszamla>true</eszamla>
    <kulcstartojelszo></kulcstartojelszo>
    <szamlaLetoltes>true</szamlaLetoltes>
    <szamlaLetoltesPld>1</szamlaLetoltesPld>
    <valaszVerzio>2</valaszVerzio>
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
    <rendelesSzam>' . $szamla[SZAMLAZZ_BILL_NUMBER] . '</rendelesSzam>
    <elolegszamla>false</elolegszamla>
    <vegszamla>true</vegszamla>
    <helyesbitoszamla>false</helyesbitoszamla>
    <dijbekero>false</dijbekero>
    <szallitolevel>false</szallitolevel>
    <szamlaszamElotag>MMSZ_TNY_</szamlaszamElotag>
    <fizetve>true</fizetve>
  </fejlec>
  <elado>
    <bank>Bank</bank>
    <bankszamlaszam>' . getOptionValue(OPTIONS_NAME_COMPANY_BANK) . '</bankszamlaszam>
    <emailReplyto>' . getOptionValue(OPTIONS_NAME_MMSZ_EMAIL) . '</emailReplyto>
    <emailTargy>Számla értesítő</emailTargy>
    <emailSzoveg>Fizetését lekönyveltük, a számlát a mellékletben találja.</emailSzoveg>
    <alairoNeve>' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . '</alairoNeve>
  </elado>
  <vevo>
    <nev>' . $szamla[SZAMLAZZ_NAME] . '</nev>
    <irsz>' . $szamla[SZAMLAZZ_ADDR_POST] . '</irsz>
    <telepules>' . $szamla[SZAMLAZZ_ADDR_CITY] . '</telepules>
    <cim>' . $szamla[SZAMLAZZ_ADDR_STREET] . '</cim>
    <email>' . $szamla[SZAMLAZZ_EMAIL] . '</email>
    <sendEmail>true</sendEmail>
    <adoszam>' . $szamla[SZAMLAZZ_TAX] . '</adoszam>
  </vevo>
  <tetelek>';
foreach (billItems($szamla[SZAMLAZZ_BILL_NUMBER]) as $bill) {
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

$xml .= '  </tetelek>
</xmlszamla>';

if (!file_exists($cookie_file)) {
    file_put_contents($cookie_file, '');
}

// a CURL inicializálása
$ch = curl_init(AGENT_URL);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_HEADER, true);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
curl_setopt($ch, CURLOPT_SAFE_UPLOAD, true);

curl_setopt($ch, CURLOPT_TIMEOUT, 30);

// Itt állítjuk be, hogy az érkező cookie a $cookie_file-ba kerüljön mentésre
curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_file);

// Ha van már cookie file-unk, és van is benne valami, elküldjük a Számlázz.hu-nak
if (file_exists($cookie_file) && filesize($cookie_file) > 0) {
    curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file);
}

// elküldjük a kérést a Számlázz.hu felé, és eltároljuk a választ
$agent_response = curl_exec($ch);

// kiolvassuk a curl-ból volt-e hiba
$http_error = curl_error($ch);

// ezekben a változókban tároljuk a szétbontott választ
$agent_header = '';
$agent_body = '';
$agent_http_code = '';

// lekérjük a válasz HTTP_CODE-ját, ami ha 200, akkor a http kommunikáció rendben volt
// ettől még egyáltalán nem biztos, hogy a számla elkészült
$agent_http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);

// a válasz egy byte kupac, ebből az első "header_size" darab byte lesz a header
$header_size = curl_getinfo($ch, CURLINFO_HEADER_SIZE);

// a header tárolása, ebben lesznek majd a számlaszám, bruttó nettó összegek, errorcode, stb.
$agent_header = substr($agent_response, 0, $header_size);

// a body tárolása, ez lesz a pdf, vagy szöveges üzenet
$agent_body = substr($agent_response, $header_size);

// a curl már nem kell, lezárjuk
curl_close($ch);

// a header soronként tartalmazza az információkat, egy tömbbe teszük a külön sorokat
$header_array = explode("\n", $agent_header);

// ezt majd true-ra állítjuk ha volt hiba
$volt_hiba = false;

// ebben lesznek a hiba információk, plusz a bodyban
$agent_error = '';
$agent_error_code = '';

// menjünk végig a header sorokon, ami "szlahu"-val kezdődik az érdekes nekünk és írjuk ki
foreach ($header_array as $val) {
    if (substr($val, 0, strlen('szlahu')) === 'szlahu') {
        echo urldecode($val) . '<br>';
        // megvizsgáljuk, hogy volt-e hiba
        if (substr($val, 0, strlen('szlahu_error:')) === 'szlahu_error:') {
            // sajnos volt
            $volt_hiba = true;
            $agent_error = substr($val, strlen('szlahu_error:'));
        }
        if (substr($val, 0, strlen('szlahu_error_code:')) === 'szlahu_error_code:') {
            // sajnos volt
            $volt_hiba = true;
            $agent_error_code = substr($val, strlen('szlahu_error_code:'));
        }
    }
}

// ha volt http hiba dobunk egy kivételt
if ($http_error != "") {
    error_log('Http hiba történt:' . $http_error);
}

if ($volt_hiba) {

    // ha a számla nem készült el kiírjuk amit lehet
    error_log('Agent hibakód: ' . $agent_error_code);
    error_log('Agent hibaüzenet: ' . urldecode($agent_error));
    error_log('Agent válasz: ' . urldecode($agent_body));

    // dobunk egy kivételt
    $output .= ('Számlakészítés sikertelen:' . $agent_error_code);
} else {
    $output .= $agent_body;
    file_put_contents($pdf_file, $agent_body);
    if (file_exists($pdf_file)) {
        header($_SERVER["SERVER_PROTOCOL"] . " 200 OK");
        header("Cache-Control: public"); // needed for internet explorer
        header("Content-Type: application/pdf");
        header("Content-Transfer-Encoding: Binary");
        header("Content-Length:" . filesize($pdf_file));
        header("Content-Disposition: attachment; filename=".$pdf_file);
        readfile($pdf_file);
        die();
    }
    $output .= '<br>Újra letölt: <a href="szamla.pdf" target="_blank">Számla pdf</a>';
}
?>