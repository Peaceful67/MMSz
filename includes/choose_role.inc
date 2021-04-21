<?php

if (!isset($selected_club)) {
    $selected_club = -1;
}
if ($selected_club < 0 AND isset($_SESSION[SESSION_SELECTED_CLUB])) {
    $selected_club = $_SESSION[SESSION_SELECTED_CLUB];
}
if (!isset($selected_branch)) {
    $selected_branch = -1;
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

    if ($selected_branch >= 0) {

        if (isset($catleader_role_id) AND $catleader_role_id == -1) {
            if (isset($check_access_catleader) AND $check_access_catleader == 'on') {
                $catleader_permission = ACCESS_BRANCH_LEADER;
                $catleader_permission |= (isset($check_access_new) AND $check_access_new == 'on') ? ACCESS_MANAGE_NEW : 0;
                $catleader_permission |= (isset($check_access_edit) AND $check_access_edit == 'on') ? ACCESS_MANAGE_EDIT : 0;
                $catleader_permission |= (isset($check_access_fai) AND $check_access_fai == 'on') ? ACCESS_MANAGE_FAI : 0;
                $catleader_permission |= (isset($check_access_all) AND $check_access_all == 'on') ? ACCESS_MANAGE_ALL : 0;
                if ($access_catleader_from < $access_catleader_to) {
                    $sql = 'INSERT INTO `' . ROLE_TABLE . '` (`'
                            . ROLE_MEMBER . '`, `' . ROLE_CLUB
                            . '`, `' . ROLE_PERMISSION . '`, `' . ROLE_FROM . '`, `' . ROLE_TO
                            . '`) VALUES ("'
                            . $role_member_id . '", "' . $selected_branch . '", "'
                            . $catleader_permission . '", "' . $access_catleader_from . '", "' . $access_catleader_to . '")';
                    $mysqliLink->query($sql);
                    logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_BRANCH_LEADER] . ') jogosultáságnak felvitele. Szakág:.' . $selected_branch . ', from:' . $access_catleader_from . ', to: ' . $access_catleader_to);
                }
            }
        } elseif (isset($check_access_catleader) AND $check_access_catleader == 'on') {
            if ($access_catleader_from < $access_catleader_to) {
                $catleader_permission = ACCESS_BRANCH_LEADER;
                $catleader_permission |= (isset($check_access_new) AND $check_access_new == 'on') ? ACCESS_MANAGE_NEW : 0;
                $catleader_permission |= (isset($check_access_edit) AND $check_access_edit == 'on') ? ACCESS_MANAGE_EDIT : 0;
                $catleader_permission |= (isset($check_access_fai) AND $check_access_fai == 'on') ? ACCESS_MANAGE_FAI : 0;
                $catleader_permission |= (isset($check_access_all) AND $check_access_all == 'on') ? ACCESS_MANAGE_ALL : 0;
                $sql = 'UPDATE `' . ROLE_TABLE . '` SET '
                        . '`' . ROLE_PERMISSION . '`=' . $catleader_permission . ', '
                        . '`' . ROLE_FROM . '`="' . $access_catleader_from . '", '
                        . '`' . ROLE_TO . '`="' . $access_catleader_to . '" '
                        . 'WHERE `' . ROLE_ID . '`=' . $catleader_role_id . ';';
                $mysqliLink->query($sql);
                logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_BRANCH_LEADER] . ') jogosultáságnak módosítása. Szakág: ' . $selected_branch . ', from:' . $access_catleader_from . ', to: ' . $access_catleader_to);
            }
        } elseif (isset($catleader_role_id)) {
            $sql = 'DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_ID . '`="' . $catleader_role_id . '"';
            $mysqliLink->query($sql);
            logger($member_id, $role_member_id, LOGGER_MEMBER_MOD, $role_member_id . ' azonosítójú felhasználó (' . $access[ACCESS_BRANCH_LEADER] . ') jogosultáságnak törlése. Szakág: ' . $selected_branch);
        }
    }

    if ($selected_club > 0) {

        if (isset($user_role_id) AND $user_role_id == -1) {
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



        if (isset($clubleader_role_id) AND $clubleader_role_id == -1) {
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


        if (isset($clubpresident_role_id) AND $clubpresident_role_id == -1) {
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


        if (isset($clubdelegate_role_id) AND $clubdelegate_role_id == -1) {
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
$view->newKeret()->putPrevent();

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
    $mmsz_role_id = $role_id;
    $check_access_mmsz = $role_on ? 'on' : 'off';
    $access_mmsz_from = $role_from;
    $access_mmsz_to = $role_to;
    $view->inputRole('check_access_mmsz', 'access_mmsz_from', 'access_mmsz_to', $access[ACCESS_MMSZ]);
    $view->putHidden('mmsz_role_id');
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
    $mmszpres_role_id = $role_id;
    $check_access_mmszpres = $role_on ? 'on' : 'off';
    $access_mmszpres_from = $role_from;
    $access_mmszpres_to = $role_to;
    $view->inputRole('check_access_mmszpres', 'access_mmszpres_from', 'access_mmszpres_to', $access[ACCESS_MMSZ_PRESIDENT]);
    $view->putHidden('mmszpres_role_id');
}
if (isControllable(ACCESS_BRANCH_LEADER)) {
    $roles = getBranchRoles($role_member_id, $selected_branch);
    if ($selected_branch < 0) {
        foreach ($roles as $r) {
            if (($r[2] & ACCESS_BRANCH_LEADER) > 0) {
                $selected_branch = $r[1];
                break;
            }
        }
    }
    $view->putElement('<hr/>');
    $view->submitDropDownChooser('Szakág', [-1 => 'Válassz!!!'] + $branches, 'selected_branch', true);
    if ($selected_branch >= 0) {
        $role_on = false;
        $role_new = false;
        $role_edit = false;
        $role_fai = false;
        $role_all = false;
        $role_from = $beginingOfYear;
        $role_to = $endOfYear;
        $role_id = -1;
        foreach ($roles as $r) {
            if ((($r[2] & ACCESS_BRANCH_LEADER) > 0) AND $selected_branch == $r[1]) {
                $role_on = true;
                $role_new = ($r[2] & ACCESS_MANAGE_NEW) > 0 ? true : false;
                $role_edit = ($r[2] & ACCESS_MANAGE_EDIT) > 0 ? true : false;
                $role_fai = ($r[2] & ACCESS_MANAGE_FAI) > 0 ? true : false;
                $role_all = ($r[2] & ACCESS_MANAGE_ALL) > 0 ? true : false;
                $role_from = $r[3];
                $role_to = $r[4];
                $role_id = $r[0];
                break;
            }
        }
        $catleader_role_id = $role_id;
        $check_access_catleader = $role_on ? 'on' : 'off';
        $access_catleader_from = $role_from;
        $access_catleader_to = $role_to;
        $view->inputRole('check_access_catleader', 'access_catleader_from', 'access_catleader_to', $access[ACCESS_BRANCH_LEADER]);
        $check_access_new = $role_new ? 'on' : 'off';
        $view->inputCheckbox('check_access_new', $access[ACCESS_MANAGE_NEW]);
        $check_access_edit = $role_edit ? 'on' : 'off';
        $view->inputCheckbox('check_access_edit', $access[ACCESS_MANAGE_EDIT]);
        if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) > 0) {
            $check_access_fai = $role_fai ? 'on' : 'off';
            $view->inputCheckbox('check_access_fai', $access[ACCESS_MANAGE_FAI]);
        }
        $check_access_all = $role_all ? 'on' : 'off';
        $view->inputCheckbox('check_access_all', $access[ACCESS_MANAGE_ALL]);
        $view->putHidden('catleader_role_id');
    }
    $view->putElement('<hr/>');
}
$clubs = getClubNameList();
$only_club_leader = isOnlyClubleader();
if ($only_club_leader) {
    $clubs = getClubsOfLeader($member_id);
}
if (1 == count($clubs)) {
    reset($clubs);
    $selected_club = key($clubs);
    if ($selected_club > 0) {
        $view->putText(getClubName($selected_club), 'Egyesület');
        $view->putHidden('selected_club');
    }
} else {
    $clubs_of_member = getClubsId($role_member_id);
    reset($clubs_of_member);
    $first_club = key($clubs_of_member);
    if ($selected_club < 0 && $first_club > 0) {
        $selected_club = $first_club;
    }
    $view->inputClub('Egyesület', [-1 => 'Válassz!!!'] + $clubs, 'selected_club', true);
}

$role_clubs = getClubsMember($role_member_id);
if (!empty($role_clubs)) {
    $view->newElement('(');
    foreach ($role_clubs as $role_club_name) {
        $view->putElement($role_club_name . ', ');
    }
    $view->endElement(')');
}

$_SESSION[SESSION_SELECTED_CLUB] = $selected_club;
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
    $user_role_id = $role_id;
    $check_access_user = $role_on ? 'on' : 'off';
    $access_user_from = $role_from;
    $access_user_to = $role_to;
    $view->inputRole('check_access_user', 'access_user_from', 'access_user_to', $access[ACCESS_USER]);
    $view->putHidden('user_role_id');
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
    $clubleader_role_id = $role_id;
    $check_access_clubleader = $role_on ? 'on' : 'off';
    $access_clubleader_from = $role_from;
    $access_clubleader_to = $role_to;
    $view->inputRole('check_access_clubleader', 'access_clubleader_from', 'access_clubleader_to', $access[ACCESS_CLUBLEADER]);
    $view->putHidden('clubleader_role_id');
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
    $clubpresident_role_id = $role_id;
    $check_access_clubpresident = $role_on ? 'on' : 'off';
    $access_clubpresident_from = $role_from;
    $access_clubpresident_to = $role_to;
    $view->inputRole('check_access_clubpresident', 'access_clubpresident_from', 'access_clubpresident_to', $access[ACCESS_CLUB_PRESIDENT]);
    $view->putHidden('clubpresident_role_id');
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
    $clubdelegate_role_id = $role_id;
    $check_access_clubdelegate = $role_on ? 'on' : 'off';
    $access_clubdelegate_from = $role_from;
    $access_clubdelegate_to = $role_to;
    $view->inputRole('check_access_clubdelegate', 'access_clubpresident_from', 'access_clubdelegate_to', $access[ACCESS_CLUB_DELEGATE]);
    $view->putHidden('clubdelegate_role_id');
    /*
      $mandat = getMandatOfClub($selected_club);
      if ($mandat > 0) {
      output_spaces(5);
      o('Mandátum:' . $mandat);
      }
     * 
     */
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
$view->submitButton('roles_save', 'Rögzítés');
$view->putHidden('edit_member');
$view->endKeret();
