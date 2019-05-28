<?php

include_once 'params.inc';
include_once FUNCTIONS . 'init.inc';

$output = '<?xml version="1.0"?>';

if (isset($_GET['categories'])) {
    $xml_cats = new SimpleXMLElement("<?xml version=\"1.0\"?><categories></categories>");
    $cats_array = array();
    $res = $mysqliLink->query('SELECT * FROM `' . CATEGORIES_TABLE . '`  WHERE 1 ORDER BY `' . CATEGORIES_SHORT . '`');
    while ($res AND $row = $res->fetch_assoc()) {
        $cat_id = $row[CATEGORIES_ID];
        $cats_array[$cat_id] = ($row);
        $cats_array[$cat_id][CATEGORIES_BRANCH] = $branches[$row[CATEGORIES_BRANCH]];
        $file_en = RULES . '_' . $cat_id . '_EN.pdf';
        $file_hu = RULES . '_' . $cat_id . '_HU.pdf';
        if (is_file($file_en)) {
            $cats_array[$cat_id]['document_en'] = URL_RULES.'_' . $cat_id . '_EN.pdf';
        }
        if (is_file($file_hu)) {
            $cats_array[$cat_id]['document_hu'] = URL_RULES.'_' . $cat_id . '_HU.pdf';
        }
    }
    array_to_xml($cats_array, $xml_cats, 'category');
    $output = $xml_cats->asXML();
}

if (isset($_GET['clubs'])) {
    $xml_clubs = new SimpleXMLElement("<?xml version=\"1.0\"?><clubs></clubs>");
    $clubs_array = array();


    $res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ACT . '`="1" ORDER BY `' . CLUB_NAME . '`');
    while ($res AND $row = $res->fetch_assoc()) {
        $club_id = $row[CLUB_ID];
        $clubs_array[$club_id] = ($row);
        $clubs_array[$club_id]['club_leaders'] = getClubLeaders($club_id);
        $clubs_array[$club_id]['mem_num'] = getNumMemberOfClub($club_id);
    }
    array_to_xml($clubs_array, $xml_clubs, 'club');
    $output = $xml_clubs->asXML();
}
if (isset($_GET['memberships'])) {
    $xml_memship = new SimpleXMLElement("<?xml version=\"1.0\"?><memberships></memberships>");
    $mem_array = array();
    $fees = getFeeIdsToday(FEE_TYPE_MEMBERSHIP);
    foreach ($member_types as $mt_key => $mt_name) {
        if (isset($fees[$mt_key])) {
            $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
                    . ' `' . FM_FEE_ID . '`=' . $fees[$mt_key]
                    . ' AND (CURDATE() BETWEEN `' . FM_FROM . '` AND `' . FM_TO . '`) ';
            $res = $mysqliLink->query($sql);
            $num = $res ? $res->num_rows : 0;
            $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
                    . ' `' . FM_FEE_ID . '`=' . $fees[$mt_key]
                    . ' AND `' . FM_BILL_ID . '`!=0'
                    . ' AND (CURDATE() BETWEEN `' . FM_FROM . '` AND `' . FM_TO . '`) ';
            $res = $mysqliLink->query($sql);
            $num_paied = $res ? $res->num_rows : 0;
            $mem_array[] = ["form" => $mt_name,
                "members" => $num,
                "paid" => $num_paied];
        }
    }
    array_to_xml($mem_array, $xml_memship, 'membership');
    $output = $xml_memship->asXML();
}
if (isset($_GET['members'])) {
    $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '` '
            . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
            . ' `active`.`' . ROLE_PERMISSION . '`!="' . ACCESS_PUBLIC . '" '
            . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
            . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
            . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) '
            . ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
    $res = $mysqliLink->query($sql);
    $output .= '<members>\n';
    $output .= '<title>Aktív tagok adatai</title>\n';
    $output .= '<member>'
            . '<id>Azonosító</id>\n'
            . '<name>Név</name>\n'
            . '<born>Született</born>\n'
            . '<paid>MMSz tagdíj</paid>\n'
            . '<category>Kategóriák</category>\n'
            . '</member>';
    while ($res AND $row = $res->fetch_assoc()) {
        $output .= '<member>';
        $output .= '<id>' . $row[MEMBER_ID] . '</id>\n';
        $output .= '<name>' . $row[MEMBER_VEZNEV] . ' ' . $row[MEMBER_KERNEV] . '</name>\n';
        $output .= '<born>' . $row[MEMBER_BORN] . '</born>\n';
        $output .= '<paid>' . (isPaidMembership($row[MEMBER_ID]) ? 1 : 0) . '</paid>\n';
        $output .= '<category>' . getListMemberOldCategories($row[MEMBER_ID]) . '</category>\n';
        $output .= '</member>';
    }

    $output .= '</members>\n';
}

if (isset($_GET["officers"])) {
    $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
    $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
            . ' (`active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
            . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
            . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_PRESIDENT . '")'
            . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
            . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
            . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
    $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '` ';
    $res = $mysqliLink->query($sql);
    $output .= '<officers>\n';
    $output .= '<title>Egyesületi tisztek</title>\n';
    $output .= '<officer>'
            . '<id>Azonosító</id>\n'
            . '<name>Név</name>\n'
            . '<email>Született</email>\n'
            . '</officer>';
    while ($res AND $row = $res->fetch_assoc()) {
        $output .= '<officer>';
        $output .= '<id>' . $row[MEMBER_ID] . '</id>\n';
        $output .= '<name>' . $row[MEMBER_VEZNEV] . ' ' . $row[MEMBER_KERNEV] . '</name>\n';
        $output .= '<email>' . $row[MEMBER_EMAIL] . '</email>\n';
        $output .= '</officer>';
    }
    $output .= '</officers>';
}
theEnd($output);
include_once INCLUDES . 'crontab.inc';

function array_to_xml($array, &$xml_user_info, $item_name) {
    foreach ($array as $key => $value) {
        if (is_array($value)) {
            if (!is_numeric($key)) {
                $subnode = $xml_user_info->addChild("$key");
                array_to_xml($value, $subnode, $item_name0);
            } else {
                $subnode = $xml_user_info->addChild($item_name);
                array_to_xml($value, $subnode, $item_name);
            }
        } elseif (!empty($value)) {
            $xml_user_info->addChild("$key", htmlspecialchars(trim($value)));
        }
    }
}
