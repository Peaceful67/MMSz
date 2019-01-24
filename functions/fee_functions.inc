<?php

if (isset($_POST[FM_CHECKBOX])) {
    foreach ($_POST[FM_CHECKBOX] as $key => $chk_item) {
        $chk_var = FM_CHECKBOX . '[' . $key . ']';
        $$chk_var = 'on';
    }
}

function getFMsOfMember($mem_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
            . ' `' . FM_MEMEBER_ID . '`=' . $mem_id;
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row;
    }
    return $ret;
}

function getFeeOfMember($id, $date, $type) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . FM_TABLE . '`.* FROM `' . FM_TABLE . '`, `' . FEE_TABLE . '` WHERE '
            . '`' . FM_TABLE . '`.`' . FM_MEMEBER_ID . '`="' . $id . '"'
            . ' AND `' . FM_TABLE . '`.`' . FM_FEE_ID . '`=`' . FEE_TABLE . '`.`' . FEE_ID . '` '
            . ' AND `' . FEE_TABLE . '`.`' . FEE_TYPE . '`="' . $type . '" '
            . ' AND ("' . $date . '" BETWEEN `' . FEE_TABLE . '`.`' . FEE_FROM . '` AND `' . FEE_TABLE . '`.`' . FEE_TO . '`) '
            . ' ORDER BY `' . FM_TABLE . '`.`' . FM_ID . '` DESC LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = $res->fetch_assoc();
    }
    return $ret;
}

function printFmOfMember($mem_id) {
    global $today, $fee_type;
    $ret = '';
    foreach ($fee_type as $type => $name) {
        $fm = getFeeOfMember($mem_id, $today, $type);
        if (!empty($fm)) {
            $ret .= $name[0] . ': ';
            if ($fm[FM_BILL_ID] == 0) {
                $ret .= ' beállítva, ';
            } elseif ($fm[FM_PAY_ID] == 0) {
                $ret .= ' fizetésre vár, ';
            } else {
                $ret .= ' rendezve, ';
            }
        }
    }
    return $ret;
}

function isDeletableMember($mem_id) {
    global $mysqliLink;
    $ret = true;
    $mem = retriveMember($mem_id);
    if ($mem[MEMBER_PASSWORD] != "") {
        $ret = false;
    }
    $sql = 'SELECT COUNT(*) AS `current` FROM `' . FM_TABLE . '` WHERE '
            . ' `' . FM_MEMEBER_ID . '`="' . $mem_id . '" ';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->fetch_assoc()["current"] > 0) {
        $ret = false;
    }
    return $ret;
}

function isPaidMembership($mem_id) {
    $ret = false;
    $fms = getPaiedFMs($mem_id);
    foreach ($fms as $fm) {
        if (getFeeById($fm[FM_FEE_ID])[FEE_TYPE] == FEE_TYPE_MEMBERSHIP) {
            $ret = true;
        }
    }
    return $ret;
}

function getPaiedFMs($mem_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
            . '`' . FM_MEMEBER_ID . '`="' . $mem_id . '" '
            . ' AND `' . FM_BILL_ID . '`>0 '
            . ' AND `' . FM_PAY_ID . '`>0 '
            . ' AND (CURDATE() BETWEEN `' . FM_FROM . '` AND `' . FM_TO . '`) ';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row;
    }
    return $ret;
}

function getUnpaidFMOfMember($id) { // FM-ID, FEE_TYPE, FEE_NAME, FEE_FROM, FEE_TO, FEE_SUM
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
            . '`' . FM_MEMEBER_ID . '`="' . $id . '"'
            . ' AND `' . FM_BILL_ID . '`="0"';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $fm_fee_id = $row[FM_FEE_ID];
        $fee = getFeeById($fm_fee_id);
        if ($fee) {
            $ret[] = array($row[FM_ID], $fee[FEE_TYPE], $fee[FEE_NAME], $fee[FEE_FROM], $fee[FEE_TO], $fee[FEE_SUM]);
        }
    }
    return $ret;
}

