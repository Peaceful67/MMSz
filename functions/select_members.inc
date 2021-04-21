<?php

define("VISIBLE_GROUPS_NAME", 'name');
define("VISIBLE_GROUPS_PERMPISSION", 'perm');
define("VISIBLE_GROUP_NOBODY", 1);
define("VISIBLE_GROUP_EVERYBODY", 2);
define("VISIBLE_GROUP_CLUB_OFFICERS", 3);
define("VISIBLE_GROUP_CLUB_LEADERS", 4);
define("VISIBLE_GROUP_CLUB_DELEGATES", 5);
define("VISIBLE_GROUP_MMSZ_PRESIDENT", 6);
define("VISIBLE_GROUP_CLUB_MATE_MEMBERS", 7);
define("VISIBLE_GROUP_CLUB_MATE_OFFICERS", 8);
define("VISIBLE_GROUP_SUBCLUBS_DELEGATE", 9);

$all_selectable_roles = [
    ACCESS_ADMIN, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT, ACCESS_BRANCH_LEADER, ACCESS_CLUBLEADER,
];


$visible_groups = [
    VISIBLE_GROUP_NOBODY => [
        VISIBLE_GROUPS_NAME => 'Senki',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_ANYBODY,
    ],
    VISIBLE_GROUP_EVERYBODY => [
        VISIBLE_GROUPS_NAME => 'Mindenki',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_MMSZ + ACCESS_ADMIN + ACCESS_MMSZ_PRESIDENT,
    ],
    VISIBLE_GROUP_CLUB_OFFICERS => [
        VISIBLE_GROUPS_NAME => 'Egyesületi tisztek',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_MMSZ + ACCESS_ADMIN + ACCESS_MMSZ_PRESIDENT,
    ],
    VISIBLE_GROUP_CLUB_LEADERS => [
        VISIBLE_GROUPS_NAME => 'Egyesületi vezetők',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_MMSZ + ACCESS_ADMIN + ACCESS_MMSZ_PRESIDENT,
    ],
    VISIBLE_GROUP_CLUB_DELEGATES => [
        VISIBLE_GROUPS_NAME => 'Egyesületi küldöttek',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_MMSZ + ACCESS_ADMIN + ACCESS_MMSZ_PRESIDENT,
    ],
    VISIBLE_GROUP_MMSZ_PRESIDENT => [
        VISIBLE_GROUPS_NAME => 'Elnökségi tagok',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_MMSZ + ACCESS_ADMIN + ACCESS_MMSZ_PRESIDENT,
    ],
    VISIBLE_GROUP_MMSZ_PRESIDENT => [
        VISIBLE_GROUPS_NAME => 'Elnökségi tagok',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_MMSZ + ACCESS_ADMIN + ACCESS_MMSZ_PRESIDENT,
    ],
    VISIBLE_GROUP_CLUB_MATE_OFFICERS => [
        VISIBLE_GROUPS_NAME => 'Saját egyesületi tisztek',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_CLUBLEADER + ACCESS_CLUB_DELEGATE + ACCESS_CLUB_PRESIDENT,
    ],
    VISIBLE_GROUP_CLUB_MATE_MEMBERS => [
        VISIBLE_GROUPS_NAME => 'Saját egyesületi tagok',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_CLUBLEADER + ACCESS_CLUB_DELEGATE + ACCESS_CLUB_PRESIDENT,
    ],
    VISIBLE_GROUP_SUBCLUBS_DELEGATE => [
        VISIBLE_GROUPS_NAME => 'Alszövetség tagegyesületi küldöttek',
        VISIBLE_GROUPS_PERMPISSION => ACCESS_SUBCLUB_LEADER
    ],
];

function getGroups($given_permission) {
    global $visible_groups;
    $ret = array();
    foreach ($visible_groups as $id => $data) {
        $perm = $data[VISIBLE_GROUPS_PERMPISSION] & $given_permission;
        if ($perm != 0) {
            $ret[$id] = $data[VISIBLE_GROUPS_NAME];
        }
    }
    return $ret;
}


