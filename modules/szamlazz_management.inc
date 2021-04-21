<?php


if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) < 1) {
    $view->warning('A számlázó modul nincs engedélyezve !');
    theEnd($output);
    exit(0);
}

if (!isset($list_from)) {
    $list_from = date('Y-m-d', strtotime($today . ' -1 month'));
}
if (!isset($list_to)) {
    $list_to = $today;
}
if (isset($send_email)) {
    $sz = new Szamlazz();
    $sz->getElementByBillNumber($send_email);
    $email_addr = $sz->getEmailAddr();
    if (!empty($email_addr)) {
        $filename = $sz->getPdfFileName($send_email);
        $email_body = 'Tisztelt ' . $sz->getName() . ' !<br><br>';
        $email_body .= 'Mellékletben ismételten küldjük a kiállított számlát.';
        send_mail_to($email_addr, 'Számla küldés (' . $send_email . ')', $email_body, $filename);
        $view->success("Email kiküldve !");
    }
}
$view->newKeret();

$sql = 'SELECT * FROM `' . SZAMLAZZ_TABLE . '` '
        . ' WHERE (`' . SZAMLAZZ_PREPARED . '` BETWEEN "' . $list_from . ' 00:00:00" AND "' . $list_to . ' 23:59:59")'
        . ' ORDER BY `' . SZAMLAZZ_PREPARED . '` DESC ';
$res = $mysqliLink->query($sql);
$view->newDiv('row align-items-end');
$view->inputDate('list_from', 'Időszak');
$view->inputDate('list_to');
$view->submitButton('submit', 'Keresés');
$view->endDiv();

if ($res->num_rows > 0) {
    $view->newTable('Elkészült számlák listázása');
    $view->putThead('', ['Azonosító', 'Vevő', 'Összeg', 'Előkészítve', 'Kiállítva', 'Művelet']);

    $sz = new Szamlazz();
    while ($row = $res->fetch_assoc()) {
        $view->newTableTr();
        $bn = $row[SZAMLAZZ_BILL_NUMBER];
        if ($sz->isPdfExists($bn)) {
            $view->putElement('<td><a target="_blank" title="Letöltés" href="' . $sz->getPdfURL($bn) . '">' . $bn . '</a></td>');
        } else {
            $view->putElement('<td>' . $bn . '</td>');
        }
        $view->putElement('<td>' . $row[SZAMLAZZ_NAME] . '</td>');
        $view->putElement('<td>' . getSumOfBills($row[SZAMLAZZ_BILL_NUMBER]) . ' Ft</td>');
        $view->putElement('<td>' . $row[SZAMLAZZ_PREPARED] . '</td>');
        $view->putElement('<td>' . ($row[SZAMLAZZ_DONE] > 0 ? $row[SZAMLAZZ_DONE] : "Nincs kiállítva!") . '</td>');
        if (!empty($row[SZAMLAZZ_EMAIL]) AND $sz->isPdfExists($bn)) {
            $view->newElement('<td>');
            $view->submitEmail('send_email', $bn, 'Számla újraküldése a ' . $row[SZAMLAZZ_EMAIL] . ' címre.', 'Biztosan el akarod a számlát újra küldeni emailben ?');
            $view->endElement('</td>');
        } else {
            $view->putElement('<td></td>');
        } $view->endTableTr();
    }
    $view->endTable();
} else {
    $view->warning('Nem készült számla ebben az időszakban !');
}