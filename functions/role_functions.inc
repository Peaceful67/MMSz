<?php

function getMemberClubNames($id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT `' . CLUB_NAME . '` FROM `' . ROLE_TABLE . '`, `' . CLUB_TABLE . '` '
            . 'WHERE `' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`="' . $id . '" '
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`=`' . CLUB_TABLE . '`.`' . CLUB_ID . '`'
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`="' . ACCESS_USER . '"'
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)'
            . 'GROUP BY `' . ROLE_CLUB . '`  ORDER BY `' . ROLE_FROM . '` DESC ;';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret .= $row[CLUB_NAME] . ', ';
    }
    return empty($ret) ? 'Egyik sem' : $ret;
}

function getMemberClubId($id) {
    global $mysqliLink;
    $ret = -1;
    $sql = 'SELECT `' . ROLE_CLUB . '` FROM `' . ROLE_TABLE . '` '
            . 'WHERE `' . ROLE_MEMBER . '`="' . $id . '" '
            . ' AND `' . ROLE_CLUB . '`>0 '
            . ' AND `' . ROLE_PERMISSION . '`!="0"'
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)'
            . 'GROUP BY `' . ROLE_CLUB . '`;';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows() > 0) {
        $ret = $res->fetch_assoc()[ROLE_CLUB];
    }
    return $ret;
}

function getDelegateClubNames($id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT `' . CLUB_NAME . '` FROM `' . ROLE_TABLE . '`, `' . CLUB_TABLE . '` '
            . 'WHERE `' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`="' . $id . '" '
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`=`' . CLUB_TABLE . '`.`' . CLUB_ID . '`'
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '"'
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)'
            . 'GROUP BY `' . ROLE_CLUB . '`  ORDER BY `' . ROLE_FROM . '` DESC ;';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret .= $row[CLUB_NAME] . ', ';
    }
    return $ret;
}

function getDelegateClub($id) { //  Ha több egyesületben is delegalt, az elsőt veszi
    global $mysqliLink;
    $ret = -1;
    $sql = 'SELECT `' . ROLE_CLUB . '` FROM `' . ROLE_TABLE . '` '
            . 'WHERE `' . ROLE_MEMBER . '`="' . $id . '" '
            . ' AND `' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_DELEGATE
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $ret = $row[ROLE_CLUB];
        }
    }
    return $ret;
}

function getDelegatesOfClub($club_id) { // Tomb a klub delegaltjairol
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT `' . ROLE_MEMBER . '` FROM `' . ROLE_TABLE . '` '
            . 'WHERE `' . ROLE_CLUB . '`="' . $club_id . '" '
            . ' AND `' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_DELEGATE
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $ret[] = $row[ROLE_MEMBER];
        }
    }
    return $ret;
}

function getClubsId($id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . ROLE_TABLE . '`, `' . CLUB_TABLE . '` '
            . 'WHERE `' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`="' . $id . '" '
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`="' . ACCESS_USER . '"'
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`=`' . CLUB_TABLE . '`.`' . CLUB_ID . '`'
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[$row[CLUB_ID]] = $row[CLUB_NAME];
    }
    if (empty($ret)) {
        $ret = array(-1 => 'Egyik sem');
    }
    return $ret;
}

function getMaxNumberOfDelegateClub($cl_id) {
    $mem = getNumMemberOfClub($cl_id);
    return floor($mem / 10);
}

function getNumberOfDelegateClub($club_id) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE '
            . ' `' . ROLE_CLUB . '`=' . $club_id . ' '
            . ' AND `' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_DELEGATE . ' '
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    if($res) {
        return $res->num_rows;
    } else {
        return 0;
    }

}

