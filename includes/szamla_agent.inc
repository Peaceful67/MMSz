<?php

define('AGENT_URL', 'https://www.szamlazz.hu/szamla/');
define('COOKIE_FILE', TMP_DIR . 'szamlazz_cookie.txt');


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
    <keltDatum>'.$today.'</keltDatum>
    <teljesitesDatum>'.$today.'</teljesitesDatum>
    <fizetesiHataridoDatum>'.$today.'</fizetesiHataridoDatum>
    <fizmod>'.$fiz_mod.'</fizmod>
    <penznem>Ft</penznem>
    <szamlaNyelve>hu</szamlaNyelve>
    <megjegyzes>Számla megjegyzés</megjegyzes>    
    <rendelesSzam></rendelesSzam>
    <elolegszamla>false</elolegszamla>
    <vegszamla>false</vegszamla>
    <helyesbitoszamla>false</helyesbitoszamla>
    <dijbekero>false</dijbekero>
    <szallitolevel>false</szallitolevel>
    <szamlaszamElotag></szamlaszamElotag>
    <fizetve>true</fizetve>
  </fejlec>
  <elado>
    <bank>Bank</bank>
    <bankszamlaszam>'.BANK_ACCOUNT.'</bankszamlaszam>
    <emailReplyto>'.MMSZ_EMAIL.'</emailReplyto>
    <emailTargy>Számla értesítő</emailTargy>
    <emailSzoveg>Fizetését lekönyveltük, a számlát a mellékletben találja.</emailSzoveg>
    <!-- <alairoNeve>Magyar Modellező Szövetség</alairoNeve> -->
  </elado>
  <vevo>
    <nev>'.$customer_name.'</nev>
    <irsz>2030</irsz>
    <telepules>Érd</telepules>
    <cim>Tárnoki út 23.</cim>
    <!-- <email>vevoneve@example.org</email> -->
    <!-- <sendEmail>true</sendEmail> -->
    <!-- <adoalany>2</adoalany> értékkészlet a dokumentációban: https://www.szamlazz.hu/szamla/agent -->
    <adoszam>12345678-1-42</adoszam>
    <postazasiNev>Kovács Bt. postázási név</postazasiNev>
    <postazasiIrsz>2040</postazasiIrsz>
    <postazasiTelepules>Budaörs</postazasiTelepules>
    <postazasiCim>Szivárvány utca 8. VI.em. 82.</postazasiCim>
    <vevoFokonyv>
      <konyvelesDatum>2012-04-10</konyvelesDatum>
      <vevoAzonosito>123456</vevoAzonosito>
      <vevoFokonyviSzam>123456</vevoFokonyviSzam>
      <folyamatosTelj>false</folyamatosTelj>
    </vevoFokonyv>    
    <!-- <alairoNeve>Vevő Aláírója</alairoNeve> -->
    <!-- <azonosito></azonosito> -->
    <!-- <telefonszam>+3630-555-55-55, Fax:+3623-555-555</telefonszam> -->
    <!-- <megjegyzes>A portáról felszólni a 214-es mellékre.</megjegyzes> -->
  </vevo><!-- Ez nem kötelező szekció, ha fuvarlevelet szeretne nyomtatni kérjük vegye 
              fel a kapcsolatot az ügyfélszolgálattal az info@szamlazz.hu email címen.
  <tetelek>
    <tetel>
      <megnevezes>Elado izé</megnevezes>
      <mennyiseg>1.0</mennyiseg>
      <mennyisegiEgyseg>db</mennyisegiEgyseg>
      <nettoEgysegar>10000</nettoEgysegar>
      <afakulcs>27</afakulcs>
      <nettoErtek>10000.0</nettoErtek>
      <afaErtek>2700.0</afaErtek>
      <bruttoErtek>12700.0</bruttoErtek>
      <megjegyzes>tétel megjegyzés 1</megjegyzes>
      <tetelFokonyv>
         <gazdasagiEsem></gazdasagiEsem> <!-- Könyvelő dönti el -->
         <gazdasagiEsemAfa></gazdasagiEsemAfa>
         <arbevetelFokonyviSzam></arbevetelFokonyviSzam>
         <afaFokonyviSzam></afaFokonyviSzam>
         <elszDatumTol></elszDatumTol>
         <elszDatumIg></elszDatumIg>
      </tetelFokonyv>      
    </tetel>
    <tetel>
      <megnevezes>Elado izé 2</megnevezes>
      <mennyiseg>2.0</mennyiseg>
      <mennyisegiEgyseg>db</mennyisegiEgyseg>
      <nettoEgysegar>10000</nettoEgysegar>
      <afakulcs>27</afakulcs>
      <nettoErtek>20000.0</nettoErtek>
      <afaErtek>5400.0</afaErtek>
      <bruttoErtek>25400.0</bruttoErtek>
      <megjegyzes>tétel megjegyzés 2</megjegyzes>
    </tetel>
  </tetelek>
</xmlszamla>';

if (!file_exists(COOKIE_FILE)) {
    file_put_contents(COOKIE_FILE, '');
}

// a CURL inicializálása
$ch = curl_init(AGENT_URL);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_HEADER, true);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

curl_setopt($ch, CURLOPT_POSTFIELDS, array('action-xmlagentxmlfile' => new CURLFile(realpath($xmlfile))));
curl_setopt($ch, CURLOPT_SAFE_UPLOAD, true);

