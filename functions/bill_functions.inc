<?php

function printBillItems($number) {
    $ret = '<div class="fees_members"><table>';
    $bills = billItems($number);
    foreach ($bills as $bill) {
        $ret .= '<tr>';
        $fm_id = $bill[BILLING_FM_ID];
        $fm = getFmById($fm_id);
        $ret .= '<td>' . getMemberName($fm[FM_MEMEBER_ID]) . '</td>';
        $ret .= '<td>' . $bill[BILLING_PRICE] . ' Ft</td>';
        $ret .= '</tr>';
    }

    $ret .= '</table></div>';
    return $ret;
}

function billItems($number) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_NUMBER . '`="' . $number . '" ';
    $res = $mysqliLink->query($sql);
    while ($row = $res->fetch_assoc()) {
        $ret[] = $row;
    }
    return $ret;
}

function generateBillingId() {
    $alphabet = "ABCDEFGHIJKLMNOPQRSTUWXYZ0123456789";
    $len = mb_strlen($alphabet) - 1;
    do {
        $id = '';
        for ($i = 0; $i < 8; $i++) {
            $n = rand(0, $len);
            $id .= $alphabet[$n];
        }
    } while (!isIdUnique($id));
    return $id;
}

function setBillPrinted($number) {
    global $mysqliLink;
    global $member_id;
    $sql = 'UPDATE `' . BILLING_TABLE . '` SET `' . BILLING_OFFLINE . '`="' . date("Y-m-d H:i:s") . '" WHERE '
            . '`' . BILLING_NUMBER . '`="' . $number . '"';
    $mysqliLink->query($sql);
    logger($member_id, '---', LOGGER_PAY, 'A ' . $number . ' azonosítójú, kiegyenlített számla nyomtatása offline ügyintézéshez');
}

function stornoBill($fm_id) {
    global $mysqliLink;
    $sql = 'DELETE FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_FM_ID . '`="' . $fm_id . '" '
            . 'AND `' . BILLING_PAYMENT_ID . '`="0"';
    $mysqliLink->query($sql);
    $sql = 'UPDATE `' . FM_TABLE . '` SET `' . FM_BILL_ID . '`="0" WHERE `' . FM_ID . '`="' . $fm_id . '"';
    $mysqliLink->query($sql);
}

function stornoBillNumber($number) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_NUMBER . '`="' . $number . '" '
            . 'AND `' . BILLING_PAYMENT_ID . '`="0"';
    $res = $mysqliLink->query($sql);
    while ($row = $res->fetch_assoc()) {
        $sql = 'UPDATE `' . FM_TABLE . '` SET `' . FM_BILL_ID . '`="0" WHERE `' . FM_ID . '`="' . $row[BILLING_FM_ID] . '"';
        $mysqliLink->query($sql);
        $sql = 'DELETE FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_ID . '`="' . $row[BILLING_ID] . '" ';
        $mysqliLink->query($sql);
    }
}

function getUnpaiedBillNumbers() {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . BILLING_NUMBER . '` FROM `' . BILLING_TABLE . '` '
            . 'WHERE `' . BILLING_PAYMENT_ID . '`="0" GROUP BY  `' . BILLING_NUMBER . '` ORDER BY `' . BILLING_TIME . '`';
    $res = $mysqliLink->query($sql);
    while ($row = $res->fetch_assoc()) {
        $ret[] = $row[BILLING_NUMBER];
    }
    return $ret;
}

function getTimeOfBills($number) {
    global $mysqliLink;
    $ret = '---';
    $sql = 'SELECT `' . BILLING_TIME . '` FROM `' . BILLING_TABLE . '` '
            . ' WHERE `' . BILLING_NUMBER . '`="' . $number . '" LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($row = $res->fetch_assoc()) {
        $ret = $row[BILLING_TIME];
    }
    return $ret;
}

function getSumOfBills($number) {
    global $mysqliLink;
    $ret = 0;
    $sql = 'SELECT `' . BILLING_PRICE . '` FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_NUMBER . '`="' . $number . '"';
    $res = $mysqliLink->query($sql);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $ret += $row[BILLING_PRICE];
        }
    }
    return $ret;
}

function getBillById($bill_id) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_ID . '`="' . $bill_id . '" LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($ret = $res->fetch_assoc()) {
        return $ret;
    } else {
        return array();
    }
}

function getBillsByNumber($number) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_NUMBER . '`="' . $number . '" ';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row;
    }
    return$ret;
}

function getPaymentById($pay_id) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . PAYMENT_TABLE . '` WHERE `' . PAYMENT_ID . '`="' . $pay_id . '" LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($res AND ( $ret = $res->fetch_assoc())) {
        return $ret;
    } else {
        return array();
    }
}