function getClubsEver($id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . ROLE_TABLE . '`, `' . CLUB_TABLE . '` '
            . 'WHERE `' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`="' . $id . '" '
//            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`="' . ACCESS_USER . '"'
            . ' AND `' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`=`' . CLUB_TABLE . '`.`' . CLUB_ID . '`'
            . 'GROUP BY `' . ROLE_CLUB . '` ORDER BY `' . ROLE_FROM . '` DESC ;';

    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[$row[ROLE_ID]] = array($row[CLUB_ID], $row[CLUB_NAME], $row[ROLE_FROM], $row[ROLE_TO]);
    }
    if (empty($ret)) {
        $ret = array(-1 => 'Egyik sem');
    }
    return $ret;
}

function getRoleById($role_id) {
    global $mysqliLink;
    $sql = 'SELECT * FROM `' . ROLE_TABLE
            . '` WHERE `' . ROLE_ID . '`="' . $role_id . '" LIMIT 1;';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows() > 0) {
        return $res->fetch_assoc();
    } else {
        return array();
    }
}

function getRolesOfMember($mem_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . ROLE_TABLE
            . '` WHERE `' . ROLE_MEMBER . '`="' . $mem_id . '" ';
    $res = $mysqliLink->query($sql);
    while($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row;
    }
    return $ret;
}

function getRoles($id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . ROLE_TABLE
            . '` WHERE `' . ROLE_MEMBER . '`="' . $id . '" '
            . 'GROUP BY `' . ROLE_PERMISSION . '` ORDER BY `' . ROLE_FROM . '` DESC';
    $res = $mysqliLink->query($sql);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $ret[] = array($row[ROLE_ID], $row[ROLE_CLUB], $row[ROLE_PERMISSION], $row[ROLE_FROM], $row[ROLE_TO]);
        }
    } else {
        $ret[-1] = array(0, 0, 0, 0, 0);
    }
    return $ret;
}

function getClubRoles($mem_id, $club_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `' . ROLE_TABLE
            . '` WHERE `' . ROLE_MEMBER . '`="' . $mem_id . '" '
            . ' AND `' . ROLE_CLUB . '`="' . $club_id . '" '
            . 'GROUP BY `' . ROLE_PERMISSION . '` ORDER BY `' . ROLE_FROM . '` DESC';
    $res = $mysqliLink->query($sql);
    if ($res) {
        while ($row = $res->fetch_assoc()) {
            $ret[] = array($row[ROLE_ID], $row[ROLE_CLUB], $row[ROLE_PERMISSION], $row[ROLE_FROM], $row[ROLE_TO]);
        }
    } else {
        $ret[-1] = array(0, 0, 0, 0, 0);
    }
    return $ret;
}

function getRoleClubleader($id) {
    global $mysqliLink;
    $ret = -1;
    $sql = 'SELECT * FROM `' . ROLE_TABLE
            . '` WHERE `' . ROLE_MEMBER . '`="' . $id . '" '
            . ' AND `' . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '"'
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`) '
            . ' LIMIT 1';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = $res->fetch_assoc()[ROLE_CLUB];
    }
    return $ret;
}

function getNumMemberOfClub($club_id) {
    global $mysqliLink;
    $ret = 0;
    $sql = 'SELECT COUNT(*) AS member_count FROM `' . ROLE_TABLE . '` WHERE '
            . ' `' . ROLE_PERMISSION . '`="' . ACCESS_USER . '"'
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)'
            . ' AND `' . ROLE_CLUB . '`="' . $club_id . '"';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = $res->fetch_assoc()["member_count"];
    }
    return $ret;
}

function isClubUsed($club_id) {
    global $mysqliLink;
    $ret = false;
    $sql = 'SELECT COUNT(*) AS member_count FROM `' . ROLE_TABLE . '` WHERE '
            . ' `' . ROLE_CLUB . '`="' . $club_id . '"';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = ($res->fetch_assoc()["member_count"] > 0) ? true : false;
    }
    return $ret;
}

function isControllable($acc) {
    global $permission;
    global $controll_access;
    foreach ($controll_access[$acc] as $allowed) {
        if ($permission & $allowed) {
            return true;
        }
    }
    return false;
}

function isMemberActive($mem_id) {
    return (getPermission($mem_id) != ACCESS_PUBLIC) ? true : false;
}

function getPermission($id) {
    global $mysqliLink;
    $retPerm = isPaidMembership($id) ? ACCESS_PAID: ACCESS_PUBLIC;
    $sql = 'SELECT `' . ROLE_PERMISSION . '` FROM `' . ROLE_TABLE . '` '
            . ' WHERE `' . ROLE_MEMBER . '`="' . $id . '" AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '` )';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $retPerm |= $row[ROLE_PERMISSION];
    }
    return $retPerm;
}

