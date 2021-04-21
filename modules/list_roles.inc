<?php

if (isset($modify_role)) {
    $_SESSION[SESSION_FOUND_MEMBERS] = serialize(array($modify_role));
    $_SESSION[SESSION_SELECTED_ITEM] = 0;
    $_SESSION[SESSION_DATA2EDIT] = 'roles';
    redirect('edit_member');
    return;
}
if (!isset($tab_officer)) {
    $tab_officer = 'assoc';
}
$view->putHidden('tab_officer');
$view->newElement('<ul class="nav nav-tabs">');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn navbar-btn btn-secondary ' . ($tab_officer == 'assoc' ? 'active' : '') . '" name="tab_officer" title="Szövetségi tisztségviselők" value="assoc">Szövetség</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn navbar-btn btn-secondary ' . ($tab_officer == 'branches' ? 'active' : '') . '" name="tab_officer" title="Szakágak tisztségviselői" value="branches">Szakágak</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn navbar-btn btn-secondary ' . ($tab_officer == 'clubs' ? 'active' : '') . '" name="tab_officer" title="Egyesületek tisztségviselői" value="clubs">Egyesületek</button>');
$view->endElement('</li>');
$view->endElement('</ul>');
$view->newKeret();
switch ($tab_officer) {

    default:
    case 'assoc':
        foreach (array(ACCESS_ADMIN, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT) as $ac) {
            $view->putElement('<h5>' . $access[$ac] . '</h5>');
            $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_PERMISSION . '`="' . $ac . '"';
            $sql .= ';';
            $res = $mysqliLink->query($sql);
            if ($res AND ( $res->num_rows > 0)) {
                while ($row = $res->fetch_assoc()) {
                    $view->newDiv('form-inline row p-1');
                    if ($today > $row[ROLE_FROM] AND $today < $row[ROLE_TO]) {
                        $view->newElement('<button class="btn btn-sm btn-success col-sm-2 mx-2" name="modify_role" id="modify_role" value="' . $row[ROLE_MEMBER] . '" >');
                    } else {
                        $view->newElement('<button class="btn btn-sm btn-danger col-sm-2 mx-2" name="modify_role" id="modify_role" value="' . $row[ROLE_MEMBER] . '" >');
                    }
                    $view->putElement(getMemberName($row[ROLE_MEMBER]));
                    $view->endElement('</button>');
                    $view->putElement($row[ROLE_FROM] . ' - ' . $row[ROLE_TO]);
                    $view->endDiv();
                }
            } else {
                $view->warning('Jelenleg senki nem tölt be ilyen funkciót !');
            }
        }
        break;
    case 'branches':
        foreach ($branches as $branch_id => $branch_name) {
            $view->putElement('<h5>' . $branch_name . ' </h5>');
            $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE '
                    . '(`' . ROLE_PERMISSION . '`&' . ACCESS_BRANCH_LEADER . ')>0 AND `' . ROLE_CLUB . '`=' . $branch_id . ' ;';
            $res = $mysqliLink->query($sql);
            if ($res AND ( $res->num_rows > 0)) {
                while ($row = $res->fetch_assoc()) {
                    $view->newDiv('form-inline row p-1');
                    if ($today > $row[ROLE_FROM] AND $today < $row[ROLE_TO]) {
                        $view->newElement('<button class="btn btn-sm btn-success col-sm-2 mx-2" name="modify_role" id="modify_role" value="' . $row[ROLE_MEMBER] . '" >');
                    } else {
                        $view->newElement('<button class="btn btn-sm btn-danger col-sm-2 mx-2" name="modify_role" id="modify_role" value="' . $row[ROLE_MEMBER] . '" >');
                    }
                    $view->putElement(getMemberName($row[ROLE_MEMBER]));
                    $view->endElement('</button>');
                    $view->putElement($row[ROLE_FROM] . ' - ' . $row[ROLE_TO]);
                    $view->endDiv();
                }
            } else {
                $view->warning('Nincs a szakágnak vezetője !');
            }
        }
        break;
    case 'clubs':
        if (!isset($selected_club)) {
            $selected_club = -1;
        }
        $clubs = [-1 => 'Válassz egyesületet!!!'] + getClubNameList();
        $view->submitDropDownChooser('', $clubs, 'selected_club', true);


        if ($selected_club > 0) {
            foreach (array(ACCESS_CLUBLEADER, ACCESS_CLUB_PRESIDENT, ACCESS_CLUB_DELEGATE) as $ac) {
                $view->putHidden('selected_club');
                $view->putElement('<h5>' . $access[$ac] . '</h5>');
                $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_PERMISSION . '`="' . $ac . '" '
                        . ' AND `' . ROLE_CLUB . '`=' . $selected_club . ';';
                $res = $mysqliLink->query($sql);
                if ($res AND ( $res->num_rows > 0)) {
                    while ($row = $res->fetch_assoc()) {
                        $view->newDiv('form-inline row p-1');
                        if ($today > $row[ROLE_FROM] AND $today < $row[ROLE_TO]) {
                            $view->newElement('<button class="btn btn-sm btn-success col-sm-2 mx-2" name="modify_role" id="modify_role" value="' . $row[ROLE_MEMBER] . '" >');
                        } else {
                            $view->newElement('<button class="btn btn-sm btn-danger col-sm-2 mx-2" name="modify_role" id="modify_role" value="' . $row[ROLE_MEMBER] . '" >');
                        }
                        $view->putElement(getMemberName($row[ROLE_MEMBER]));
                        $view->endElement('</button>');
                        $view->putElement($row[ROLE_FROM] . ' - ' . $row[ROLE_TO]);
                        $view->endDiv();
                    }
                } else {
                    $view->warning('Jelenleg senki nem tölt be ilyen funkciót !');
                }
            }
        }
        break;
}
$view->endKeret();

