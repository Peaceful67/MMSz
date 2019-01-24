<?php

function isLicenceDeleteable($l_id) {
    global $mysqliLink;
    $ret = true;
    $sql = 'SELECT * FROM `' . LM_TABLE . '` WHERE `' . LM_LICENCE . '`=' . $l_id . ';';
    $res = $mysqliLink->query($sql);
    if ($res) {
        $ret = ($res->num_rows > 0) ? false : true;
    }
    return $ret;

}


function getLicenceMember($lic_id, $mem_id) {
    global $mysqliLink;
    $ret = array();
    $sql = 'SELECT * FROM `'.LM_TABLE.'` WHERE '
            . '`'.LM_LICENCE.'`='.$lic_id.' '
            . 'AND `'.LM_MEMBER.'`='.$mem_id.' LIMIT 1;';
    $res =  $mysqliLink->query($sql);
    if($res) {
        $ret = $res->fetch_assoc();
    }
    return $ret;
}