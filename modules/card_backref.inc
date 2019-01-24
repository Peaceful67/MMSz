<?php

require_once (SIMPLEPAY . 'config.inc');
require_once (SIMPLEPAY . 'SimplePayment.class.inc');

$output .= "<script>
        window.setInterval(function() {
            reloadIFrame()
        }, 5000);

        function reloadIFrame() {
            console.log('reloading..');
            document.getElementById('ios').contentWindow.location.reload();
        }
    </script>";


if (isset($_SESSION[SESSION_SP_BACKREF])) {
    $backref = unserialize($_SESSION[SESSION_SP_BACKREF]);
    $backref->checkResponse();
    if (isBillPaied($backref->order_ref)) {
//        error_log("Backref : Megvan a fizetés");
        redirect("simplepay_ipn");
        exit(0);
    }
    logger($member_id, -1, LOGGER_SIMPLEPAY, 'Visszatérés az OTP felületről a ' . $backref->order_ref . ' azonosítójú számlák befizetéséből.');
    $output .= ' <iframe class="keret" width="650px" height="150px" id="ios" src="' . SIMPLEPAY . 'ios.php?bill_number=' . $backref->order_ref . '"></iframe><br>';
}
?>