function getClubLeaders($club_id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT `' . ROLE_MEMBER . '` FROM `' . ROLE_TABLE . '` WHERE `'
            . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
            . 'AND `' . ROLE_CLUB . '`="' . $club_id . '" '
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret .= getMemberName($row[ROLE_MEMBER]) . ', ';
    }
    if (empty($ret)) {
        $ret = 'Nincs megadva';
    }
    return $ret;
}

function getClubLeaderIds($club_id) {
    global $mysqliLink;
    $ret = array();    
    $sql = 'SELECT `' . ROLE_MEMBER . '` FROM `' . ROLE_TABLE . '` WHERE `'
            . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
            . 'AND `' . ROLE_CLUB . '`="' . $club_id . '" '
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $ret[] = $row[ROLE_MEMBER];
    }
    return $ret;
}

function getClubDelegates($club_id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT `' . ROLE_MEMBER . '` FROM `' . ROLE_TABLE . '` WHERE `'
            . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
            . 'AND `' . ROLE_CLUB . '`="' . $club_id . '" '
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $m = retriveMember($row[ROLE_MEMBER]);
        $ret .= $m[MEMBER_VEZNEV] . ' ' . $m[MEMBER_KERNEV] . ', ';
    }
    if (empty($ret)) {
        $ret = 'Nincs megadva';
    }
    return $ret;
}

function getClubPresidents($club_id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT `' . ROLE_MEMBER . '` FROM `' . ROLE_TABLE . '` WHERE `'
            . ROLE_PERMISSION . '`="' . ACCESS_CLUB_PRESIDENT . '" '
            . 'AND `' . ROLE_CLUB . '`="' . $club_id . '" '
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $m = retriveMember($row[ROLE_MEMBER]);
        $ret .= $m[MEMBER_VEZNEV] . ' ' . $m[MEMBER_KERNEV] . ', ';
    }
    return $ret;
}

function setClubOfNewMember($mem_id, $club_id) {
    global $mysqliLink;
    global $today, $neverEnding, $member_id;
    $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
            . ROLE_MEMBER . '`, `' . ROLE_CLUB
            . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
            . '`) VALUES ("'
            . $mem_id . '", "' . $club_id . '", "'
            . ACCESS_USER . '", "' . $today . '", "' . $neverEnding . '")';
    $mysqliLink->query($sql);
    logger($member_id, $mem_id, LOGGER_MEMBER_MOD, $mem_id . ' azonosítójú új felhasználó automatikus egyesület beállítása. Egyesület:' . $club_id . ', from:' . $today . ', to: ' . $neverEnding);
}

function getNameOfCatLeaders($cat_id) {
    global $mysqliLink;
    $ret = '';
    $sql = 'SELECT `' . ROLE_MEMBER . '` FROM `' . ROLE_TABLE . '` WHERE `'
            . ROLE_PERMISSION . '`=' . ACCESS_CATEGORY_LEADER . ' '
            . 'AND `' . ROLE_CATEGORY_LEADER . '`="' . $cat_id . '" '
            . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
    $res = $mysqliLink->query($sql);    
    while ($res AND $row = $res->fetch_assoc()) {
        $ret .= getMemberName($row[ROLE_MEMBER]) . ', ';
    }
    if(empty($ret)) {
        $ret = '---';
    }
    return $ret;
    
}