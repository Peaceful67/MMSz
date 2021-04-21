<?php

define("PAY_METHOD_TRANSFER", "Banki utalás");
define("PAY_METHOD_CARD", "Kártyás fizetés");
define("PAY_METHOD_DIRECTLY", "Azonnali könyvelés");

if (isset($simplepay_number) AND isBillPaied($simplepay_number)) {
    redirect("simplepay_ipn");
    exit(0);
}

if (isset($generate_bill)) {
    $number = generateBillingId();
    $email = getMemberEmail($member_id);
    if(isset($invoice_email) AND filter_var($invoice_email, FILTER_VALIDATE_EMAIL)) {
        $email .=', '.$invoice_email;
    }
    if ($generate_bill == PAY_METHOD_TRANSFER) {
        if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
            $szamla = new Szamlazz();
            $szamla->insertSzamla($number, $email, $invoice_name, $invoice_addr_post, $invoice_addr_city, $invoice_addr_street, $invoice_tax);
        }
        $fms2bill = unserialize($_SESSION[SESSION_FEE_TO_BILL]);
        $fee2bill_sum = 0;
        $email_msg = "Tisztelt " . getMemberName($member_id) . "!<br><br>Utalásos fizetést indítottál. ";
        $email_msg .= '<br>Az összeget a ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . ' '
                . getOptionValue(OPTIONS_NAME_COMPANY_BANK_ACCOUNT) . ' számú számlájára utald!<br>';
        $email_msg .= 'A közlemény rovatba a ' . $number . ' azonosítót add meg !<br>'
                . 'Amennyiben három napon belül nem jelenik meg a befizetésed az online rendszerben, lépj kapcsolatba az ' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . '-szel ' . getOptionValue(OPTIONS_NAME_MMSZ_EMAIL) . ' címen!<br>'
                . 'A befizetésed lekönyvelésekor értesítést fog a rendszer küldeni a ' . getMemberEmail($member_id) . ' email címre.<br><br>';
        foreach ($fms2bill as $fm2bill) {
            $email_msg .= '<hr>Tétel: <br>' . FM_to_String($fm2bill[0]) . '<br>';
            $fee2bill_sum += $fm2bill[1];
            createOneBill($fm2bill[0], $number);
        }
        $email_msg .= '<br><b>Összesen :' . $fee2bill_sum . ' Ft</b>';
        $msg = "Utalásos fizetés indítása. Azonosító: " . $number . " Összesen :" . $fee2bill_sum . ' Ft<br><br>';
        logger($member_id, -1, LOGGER_BILL, $msg);
        mail_create_bill($email_msg);
        $view->newKeret();
        $view->success('Az alábbi adatokkal indítsd el az utalást:');
        $view->info('<b>Címzett: ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . '<br>'
                . 'Bankszámla szám: ' . getOptionValue(OPTIONS_NAME_COMPANY_BANK_ACCOUNT) . '<br>'
                . 'Közlemény: ' . $number . '<br>Összeg: ' . $fee2bill_sum . ' Ft</b><br>'
                . 'A fenti adatokat emailben is elküldte a rendszer.<br>'
                . 'Amikor az utalást beérkezik és azt az ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . ' lekönyveli, emailben értesítést fogsz kapni a '
                . getMemberEmail($member_id) . ' email címre.');
        $view->warning('Amennyiben mégsem szeretnéd a fenti tételt utalással kiegyenlíteni, törölheted az "Adminisztráció" fül "Számla storno" gombja alatt.');
        $view->endKeret();
    } elseif ($generate_bill == PAY_METHOD_CARD) {
        $fms2bill = unserialize($_SESSION[SESSION_FEE_TO_BILL]);
        $simplepay_number = 'SC_' . $number;
        if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
            $szamla = new Szamlazz();
            $szamla->insertSzamla($simplepay_number, $email, $invoice_name, $invoice_addr_post, $invoice_addr_city, $invoice_addr_street, $invoice_tax);
        }
        $simplepay_sum = 0;
        foreach ($fms2bill as $fm2bill) {
            $simplepay_sum += $fm2bill[1];
            fms2bill($fm2bill[0], $simplepay_number);
        }
        logger($member_id, -1, LOGGER_BILL, "Bankkártyás fizetés indítása. Azonosító: " . $simplepay_number . " Összeg :" . $simplepay_sum);
        include_once INCLUDES . 'card_pay.inc';
    } elseif ($generate_bill == PAY_METHOD_DIRECTLY) {
        $fms2bill = unserialize($_SESSION[SESSION_FEE_TO_BILL]);
        $fee2bill_sum = 0;
        $number = 'DR_' . $number;
        if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
            $szamla = new Szamlazz();
            $szamla->insertSzamla($number, $email, $invoice_name, $invoice_addr_post, $invoice_addr_city, $invoice_addr_street, $invoice_tax);
        }
        $email_msg = "Azonnal lekönyvelt fizetést indítottál. ";
        $email_msg .= '<br>Az összegnek már korábban az ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . ' '
                . getOptionValue(OPTIONS_NAME_COMPANY_BANK_ACCOUNT) . ' számú számlájára kellett érkeznie!<br>';
        $email_msg .= 'Az utalás belső azonosítója : ' . $number . ' !<br><br>';

        foreach ($fms2bill as $fm2bill) {
            $fee2bill_sum += $fm2bill[1];
            createOneBill($fm2bill[0], $number);
        }
        $email_msg .= '<br><b>Összesen :' . $fee2bill_sum . ' Ft</b>';
        mail_create_bill($email_msg);
        $msg = "Azonnal könyvelt fizetés. Azonosító: " . $number . " Összesen :" . $fee2bill_sum . ' Ft<br><br>';
        logger($member_id, -1, LOGGER_BILL, $msg);
        pay_bills($number);
        logger($member_id, -1, LOGGER_PAY, 'A ' . $number . ' azonosítójú számlák befizetésének rögzítése');
        $view->newKeret();
        $view->success('A ' . $number . ' azonosítójú, ' . $fee2bill_sum . ' Ft összegű befizetés le van könyvelve!');
        $view->endKeret();
    }
    unset($_SESSION[SESSION_FEE_TO_BILL]);
    return;
}