function getUnpaidFMOfMemberFeeType($id, $ft) { // FM-ID, FEE_TYPE, FEE_NAME, FEE_FROM, FEE_TO, FEE_SUM
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . FM_TABLE . '`.* FROM (`' . FM_TABLE . '` '
            . 'INNER JOIN `' . FEE_TABLE . '`  ON ('
            . ' `' . FM_TABLE . '`.`' . FM_FEE_ID . '`=`' . FEE_TABLE . '`.`' . FEE_ID . '` '
            . ' AND `' . FEE_TABLE . '`.`' . FEE_TYPE . '`=' . $ft
            . ' )) WHERE '
            . '`' . FM_MEMEBER_ID . '`="' . $id . '"'
            . ' AND `' . FM_BILL_ID . '`=0 ';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $fm_fee_id = $row[FM_FEE_ID];
        $fee = getFeeById($fm_fee_id);
        if ($fee) {
            $ret[] = array($row[FM_ID], $fee[FEE_TYPE], $fee[FEE_NAME], $fee[FEE_FROM], $fee[FEE_TO], $fee[FEE_SUM]);
        }
    }
    return $ret;
}

function updateFeeOfMember($id, $fee_id) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
            . '`' . FM_MEMEBER_ID . '`="' . $id . '"'
            . ' AND `' . FM_FEE_ID . '`="' . $fee_id . '"'
            . ' LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($res AND $row = $res->fetch_assoc()) {
        if ($row[FM_BILL_ID] != 0) {
            return false; // Már elindult a fizetési folyamat
        }
    }
    $mysqliLink->query('DELETE FROM `' . FM_TABLE . '` WHERE `'
            . FM_MEMEBER_ID . '`="' . $id . '"'
            . ' AND `' . FM_FEE_ID . '`="' . $fee_id . '"');
    $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`, `' . FM_FEE_ID . '`)'
            . ' VALUES ("' . $id . '", "' . $fee_id . '")';
    $mysqliLink->query($sql);
    return true;
}

function isFeeChangeableInFM($mem_id, $fee_id) { // fee-id típusúvan van olyan fm, ami ebben az időszakban bill==0
    global $mysqliLink;
    $ret = true;
    $fee = getFeeById($fee_id);
    $sql = 'SELECT `' . FM_TABLE . '`.* FROM `' . FM_TABLE . '`, `' . FEE_TABLE . '` WHERE  '
            . ' `' . FEE_TABLE . '`.`' . FEE_TYPE . '`=' . $fee[FEE_TYPE] . ' '
            . ' AND `' . FM_TABLE . '`.`' . FM_MEMEBER_ID . '`=' . $mem_id
            . ' AND `' . FM_TABLE . '`.`' . FM_BILL_ID . '`!=0'
            . ' AND `' . FM_TABLE . '`.`' . FM_FEE_ID . '`=`' . FEE_TABLE . '`.`' . FEE_ID . '` '
            . ' AND `' . FEE_TABLE . '`.`' . FEE_FROM . '`="' . $fee[FEE_FROM] . '"'
            . ' AND `' . FEE_TABLE . '`.`' . FEE_TO . '`="' . $fee[FEE_TO] . '"';
    $res = $mysqliLink->query($sql);
    if ($res and $res->num_rows > 0) {
        $ret = false;
    }
    return $ret;
}

function isClubFeeChangeableInFM($mem_id, $fee_id) { // fee-id típusúvan van olyan fm, ami ebben az időszakban bill==0
    global $mysqliLink;
    $ret = true;
    $fee = getFeeById($fee_id);
    $sql = 'SELECT `' . FM_TABLE . '`.* FROM `' . FM_TABLE . '`, `' . FEE_TABLE . '` WHERE  '
            . ' `' . FEE_TABLE . '`.`' . FEE_TYPE . '`=' . $fee[FEE_TYPE] . ' '
            . ' AND `' . FM_TABLE . '`.`' . FM_MEMEBER_ID . '`=' . $mem_id
            . ' AND `' . FM_TABLE . '`.`' . FM_BILL_ID . '`!=0'
            . ' AND `' . FM_TABLE . '`.`' . FM_FEE_ID . '`=`' . FEE_TABLE . '`.`' . FEE_ID . '` '
            . ' AND `' . FEE_TABLE . '`.`' . FEE_FROM . '`="' . $fee[FEE_FROM] . '"'
            . ' AND `' . FEE_TABLE . '`.`' . FEE_TO . '`="' . $fee[FEE_TO] . '"'
            . ' AND `' . FEE_TABLE . '`.`' . FEE_CLUB . '`="' . $fee[FEE_CLUB] . '"';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        $ret = false;
    }
    return $ret;
}

function deleteFeeNameIfExists($mem_id, $ft, $new_fee_id) {
    global $mysqliLink;
    $new_fee = getFeeById($new_fee_id);
    $old_fee_ids = getFeeIds($ft, $new_fee[FEE_FROM], $new_fee[FEE_TO]);
    foreach ($old_fee_ids as $old_fee_id) {
        $sql = 'DELETE FROM `' . FM_TABLE . '` WHERE `' . FM_MEMEBER_ID . '`="' . $mem_id . '" '
                . ' AND `' . FM_FEE_ID . '`="' . $old_fee_id . '" ';
        $mysqliLink->query($sql);
    }
}

function getFeeIds($type, $from, $to) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . FEE_ID . '` FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . $type . '"'
            . ' AND `' . FEE_FROM . '`="' . $from . '" '
            . 'AND `' . FEE_TO . '`="' . $to . '" ';
    $res = $mysqliLink->query($sql);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $ret[] = $row[FEE_ID];
        }
    }
    return $ret;
}

