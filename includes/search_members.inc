
<?php

define("PAID_ONLY", 1);
define("PAID_NOT", 2);
define("PAID_ALL", 3);

define("ACTIVE_ONLY", 4);
define("ACTIVE_NOT", 5);
define("ACTIVE_ALL", 6);
define("ACTIVE_DELETED", 7);

define("LICENCE_WITH", 10);
define("LICENCE_WITHOUT", 11);
define("LICENCE_ALL", 12);


define("SEARCH_FEE_YEAR", 'search_fee_year');
define("SEARCH_FEE_TYPE", 'search_fee_type');
define("SEARCH_FEE_NAME", 'search_fee_name');
define("SEARCH_FEE_CLUB", 'search_fee_club');
define("SEARCH_SELECTED_BRANCH", 'search_branch');

if (!isset($searchByFees)) {
    $searchByFees = false;
}

if (isset($_SESSION[SESSION_SEARCH_VALUES])) {
    $search_values = unserialize($_SESSION[SESSION_SEARCH_VALUES]);
    $reload = true;
} else {
    $reload = false;
}

if (!isset($listed_member)) {
    $listed_member = $reload ? $search_values[SEARCH_LISTED_MEMBER] : 0;
}

if (!isset($search)) {
    $search = $reload ? $search_values[SEARCH_SEARCH] : '';
} else {
    isNewSearch($search, SEARCH_SEARCH);
}
if (!isset($selected_fee_id)) {
    $selected_fee_id = -1;
}

if (!isset($selected_active)) {
    $selected_active = $reload ? $search_values[SEARCH_SELECTED_ACTIVE] : ACTIVE_ONLY;
} else {
    isNewSearch($selected_active, SEARCH_SELECTED_ACTIVE);
}


if (!isset($selected_paid)) {
    $selected_paid = $reload ? $search_values[SEARCH_SELECTED_PAID] : PAID_ALL;
} else {
    isNewSearch($selected_paid, SEARCH_SELECTED_PAID);
}

if (!isset($selected_licence)) {
    $selected_licence = $reload ? $search_values[SEARCH_SELECTED_LICENCE] : LICENCE_ALL;
} else {
    isNewSearch($selected_licence, SEARCH_SELECTED_LICENCE);
}

if (!isset($search_by_id)) {
    $search_by_id = $reload ? $search_values[SEARCH_SEARCH_BY_ID] : '';
} else {
    isNewSearch($search_by_id, SEARCH_SEARCH_BY_ID);
}

if (!isset($order_by)) {
    $order_by = $reload ? $search_values[SEARCH_ORDER_BY] : MEMBER_VEZNEV;
} else {
    isNewSearch($order_by, SEARCH_ORDER_BY);
}
if (!isset($selected_club)) {
    $selected_club = $reload ? $search_values[SEARCH_SELECTED_CLUB] :
            ((isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) ?
            -1 : isClubLeader());
} else {
    isNewSearch($selected_club, SEARCH_SELECTED_CLUB);
}
if (!isset($selected_branch)) {
    $selected_branch = $reload ? $search_values[SEARCH_SELECTED_BRANCH] :
            ((isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) ?
            -1 : isBranchLeader());
} else {
    isNewSearch($selected_branch, SEARCH_SELECTED_BRANCH);
}
if (!isset($search_fee_year)) {
    $search_fee_year = $reload ? $search_values[SEARCH_FEE_YEAR] : $this_year;
} else {
    isNewSearch($search_fee_year, SEARCH_FEE_YEAR);
}
if (!isset($search_fee_type)) {
    $search_fee_type = $reload ? $search_values[SEARCH_FEE_TYPE] : -1;
} else {
    isNewSearch($search_fee_type, SEARCH_FEE_TYPE);
}
if (!isset($search_fee_name)) {
    $search_fee_name = $reload ? $search_values[SEARCH_FEE_NAME] : -1;
} else {
    isNewSearch($search_fee_name, SEARCH_FEE_NAME);
}
if (!isset($search_fee_club)) {
    $search_fee_club = $reload ? $search_values[SEARCH_FEE_CLUB] : -1;
} else {
    isNewSearch($search_fee_club, SEARCH_FEE_CLUB);
}