function createOneBillMember($fm_id, $billing_number, $mem_id) {
    global $mysqliLink;
    $fm = getFeeOfMemberById($fm_id);
    $price = getFeeById($fm[FM_FEE_ID])[FEE_SUM];
    $sql = 'INSERT INTO `' . BILLING_TABLE . '` (`' . BILLING_FM_ID . '`, `'
            . BILLING_PAYMENT_ID . '`, `' . BILLING_CREATOR . '`, `' . BILLING_PRICE . '`, `' . BILLING_NUMBER . '`) VALUES ('
            . '"' . $fm_id . '", "0", "' . $mem_id . '", "' . $price . '", "' . $billing_number . '")';
    $mysqliLink->query($sql);
    $bill_id = $mysqliLink->insert_id;
    $sql = 'UPDATE `' . FM_TABLE . '` SET `' . FM_BILL_ID . '`="' . $bill_id . '" WHERE `' . FM_ID . '`="' . $fm_id . '" LIMIT 1';
    $mysqliLink->query($sql);
}

function createOneBill($fm_id, $billing_number) {
    global $member_id;
    createOneBillMember($fm_id, $billing_number, $member_id);
}

function pay_bills($number, $bank_ref = 0) {
    global $member_id;
    global $mysqliLink;
    global $today;
    global $output;
    if (!isset($member_id)) {
        $member_id = 0;
    }
    $sql = 'INSERT INTO `' . PAYMENT_TABLE . '` (`' . PAYMENT_BILL_NUMBER . '`, `' . PAYMENT_CASHIER . '` , `' . PAYMENT_BANK_REFNO . '`)'
            . ' VALUES ("' . $number . '", "' . $member_id . '", "' . $bank_ref . '")';
    $mysqliLink->query($sql);
    $pay_id = $mysqliLink->insert_id;
    $sql = 'UPDATE `' . BILLING_TABLE . '` SET `' . BILLING_PAYMENT_ID . '`="' . $pay_id . '" WHERE '
            . '`' . BILLING_PAYMENT_ID . '`="0" AND '
            . '`' . BILLING_NUMBER . '`="' . $number . '"';
    $mysqliLink->query($sql);
    $bills = getBillsByNumber($number);
    foreach ($bills as $bill) {
        $fm_id = $bill[BILLING_FM_ID];
        $sql = 'UPDATE `' . FM_TABLE . '` SET `' . FM_PAY_ID . '`="' . $pay_id . '" WHERE `' . FM_ID . '`="' . $fm_id . '" ';
        $mysqliLink->query($sql);
        notificationFAI($fm_id);
    }

    if (!empty($bills)) {
        notificationOfPayment($number);
        if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA) > 0) {
            $sz = new Szamlazz();
            if ($sz->getElementByBillNumber($number)) {
                $pdf = $sz->makeSzamla();
                if (empty($pdf)) {
                    unset($_SESSION[SESSION_SZAMLAZZ_RESULT]);
                } else {
                    $_SESSION[SESSION_SZAMLAZZ_RESULT] = $sz->szamlaToString();
                    logger($member_id, -1, LOGGER_SZAMLAZZ, 'A ' . $number . ' azonosítójú számla kiállítva !');
                }
            } else {
                unset($_SESSION[SESSION_SZAMLAZZ_RESULT]);
                mail_system_error("Kellene szamlazni, de nincs kinek ");
            }
            ob_end_clean();
            ob_start();
            redirect('szamlazz_res');
            exit(0);
        }
    }
}

function isBillPaied($number) {
    global $mysqliLink;
    $ret = false;
    $sql = 'SELECT * FROM `' . BILLING_TABLE . '` WHERE '
            . ' `' . BILLING_NUMBER . '`="' . $number . '" '
            . ' AND `' . BILLING_PAYMENT_ID . '`!=0 ';
    $res = $mysqliLink->query($sql);
    if ($res AND ( $res->num_rows > 0)) {
        $ret = true;
    }
    return $ret;
}

function getCreatorOfBill($number) {
    global $mysqliLink;
    $ret = -1;
    $sql = 'SELECT `' . BILLING_CREATOR . '` FROM `' . BILLING_TABLE . '` '
            . 'WHERE `' . BILLING_NUMBER . '`="' . $number . '" GROUP BY `' . BILLING_CREATOR . '`';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = $res->fetch_assoc()[BILLING_CREATOR];
    }
    return $ret;
}

function getUnpaidBillsCreatedBy($mem_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . BILLING_FM_ID . '` FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_CREATOR . '`="' . $mem_id . '" '
            . 'AND `' . BILLING_PAYMENT_ID . '`="0"';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row[BILLING_FM_ID];
    }
    return $ret;
}

function getPaymentByBillNumber($number) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . PAYMENT_TABLE . '` WHERE `' . PAYMENT_BILL_NUMBER . '`="' . $number . '" LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($ret = $res->fetch_assoc()) {
        return $ret;
    } else {
        return array();
    }
}

