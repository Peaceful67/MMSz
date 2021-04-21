<?php

if (!isset($edit_club)) {
    $edit_club = -1;
}

$clubs = getClubsOfLeader($member_id);
if (count($clubs) == 1) {
    reset($clubs);
    $edit_club = key($clubs);
} else {
    $view->inputClub('Egyesület', [-1 => 'Válassz!!!'] + $clubs, 'edit_club', true);
}
if ($edit_club > 0) {
    include_once INCLUDES . 'club_editor.inc';
}
