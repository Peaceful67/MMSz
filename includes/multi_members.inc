<?php

if(isset($delete_member)) {
    wipeMember($delete_member);
}
$sql = 'SELECT * FROM `' . MEMBER_TABLE . '` ;';
$res = $mysqliLink->query($sql);
$unique = $multi = 0;
$found = array();
while ($res AND $row = $res->fetch_assoc()) {
    $already = similarMembers($row[MEMBER_VEZNEV], $row[MEMBER_KERNEV], $row[MEMBER_EMAIL], $row[MEMBER_BORN]);
    if (count($already) > 1) {
        if (!in_array($row[MEMBER_ID], $found)) {
            $output .= '<label>Egyezőnek vélt tagok:</label>';
            foreach ($already as $id => $member) {
                $output .= '<button type="submit" style="margin: 3px; width: 50px;" name="delete_member" value="' . $id . '" title="'                         
                        . $member[MEMBER_VEZNEV] . '&nbsp;' . $member[MEMBER_KERNEV]
                        . ' törlése. ('
                        . $member[MEMBER_EMAIL] . '&nbsp;;'
                        . $member[MEMBER_BORN] . ')"'
                        . ((isDeletableMember($id)  && ($permission & (ACCESS_ADMIN | ACCESS_MMSZ | ACCESS_CLUBLEADER)) != 0 )? '': ' disabled ')
//                        . 'onClick="return confirm(\'Biztosan végelegesen törölni akarod a ' . $id . ' azonosítójú tagot?\');"'
                        . ' >'.$id . '</button>&nbsp;&nbsp;&nbsp;';
                $found[] = $id;
            }
            $output .= '<br>';
            $multi++;
        }
    } else {
        $unique++;
    }
}
logger($member_id, -1, LOGGER_DATABASE, ' Hasonló tagok keresése. Egyedi: ' . $unique . ', egyező: ' . $multi);
$output .= 'Eredmény: <br>Átvizsgált egyedi tagok száma: ' . $unique . '<br>Fellelt egyezőségek száma:' . $multi . '<br>';
