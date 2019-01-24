<?php

$edit_club = getRoleClubleader($member_id);
if($edit_club >0) {
    include_once INCLUDES.'club_editor.inc';
}

