
<?php

define("VIEW_BORN", 1);
define("VIEW_CATEGORY", 2);
define("VIEW_EMAIL", 3);
define("VIEW_ADDR", 4);
define("VIEW_LICENCE", 5);
define("VIEW_CLUB", 6);
define("VIEW_FEES", 7);
define("VIEW_ROLES", 8);

$all_columns = array(
    VIEW_BORN => "Született",
    VIEW_CATEGORY => "Kategóriák",
    VIEW_EMAIL => "Email cím",
    VIEW_ADDR => "Cím",
    VIEW_LICENCE => "Licensz",
    VIEW_CLUB => "Egyesület",
    VIEW_FEES => "Díjak",
    VIEW_ROLES => "Szerepek",
);

$isSetView = false;
$view_columns = array();
foreach ($all_columns as $view_chkindex => $view_chkname) {
    $chk = 'view_' . $view_chkindex;
    if (isset($$chk) AND $ $chk == 'on') {
        $isSetView = true;
        $view_columns[] = $view_chkindex;
    }
}
if (!$isSetView) {
    if (isset($_SESSION[SESSION_VIEW_COLUMNS])) {
        $view_columns = unserialize($_SESSION[SESSION_VIEW_COLUMNS]);
    } else {
        $view_columns = array(1, 2, 3);
    }
}
$_SESSION[SESSION_VIEW_COLUMNS] = serialize($view_columns);

if (isset($delete_member)) {
    $mysqliLink->query('DELETE FROM `' . ROLE_TABLE . '` WHERE `' . ROLE_MEMBER . '`=' . $delete_member);
    $mysqliLink->query('DELETE FROM `' . FM_TABLE . '` WHERE `' . FM_MEMEBER_ID . '`=' . $delete_member);
    logger($member_id, $delete_member, LOGGER_MEMBER_MOD, $delete_member . ' azonosítójú tag törlése.');
}
if (isset($selected_item)) {
    $_SESSION[SESSION_SELECTED_ITEM] = $selected_item;
    $_SESSION[SESSION_MOD] = $admin_menu["edit_member"][SETUP_MOD_ID];
    include_once (MODULES . $admin_menu["edit_member"][SETUP_MOD_MODULE]);
    return;
}
include_once INCLUDES . 'search_members.inc';
unset($_SESSION[SESSION_FOUND_MEMBERS]);
unset($found_member_ids);
if ($total_member > 0) {
    $output .= '<div class="lists"> <form id="members" method="post"> '
            . '<table border="1"><caption>Tagok listázása</caption>';
    $output .= '<tr><th>Sorszám</th><th>Név</th>';
    foreach ($view_columns as $view_index) {
        $output .= '<th>' . $all_columns[$view_index] . '</th>';
    }
    $output .= '<th>Művelet</th></tr>';
    $found_member_ids = array();
    $edited_item = 0;
    while ($res AND $row = $res->fetch_assoc()) {
        $found_member_ids[$edited_item++] = $row[MEMBER_ID];
    }
    $_SESSION[SESSION_FOUND_MEMBERS] = serialize($found_member_ids);
    $res->data_seek($listed_member);
    $listed_item = PAGE_MEMBERS;
    $edited_item = 0;
    while ($res AND $row = $res->fetch_assoc()) {
        $listed_item--;
        if ($listed_item < 0) {
            break;
        }
        $output .= '<tr>';
        $paid = '';
        if (isPaidMembership($row[MEMBER_ID])) {
            $paid = '<image src="' . IMAGES . 'ok.jpg">';
        }
        $output .= '<td>' . $row[MEMBER_ID] . $paid . '</td>';
        $output .= '<td>' . $row[MEMBER_VEZNEV] . " " . $row[MEMBER_KERNEV] . "</td>";
        foreach ($view_columns as $view_index) {
            switch ($view_index) {
                case VIEW_BORN:
                    $output .= '<td>' . $row[MEMBER_BORN] . "</td>";
                    break;
                case VIEW_CATEGORY:
                    $output .= "<td>" . getListMemberCategories($row[MEMBER_ID]) . "</td>";
                    break;
                case VIEW_EMAIL:
                    $output .= "<td>" . $row[MEMBER_EMAIL] . "</td>";
                    break;
                case VIEW_ADDR:
                    $output .= "<td>" . $row[MEMBER_ADDR_POST] . ' ' . $row[MEMBER_ADDR_CITY] . ', ' . $row[MEMBER_ADDR_STREET] . "</td>";
                    break;
                case VIEW_LICENCE:
                    $output .= "<td>" . $row[MEMBER_LICENCE] . "</td>";
                    break;
                case VIEW_CLUB:
                    $output .= "<td>" . getMemberClubNames($row[MEMBER_ID]) . "</td>";
                    break;
                case VIEW_FEES:
                    $output .= "<td>" . printFmOfMember($row[MEMBER_ID]) . "</td>";
                    break;
                case VIEW_ROLES:
                    $output .= "<td>" . printPermissions($row[MEMBER_ID]) . "</td>";
                    break;
            }
        }
        $output .= '<td>';
        if (isDeletableMember($row[MEMBER_ID]) && ($permission & (ACCESS_ADMIN | ACCESS_MMSZ | ACCESS_CLUBLEADER)) != 0) {
            $output .= '<button  type="submit" name="delete_member" value="' . $row[MEMBER_ID] . '" onClick="return confirm(\'Biztosan tölni akarod a ' . $row[MEMBER_ID] . ' azonosítójú tagot?\')"><img src="' . IMAGES . 'delete.jpg" title="Törlés"></button>';
        }
        $output .= '<button  type="submit" name="selected_item" value="' . ($edited_item + $listed_member) . '"><img src="' . IMAGES . 'edit.jpg" title="Szerkesztés"></button>';
        $output .= '</td>';
        $output .= "</tr>";
        $edited_item++;
    }
    $output .= "</table></form></div>";
} else {
    warning("Nincs találat !");
}