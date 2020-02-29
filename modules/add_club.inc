<?php

if (!isset($add_club_from)) {
    $add_club_from = $beginingOfYear;
}
if (!isset($add_club_to)) {
    $add_club_to = $neverEnding;
}
if (!isset($add_club_mem_id)) {
    $add_club_mem_id = 0;
}
if (isset($require_member)) {
    unset($save_add_club);
    if ($add_club_mem_id <= 0 || $add_club_mem_id > getLastMemberId()) {
        warning("Adj meg érvényes azonosítót !");
        unset($member_to_add);
    } else {
        $member_to_add = $add_club_mem_id;
    }
}

if (isset($delete_club)) {
    logger($member_id, $add_club_mem_id, LOGGER_MEMBER_MOD, $add_club_mem_id . ' azonosítójú felhasználó (' . $access[ACCESS_USER] . ') jogosultáságnak törlése. Klub: ' . getRoleById($delete_club)[ROLE_CLUB]);
    $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $delete_club . '"';
    $mysqliLink->query($sql);
}
if (isset($save_add_club)) {
    if ($selected_club < 0) {
        warning('Válassz egyesületet !');
    } else {
        $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                . ROLE_MEMBER . '`, `' . ROLE_CLUB
                . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                . '`) VALUES ("'
                . $add_club_mem_id . '", "' . $selected_club . '", "'
                . ACCESS_USER . '", "' . $add_club_from . '", "' . $add_club_to . '")';
        $mysqliLink->query($sql);
        logger($member_id, $add_club_mem_id, LOGGER_MEMBER_MOD, $add_club_mem_id . ' azonosítójú felhasználó (' . $access[ACCESS_USER] . ') jogosultáságnak felvitele. Klub:.' . $selected_club . ', from:' . $add_club_from . ', to: ' . $add_club_to);
    }
}

$output .= '<div class="editor"><form method="post">';
$output .= '<label>Azonosító:</label><input min="0" max="9999" type="number" name="add_club_mem_id" value="' . $add_club_mem_id . '" onchange="submit();">';
$output .= ' <input type="submit" name="require_member" value="Lekérdezés"><br>';

if (isValidMemberId($add_club_mem_id)) {
    $output .= '<label>Név:</label>' . getMemberName($add_club_mem_id) . '<br>';
    $output .= '<label>Egyesületi tagságok:</label><br>';
    $isAlreadyGot = false;
    foreach (getClubsEver($add_club_mem_id) as $role_id => $role) {
        $output .= $role[2] . ' : ' . $role[3] . ' => ' . $role[1];
        $club_leader_id = getRoleClubleader($member_id);
        if ($club_leader_id == $role[0]) {
            $isAlreadyGot = true;
        }
        if ($club_leader_id == $role[0] || isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ)) {
            $output .= '<input type="image" title="Törlés" name="delete_club" value="' . $role_id . '" src="' . IMAGES . 'delete.jpg">';
        }
        $output .= '<br>';
    }
//    $output .= '<input type="hidden" name="add_club_mem_id" value="' . $add_club_mem_id . '">';
    if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ)) {
        $output .= '<input type="date" name="add_club_from" size="10" value="' . $add_club_from . '">'
                . '<input type="date" name="add_club_to" size="10" value="' . $add_club_to . '">';
        if (!isset($selected_club)) {
            $selected_club = -1;
        }
        $output .= '<select id="selected_club" name="selected_club" >';
        $output .= '<option value="-1">Válassz egyesületet !</option>';
        $clubs = getClubNameList();
        $clubs_of_member = getClubsId($role_member_id);
        foreach ($clubs as $club_id => $club_name) {
            $output .= '<option value="' . $club_id . '" ' . (empty($clubs_of_member[$club_id]) ? "" : "selected") . '>' . $club_name . '</option>';
        }
        $output .= '</select>';
        $output .= '<input type="submit" name="save_add_club" value="Rögzít"><br>';
    } elseif(!$isAlreadyGot) {
        $output .= '<input type="date" name="add_club_from" size="10" value="' . $add_club_from . '">'
                . '<input type="date" name="add_club_to" size="10" value="' . $add_club_to . '">';
        $selected_club = getRoleClubleader($member_id);
        $output .= getClubName($selected_club);
        $output .= '<input type="hidden" name="selected_club" value="' . $selected_club . '">';
        $output .= '<input type="submit" name="save_add_club" value="Rögzít"><br>';
    }
    $output .= '</form>';
    $output .= '</div>';
}
