<?php

if (!isset($edit_member)) {
    $edit_member = -1;
}
$base_data_id = $edit_member;
if ($base_data_id > 0) {
    $_SESSION[SESSION_FOUND_MEMBERS] = serialize(array($edit_member));
    $_SESSION[SESSION_SELECTED_ITEM] = 0;
    include_once MODULES . 'edit_member.inc';
} else {
    include_once INCLUDES . 'base_data.inc';
}

