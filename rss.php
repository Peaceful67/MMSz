<?php

include_once 'params.inc';
include_once FUNCTIONS . 'init.inc';


if (isset($_GET['clubs'])) {
    $xml_clubs = new SimpleXMLElement("<?xml version=\"1.0\"?><clubs></clubs>");
    $clubs_array = array();


    $res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ACT . '`="1" ORDER BY `' . CLUB_NAME . '`');
    while ($res AND $row = $res->fetch_assoc()) {
        $club_id = $row[CLUB_ID];
        $clubs_array[$club_id] = ($row);
        $clubs_array[$club_id]['mem_num'] = getNumMemberOfClub($row[CLUB_ID]);
    }
    array_to_xml($clubs_array,$xml_clubs);
    $output = $xml_clubs->asXML();
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


function array_to_xml($array, &$xml_user_info) {
    foreach($array as $key => $value) {
        if(is_array($value)) {
            if(!is_numeric($key)){
                $subnode = $xml_user_info->addChild("$key");
                array_to_xml($value, $subnode);
            }else{
                $subnode = $xml_user_info->addChild("item$key");
                array_to_xml($value, $subnode);
            }
        }else {
            $xml_user_info->addChild("$key",htmlspecialchars("$value"));
        }
    }
}