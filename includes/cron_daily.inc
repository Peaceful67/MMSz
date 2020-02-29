<?php
logger(-1, -1, LOGGER_CRONTAB, "Napi rutin");

$sql = 'SELECT * FROM `'.ROLE_TABLE.'` WHERE `'.ROLE_PERMISSION.'`!=0 '
        . 'AND (CURRENT_DATE BETWEEN `'.ROLE_FROM.'` AND `'.ROLE_TO.'`);';
$res = $mysqliLink->query($sql);
while($res AND $row = $res->fetch_assoc()) {
    $mysqliLink->query('DELETE FROM `'.ROLE_TABLE.'` WHERE `'.ROLE_MEMBER.'`='.$row[ROLE_MEMBER].' '
            . ' AND `'.ROLE_PERMISSION.'`=0 ;');
}

    