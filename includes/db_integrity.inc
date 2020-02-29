<?php

// members
$sql = 'SELECT * FROM `' . MEMBER_TABLE . '` ';
$res = $mysqliLink->query($sql);
$mem_error = false;
$num = 0;
$ins_role = 0;
while ($res AND $row = $res->fetch_assoc()) {
    $role_res = $mysqliLink->query('SELECT * FROM `'.ROLE_TABLE.'` WHERE `'.ROLE_MEMBER.'`='.$row[MEMBER_ID]);
    if($role_res AND $role_res->num_rows==0) {
        $mysqliLink->query('INSERT INTO `'.ROLE_TABLE.'` '
                . '(`permission`, `member_id`, `date_from`, `date_to`, `club_id`) '
                . 'VALUES (0, '.$row[MEMBER_ID].',"'.$today.'","2099-12-31",0)');
        $ins_role++;
    }
    $num++;
}
if ($mem_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba members táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `members` táblában");
} else {
    
    $output .= '<img src="' . IMAGES . 'ok.jpg">members tábla ' . $num . ' eleme rendben '
            . $ins_role.' elemet létrehoztunk a ROLE táblában.<br>';
}
$error |= $mem_error;


// categories
$sql = 'SELECT * FROM `' . CATEGORIES_TABLE . '` ';
$res = $mysqliLink->query($sql);
$cat_error = false;
$num = 0;
while ($res AND $row = $res->fetch_assoc()) {
    $num++;
    if (!isset($branches[$row[CATEGORIES_BRANCH]])) {
        $cat_error = true;
    }
}
if ($cat_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba categories táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `categories` táblában");
} else {
    $output .= '<img src="' . IMAGES . 'ok.jpg">categories tábla ' . $num . ' eleme rendben<br>';
}
$error |= $cat_error;