if (isset($gb_cancel)) {
    unset($_SESSION[SESSION_FEE_TO_BILL]);
    redirect(get_back());
}
if (isset($_SESSION[SESSION_FEE_TO_BILL])) {
    $fee2bill_sum = 0;
    $fms2bill = unserialize($_SESSION[SESSION_FEE_TO_BILL]);
    foreach ($fms2bill as $fm2bill) {
        $fee2bill_sum += $fm2bill[1];
    }
    $submit_enabled = true;
    if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
        include INCLUDES . 'get_invoice_customer.inc';
        $submit_enabled = !(empty($invoice_name) OR empty($invoice_addr_post) OR empty($invoice_addr_city) OR empty($invoice_addr_street) OR empty($invoice_tax));
    }
    $view->putHidden('member_id');
    if ($submit_enabled) {
        $view->info('Az alábbi módokon indíthatod el a <b>' . $fee2bill_sum . ' Ft </b>összegű fizetést:');
        $view->newDiv('form-inline');
        $view->submitButton('generate_bill', PAY_METHOD_TRANSFER, PAY_METHOD_TRANSFER, 'Átutalás esetén a banki értesítőre írt azonosító alapján egy adminisztrációs lépés teszi rendezetté a díjtételt/díjtételeket néhány nap múlva.', 'Biztosan indítani akarod a ' . $fee2bill_sum . ' Ft összegű ' . PAY_METHOD_TRANSFER . ' ?');
        $view->submitButton('generate_bill', PAY_METHOD_CARD, PAY_METHOD_CARD, 'Bankkártyás fizetés esetén az kifizetett összeg azonnal rendezetté teszi a tag/tagok díjtételeit.', 'Át leszel irányítva az OTP felületére a  ' . $fee2bill_sum . ' Ft kifizteéshez.');
        if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ)) {
            $view->submitButton('generate_bill', PAY_METHOD_DIRECTLY, PAY_METHOD_DIRECTLY, 'Korábban berkezett utalás lekönyvelése', 'Biztosan indítani akarod a  ' . $fee2bill_sum . ' Ft összegű ' . PAY_METHOD_DIRECTLY . ' ?');
      }
        $view->submitBack('gb_cancel', 'A fizetés megszakítása');
        $view->endDiv();
    } else {
        $view->danger('Töltsd ki a számla vevőadatokat a <b>' . $fee2bill_sum . ' Ft </b>összegű fizetés indítása előtt !');
        $view->submitBack('gb_cancel', 'A fizetés megszakítása');
    }
    if (isset($edit_member)) {
        $view->putHidden('edit_member');
    }
     $view->putElement('<a href="http://simplepartner.hu/PaymentService/Fizetesi_tajekoztato.pdf" '
            . 'target="_blank"> <img width="197" height="197" src="' . IMAGES . 'simple_logo.png" title=" SimplePay - Online bankkártyás fizetés" alt=" SimplePay vásárlói tájékoztató"> </a>');
    $view->endKeret();
}