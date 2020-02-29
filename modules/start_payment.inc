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
    if ($generate_bill == PAY_METHOD_TRANSFER) {
        if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
            $szamla = new Szamlazz();
            $szamla->insertSzamla($number, $email, $invoice_name, $invoice_addr_post, $invoice_addr_city, $invoice_addr_street, $invoice_tax);
        }
        $fms2bill = unserialize($_SESSION[SESSION_FEE_TO_BILL]);
        $fee2bill_sum = 0;
        $email_msg = "Tisztelt " . getMemberName($member_id) . "!<br><br>Utalásos fizetést indítottál. ";
        $email_msg .= '<br>Az összeget a ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME).' '
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
        $output .= '<div class="editor">';
        $output .= '<div class="keret">';
        $output .= 'Az alábbi adatokkal indítsd el az utalást:<br><b>Címzett: ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . '<br>'
                . 'Bankszámla szám: ' . getOptionValue(OPTIONS_NAME_COMPANY_BANK_ACCOUNT) . '<br>';
        $output .= 'Közlemény: ' . $number . '<br>Összeg: ' . $fee2bill_sum . ' Ft</b><br>';

        $output .= 'A fenti adatokat emailben is elküldte a rendszer.<br>'
                . 'Amikor az utalást beérkezik és azt az ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . ' lekönyveli, emailben értesítést fogsz kapni a '
                . getMemberEmail($member_id) . ' email címre.';
        $output .= '<br>Amennyiben mégsem szeretnéd a fenti tételt utalással kiegyenlíteni, törölheted az "Adminisztráció" fül "Számla storno" gombja alatt.';
        $output .= '</div></div>';
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
        $email_msg .= '<br>Az összegnek már korábban az ' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME).' '
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

        $output .= '<div class="keret">';
        $output .= 'A ' . $number . ' azonosítójú, ' . $fee2bill_sum . ' Ft összegű befizetés le van könyvelve!<br>';
        $output .= '</div>';
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


    $output .= '<form  method="post" enctype="multipart/form-data">';
    $output .= '<div class="editor">';
    $submit_enabled = true;
    if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
        include INCLUDES . 'get_invoice_customer.inc';
        $submit_enabled = !(empty($invoice_name) OR empty($invoice_addr_post) OR empty($invoice_addr_city) OR empty($invoice_addr_street));
    }
    if ($submit_enabled) {
        $output .= 'Az alábbi módokon indíthatod el a <b>' . $fee2bill_sum . ' Ft </b>összegű fizetést:<br>';
    } else {
        $output .= 'Töltsd ki a számla vevőadatokat a <b>' . $fee2bill_sum . ' Ft </b>összegű fizetés indítása előtt !<br>';
    }
    if (isset($edit_member)) {
        $output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
    }
    $output .= '<input ' . ($submit_enabled ? "" : "disabled") . ' type="submit" name="generate_bill" value="' . PAY_METHOD_TRANSFER . '" title="Átutalás esetén a banki értesítőre írt azonosító alapján egy adminisztrációs lépés teszi rendezetté a díjtételt/díjtételeket néhány nap múlva."'
            . 'onClick="return confirm(\'Biztosan indítani akarod a ' . $fee2bill_sum . ' Ft összegű ' . PAY_METHOD_TRANSFER . ' ?\')">&nbsp;&nbsp;&nbsp;';
    $output .= '<input type="hidden" name="member_id" value="' . $member_id . '">';
    $output .= '<input  ' . ($submit_enabled ? "" : "disabled") . ' type="submit" name="generate_bill" value="' . PAY_METHOD_CARD . '" title="Bankkártyás fizetés esetén az kifizetett összeg azonnal rendezetté teszi a tag/tagok díjtételeit."'
            . 'onClick="return confirm(\'Át leszel irányítva az OTP felületére a  ' . $fee2bill_sum . ' Ft rendezéséhez\n'
            . 'Adattovábbítási nyilatkozat: \n'
            . 'Elfogadom, hogy a Magyar Modellező Szövetség (1149 Budapest, Buzogány u. 7.) által a tag.modellsport.hu felhasználói adatbázisában tárolt alábbi személyes adataim átadásra kerüljenek az OTP Mobil Kft. (1093 Budapest, Közraktár u. 30-32.), mint adatkezelő részére. A továbbított adatok köre: vezetéknév, keresztnév, ország, telefonszám, e-mail cím.\')">&nbsp;&nbsp;&nbsp;';

    if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ)) {
        $output .= '<input  ' . ($submit_enabled ? "" : "disabled") . ' type="submit" name="generate_bill" value="' . PAY_METHOD_DIRECTLY . '" title="Korábban berkezett utalás lekönyvelése" '
                . 'onClick="return confirm(\'Biztosan indítani akarod a  ' . $fee2bill_sum . ' Ft összegű ' . PAY_METHOD_DIRECTLY . ' ?\')">&nbsp;&nbsp;&nbsp;';
    }
    $output .= '<input type="submit" name="gb_cancel" title="A fizetés megszakítása" value="Vissza">';
    $output .= '<br><a href="http://simplepartner.hu/PaymentService/Fizetesi_tajekoztato.pdf" '
            . 'target="_blank"> <img width="197" height="197" src="' . IMAGES . 'simple_logo.png" title=" SimplePay - Online bankkártyás fizetés" alt=" SimplePay vásárlói tájékoztató"> </a>';
    $output .= '</form></div>';
}