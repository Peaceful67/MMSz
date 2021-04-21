<?php

define("CLM_CHK", "lm_chk_");
define("CLM_FROM", "date_from_");
define("CLM_TO", "date_to_");
if (isset($lm_save)) {
    $log = '';
    foreach ($licences as $key => $value) {
        $chk = CLM_CHK . $key;
        $from = CLM_FROM . $key;
        $to = CLM_TO . $key;
        $sql = 'DELETE FROM `' . LM_TABLE . '` WHERE ' // Mindenkepp kitoroljuk
                . '`' . LM_TYPE_ID . '`=' . $key . ' AND `' . LM_MEMBER . '`=' . $licences_member_id . ';';
        $mysqliLink->query($sql);
        if (isset($$chk) AND $ $chk == "on") {
            $sql = 'INSERT INTO `' . LM_TABLE . '` '
                    . '(`' . LM_TYPE_ID . '`, `' . LM_MEMBER . '`, `' . LM_FROM . '`,`' . LM_TO . '`) VALUES '
                    . '(' . $key . ', ' . $licences_member_id . ', "' . $$from . '", "' . $$to . '");';
            $mysqliLink->query($sql);
            $log .= $value . ':' . $$from . '-' . $$to . ', ';
        }
    }
    logger($member_id, $licences_member_id, LOGGER_LICENCES, 'Jogosítások beállítva:' . $log);
}

$view->newKeret();
foreach ($licences as $key => $value) {
    $lm = getLicenceMember($key, $licences_member_id);
    if (empty($lm)) {
        $lm_from = $today;
        $lm_to = addDatePeriod($today, $fee_type[FEE_TYPE_LICENCES][1]);
        $lm_checked = false;
    } else {
        $lm_from = $lm[LM_FROM];
        $lm_to = $lm[LM_TO];
        $lm_checked = true;
    }
    $chk = CLM_CHK . $key;
    $$chk = $lm_checked ? 'on' : 'off';
    $from = CLM_FROM . $key;
    $$from = $lm_from;
    $to = CLM_TO . $key;
    $$to = $lm_to;
    $view->inputRole($chk, $from, $to, $value);
}
$view->submitButton('lm_save', 'Rögzítés');
$view->endKeret();
