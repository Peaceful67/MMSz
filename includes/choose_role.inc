<?php

if (!isset($selected_club)) {
    $selected_club = -1;
}
if (isset($roles_save)) {
    if (!isset($mmsz_role_id)) {
        $mmsz_role_id = -2;
    }
    if ($mmsz_role_id == -1) { //Újat kell felvenni
        if (isset($check_access_mmsz) AND $check_access_mmsz == 'on') {
            if ($access_mmsz_from < $access_mmsz_to) {
                $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                        . ROLE_MEMBER . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                        . '`) VALUES ("'
                        . $role_member_id . '", "' . ACCESS_MMSZ . '", "' . $access_mmsz_from . '", "' . $access_mmsz_to . '")';
                logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_MMSZ] . ') jogosultáságnak felvitele. from:' . $access_mmsz_from . ', to: ' . $access_mmsz_to);
                $mysqliLink->query($sql);
            }
        }
    } elseif (isset($check_access_mmsz) AND $check_access_mmsz == 'on') {
        if ($access_mmsz_from < $access_mmsz_to) {
            $sql = 'UPDATE `' . ROLE_TABLE . '` SET '
                    . '`' . ROLE_FROM . '`="' . $access_mmsz_from . '", '
                    . '`' . ROLE_TO . '`="' . $access_mmsz_to . '" '
                    . 'WHERE `' . ROLE_ID . '`="' . $mmsz_role_id . '"';
            logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_MMSZ] . ') jogosultáságnak módosítása. from:' . $access_mmsz_from . ', to: ' . $access_mmsz_to);
            $mysqliLink->query($sql);
        }
    } elseif ($mmsz_role_id != -2) { //Letezot kell torolni
        $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $mmsz_role_id . '"';
        logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_MMSZ] . ') jogosultáságnak törlése');
        $mysqliLink->query($sql);
    }

    if (!isset($mmszpres_role_id)) {
        $mmszpres_role_id = -2;
    }
    if ($mmszpres_role_id == -1) {
        if (isset($check_access_mmszpres) AND $check_access_mmszpres == 'on') {
            if ($access_mmszpres_from < $access_mmszpres_to) {
                $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                        . ROLE_MEMBER . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                        . '`) VALUES ("'
                        . $role_member_id . '", "' . ACCESS_MMSZ_PRESIDENT . '", "' . $access_mmszpres_from . '", "' . $access_mmszpres_to . '")';
                $mysqliLink->query($sql);
                logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_MMSZ_PRESIDENT] . ') jogosultáságnak felvitele. from:' . $access_mmszpres_from . ', to: ' . $access_mmszpres_to);
            }
        }
    } elseif (isset($check_access_mmszpres) AND $check_access_mmszpres == 'on') {
        if ($access_mmszpres_from < $access_mmszpres_to) {
            $sql = 'UPDATE `' . ROLE_TABLE . '` SET '
                    . '`' . ROLE_FROM . '`="' . $access_mmszpres_from . '", '
                    . '`' . ROLE_TO . '`="' . $access_mmszpres_to . '" '
                    . 'WHERE `' . ROLE_ID . '`="' . $mmszpres_role_id . '"';
            logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_MMSZ_PRESIDENT] . ') jogosultáságnak módosítása. from:' . $access_mmszpres_from . ', to: ' . $access_mmszpres_to);
            $mysqliLink->query($sql);
        }
    } elseif ($mmszpres_role_id != -2) {
        $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $mmszpres_role_id . '"';
        logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_MMSZ_PRESIDENT] . ') jogosultáságnak törlése');
        $mysqliLink->query($sql);
    }

    if ($selected_club > 0) {

        if ($user_role_id == -1) {
            if (isset($check_access_user) AND $check_access_user == 'on') {
                if ($access_user_from < $access_user_to) {
                    $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                            . ROLE_MEMBER . '`, `' . ROLE_CLUB
                            . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                            . '`) VALUES ("'
                            . $role_member_id . '", "' . $selected_club . '", "'
                            . ACCESS_USER . '", "' . $access_user_from . '", "' . $access_user_to . '")';
                    $mysqliLink->query($sql);
                    logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_USER] . ') jogosultáságnak felvitele. Klub:.' . $selected_club . ', from:' . $access_user_from . ', to: ' . $access_user_to);
                }
            }
        } elseif (isset($check_access_user) AND $check_access_user == 'on') {
            if ($access_user_from < $access_user_to) {
                $sql = 'UPDATE `' . ROLE_TABLE . '` SET '
                        . '`' . ROLE_FROM . '`="' . $access_user_from . '", '
                        . '`' . ROLE_TO . '`="' . $access_user_to . '" '
                        . 'WHERE `' . ROLE_ID . '`="' . $user_role_id . '"';
                $mysqliLink->query($sql);
                logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_USER] . ') jogosultáságnak módosítása. Klub: ' . $selected_club . ', from:' . $access_user_from . ', to: ' . $access_user_to);
            }
        } elseif (isset($user_role_id)) {
            $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $user_role_id . '"';
            $mysqliLink->query($sql);
            logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_USER] . ') jogosultáságnak törlése. Klub: ' . $selected_club);
        }



        if ($clubleader_role_id == -1) {
            if (isset($check_access_clubleader) AND $check_access_clubleader == 'on') {
                if ($access_clubleader_from < $access_clubleader_to) {
                    $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                            . ROLE_MEMBER . '`, `' . ROLE_CLUB
                            . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                            . '`) VALUES ("'
                            . $role_member_id . '", "' . $selected_club . '", "'
                            . ACCESS_CLUBLEADER . '", "' . $access_clubleader_from . '", "' . $access_clubleader_to . '")';
                    $mysqliLink->query($sql);
                    logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUBLEADER] . ') jogosultáságnak felvitele. Klub:.' . $selected_club . ', from:' . $access_clubleader_from . ', to: ' . $access_clubleader_to);
                }
            }
        } elseif (isset($check_access_clubleader) AND $check_access_clubleader == 'on') {
            if ($access_clubleader_from < $access_clubleader_to) {
                $sql = 'UPDATE `' . ROLE_TABLE . '` SET '
                        . '`' . ROLE_FROM . '`="' . $access_clubleader_from . '", '
                        . '`' . ROLE_TO . '`="' . $access_clubleader_to . '" '
                        . 'WHERE `' . ROLE_ID . '`="' . $clubleader_role_id . '"';
                $mysqliLink->query($sql);
                logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUBLEADER] . ') jogosultáságnak módosítása. Klub: ' . $selected_club . ', from:' . $access_clubleader_from . ', to: ' . $access_clubleader_to);
            }
        } elseif (isset($clubleader_role_id)) {
            $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $clubleader_role_id . '"';
            $mysqliLink->query($sql);
            logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUBLEADER] . ') jogosultáságnak törlése. Klub: ' . $selected_club);
        }


        if ($clubpresident_role_id == -1) {
            if (isset($check_access_clubpresident) AND $check_access_clubpresident == 'on') {
                if ($access_clubpresident_from < $access_clubpresident_to) {
                    $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                            . ROLE_MEMBER . '`, `' . ROLE_CLUB
                            . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                            . '`) VALUES ("'
                            . $role_member_id . '", "' . $selected_club . '", "'
                            . ACCESS_CLUB_PRESIDENT . '", "' . $access_clubpresident_from . '", "' . $access_clubpresident_to . '")';
                    $mysqliLink->query($sql);
                    logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUB_PRESIDENT] . ') jogosultáságnak felvitele. Klub:.' . $selected_club . ', from:' . $access_clubpresident_from . ', to: ' . $access_clubpresident_to);
                }
            }
        } elseif (isset($check_access_clubpresident) AND $check_access_clubpresident == 'on') {
            if ($access_clubpresident_from < $access_clubpresident_to) {
                $sql = 'UPDATE `' . ROLE_TABLE . '` SET '
                        . '`' . ROLE_FROM . '`="' . $access_clubpresident_from . '", '
                        . '`' . ROLE_TO . '`="' . $access_clubpresident_to . '" '
                        . 'WHERE `' . ROLE_ID . '`="' . $clubpresident_role_id . '"';
                $mysqliLink->query($sql);
                logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUB_PRESIDENT] . ') jogosultáságnak módosítása. Klub: ' . $selected_club . ', from:' . $access_clubpresident_from . ', to: ' . $access_clubpresident_to);
            }
        } elseif (isset($clubpresident_role_id)) {
            $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $clubpresident_role_id . '"';
            $mysqliLink->query($sql);
            logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUB_PRESIDENT] . ') jogosultáságnak törlése. Klub: ' . $selected_club);
        }


        if ($clubdelegate_role_id == -1) {
            if (isset($check_access_clubdelegate) AND $check_access_clubdelegate == 'on') {
                if ($access_clubdelegate_from < $access_clubdelegate_to) {
                    $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                            . ROLE_MEMBER . '`, `' . ROLE_CLUB
                            . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                            . '`) VALUES ("'
                            . $role_member_id . '", "' . $selected_club . '", "'
                            . ACCESS_CLUB_DELEGATE . '", "' . $access_clubdelegate_from . '", "' . $access_clubdelegate_to . '")';
                    $mysqliLink->query($sql);
                    logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUB_DELEGATE] . ') jogosultáságnak felvitele. Klub:.' . $selected_club . ', from:' . $access_clubdelegate_from . ', to: ' . $access_clubdelegate_to);
                }
            }
        } elseif (isset($check_access_clubdelegate) AND $check_access_clubdelegate == 'on') {
            if ($access_clubdelegate_from < $access_clubdelegate_to) {
                $sql = 'UPDATE `' . ROLE_TABLE . '` SET '
                        . '`' . ROLE_FROM . '`="' . $access_clubdelegate_from . '", '
                        . '`' . ROLE_TO . '`="' . $access_clubdelegate_to . '" '
                        . 'WHERE `' . ROLE_ID . '`="' . $clubdelegate_role_id . '"';
                $mysqliLink->query($sql);
                logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUB_DELEGATE] . ') jogosultáságnak módosítása. Klub: ' . $selected_club . ', from:' . $access_clubdelegate_from . ', to: ' . $access_clubdelegate_to);
            }
        } elseif (isset($clubdelegate_role_id)) {
            $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $clubdelegate_role_id . '"';
            $mysqliLink->query($sql);
            logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_CLUB_DELEGATE] . ') jogosultáságnak törlése. Klub: ' . $selected_club);
        }
    }
    reloadEditMember();
}