function getFeeIdsToday($type) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . $type . '" '
            . 'AND (CURDATE() BETWEEN `' . FEE_FROM . '` AND `' . FEE_TO . '`) ';
    $res = $mysqliLink->query($sql);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $ret[$row[FEE_NAME]] = $row[FEE_ID];
        }
    }
    return $ret;
}

function deleteClubFeeIfExists($mem_id, $club, $new_fee_id) {
    global $mysqliLink;
    $new_fee = getFeeById($new_fee_id);
    $old_fee_ids = getClubFeeIds($club, $new_fee[FEE_FROM], $new_fee[FEE_TO]);
    foreach ($old_fee_ids as $old_fee_id) {
        $sql = 'DELETE FROM `' . FM_TABLE . '` WHERE `' . FM_MEMEBER_ID . '`="' . $mem_id . '" '
                . ' AND `' . FM_FEE_ID . '`="' . $old_fee_id . '" ';
        $mysqliLink->query($sql);
    }
}

function getClubFeeIds($club, $from, $to) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . FEE_ID . '` FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . FEE_TYPE_CLUB . '"'
            . ' AND `' . FEE_CLUB . '`="' . $club . '" '
            . ' AND `' . FEE_FROM . '`="' . $from . '" '
            . 'AND `' . FEE_TO . '`="' . $to . '" ';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row[FEE_ID];
    }
    return $ret;
}

function getFmById($fm_id) {
    global $mysqliLink;
    $ret = false;
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
            . '`' . FM_ID . '`="' . $fm_id . '"'
            . ' LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        $ret = $res->fetch_assoc();
    }
    return $ret;
}

function FM_to_String($fm_id, $sep = '<br>') {
    global $fee_type, $club_categories;
    $ret = '';
    if ($fm = getFmById($fm_id)) {
        $fee = getFeeById($fm[FM_FEE_ID]);
        $ret .= 'Tag: ' . getMemberName($fm[FM_MEMEBER_ID]) . '(' . $fm[FM_MEMEBER_ID] . ')' . $sep;
        $ret .= 'Díj típus:' . $fee_type[$fee[FEE_TYPE]][0] . $sep;
        $ret .= 'Díj megnevezés: ';
        switch ($fee[FEE_TYPE]) {
            case FEE_TYPE_CLUB:
                $ret .= $club_categories[$fee[FEE_NAME]] . $sep;
                $ret .= 'Egyesület: ' . getClubName($fee[FEE_CLUB]);
                break;
            default:
                $ret .= printFeeName($fee);
                break;
        }
        $ret .= $sep . 'Összeg:' . $fee[FEE_SUM] . ' Ft';
    }
    return $ret;
}

function getFeeById($fee_id) {
    global $mysqliLink;
    $ret = false;
    $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_ID . '`="' . $fee_id . '"'
            . ' LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        $ret = $res->fetch_assoc();
    }
    return $ret;
}