$_SESSION[SESSION_SEARCH_VALUES] = serialize(array(
    SEARCH_LISTED_MEMBER => $listed_member,
    SEARCH_SEARCH => $search,
    SEARCH_SELECTED_ACTIVE => $selected_active,
    SEARCH_SELECTED_PAID => $selected_paid,
    SEARCH_SELECTED_LICENCE => $selected_licence,
    SEARCH_SEARCH_BY_ID => $search_by_id,
    SEARCH_ORDER_BY => $order_by,
    SEARCH_SELECTED_CLUB => $selected_club,
    SEARCH_FEE_YEAR => $search_fee_year,
    SEARCH_FEE_TYPE => $search_fee_type,
    SEARCH_FEE_NAME => $search_fee_name,
    SEARCH_FEE_CLUB => $search_fee_club,
    SEARCH_SELECTED_BRANCH => $selected_branch,
        ));
$sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM `' . MEMBER_TABLE . '` ';
if ($search_by_id > 0) {
    $search = '';
    $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM `' . MEMBER_TABLE . '` WHERE `' . MEMBER_ID . '`="' . $search_by_id . '" LIMIT 1';
} else {
    $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (((`' . MEMBER_TABLE . '`';
    if ($selected_branch >= 0) {
        $sql .= ' INNER JOIN `' . CATEGORIES_TABLE . '` ON '
                . '`' . CATEGORIES_TABLE . '`.`' . CATEGORIES_BRANCH . '`=' . $selected_branch . ' '
                . ' INNER JOIN `' . CM_TABLE . '` ON ('
                . ' `' . CATEGORIES_TABLE . '`.`' . CATEGORIES_ID . '`=`' . CM_TABLE . '`.`' . CM_CATEGORY . '` '
                . 'AND `' . CM_TABLE . '`.`' . CM_MEMBER_ID . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`)';
    }
    if ($selected_club > 0) {
        $sql .= ' INNER JOIN `' . ROLE_TABLE . '` ON (`' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`=' . $selected_club . ' '
                . ' AND ((`' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '` & (' . ACCESS_CLUB_MEMBER . ')) > 0) '
                . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '` '
                . ' AND (CURDATE() BETWEEN `' . ROLE_TABLE . '`.`' . ROLE_FROM . '` AND `' . ROLE_TABLE . '`.`' . ROLE_TO . '`)))';
    } else {
        $sql .= ')';
    }
    switch ($selected_active) {
        case ACTIVE_ONLY:
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`!=0 '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . (($selected_club > 0) ? ' AND `active`.`' . ROLE_CLUB . '`=' . $selected_club . ' ' : '')
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            break;
        case ACTIVE_NOT:
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`=0 '
                    . (($selected_club > 0) ? ' AND `active`.`' . ROLE_CLUB . '`=' . $selected_club . ' ' : '')
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            break;
        case ACTIVE_DELETED:
            $sql .= ' LEFT JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . '`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` )) ';
            $selected_paid = PAID_ALL;
            break;

        default:
            $sql .= ')';
            break;
    }
    switch ($selected_paid) {
        case PAID_NOT:
            $sql .= ' INNER JOIN `' . FM_TABLE . '` AS `paid_fm` ON ('
                    . ' `paid_fm`.`' . FM_PAY_ID . '`="0" '
                    . (($searchByFees AND $selected_fee_id > 0) ?
                    (' AND `paid_fm`.`' . FM_FEE_ID . '`=' . $selected_fee_id . ' ') : '')
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`paid_fm`.`' . FM_MEMEBER_ID . '` '
                    . ' AND (CURDATE() BETWEEN `paid_fm`.`' . FM_FROM . '` AND `paid_fm`.`' . FM_TO . '`))'
                    . ' INNER JOIN `' . FEE_TABLE . '` AS `paid_fee` ON ('
                    . ' `paid_fm`.`' . FM_FEE_ID . '`=`paid_fee`.`' . FEE_ID . '` '
                    . ' AND `paid_fee`.`' . FEE_TYPE . '`="' . FEE_TYPE_MEMBERSHIP . '" '
                    . ' ))';
            break;
        case PAID_ONLY:
            $sql .= ' INNER JOIN `' . FM_TABLE . '` AS `paid_fm` ON ('
                    . ' `paid_fm`.`' . FM_PAY_ID . '`!="0" '
                    . (($searchByFees AND $selected_fee_id > 0) ?
                    (' AND `paid_fm`.`' . FM_FEE_ID . '`=' . $selected_fee_id . ' ') : '')
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`paid_fm`.`' . FM_MEMEBER_ID . '` '
                    . ' AND (CURDATE() BETWEEN `paid_fm`.`' . FM_FROM . '` AND `paid_fm`.`' . FM_TO . '`))'
                    . ' INNER JOIN `' . FEE_TABLE . '` AS `paid_fee` ON ('
                    . ' `paid_fm`.`' . FM_FEE_ID . '`=`paid_fee`.`' . FEE_ID . '` '
                    . ' AND `paid_fee`.`' . FEE_TYPE . '`="' . FEE_TYPE_MEMBERSHIP . '" '
                    . ' ))';
            break;
        case PAID_ALL:
        default:
            $sql .= (($searchByFees AND $selected_fee_id > 0) ?
                    (' INNER JOIN `' . FM_TABLE . '` AS `paid_fm` ON (`paid_fm`.`' . FM_FEE_ID . '`=' . $selected_fee_id . ' )'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`paid_fm`.`' . FM_MEMEBER_ID . '` ') : '')
                    . ' ) ';
            break;
    }
    if (strlen($search) < 3 AND ! empty($search)) {
        $view->warning('Legalább 3 karaktert adj meg a szűréshez');
    }
    if (strlen($search) >= 3) {
        $sql .= ' WHERE ( CONCAT (`' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '`, " ", `'
                . MEMBER_TABLE . '`.`' . MEMBER_KERNEV . '`)' . ' LIKE "%' . $search . '%" )';
    } else {
        $sql .= ' WHERE 1 ';
    }
    switch ($selected_licence) {
        case LICENCE_WITH:
            $sql .= ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_LICENCE . '`>"" ';
            break;
        case LICENCE_WITHOUT:
            $sql .= ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_LICENCE . '`="" ';
            break;
        default:
            break;
    }
    $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
    switch ($order_by) {
        case MEMBER_ID: $sql .= ' ORDER BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ASC ;';
            break;
        case MEMBER_VEZNEV:
        default:
            $sql .= ' ORDER BY `' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '`, `' . MEMBER_TABLE . '`.`' . MEMBER_KERNEV . '` ASC ;';
            break;
    }
}
//error_log('search_members:' . $sql);
$res = $mysqliLink->query($sql);
if ($res) {
    $total_member = $res->num_rows;
} else {
    $total_member = 0;
    mail_system_error('Hiba a lekérdezésben: ' . $sql);
}
if ($searchByFees AND $selected_fee_id < 0) {
    $total_member = 0;
}
if (isset($filtered_newsletter) AND $total_member > 0) {
    $filtered = array();
    while ($res AND $row = $res->fetch_assoc()) {
        if (!empty($row[MEMBER_EMAIL])) {
            $filtered[] = $row[MEMBER_ID];
        }
    }
    $_SESSION[SESSION_FILTERED_NEWSLETTER] = serialize($filtered);
    redirect("news_letters");
} else {
    unset($_SESSION[SESSION_FILTERED_NEWSLETTER]);
}

$view->newKeret();
if ($searchByFees) {
    include_once INCLUDES . 'search_fee.inc';
    $view->submitButton('search_submit', 'Keresés');
} else {
    $view->putHidden('selected_fee_id');
    $view->newDiv('form-inline');
    $view->inputSearch('search', 'Név');
    $view->inputMemberId('search_by_id', 'Azonosító');
    $view->submitButton('search_submit', 'Keresés');
    if ($total_member > 0 AND ( isPermitted(getAccessOfModule("news_letters")))) {
        $view->submitButton('filtered_newsletter', 'Körlevél', -1, 'Csak ez email címmel rendelkezőknek !', 'Biztosan körlevelet akarsz küldeni a ' . $total_member . ' kiválsztott tagnak ?');
    }
    $view->info($total_member . ' tag');
    $view->endDiv();
}
$given_filters = 'Aktív szűrők:';
$view->newDiv('form-inline');
$view->submitDropDownChooser('Rendezés', [MEMBER_ID => 'Azonosító', MEMBER_VEZNEV => 'Név'], 'order_by', true);
$view->putModalToggler('filters', 'Szűrők', $given_filters);
$view->newModal('filters', 'Szűrők megadás');
$search_branches = array();
if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $search_branches = [-1 => 'Összes'] + $branches;
} elseif (isPermitted(ACCESS_BRANCH_LEADER)) {
    $search_branches = [-1 => 'Összes'] + getBranchesOfLeader($member_id);
}
if (count($search_branches) > 0) {
    $view->submitDropDownChooser('Szakágak', $search_branches, 'selected_branch', true);
}
if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $clubs = [-1 => 'Összes'] + getClubNameList();
    $view->submitDropDownChooser('Egyesület', $clubs, 'selected_club', true);
} else {
    $selected_club = putSelectionOfClub('selected_club');
}
$view->submitDropDownChooser('Tagság', [ACTIVE_ALL => 'Nincs szűrés', ACTIVE_ONLY => 'Aktívak', ACTIVE_NOT => 'Nem aktívak'], 'selected_active', true);


$view->submitDropDownChooser('Fizetés', [PAID_ALL => 'Nincs szűrés', PAID_ONLY => 'Fizetve', PAID_NOT => 'Rendezetlen'], 'selected_paid', true);
if (getOptionValue(OPTIONS_NAME_FAI_ENABLED) > 0) {
    $view->submitDropDownChooser('FAI licensz', [LICENCE_ALL => 'Összes', LICENCE_WITH => 'Van', LICENCE_WITHOUT => 'Nincs'], 'selected_licence', true);
}
$view->endModal();
$view->inputDropDownSelector('Oszlopok', 'view_', $all_columns, $view_columns, true);



if ($total_member > PAGE_MEMBERS) {
    $total_pages = intdiv($total_member - 1, PAGE_MEMBERS) + 1;
    $current_page = intdiv($listed_member, PAGE_MEMBERS); // Az első lap 0-s  
    $first_page = max(0, min($current_page - 1, $total_pages - PAGERS));
    $view->newElement('<nav aria-label="search_members">');
    $view->newElement('<ul class="pagination m-2">');
    $view->newElement('<li class="page-item">');
    $view->putElement('<button class="page-link" name="listed_member" id="listed_member" value="0" title="Első oldal"><span aria-hidden="true">&laquo;</span><span class="sr-only">Első</span></button>');
    $view->endElement('</li>');
    for ($i = 0; $i < min(PAGERS, $total_pages); $i++) {
        $page_active = ($first_page + $i) == $current_page;
        $page_num = $first_page + $i;
        $goto_member = $page_num * PAGE_MEMBERS;
        $view->newElement('<li class="page-item' . ($page_active ? ' active" aria-current="page"' : '"') . '>');
        $view->putElement('<button class="page-link" name="listed_member" id="listed_member" value="' . $goto_member . '" title="' . ($page_num + 1) . '. oldal" >' . ($page_num + 1) . ($page_active ? '<span class="sr-only">(current)</span>' : '') . '</button>');
        $view->endElement('</li>');
    }
    $view->newElement('<li class="page-item">');
    $view->putElement('<button class="page-link" name="listed_member" id="listed_member" value="' . ($total_member - PAGE_MEMBERS) . '" title="Utolsó oldal"><span aria-hidden="true">&raquo;</span><span class="sr-only">Utolsó</span></button>');
    $view->endElement('</li>');
    $view->endElement('</ul>');
    $view->endElement('</nav>');
}
$view->endDiv();
$view->endKeret();

function isNewSearch($field, $id) {
    global $search_values, $listed_member;
    if (!isset($search_values)) {
        return;
    }
    if ($field != $search_values[$id]) {
        $listed_member = 0;
    }
}