// fees
$sql = 'SELECT * FROM `' . FEE_TABLE . '` ';
$res = $mysqliLink->query($sql);
$fee_error = false;
$num = 0;
while ($res AND $row = $res->fetch_assoc()) {
    $num++;
    if (!isset($fee_type[$row[FEE_TYPE]])) {
        $output .= 'Nem létező FEE_TYPE: ' . $row[FEE_ID] . '<br>';
        $fee_error = true;
    } else {
        switch ($row[FEE_TYPE]) {
            case FEE_TYPE_MEMBERSHIP:
                if (!isset($member_types[$row[FEE_NAME]])) {
                    $output .= 'Nem létező member_types: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;
            case FEE_TYPE_INSURANCE:
                if (!isset($member_insurances[$row[FEE_NAME]])) {
                    $output .= 'Nem létező member_insurances: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;

            case FEE_TYPE_ORGANIZATION:
                if (!isset($organizations[$row[FEE_NAME]])) {
                    $output .= 'Nem létező organization: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;

            case FEE_TYPE_CLUB:
                if (!isset($club_categories[$row[FEE_NAME]])) {
                    $output .= 'Nem létező club_categories: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                $club = getClubById($row[FEE_CLUB]);
                if (empty($club)) {
                    $output .= 'Nem létező club_id: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;

            case FEE_TYPE_ADMINISTRATION:
                if (!isset($administrations[$row[FEE_NAME]])) {
                    $output .= 'Nem létező administration: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;
            case FEE_TYPE_ASSOCIATION:
                if (!isset($associations[$row[FEE_NAME]])) {
                    $output .= 'Nem létező association: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;
            case FEE_TYPE_LICENCES:
                if (!isset($licences[$row[FEE_NAME]])) {
                    $output .= 'Nem létező licence: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;
            case FEE_TYPE_OCCASION:
                if (!isset($occasions[$row[FEE_NAME]])) {
                    $output .= 'Nem létező occasion: ' . $row[FEE_ID] . '<br>';
                    $fee_error = true;
                }
                break;

            default:
                $output .= 'Nem kezelt FEE_TYPE: ' . $row[FEE_ID] . '<br>';
                break;
        }
    }
}

if ($fee_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba `fees` táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `fees` táblában");
} else {
    $output .= '<img src="' . IMAGES . 'ok.jpg">'
            . 'fees tábla ' . $num . ' eleme rendben<br>';
}
$error |= $fee_error;

// Category_member

$sql = 'SELECT * FROM `' . CM_TABLE . '`';
$res = $mysqliLink->query($sql);
$cm_error = false;
$num = 0;
while ($res AND $row = $res->fetch_assoc()) {
    $num++;
    if (!isValidMemberId($row[CM_MEMBER_ID])) {
        $cm_error = true;
    }
    $cat = getCategoryById($row[CM_CATEGORY]);
    if (empty($cat)) {
        $cm_error = true;
    }
}
if ($cm_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba a category_member táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `category_member` táblában");
} else {
    $output .= '<img src="' . IMAGES . 'ok.jpg">'
            . 'category_member tábla ' . $num . ' eleme rendben<br>';
}
$error |= $cm_error;

// Billing
$sql = 'SELECT * FROM `' . BILLING_TABLE . '` ';
$res = $mysqliLink->query($sql);
$bill_error = false;
$num = 0;
while ($res and $row = $res->fetch_assoc()) {
    $num++;
    $fm = getFmById($row[BILLING_FM_ID]);
    if ($fm) {
        $fee = getFeeById($fm[FM_FEE_ID]);
        if($fee[FEE_SUM] != $row[BILLING_PRICE]) {
            $output .= 'Fee összeg nem egyezik a BILL összeggel: ' . $row[BILLING_ID] . ', Fee:'.$fee[FEE_SUM].' Ft, Bill: '.$row[BILLING_PRICE].' Ft<br>';
            $bill_error = true;            
        }
        if ($fm[FM_BILL_ID] != $row[BILLING_ID]) {
            $output .= 'FM Bill_ID nem ide mutat:' . $row[BILLING_ID] . '<br>';
            $bill_error = true;
        }
        if ($row[BILLING_PAYMENT_ID] != $fm[FM_PAY_ID]) {
            $output .= 'FM Payment ID != BILL Payment ID: ' . $row[BILLING_ID] . '<br>';
            $bill_error = true;
        }
        if (!isValidMemberId($row[BILLING_CREATOR])) {
            $output .= 'Nem létező creator: ' . $row[BILLING_ID] . '<br>';
            $bill_error = true;
        }
    } else {
        $bill_error = true;
        $output .= 'Nem létező FM: ' . $row[BILLING_ID] . '<br>';
    }
    $pay_id = $row[BILLING_PAYMENT_ID];
    if ($pay_id > 0) {
        $pay = getPaymentById($pay_id);
        if (empty($pay)) {
            $output .= 'Nem létező payment: ' . $row[BILLING_ID] . '<br>';
            $bill_error = true;
        } else {
            if ($row[BILLING_NUMBER] != $pay[PAYMENT_BILL_NUMBER]) {
                $output .= 'BILL_NUMBER != PAYMENT_BILL_NUMBER: ' . $row[BILLING_ID] . '<br>';
                $bill_error = true;
            }
        }
    }
}
if ($bill_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba billing táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `billing` táblában");
} else {
    $output .= '<img src="' . IMAGES . 'ok.jpg">billing tábla ' . $num . ' eleme rendben<br>';
}
$error |= $cat_error;

// fee_members
$sql = 'SELECT * FROM `' . FM_TABLE . '` ';
$res = $mysqliLink->query($sql);
$fm_error = false;
$num = 0;
while ($res AND $row = $res->fetch_assoc()) {
    $num++;
    $fee = getFeeById($row[FM_FEE_ID]);
    if (!$fee) {
        $output .= 'Nem létező FEE: ' . $row[FM_ID] . '<br>';
        $fm_error = true;
    } else {
        if ($row[FM_FROM] != $fee[FEE_FROM] OR $row[FM_TO] != $fee[FEE_TO]) {
            $output .= 'Hibás from-to időszak: ' . $row[FM_ID] . '<br>';
            $fm_error = true;
        }
        if ($row[FM_BILL_ID] > 0) {
            $bill = getBillById($row[FM_BILL_ID]);
            if ($fee[FEE_SUM] != $bill[BILLING_PRICE]) {
                $output .= 'Megváltozott összeg: ' . $row[FM_ID] . '<br>';
                $fm_error = true;
            }
            if ($bill[BILLING_FM_ID] != $row[FM_ID]) {
                $output .= 'Hibás billing hivatkozás: ' . $row[FM_ID] . '<br>';
                $fm_error = true;
            }
        }
    }
}
if ($fm_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba fee_members táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `fee_members` táblában");
} else {
    $output .= '<img src="' . IMAGES . 'ok.jpg">fee_members tábla ' . $num . ' eleme rendben<br>';
}
$error |= $fm_error;


// roles
$sql = 'SELECT * FROM `' . ROLE_TABLE . '` ';
$res = $mysqliLink->query($sql);
$role_error = false;
$num = 0;
while ($res AND $row = $res->fetch_assoc()) {
    $num++;
    if (!isValidMemberId($row[ROLE_MEMBER])) {
        $output .= 'Nem létező tag : ' . $row[ROLE_ID] . '<br>';
        $role_error = true;
    }
}
if ($role_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba role_member táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `role_member` táblában");
} else {
    $output .= '<img src="' . IMAGES . 'ok.jpg">role_member tábla ' . $num . ' eleme rendben<br>';
}
$error |= $role_error;

// payments
$sql = 'SELECT * FROM `' . PAYMENT_TABLE . '` ';
$res = $mysqliLink->query($sql);
$pay_error = false;
$num = 0;
while ($res AND $row = $res->fetch_assoc()) {
    if (!isValidMemberId($row[PAYMENT_CASHIER]) AND $row[PAYMENT_BANK_REFNO]==0) {
        $output .= 'Nem létező pénztáros: ' . $row[PAYMENT_ID] . '<br>';
        $pay_error = true;
    }
    foreach (getBillsByNumber($row[PAYMENT_BILL_NUMBER]) as $bill) {
        if ($bill[BILLING_PAYMENT_ID] != $row[PAYMENT_ID]) {
            $output .= 'Nem egyező billpayment_id: ' . $row[PAYMENT_ID] . '<br>';
            $pay_error = true;
        }
    }
    $num++;
}
if ($pay_error) {
    $output .= '<img src="' . IMAGES . 'delete.jpg">Hiba payments táblában<br>';
    logger($member_id, -1, LOGGER_DATABASE, "Hiba `payments` táblában");
} else {
    $output .= '<img src="' . IMAGES . 'ok.jpg">payments tábla ' . $num . ' eleme rendben<br>';
}
$error |= $pay_error;
