<?php


function FAI_get($id_lic) {
    $json_arr = array();
    $str = getOptionValue(OPTIONS_NAME_FAI_URL) . FAI_LICENCE_GET . ''
            . '/' . $id_lic . '?'
            . 'auth_username="' . getOptionValue(OPTIONS_NAME_FAI_AUTH_USER) . '"'
            . '&auth_password="' . base64_encode(getOptionValue(OPTIONS_NAME_FAI_AUTH_PASSWD)) . '"';
    $answer = file_get_contents($str);
    try {
        $json_arr = json_decode($answer, true);
    } catch (Exception $exc) {
        error_log($exc->getTraceAsString());
        warning('Hiba a FAI rendszerben');
    }
    return $json_arr;
}

function FAI_search($mem_id) {
    $str = getOptionValue(OPTIONS_NAME_FAI_URL) . FAI_LICENCE_SEARCH . '?'
            . 'auth_username="' . getOptionValue(OPTIONS_NAME_FAI_AUTH_USER) . '"'
            . '&auth_password="' . base64_encode(getOptionValue(OPTIONS_NAME_FAI_AUTH_PASSWD)) . '"'
            . '&country=HUN'
            . '&search_number=' . mem2FaiMem($mem_id);   
    return queryFai($str);
}

function FAI_search_last($mem_id) {
    $json_array = FAI_search($mem_id);
    $ret = array();
    if (!empty($json_array)) {
        $ret = end($json_array);
    }
    return $ret;
}

