<?php

if (!isset($congress_title)) {
    $congress_title = '';
}
if (!isset($congress_description)) {
    $congress_description = '';
}

if (isset($congress_congress_save)) {
    $selected_open_congress_old = -2;
    $congressOnline->modifyCongressTitle($congress_title);
    $congressOnline->modifyCongressDescription($congress_description);
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
    }
    $chk = 'option_chk_auth_mand';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $option |= CONGRESS_OPTIONS_AUTH_MANDATORY;
    }

    $congressOnline->setCongressOption($option);
}

$congress_title = $congressOnline->getCongressTitle();
$congress_description = $congressOnline->getCongressDescription();

output_label('Cím');
$output .= '<input type="text" size="80" maxlength="250"  name="congress_title" value="' . $congress_title . '">';
output_ln();
output_label('Leírás');
$output .= '<textarea rows="5" cols="100" maxlength="1000" name="congress_description">' . $congress_description . '</textarea>';
output_ln();

$output .= '<h3>Beállítások</h3>';

if (isset($close_congress)) {
    $congressOnline->closeCongress();
    logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_open_congress . ' azonosítójú szavazás létrehozva.');
    redirect('congress_online');
}
output_label('Név szerinti szavazás');
output_set_checkbox('option_chk_byname', $congressOnline->isCongressByName());
output_spaces(4);
o('Nyilvános a szavozók szavazata.');
output_ln();
output_label('Mindenki láthatja');
output_set_checkbox('option_chk_public', $congressOnline->isCongressPublic());
output_spaces(4);
o('A gyűlés kiírása nyilvános.');
output_ln();
output_label('Emailben is lehet szavazni');
output_set_checkbox('option_chk_byemail', $congressOnline->isCongressByEmail());
output_spaces(4);
o('Emailben kapott linken keresztül is lehet szavazni.');
output_ln();
output_label('Utólagos napirend');
output_set_checkbox('option_chk_appendable', $congressOnline->isCongressAppendable());
output_spaces(4);
o('Napirendek tárgyalása közben lehetőség van új napirend felvételére.');
output_ln();
output_label('Automatikus napirend');
output_set_checkbox('option_chk_auto_item', $congressOnline->isCongressAutoItem());
output_spaces(4);
o('Automatikus szavazás létrehozása a napirendek és hitelesítők elfogadásához.');
output_ln();
output_label('Jegyzőkönyv vezető');
output_set_checkbox('option_chk_auth_mand', $congressOnline->isCongressAuthMandatory());
output_spaces(4);
o('Kötelező megadni két jegyzőkönyv vezetőt');
output_ln(2);

if (!empty($error_arr)) {
    $output .= '<p style="color:red">';
    foreach ($error_arr as $err) {
        $output .= $err;
        output_ln();
    }
    $output .= '</p>';
}

$output .= '<button name="congress_congress_save"><img src="' . IMAGES . 'save.jpg"></button>';