function getVisibleMembersOfRole($role_id) {
    global $mysqliLink;
    $visible = array();
    $role = getRoleById($role_id);
    if (empty($role)) {
        return array();
    }
    switch ($role[ROLE_PERMISSION]) {
        case ACCESS_ADMIN:
        case ACCESS_MMSZ:
        case ACCESS_MMSZ_PRESIDENT:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`, `' . MEMBER_KERNEV . '`, `' . MEMBER_VEZNEV . '`, `' . MEMBER_BORN . '`, '
                    . ' `' . CLUB_TABLE . '`.`' . CLUB_ID . '`, `' . CLUB_TABLE . '`.`' . CLUB_NAME . '` '
                    . ' FROM `' . MEMBER_TABLE . '` '
                    . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND ('
                    . ' `active`.`' . ROLE_PERMISSION . '`&(' . ACCESS_USER . ' | ' . ACCESS_CLUBLEADER . ' | ' . ACCESS_MMSZ
                    . ' | ' . ACCESS_MMSZ_PRESIDENT . ' | ' . ACCESS_CLUB_DELEGATE . ' | ' . ACCESS_CLUB_PRESIDENT . ' | ' . ACCESS_SUBCLUB_LEADER . ')!=0'
                    . ') AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))'
                    . ' INNER JOIN `' . CLUB_TABLE . '` ON (`' . CLUB_TABLE . '`.`' . CLUB_ID . '`=`active`.`' . ROLE_CLUB . '` )'
                    . ' WHERE `' . MEMBER_EMAIL . '`!="" '
                    . ' GROUP BY `' . MEMBER_ID . '` '
                    . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';
            break;
        case ACCESS_CLUBLEADER:
        case ACCESS_CLUB_DELEGATE:
        case ACCESS_CLUB_PRESIDENT:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.*, `' . CLUB_TABLE . '`.* FROM `' . MEMBER_TABLE . '` '
                    . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND  '
                    . ' `active`.`' . ROLE_CLUB . '`=' . $role[ROLE_CLUB] . ' AND ('
                    . ' `active`.`' . ROLE_PERMISSION . '`&(' . ACCESS_USER . ' | ' . ACCESS_CLUBLEADER . ' | ' . ACCESS_CLUB_DELEGATE . ' | ' . ACCESS_CLUB_PRESIDENT . ')!=0 '
                    . ') AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))'
                    . ' INNER JOIN `' . CLUB_TABLE . '` ON (`' . CLUB_TABLE . '`.`' . CLUB_ID . '`=`active`.`' . ROLE_CLUB . '` )'
                    . ' WHERE `' . MEMBER_EMAIL . '`!="" '
                    . ' GROUP BY `' . MEMBER_ID . '` '
                    . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';
            break;
        default:
            return array();
    }
//    error_log($sql);
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $visible[] = $row;
    }
    return $visible;
}

function getDelegatesOfSubclubs($club_id) {
    global $mysqliLink;
    $visible = array();
    $subclubs = unserialize(getClubById($club_id)[CLUB_SUBCLUBS]);
    foreach ($subclubs as $cl_id) {
        $sql = 'SELECT `' . MEMBER_TABLE . '`.*, `' . CLUB_TABLE . '`.* FROM `' . MEMBER_TABLE . '` '
                . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                . ' `active`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND  '
                . ' `active`.`' . ROLE_CLUB . '`=' . $cl_id . ' AND ('
                . '( `active`.`' . ROLE_PERMISSION . '`& ' . ACCESS_CLUB_DELEGATE . ')!=0 '
                . ') AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))'
                . ' INNER JOIN `' . CLUB_TABLE . '` ON (`' . CLUB_TABLE . '`.`' . CLUB_ID . '`=' . $cl_id . ' )'
                . ' WHERE `' . MEMBER_EMAIL . '`!="" '
                . ' GROUP BY `' . MEMBER_ID . '` '
                . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';
//        error_log($sql);
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $visible[] = $row;
        }
    }
    return $visible;
}

function getVisibleMembers() {
    global $mysqliLink, $member_id;
    $visible = array();
    if (isPermitted(ACCESS_ADMIN | ACCESS_MMSZ | ACCESS_MMSZ_PRESIDENT)) {
        $sql = 'SELECT `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` FROM `' . MEMBER_TABLE . '` '
                . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                . ' `active`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND ('
                . ' `active`.`' . ROLE_PERMISSION . '`&(' . ACCESS_USER . ' | ' . ACCESS_CLUBLEADER . ' | ' . ACCESS_MMSZ
                . ' | ' . ACCESS_MMSZ_PRESIDENT . ' | ' . ACCESS_CLUB_DELEGATE . ' | ' . ACCESS_CLUB_PRESIDENT . ') !=0'
                . ') AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))'
                . ' WHERE `' . MEMBER_EMAIL . '`!="" '
                . ' GROUP BY `' . MEMBER_ID . '`'
                . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            if (!in_array($row[MEMBER_ID], $visible)) {
                $visible[] = $row[MEMBER_ID];
            }
        }
        return $visible; // Ebben minden benne van, tovább nem keresünk
    }
    if (isPermitted(ACCESS_CLUBLEADER | ACCESS_CLUB_DELEGATE | ACCESS_CLUB_PRESIDENT)) {
        $clubs = getClubsMember($member_id);
        $sql = 'SELECT `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`, `active`.`' . ROLE_CLUB . '` FROM `' . MEMBER_TABLE . '` '
                . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                . ' `active`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND ('
                . ' `active`.`' . ROLE_PERMISSION . '`&(' . ACCESS_USER . ' | ' . ACCESS_CLUBLEADER . ' | ' . ACCESS_CLUB_DELEGATE . ' | ' . ACCESS_CLUB_PRESIDENT . ')!=0'
                . ') AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))'
                . ' WHERE `' . MEMBER_EMAIL . '`!="" '
                . ' GROUP BY `' . MEMBER_ID . '`'
                . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            if (!in_array($row[MEMBER_ID], $visible) AND isset($clubs[$row[ROLE_CLUB]])) {
                $visible[] = $row[MEMBER_ID];
            }
        }
        $sub_clubs = getSubclubsOfLeader($member_id);
        if (!empty($sub_clubs)) {
            $sql = 'SELECT `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`, `active`.`' . ROLE_CLUB . '` FROM `' . MEMBER_TABLE . '` '
                    . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND ('
                    . ' `active`.`' . ROLE_PERMISSION . '`=' . ACCESS_CLUBLEADER . '  OR '
                    . ' `active`.`' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_DELEGATE . '  OR '
                    . ' `active`.`' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_PRESIDENT
                    . ') AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))'
                    . ' WHERE `' . MEMBER_EMAIL . '`!="" '
                    . ' GROUP BY `' . MEMBER_ID . '`'
                    . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                if (!in_array($row[MEMBER_ID], $visible) AND isset($sub_clubs[$row[ROLE_CLUB]])) {
                    $visible[] = $row[MEMBER_ID];
                }
            }
        }
    }
    return $visible;
}

