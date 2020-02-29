<?php

$collected_fms = array();
foreach ($fms2bill as $fm2bill) {
    $chk = FM_CHECKBOX . '[' . $fm2bill[0] . ']';
    if (isset($$chk) && $$chk == "on") {
        $collected_fms[] = $fms2bill;
    }
}
$_SESSION[SESSION_FEE_TO_BILL] = serialize($collected_fms);