$roles = getRoles($role_member_id);
$output .= '<div class="keret">';
$output .= '<form  method="post" enctype="multipart/form-data">';

if (isControllable(ACCESS_MMSZ)) {
    $role_on = false;
    $role_from = $beginingOfYear;
    $role_to = $endOfYear;
    $role_id = -1;
    foreach ($roles as $r) {
        if (($r[2] & ACCESS_MMSZ) > 0) {
            $role_on = true;
            $role_from = $r[3];
            $role_to = $r[4];
            $role_id = $r[0];
            break;
        }
    }
    $output .= '<label></label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Kezdet'
            . '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Vég<br>';
    $output .= '<label>' . $access[ACCESS_MMSZ] . '</label>';
    $output .= '<input type="checkbox" name="check_access_mmsz" ' . ($role_on ? 'checked' : '') . '>'
            . '<input type="date" name="access_mmsz_from" size="10" value="' . $role_from . '">'
            . '<input type="date" name="access_mmsz_to" size="10" value="' . $role_to . '">'
            . '<input type="hidden" name="mmsz_role_id" value="' . $role_id . '">';
    if ($role_on AND $today > $role_to) {
        $output .= '&nbsp;&nbsp;&nbsp;<font color="red">Lejárt</font>';
    }
    $output .= '<br>';
}
if (isControllable(ACCESS_MMSZ_PRESIDENT)) {
    $role_on = false;
    $role_from = $beginingOfYear;
    $role_to = $endOfYear;
    $role_id = -1;
    foreach ($roles as $r) {
        if (($r[2] & ACCESS_MMSZ_PRESIDENT) > 0) {
            $role_on = true;
            $role_from = $r[3];
            $role_to = $r[4];
            $role_id = $r[0];
            break;
        }
    }
    $output .= '<label>' . $access[ACCESS_MMSZ_PRESIDENT] . '</label>';
    $output .= '<input type="checkbox" name="check_access_mmszpres" ' . ($role_on ? 'checked' : '') . '>'
            . '<input type="date" name="access_mmszpres_from" size="10" value="' . $role_from . '">'
            . '<input type="date" name="access_mmszpres_to" size="10" value="' . $role_to . '">'
            . '<input type="hidden" name="mmszpres_role_id" value="' . $role_id . '">';
    if ($role_on AND $today > $role_to) {
        $output .= '&nbsp;&nbsp;&nbsp;<font color="red">Lejárt</font>';
    }
    $output .= '<br>';
}
if (isControllable(ACCESS_MMSZ) || isControllable(ACCESS_MMSZ_PRESIDENT)) {
    $output .= '<hr>';
}
if (isOnlyClubleader()) {
    $selected_club = getRoleClubleader($member_id);
    if ($selected_club > 0) {
        $output .= '<label>Egyesület : </label>' . getClubName($selected_club) . '<br>';
        $output .= '<input type="hidden" name="selected_club" value="' . $selected_club . '">';
    }
} else {
    $output .= '<label>Egyesület : </label>'
            . '<select id="selected_club" name="selected_club" onchange="submit();">';
    $output .= '<option value="-1">Válassz egyesületet !</option>';
    $clubs = getClubNameList();
    $clubs_of_member = getClubsId($role_member_id);
    reset($clubs_of_member);
    $first_club = key($clubs_of_member);
    if ($selected_club < 0 && $first_club > 0) {
        $selected_club = $first_club;
    }
    foreach ($clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }
    $output .= '</select><br>';
}

