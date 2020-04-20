<?php

$congressItems->modifyItemTitle($item_title);
$congressItems->modifyItemDescription($item_description);
$option = 0;
$chk = 'option_chk_byname';
if (isset($$chk) AND ( $$chk == 'on')) {
    $option |= CONGRESS_OPTION_BY_NAME;
}
$chk = 'option_chk_public';
if (isset($$chk) AND ( $$chk == 'on')) {
    $option |= CONGRESS_OPTION_PUBLIC;
}
$chk = 'option_chk_byemail';
if (isset($$chk) AND ( $$chk == 'on')) {
    $option |= CONGRESS_OPTION_BY_EMAIL;
}
$chk = 'option_chk_multichoice';
if (isset($$chk) AND ( $$chk == 'on')) {
    $option |= CONGRESS_OPTION_MULTICHOICE;
}
$congressItems->setItemOption($option);
error_log('Files: '.print_r($_FILES, true));
if (isset($_FILES["congress_item_file"]) AND ! empty($_FILES["congress_item_file"]["tmp_name"])) {
    error_log('Doc file:'.$doc_file);
    move_uploaded_file($_FILES["congress_item_file"]["tmp_name"], $doc_file);
 //   logger($member_id, -1, LOGGER_CONGRESS_ONLINE, "A " . $selected_congress_item . " azonosítójú napirendhez melléklet feltöltve.");
}

if (isset($selected_congress_item_dependencies)) {
    $dependecy = array();
    foreach ($selected_congress_item_dependencies as $depItemId) {
        if ($depItemId != $selected_congress_item) {
            $dependecy[] = $depItemId;
        }
    }
    $congressItems->setDependency($dependecy);
}
logger($member_id, -1, LOGGER_CONGRESS_ONLINE, $selected_congress_item . ' azonosítújú napirend módosítása. '
        . 'Bellítások:' . $option);
