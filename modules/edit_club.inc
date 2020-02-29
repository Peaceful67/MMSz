<?php

if (!isset($edit_club)) {
    $edit_club = -1;
}

$clubs = getClubsOfLeader($member_id);
if (count($clubs) == 1) {
    reset($clubs);
    $edit_club = key($clubs);
} else {
    $output .= '<form method="post">';
    $output .= '<label>Egyesület : </label>'
            . '<select id="edit_club" name="edit_club" onchange="submit();">';
    $output .= '<option value="-1">Válassz egyesületet !</option>';
    foreach ($clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $edit_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }
    $output .= '</select><br><br>';
    $output .= '</form>';
}
if ($edit_club > 0) {
    include_once INCLUDES . 'club_editor.inc';
}
