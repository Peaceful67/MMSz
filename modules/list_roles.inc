<?php

if (isset($modify_role)) {
    $_SESSION[SESSION_FOUND_MEMBERS] = serialize(array($modify_role));
    $_SESSION[SESSION_SELECTED_ITEM] = 0;
    $_SESSION[SESSION_DATA2EDIT] = 'roles';
    redirect('edit_member');
    return;
}

$output .= '<form method="post">';
$output .= '<div class="editor"><div class="keret">';
foreach (array(ACCESS_ADMIN, ACCESS_MMSZ, ACCESS_MMSZ_PRESIDENT) as $ac) {
    $first_line = true;
    $output .= '<b><label>' . $access[$ac] . ': </label></b>';
    $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_PERMISSION . '`="' . $ac . '"';
    $sql .= ';';
    $res = $mysqliLink->query($sql);
    if ($res AND ( $res->num_rows > 0)) {
        while ($row = $res->fetch_assoc()) {
            if ($first_line) {
                $first_line = false;
            } else {
                $output .= '<label> </label>';
            }
            if ($today > $row[ROLE_FROM] AND $today < $row[ROLE_TO]) {
                $output .= '<button style="color:green";';
            } else {
                $output .= '<button style="color:red";';
            }
            $output .= ' name="modify_role" value="' . $row[ROLE_MEMBER] . '" >' . getMemberName($row[ROLE_MEMBER]);
            $output .= '</button>&nbsp;&nbsp;';
            $output .= $row[ROLE_FROM] . ' - ' . $row[ROLE_TO];
            $output .= '<br>';
        }
    } else {
        $output .= 'Jelenleg senki nem tölt be ilyen funkciót !';
    }
    $output .= '<br>';
}
$output .= '</div>';
$output .= '<div class="keret">';
$output .= '<b>Szakágak:</b><br>';
foreach ($branches as $branch_id => $branch_name) {
    $output .= '<b><label>' . $branch_name . ': </label></b>';
    $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE '
            . '`' . ROLE_PERMISSION . '`=' . ACCESS_CATEGORY_LEADER . ' AND `' . ROLE_CLUB . '`=' . $branch_id . ' ;';
    $res = $mysqliLink->query($sql);
    if ($res AND ( $res->num_rows > 0)) {
        while ($row = $res->fetch_assoc()) {
            $first_line = true;
            if ($first_line) {
                $first_line = false;
            } else {
                $output .= '<label> </label>';
            }
            if ($today > $row[ROLE_FROM] AND $today < $row[ROLE_TO]) {
                $output .= '<button style="color:green";';
            } else {
                $output .= '<button style="color:red";';
            }
            $output .= ' name="modify_role" value="' . $row[ROLE_MEMBER] . '" >' . getMemberName($row[ROLE_MEMBER]);
            $output .= '</button>&nbsp;&nbsp;';
            $output .= $row[ROLE_FROM] . ' - ' . $row[ROLE_TO];
            $output .= '<br>';
        }
    } else {
        $output .= 'Nincsa a szakágnak vezetője !';
    }
    $output .= '<br>';
}
$output .= '</div>';
if (!isset($selected_club)) {
    $selected_club = 0;
}
$output .= '<div class="keret"><br>';
$output .= 'Egyesület: <select  name="selected_club" onChange="submit();">';
$output .= '<option value="0">Válassz egyesületet !</option>';
$clubs = getClubNameList();
foreach ($clubs as $club_id => $club_name) {
    $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
}
$output .= '</select>';


if ($selected_club > 0) {
    foreach (array(ACCESS_CLUBLEADER, ACCESS_CLUB_PRESIDENT, ACCESS_CLUB_DELEGATE) as $ac) {
        $first_line = true;
        $output .= '<br><b><label>' . $access[$ac] . ': </label></b>';
        $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_PERMISSION . '`="' . $ac . '" '
                . ' AND `' . ROLE_CLUB . '`=' . $selected_club . ';';
        $res = $mysqliLink->query($sql);
        if ($res AND ( $res->num_rows > 0)) {
            while ($row = $res->fetch_assoc()) {
                if ($first_line) {
                    $first_line = false;
                } else {
                    $output .= '<br><label> </label>';
                }
                if ($today > $row[ROLE_FROM] AND $today < $row[ROLE_TO]) {
                    $output .= '<button style="color:green";';
                } else {
                    $output .= '<button style="color:red";';
                }
                $output .= ' name="modify_role" value="' . $row[ROLE_MEMBER] . '" >' . getMemberName($row[ROLE_MEMBER]);
                $output .= '</button>&nbsp;&nbsp;';
                $output .= $row[ROLE_FROM] . ' - ' . $row[ROLE_TO];
            }
        } else {
            $output .= 'Jelenleg senki nem tölt be ilyen funkciót !';
        }
    }
    $output .= '<br>';
}
$output .= '</div><br>';

$output .= '</div>';
$output .= '</form>';

