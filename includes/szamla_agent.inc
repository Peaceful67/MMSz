<?php

define('AGENT_URL', 'https://www.szamlazz.hu/szamla/');
$cookie_file =  TMP_DIR . 'szamlazz_cookie_'.$szamla[SZAMLAZZ_BILL_NUMBER].'.txt';
$xml_file = TMP_DIR . 'szamlazz_xml_'.$szamla[SZAMLAZZ_BILL_NUMBER].'.xml';

$xml = '<?xml version="1.0" encoding="UTF-8"?>
<xmlszamla xmlns="http://www.szamlazz.hu/xmlszamla" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.szamlazz.hu/xmlszamla http://www.szamlazz.hu/docs/xsds/agent/xmlszamla.xsd">
  <beallitasok>
    <felhasznalo>peaceful.app@gmail.com</felhasznalo>
    <jelszo>Baksa-67</jelszo>
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
    <bankszamlaszam>' . BANK_ACCOUNT . '</bankszamlaszam>
    <emailReplyto>' . getOptionValue(OPTIONS_NAME_MMSZ_EMAIL) . '</emailReplyto>
    <emailTargy>Számla értesítő</emailTargy>
    <emailSzoveg>Fizetését lekönyveltük, a számlát a mellékletben találja.</emailSzoveg>
    <alairoNeve>' . COMPANY_LONG_NAME . '</alairoNeve>
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