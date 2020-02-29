<?php

$sql = 'SELECT * FROM `' . MEMBER_TABLE . '` ;';
$res = $mysqliLink->query($sql);
$unique = $multi = 0;
$found = array();
error_log('Similaty');
while ($res AND $row = $res->fetch_assoc()) {
    $already = similarMembers($row[MEMBER_VEZNEV], $row[MEMBER_KERNEV], $row[MEMBER_EMAIL], $row[MEMBER_BORN]);
    if (count($already) > 1) {
        if (!in_array($row[MEMBER_ID], $found)) {
            error_log('Similar');
            $output .= '<label>Egyezőnek vélt tagok azonosítói:</label>';            
            foreach ($already as $id => $member) {
                error_log($id);
                $output .= $id . ' ';
                $found[] = $id;
            }
            $output .= '<br>';
            $multi++;
        }
    } else {
        $unique++;
    }
}
logger($member_id, -1, LOGGER_DATABASE, ' Hasonló tagok keresése. Egyedi: '.$unique.', egyező: '.$multi);
$output .= 'Eredmény: <br>Átvizsgált egyedi tagok száma: ' . $unique . '<br>Fellelt egyezőségek száma:' . $multi . '<br>';
