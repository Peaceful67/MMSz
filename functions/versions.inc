<?php

define("VERSION_TITLE", 0);
define("VERSION_DESCRIPTION", 1);


$update_versions = [
    '2.5' => [
        VERSION_TITLE => 'A Modellező Szövetségenk korábban létrehozott alapnak tekintett állapot.',
        VERSION_DESCRIPTION => 'A használati leírás szerinti funkciók az Online Gyűlés funkciókkal bezárólag.',
    ],
    '2.6' => [
        VERSION_TITLE => 'Javítások a Vitorlázó szövetség kérésére (2020.11.05)',
        VERSION_DESCRIPTION => '<ul>'
        . '<li>Egy alszövetség tagegyesületeinek szerkeszthetősége</li>'
        . '<li>Egyesület mandátumszámának megadási lehetősége. Ha ez nem 0,  a küldött szavazata a mandátumszámmal szorzódik és így is jelenik meg.</li>'
        . '<li>Javítás: a taglista keresőn beállított egyesületi szűrő esetén a tag szerepeinek szerkesztésénél rögtön a kiválasztott egyesület jelenik meg.</li>'
        . '<li>A szerepkör szerkesztésnél az érintett egyesületek listázáva.</li>'
        . '<li>Javítás: a szerepkör szereksztésnél a kiválasztott egyesület nem változik újratöltésnél.</li>'
        . '<li>Vezető által létrehozott új tag esetén automatikus egyesületi tagság beállítása.</li>'
        . '<li>A szerepkör szerkesztésnél az egyesületi vezető csak tagság időintervallumát módosíthatja, nem törlheti.</li>'
        . '</ul>',
    ],
    '2.7' => [
        VERSION_TITLE => 'Dokumentum kezelő képzést támogató modul alapok',
        VERSION_DESCRIPTION => '<ul>'
        . '<li>Új fajta radio és checkbox bevezetése</li>'
        . '<li>A lapokon különböző funciók közötti váltás gombokkal</li>'
        . '<li>Képzést támogató törzsadatok szerkesztése</li>'
        . '<li>Képzéseim szerkesztés.</li>'
        . '<li>Képzettségeim szerkesztése</li>'
        . '</ul>',
    ],
    '3.0' => [
        VERSION_TITLE => 'Bootstrap alapú megjelenés',
        VERSION_DESCRIPTION => '<ul>'
        . '<li>Bootstrap beépítése</li>'
        . '<li>Helper osztály létrehozása</li>'
        . '<li>Modulok és includok átírása</li>'
        . '</ul>',
    ],
];