function downloadPdfOfBill($number) {
    $payment = getPaymentByBillNumber($number);
    $body = '<img style="vertical-align: top" src="' . COMPANY_LOGO . '">';
    $body .= '<B>' . $number . '</B>';
    $creator = getCreatorOfBill($number);
    $body .= '<br><label>Kiállítva: </label>' . getTimeOfBills($number) . '&nbsp;&nbsp;&nbsp;'
            . getMemberName($creator) . ' (' . $creator . ')<br>';
    $body .= '<label>Beérkezett: </label>' . $payment[PAYMENT_TIME] . ', ';
    $body .= '<label>Összeg: </label>' . getSumOfBills($number) . ' Ft<br>'
            . '<label>Nyomtatva: </label>' . date("Y-m-d H:i:s") . '<h2>Tételek:</h2>';
    foreach (billItems($number) as $bill) {
        $fm = getFmById($bill[BILLING_FM_ID]);
        $fee = getFeeById($fm[FM_FEE_ID]);
        if ($fee[FEE_TYPE] == FEE_TYPE_CLUB) {
            $body .= '<p style="color: red">Tovább fizetni : </p>';
        }
        $body .= FM_to_String($fm[FM_ID], '<br>');
        $body .= '<hr>';
    }
    $body .= '</div>';
    require_once CLASSES . '/vendor/autoload.php';
    $pdf = new \Mpdf\Mpdf(['tempDir' => TMP_DIR], ['mode' => '+aCJK'], ['format' => 'A4'], ['orientation' => 'L']);
    $pdf->WriteHTML(iconv("UTF-8", "UTF-8", $body));
    $pdf->SetFooter('{PAGENO}');
    $pdf->Output($number . '_' . date("Y-m-d_H-i-s") . '.pdf', 'D');
}

function fms2bill($fm_id, $bill_number) {
    global $mysqliLink;
    global $member_id;
    $sql = 'INSERT INTO `' . FMS2BILL_TABLE . '` (`' . FMS2BILL_MEM_ID . '`, `' . FMS2BILL_FM_ID . '`, `' . FMS2BILL_BILL_NUMBER . '`) '
            . ' VALUES ("' . $member_id . '", "' . $fm_id . '", "' . $bill_number . '")';
    $mysqliLink->query($sql);
}

function createBillsFromFMS($bill_number) {
    global $mysqliLink;
    $ret = false;
    $sql = 'SELECT * FROM `' . FMS2BILL_TABLE . '` WHERE `' . FMS2BILL_BILL_NUMBER . '`="' . $bill_number . '"';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        createOneBillMember($row[FMS2BILL_FM_ID], $bill_number, $row[FMS2BILL_MEM_ID]);
        $ret = true;
    }
    $mysqliLink->query('DELETE FROM `' . FMS2BILL_TABLE . '` WHERE `' . FMS2BILL_BILL_NUMBER . '`="' . $bill_number . '"');
    return $ret;
}

function notificationOfPayment($number) {
    $bills = getBillsByNumber($number);
    if (empty($bills))
        return;
    $sum = 0;
    $email_id = $bills[0][BILLING_CREATOR];
    foreach ($bills as $bill) {
        $sum += $bill[BILLING_PRICE];
    }
    $subject = getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' befizetés beérkezett';
    $body = 'Tisztelt ' . getMemberName($email_id) . ' !<br><br>'
            . 'A ' . $number . ' azonosítójú, ' . $sum . ' Ft összegű befizetésed a tagnyilvántartó rendszerben le lett könyvelve.<br>';
    send_notification($email_id, $subject, $body);
}

function getLastCustomer($mem_id) {
    global $mysqliLink;
    $sql = 'SELECT `' . BILLING_NUMBER . '` FROM `' . BILLING_TABLE . '` WHERE `' . BILLING_CREATOR . '`=' . $mem_id
            . ' ORDER BY `' . BILLING_TIME . '` DESC LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if (!$res OR $res->num_rows == 0) {
        return false;
    }
    $bill = $res->fetch_assoc();
    $sql = 'SELECT * `' . SZAMLAZZ_TABLE . '` WHERE `' . SZAMLAZZ_BILL_NUMBER . '`="' . $BILL[BILLING_NUMBER] . '" ;';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        return $res->fetch_assoc();
    } else {
        return false;
    }
}

function getFmSumFromBill($fm) {
    $ret = '---';
    if(!isset($fm) OR empty($fm) OR !isset($fm[FM_BILL_ID])) {
        return $ret;
    }
    $bill = getBillById($fm[FM_BILL_ID]);
    if(empty($bill)) {
        return $ret;
    } else  {
        return $bill[BILLING_PRICE];
    }
}