function printFeeName($fee) {
    global $club_categories, $member_types, $organizations, $member_insurances, $administrations, $associations;
    switch ($fee[FEE_TYPE]) {
        case FEE_TYPE_CLUB:
            return $club_categories[$fee[FEE_NAME]];
        case FEE_TYPE_MEMBERSHIP:
            return $member_types[$fee[FEE_NAME]];
        case FEE_TYPE_INSURANCE:
            return $member_insurances[$fee[FEE_NAME]];
        case FEE_TYPE_ORGANIZATION:
            return $organizations[$fee[FEE_NAME]];
        case FEE_TYPE_ADMINISTRATION:
            return $administrations[$fee[FEE_NAME]];
        case FEE_TYPE_ASSOCIATION:
            return $associations[$fee[FEE_NAME]];
        default:
            return "---";
    }
}

function getFeeName($mem_id, $date, $type) {
    $fee = getFeeOfMember($mem_id, $date, $type);
    if (empty($fee)) {
        $name = -1;
    } else {
        $name = getFeeById($fee[FM_FEE_ID])[FEE_NAME];
    }
    return $name;
}

function getFeeOfMemberById($id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE `' . FM_ID . '`="' . $id . '" LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = $res->fetch_assoc();
    }
    return $ret;
}

function getSumOfFees($fees) {
    $ret = 0;
    foreach ($fees as $fee_id) {
        $fm = getFeeOfMemberById($fee_id);
        $ret += getFeeById($fm[FM_FEE_ID])[FEE_SUM];
    }
    return $ret;
}

function isPayedMembersip($id, $year) {
    $mem_fee = getFeeOfMember($id, $year, FEE_TYPE_MEMBERSHIP);
    return $mem_fee[FM_BILL_ID] == 0 ? false : true;
}

function isPayedOrganization($id, $year) {
    $mem_fee = getFeeOfMember($id, $year, FEE_TYPE_ORGANIZATION);
    return $mem_fee[FM_BILL_ID] == 0 ? false : true;
}

function getPriceOfMemberShip($id, $year) {
    $mem_fee = getFeeOfMember($id, $year, FEE_TYPE_MEMBERSHIP);
    $ret = getFeeById($mem_fee[FM_FEE_ID])[FEE_SUM];
    $mem_fee = getFeeOfMember($id, $year, FEE_TYPE_INSURANCE);
    $ret += getFeeById($mem_fee[FM_FEE_ID])[FEE_SUM];
    return $ret;
}

function getPriceOfOrganization($id, $year) {
    $mem_fee = getFeeOfMember($id, $year, FEE_TYPE_ORGANIZATION);
    $fee = getFeeById($mem_fee[FM_FEE_ID]);
    return $fee[FEE_SUM];
}

function getFirstUndefinedFeeDate($type, $name) {
    global $mysqliLink;
    $ret = VERY_FIRST_DATE;
    $sql = 'SELECT `' . FEE_TO . '` FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . $type . '" AND '
            . '`' . FEE_NAME . '`="' . $name . '" ORDER BY `' . FEE_TO . '` DESC';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        $last = $res->fetch_assoc()[FEE_TO];
        $ret = addPeriodToDate($last, FEE_NEXT_DAY);
    }
    return $ret;
}

function getFirstUndefinedClubFeeDate($club, $category) {
    global $mysqliLink;
    $ret = VERY_FIRST_DATE;
    $sql = 'SELECT `' . FEE_TO . '` FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . FEE_TYPE_CLUB . '" AND '
            . '`' . FEE_NAME . '`="' . $category . '" AND '
            . '`' . FEE_CLUB . '`="' . $club . '" ORDER BY `' . FEE_TO . '` DESC';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        $last = $res->fetch_assoc()[FEE_TO];
        $ret = addPeriodToDate($last, FEE_NEXT_DAY);
    }
    return $ret;
}

function isFeeMember($mem_id, $fee_id) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
            . '`' . FM_MEMEBER_ID . '`="' . $mem_id . '" AND '
            . '`' . FM_FEE_ID . '`="' . $fee_id . '" ';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        return true;
    } else {
        return false;
    }
}

function getFees($type, $name) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . $type . '" AND '
            . '`' . FEE_NAME . '`="' . $name . '" ORDER BY `' . FEE_TO . '` ASC';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = array($row[FEE_ID], $row[FEE_FROM], $row[FEE_TO], $row[FEE_SUM]);
    }
    return $ret;
}

