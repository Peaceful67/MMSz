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
        $view->danger("Adj meg érvényes azonosítót !");
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
        $view->danger('Válassz egyesületet !');
    } elseif (in_array($selected_club, getClubsId($add_club_mem_id))) {
        $view->danger('Ennek az egyesületnek már tagja');
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

//$output .= '<div class="editor">';
   $view->newDiv('row');
   $view->inputMemberId('add_club_mem_id', 'Azonosító');
   $view->submitButton('require_member', 'Lekérdezés');
   $view->endDiv();

if (isValidMemberId($add_club_mem_id)) {
    $club_leader_id = getRoleClubleader($member_id);
    $view->putElement('<label>Név:</label>' . getMemberName($add_club_mem_id) . '<br>');
    $view->putElement('<label>Egyesületi tagságai:</label><br>');
    $isAlreadyGot = false;
    $view->newDiv('panel panel-default');
    foreach (getClubsEver($add_club_mem_id) as $role_id => $role) {
        $view->newDiv('panel-body');
        if ($club_leader_id == $role[0] || isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ)) {
            $view->submitDelete('delete_club', $role_id, 'A tag törlése a ' . $role[1] . ' egyesületből', 'Biztosan törölni akarod ?');
        }
        $view->putElement($role[2] . ' : ' . $role[3] . ' => ' . $role[1]);
        if ($club_leader_id == $role[0]) {
            $isAlreadyGot = true;
        }
        $view->endDiv();
    }
    $view->endDiv();
//    $output .= '<input type="hidden" name="add_club_mem_id" value="' . $add_club_mem_id . '">';
    if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ)) {
        $clubs_of_member = getClubsId($add_club_mem_id);
        if (!isset($selected_club)) {
            $selected_club = key($clubs_of_member);
        }
        $view->newDiv('row');
        $view->inputDate('add_club_from');
        $view->inputDate('add_club_to');
        $view->inputClub('', getClubNameList(), 'selected_club', false);
        $view->submitButton('save_add_club', 'Rögzít');
        $view->endDiv();
    } elseif (!$isAlreadyGot) {
        $view->newDiv('row');
        $view->inputDate('add_club_from');
        $view->inputDate('add_club_to');
        $selected_club = getRoleClubleader($member_id);
        $view->putElement(getClubName($selected_club));
        $view->putHidden('selected_club');

        $view->submitButton('save_add_club', 'Rögzít');
        $view->endDiv();
    }

    $output .= '</div>';
}
