<?php

if (isset($save_item)) {
    $old_selected_congress_item = -2;
    $congressItems->setSelectedItem($selected_congress_item)->modifyItemTitle($item_title);
    $congressItems->modifyItemDescription($item_description);
    if (isset($_FILES["congress_item_file"]) AND ! empty($_FILES["congress_item_file"]["tmp_name"])) {
        move_uploaded_file($_FILES["congress_item_file"]["tmp_name"], $doc_file);
        logger($member_id, -1, LOGGER_CONGRESS_ONLINE, "A " . $selected_congress_item . " azonosítójú napirendhez melléklet feltöltve.");
    }
}
if (isset($save_item) OR (isset($selected_congress_item) AND isset($old_selected_congress_item) AND $selected_congress_item > 0 AND $selected_congress_item==$old_selected_congress_item)) {
    $congressItems->setSelectedItem($selected_congress_item);
    $item_option = 0;
    $chk = 'option_chk_byname';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $item_option |= CONGRESS_OPTIONS_BY_NAME;
    }
    $chk = 'option_chk_byemail';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $item_option |= CONGRESS_OPTIONS_BY_EMAIL;
    }
    $chk = 'option_chk_multichoice';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $item_option |= CONGRESS_OPTIONS_MULTICHOICE;
    }
    $chk = 'option_chk_yes_no';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $item_option |= CONGRESS_OPTIONS_YES_NO;
    }
    $chk = 'option_chk_abstain';
    if (isset($$chk) AND ( $$chk == 'on')) {
        $item_option |= CONGRESS_OPTIONS_ABSTAIN_ENABLED;
    }
    if (!isset($decision_radio)) {
        $decision_radio = CONGRESS_OPTIONS_DECISION_50;
    }
    foreach ($decision_options as $decision_opt => $decision_name) {
        if ($decision_radio == $decision_opt) {
            $item_option |= $decision_opt;
        } else {
            $item_option &= ~$decision_opt;
        }
    }
    $congressItems->setItemOption($item_option);
    if (isset($save_item) AND isset($_FILES["congress_item_file"]) AND ! empty($_FILES["congress_item_file"]["tmp_name"])) {
        move_uploaded_file($_FILES["congress_item_file"]["tmp_name"], $doc_file);
        logger($member_id, -1, LOGGER_CONGRESS_ONLINE, "A " . $selected_congress_item . " azonosítójú napirendhez melléklet feltöltve.");
    }

    if (isset($multichk_item_dependencies)) {
        $congressItems->setDependency($multichk_item_dependencies);
    } else {
        $congressItems->setDependency(array());
    }
}