function getMemberGroup($group_id, $club_id) {
    global $mysqliLink, $member_id;
    switch ($group_id) {
        default:
        case VISIBLE_GROUP_NOBODY:
        case -1:
            return false;
        case VISIBLE_GROUP_CLUB_OFFICERS:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' (`active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_PRESIDENT . '")'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case VISIBLE_GROUP_CLUB_LEADERS:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' (`active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" )'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case VISIBLE_GROUP_CLUB_DELEGATES:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' ( `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
                    . ' )'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case VISIBLE_GROUP_MMSZ_PRESIDENT:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM `' . MEMBER_TABLE . '`, `' . ROLE_TABLE . '` WHERE '
                    . '`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`'
                    . ' AND `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`="' . ACCESS_MMSZ_PRESIDENT . '"'
                    . ' AND (CURDATE() BETWEEN `' . ROLE_TABLE . '`.`' . ROLE_FROM . '` AND `' . ROLE_TABLE . '`.`' . ROLE_TO . '`)';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case VISIBLE_GROUP_EVERYBODY:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '` '
                    . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`!="' . ACCESS_PUBLIC . '" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case VISIBLE_GROUP_CLUB_MATE_OFFICERS:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' (`active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_PRESIDENT . '")'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) '
                    . ' WHERE `active`.`' . ROLE_CLUB . '`=' . $club_id;
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case VISIBLE_GROUP_CLUB_MATE_MEMBERS:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' (`active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_USER . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_PRESIDENT . '")'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) '
                    . ' WHERE `active`.`' . ROLE_CLUB . '`=' . $club_id;
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case 'asdad': //5 => array("Rendezetlen tagok", "Aktív tagok, akiknek van előírt, de ki nem fizetett díjtételük, és van emailcímük")
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM ((`' . MEMBER_TABLE . '` '
                    . ' INNER JOIN `' . FM_TABLE . '` AS `paid_fm` ON ('
                    . ' `paid_fm`.`' . FM_PAY_ID . '`="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`paid_fm`.`' . FM_MEMEBER_ID . '` '
                    . ' AND (CURDATE() BETWEEN `paid_fm`.`' . FM_FROM . '` AND `paid_fm`.`' . FM_TO . '`))'
                    . ' INNER JOIN `' . FEE_TABLE . '` AS `paid_fee` ON ('
                    . ' `paid_fm`.`' . FM_FEE_ID . '`=`paid_fee`.`' . FEE_ID . '` '
                    . ' AND `paid_fee`.`' . FEE_TYPE . '`="' . FEE_TYPE_MEMBERSHIP . '" )'
                    . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '` != "' . ACCESS_PUBLIC . '" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` = `active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '` != "" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`)))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
            break;
        case VISIBLE_GROUP_SUBCLUBS_DELEGATE:
            $club = getClubById($club_id);
            if (empty($club)) {
                $sql = '';
                break;
            }
            $subclubs = unserialize($club[CLUB_SUBCLUBS]);
            if (empty($subclubs)) {
                $sql = '';
                break;
            }
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND ('
//                    . ' `active`.`' . ROLE_PERMISSION . '`=' . ACCESS_CLUBLEADER . '  OR '
                    . ' `active`.`' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_DELEGATE
//                    . ' `active`.`' . ROLE_PERMISSION . '`=' . ACCESS_CLUB_PRESIDENT
                    . ') AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`)))'
                    . ' WHERE';
            foreach ($subclubs as $cl_id) {
                $sql .= ' `active`.`' . ROLE_CLUB . '`=' . $cl_id . ' OR';
            }
            $sql = substr($sql, 0, -2); // Az utolsó OR-t levagjuk
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
 
            break;
    }
    return $mysqliLink->query($sql);
}
