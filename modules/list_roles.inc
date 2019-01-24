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
$output .= '<h2>Egyesületek: </h2>';
$clubs = getClubNameList();
foreach ($clubs as $club_id => $club_name) {
    $output .= '<div class="keret"><b>' . $club_name . '</b><br>';
    foreach (array(ACCESS_CLUBLEADER, ACCESS_CLUB_PRESIDENT, ACCESS_CLUB_DELEGATE) as $ac) {
        $first_line = true;
        $output .= '<b><label>' . $access[$ac] . ': </label></b>';
        $sql = 'SELECT * FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_PERMISSION . '`="' . $ac . '" '
                . ' AND `' . ROLE_CLUB . '`=' . $club_id . ';';
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
            $output .= 'Jelenleg senki nem tölt be ilyen funkciót !<br>';
        }
    }
    $output .= '</div><br>';
}
$output .= '</div>';
$output .= '</form>';

