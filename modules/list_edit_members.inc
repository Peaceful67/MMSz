
<?php

define("VIEW_BORN", 1);
define("VIEW_CATEGORY", 2);
define("VIEW_EMAIL", 3);
define("VIEW_ADDR", 4);
define("VIEW_LICENCE", 5);
define("VIEW_CLUB", 6);
define("VIEW_FEES", 7);
define("VIEW_ROLES", 8);
define("VIEW_GDPR", 9);

$all_columns = array(
    VIEW_BORN => "Született",
    VIEW_CATEGORY => "Kategóriák",
    VIEW_EMAIL => "Email cím",
    VIEW_ADDR => "Cím",
    VIEW_LICENCE => "Licensz",
    VIEW_CLUB => "Egyesület",
    VIEW_FEES => "Díjak",
    VIEW_ROLES => "Szerepek",
    VIEW_GDPR => "GDPR",
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
    deleteMember($delete_member);
}
if (isset($selected_item)) {
    if (isset($_SESSION[SESSION_SEARCH_VALUES])
            AND isset($_SESSION[SESSION_SEARCH_VALUES])
            AND isset($_SESSION[SESSION_SEARCH_VALUES][SEARCH_SELECTED_CLUB])
            AND $_SESSION[SESSION_SEARCH_VALUES][SEARCH_SELECTED_CLUB] > 0) {
        $_SESSION[SESSION_SELECTED_CLUB] = $_SESSION[SESSION_SEARCH_VALUES][SEARCH_SELECTED_CLUB];
    }
    $_SESSION[SESSION_SELECTED_ITEM] = $selected_item;
    $_SESSION[SESSION_MOD] = $admin_menu["edit_member"][SETUP_MOD_ID];
    include_once (MODULES . $admin_menu["edit_member"][SETUP_MOD_MODULE]);
    return;
}
include_once INCLUDES . 'search_members.inc';
unset($_SESSION[SESSION_FOUND_MEMBERS]);
unset($found_member_ids);
if ($total_member > 0) {
    $view->newDiv('table-responsive-sm');
    $view->newElement('<table class="table table-bordered table-hover table-sm p-1"><caption>Tagok listázása</caption>');
    $view->newElement('<thead>');
    $view->newElement('<tr>');
    $view->putElement('<th scope="col">Sorszám</th><th scope="col">Név</th>');
    foreach ($view_columns as $view_index) {
        $view->putElement('<th scope="col">' . $all_columns[$view_index] . '</th>');
    }
    $view->putElement('<th scope="col">Művelet</th>');
    $view->endElement('</tr>');
    $view->endElement('</thead>');
    $view->newElement('<tbody>');
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
        $view->newElement('<tr' . (isPaidMembership($row[MEMBER_ID]) ? ' class="table-success"' : '') . '>');
        $view->putElement('<th scope="row">' . $row[MEMBER_ID] . '</th>');
        $view->putElement('<td>' . $row[MEMBER_VEZNEV] . " " . $row[MEMBER_KERNEV] . '</td>');
        foreach ($view_columns as $view_index) {
            switch ($view_index) {
                case VIEW_BORN:
                    $view->putElement('<td>' . $row[MEMBER_BORN] . '</td>');
                    break;
                case VIEW_CATEGORY:
                    $view->putElement("<td>" . getListMemberCategories($row[MEMBER_ID]) . "</td>");
                    break;
                case VIEW_EMAIL:
                    $view->putElement("<td>" . $row[MEMBER_EMAIL] . "</td>");
                    break;
                case VIEW_ADDR:
                    $view->putElement("<td>" . $row[MEMBER_ADDR_POST] . ' ' . $row[MEMBER_ADDR_CITY] . ', ' . $row[MEMBER_ADDR_STREET] . "</td>");
                    break;
                case VIEW_LICENCE:
                    $view->putElement("<td>" . $row[MEMBER_LICENCE] . "</td>");
                    break;
                case VIEW_CLUB:
                    $view->putElement("<td>" . getMemberClubNames($row[MEMBER_ID]) . "</td>");
                    break;
                case VIEW_FEES:
                    $view->putElement("<td>" . printFmOfMember($row[MEMBER_ID]) . "</td>");
                    break;
                case VIEW_ROLES:
                    $view->putElement("<td>" . printRoles($row[MEMBER_ID]) . "</td>");
                    break;
                case VIEW_GDPR:
                    $view->putElement("<td>" . printGDPR($row[MEMBER_GDPR]) . "</td>");
                    break;
            }
        }
        $view->newElement('<td>');
        if (isDeletableMember($row[MEMBER_ID]) && ($permission & (ACCESS_ADMIN | ACCESS_MMSZ | ACCESS_CLUBLEADER)) != 0) {
            $view->submitDelete('delete_member', $row[MEMBER_ID], 'Törlés', 'Biztosan tölni akarod a ' . $row[MEMBER_ID] . ' azonosítójú tagot?');
        }
        $view->submitEdit('selected_item', ($edited_item + $listed_member));
        $view->endElement('</td>');
        $view->endElement('</tr>');
        $edited_item++;
    }
    $view->endElement('</tbody>');
    $view->endElement('</table>');
    $view->endDiv();
} else {
    $view->warning('Nincs találat !');
}