$roles = getClubRoles($role_member_id, $selected_club);
if (isControllable(ACCESS_USER)) {
    $role_id = -1;
    $role_on = false;
    $role_from = $beginingOfYear;
    $role_to = $neverEnding;
    foreach ($roles as $r) {
        if (($r[2] & ACCESS_USER) > 0) {
            $role_on = true;
            $role_from = $r[3];
            $role_to = $r[4];
            $role_id = $r[0];
            break;
        }
    }
    $output .= '<label>' . $access[ACCESS_USER] . '</label>';
    $output .= '<input type="checkbox" name="check_access_user" ' . ($role_on ? 'checked' : '') . '>'
            . '<input type="date" name="access_user_from" size="10" value="' . $role_from . '">'
            . '<input type="date" name="access_user_to" size="10" value="' . $role_to . '">'
            . '<input type="hidden" name="user_role_id" value="' . $role_id . '">';
    if ($role_on AND $today > $role_to) {
        $output .= '&nbsp;&nbsp;&nbsp;<font color="red">Lejárt</font>';
    }
    $output .= '<br>';
}
if (isControllable(ACCESS_CLUBLEADER)) {
    $role_id = -1;
    $role_on = false;
    $role_from = $beginingOfYear;
    $role_to = $endOfYear;
    foreach ($roles as $r) {
        if (($r[2] & ACCESS_CLUBLEADER) > 0) {
            $role_on = true;
            $role_from = $r[3];
            $role_to = $r[4];
            $role_id = $r[0];
            break;
        }
    }
    $output .= '<label>' . $access[ACCESS_CLUBLEADER] . '</label>';
    $output .= '<input type="checkbox" name="check_access_clubleader" ' . ($role_on ? 'checked' : '') . '>'
            . '<input type="date" name="access_clubleader_from" size="10" value="' . $role_from . '">'
            . '<input type="date" name="access_clubleader_to" size="10" value="' . $role_to . '">'
            . '<input type="hidden" name="clubleader_role_id" value="' . $role_id . '">';
    if ($role_on AND $today > $role_to) {
        $output .= '&nbsp;&nbsp;&nbsp;<font color="red">Lejárt</font>';
    }
    $output .= '<br>';
}
if (isControllable(ACCESS_CLUB_PRESIDENT)) {
    $role_id = -1;
    $role_on = false;
    $role_from = $beginingOfYear;
    $role_to = $endOfYear;
    foreach ($roles as $r) {
        if (($r[2] & ACCESS_CLUB_PRESIDENT) > 0) {
            $role_on = true;
            $role_from = $r[3];
            $role_to = $r[4];
            $role_id = $r[0];
            break;
        }
    }
    $output .= '<label>' . $access[ACCESS_CLUB_PRESIDENT] . '</label>';
    $output .= '<input type="checkbox" name="check_access_clubpresident" ' . ($role_on ? 'checked' : '') . '>'
            . '<input type="date" name="access_clubpresident_from" size="10" value="' . $role_from . '">'
            . '<input type="date" name="access_clubpresident_to" size="10" value="' . $role_to . '">'
            . '<input type="hidden" name="clubpresident_role_id" value="' . $role_id . '">';
    if ($role_on AND $today > $role_to) {
        $output .= '&nbsp;&nbsp;&nbsp;<font color="red">Lejárt</font>';
    }
    $output .= '<br>';
}
if (isControllable(ACCESS_CLUB_DELEGATE)) {
    $role_id = -1;
    $role_on = false;
    $role_from = $beginingOfYear;
    $role_to = $endOfYear;
    foreach ($roles as $r) {
        if (($r[2] & ACCESS_CLUB_DELEGATE) > 0) {
            $role_on = true;
            $role_from = $r[3];
            $role_to = $r[4];
            $role_id = $r[0];
            break;
        }
    }
    $output .= '<label>' . $access[ACCESS_CLUB_DELEGATE] . '</label>';
    $output .= '<input type="checkbox" name="check_access_clubdelegate" ' . ($role_on ? 'checked' : '') . '>'
            . '<input type="date" name="access_clubdelegate_from" size="10" value="' . $role_from . '">'
            . '<input type="date" name="access_clubdelegate_to" size="10" value="' . $role_to . '">'
            . '<input type="hidden" name="clubdelegate_role_id" value="' . $role_id . '">';
    if ($role_on AND $today > $role_to) {
        $output .= '&nbsp;&nbsp;&nbsp;<font color="red">Lejárt</font>';
    }
    $output .= '<br>';
}


if (isset($selected_club)) {
    $roles = getRoles($role_member_id);
    if (key_exists($selected_club, $roles)) {
        $membership_from = $roles[$selected_club][1];
        $membership_to = $roles[$selected_club][2];
    } else {
        unset($membership_from);
        unset($membership_to);
    }
}

$output .= '<input type="submit" name="roles_save" value="Rögzítés" >'
        . '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
$output .= '</form></div>';