function getFee($type, $name, $date) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`=' . $type . ' AND '
            . '`' . FEE_NAME . '`=' . $name . ' AND '
            . '("' . $date . '" BETWEEN `' . FEE_FROM . '` AND `' . FEE_TO . '`) LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        return $res->fetch_assoc();
    } else {
        return array();
    }
}

function getFeesOfYear($type, $year) {
    global $mysqliLink;
    $beggining = $year . '-01-01';
    $end = $year . '-12-31';
    $ret = array();
    $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . $type . '" AND '
            . '`' . FEE_FROM . '`>="' . $beggining . '" AND '
            . '`' . FEE_TO . '`<="' . $end . '" '
            . 'ORDER BY `' . FEE_CLUB . '` ASC';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row;
    }
    return $ret;
}

function getFeesOfClub($club, $category) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
            . '`' . FEE_TYPE . '`="' . FEE_TYPE_CLUB . '" AND '
            . '`' . FEE_NAME . '`="' . $category . '" AND '
            . '`' . FEE_CLUB . '`="' . $club . '" ORDER BY `' . FEE_TO . '` ASC';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = array($row[FEE_ID], $row[FEE_FROM], $row[FEE_TO], $row[FEE_SUM]);
    }
    return $ret;
}

function addPeriodToDate($last, $period_type) {
    $ret = $last;
    switch ($period_type) {
        case FEE_PERIOD_YEAR:
            $ret = date('Y-m-d', strtotime($last . ' + 1 year - 1 day'));
            break;
        case FEE_PERIOD_HALF:
            $ret = date('Y-m-d', strtotime($last . ' + 6 month - 1 day'));
            break;
        case FEE_PERIOD_QUATER:
            $ret = date('Y-m-d', strtotime($last . ' + 3 month - 1 day'));
            break;
        case FEE_PERIOD_MONTH:
            $ret = date('Y-m-d', strtotime($last . ' + 1 month - 1 day'));
            break;
        case FEE_PERIOD_WEEK:
            $ret = date('Y-m-d', strtotime($last . ' + 7 day - 1 day'));
            break;
        case FEE_PERIOD_DAY:
            $ret = date('Y-m-d', strtotime($last));
            break;
        case FEE_NEXT_DAY:
            $ret = date('Y-m-d', strtotime($last . ' + 1 day'));
            break;
    }
    return $ret;
}

function isFeeDeleteable($fee_id) {
    global $mysqliLink;
    $ret = true;
    $sql = 'SELECT `' . FM_FEE_ID . '` FROM `' . FM_TABLE . '` WHERE `' . FM_FEE_ID . '`="' . $fee_id . '"';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = ($res->num_rows > 0) ? false : true;
    }
    return $ret;
}

function isFeeTypeDeleteable($type, $name) {
    global $mysqliLink;
    $ret = true;
    $sql = 'SELECT * FROM `' . FEE_TABLE . '` '
            . 'WHERE `' . FEE_TYPE . '`="' . $type . '" AND `' . FEE_NAME . '`="' . $name . '";';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = ($res->num_rows > 0) ? false : true;
    }
    return $ret;
}

function isClubMembership($cl_id) {
    global $mysqliLink;
    $ret = 0;
    $sql = 'SELECT * FROM (`' . FM_TABLE . '` INNER JOIN `' . FEE_TABLE . '` ON ('
            . '`' . FM_TABLE . '`.`' . FM_FEE_ID . '`=`' . FEE_TABLE . '`.`' . FEE_ID . '` '
            . ' AND `' . FEE_TABLE . '`.`' . FEE_TYPE . '`=' . FEE_TYPE_ASSOCIATION . ')'
            . ' INNER JOIN `' . ROLE_TABLE . '` ON ('
            . ' `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`!=' . ACCESS_PUBLIC
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`=`' . FM_TABLE . '`.`' . FM_MEMEBER_ID . '` '
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`=' . $cl_id . '))'
            . ' WHERE (CURDATE() BETWEEN `' . FM_TABLE . '`.`' . FM_FROM . '` AND `' . FM_TABLE . '`.`' . FM_TO . '`)'
            . ' AND `' . FM_TABLE . '`.`' . FM_PAY_ID . '`!=0 GROUP BY `' . FM_TABLE . '`.`' . FM_ID . '`';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = $res->num_rows;
    }
    return $ret;
}
