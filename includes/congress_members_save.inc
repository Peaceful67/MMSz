<?php

if (isset($old_selected_role) AND isset($selected_role) AND $old_selected_role != $selected_role) {
    $selected_addr = -1;
}
if (isset($old_selected_addr) AND isset($selected_addr) AND $old_selected_addr != $selected_addr) {
    $congressDelegates->removeAllMembers($selected_open_congress);
    $congressOnline->setCongressAuth1(-1);
    $congressOnline->setCongressAuth2(-1);
    $congressOnline->setCongressByRole($selected_role);
    $congressOnline->setCongressByGroup($selected_addr);
    $res = getMemberGroup($selected_addr, getRoleById($selected_role)[ROLE_CLUB]);
    while ($res AND $row = $res->fetch_assoc()) {
        $congressDelegates->addMember($selected_open_congress, $row[MEMBER_ID]);
    }
}
if (isset($changed_checkbox_add) AND $changed_checkbox_add > 0) {
    $congressDelegates->alterMember($selected_open_congress, $changed_checkbox_add);
    $selected_addr = -1;
    $congressOnline->setCongressByGroup($selected_addr);
}
if (isset($old_selected_auth1) AND isset($selected_auth1) AND $old_selected_auth1 != $selected_auth1) {
    $congressOnline->setCongressAuth1($selected_auth1);
}
if (isset($old_selected_auth2) AND isset($selected_auth2) AND $old_selected_auth2 != $selected_auth2) {
    $congressOnline->setCongressAuth2($selected_auth2);
}
