<?php

if (isset($congress_congress_save)) {  // Ide vagy save gomb miatt, vagy a checkboxok változása miatt jöhetünk
    $congressOnline->modifyCongressTitle($congress_title);
    $congressOnline->modifyCongressDescription($congress_description);
} elseif ($selected_open_congress > 0 AND $selected_open_congress == $selected_open_congress_old) {
    $option = 0;
    $chk = 'option_chk_byname';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_BY_NAME;
    }
    $chk = 'option_chk_public';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_PUBLIC;
    }
    $chk = 'option_chk_byemail';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_BY_EMAIL;
    }
    $chk = 'option_chk_appendable';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_APPENDABLE;
    }
    $chk = 'option_chk_auto_item';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_AUTO_ITEM;
        $option |= CONGRESS_OPTIONS_AUTH_MANDATORY;
    }
    $chk = 'option_chk_auth_mand';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_AUTH_MANDATORY;
    }
    $chk = 'option_chk_base_voted';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_POLL_BASE_VOTED;
    }
    if (!isset($mandate_radio)) {
        $mandate_radio = CONGRESS_MANDATE_TYPE_ONE;
    }
    $congressOnline->setCongressOption($option);
    $congressOnline->setCongressMandateType($mandate_radio);
}