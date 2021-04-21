<?php

if ($congress_part == $congress_part_old) {
    switch ($congress_part) {
        case 'members': // Save congress_member on submit
            include_once INCLUDES . 'congress_members_save.inc';
            break;
        case 'congress': // Save congress_congress on submit
            include_once INCLUDES . 'congress_congress_save.inc';
            break;
        case 'items': // save congress_items on submit
            include_once INCLUDES . 'congress_item_save.inc';
            break;
    }
}