
<?php

define("PAID_ONLY", 1);
define("PAID_NOT", 2);
define("PAID_ALL", 3);

define("ACTIVE_ONLY", 4);
define("ACTIVE_NOT", 5);
define("ACTIVE_ALL", 6);

define("LICENCE_WITH", 7);
define("LICENCE_WITHOUT", 8);
define("LICENCE_PAID", 9);
define("LICENCE_ALL", 10);

if (isset($_SESSION[SESSION_SEARCH_VALUES])) {
    
} else {
    
}

if (!isset($listed_member)) {
    $listed_member = 0;
}
if (!isset($search)) {
    $search = '';
}
if (!isset($selected_active)) {
    $selected_active = ACTIVE_ONLY;
}

if (!isset($selected_paid)) {
    $selected_paid = PAID_ALL;
}

if (!isset($selected_licence)) {
    $selected_licence = LICENCE_ALL;
}

if (!isset($search_by_id)) {
    $search_by_id = '';
}

if (!isset($order_by)) {
    $order_by = MEMBER_VEZNEV;
}
if (!isset($selected_club)) {
    if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
        $selected_club = 0;
    } else {
        $selected_club = isClubLeader();
    }
}
$sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM `' . MEMBER_TABLE . '` ';
if ($search_by_id > 0) {
    $search = '';
    $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM `' . MEMBER_TABLE . '` WHERE `' . MEMBER_ID . '`="' . $search_by_id . '" LIMIT 1';
} else {
    $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM ((((`' . MEMBER_TABLE . '`';

    if ($selected_club > 0) {
        $sql .= ' INNER JOIN `' . ROLE_TABLE . '` ON (`' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`="' . $selected_club . '" '
                . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '` '
                . ' AND (CURDATE() BETWEEN `' . ROLE_TABLE . '`.`' . ROLE_FROM . '` AND `' . ROLE_TABLE . '`.`' . ROLE_TO . '`)))';
    } else {
        $sql .= ')';
    }
    switch ($selected_active) {
        case ACTIVE_ONLY:
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`!="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            break;
        case ACTIVE_NOT:
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            break;
        default:
            /*
              $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
              . ' `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` )) ';
             */
            $sql .= ' )';
            break;
    }
    switch ($selected_paid) {
        case PAID_NOT:
            $sql .= ' INNER JOIN `' . FM_TABLE . '` AS `paid_fm` ON ('
                    . ' `paid_fm`.`' . FM_PAY_ID . '`="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`paid_fm`.`' . FM_MEMEBER_ID . '` '
                    . ' AND (CURDATE() BETWEEN `paid_fm`.`' . FM_FROM . '` AND `paid_fm`.`' . FM_TO . '`))'
                    . ' INNER JOIN `' . FEE_TABLE . '` AS `paid_fee` ON ('
                    . ' `paid_fm`.`' . FM_FEE_ID . '`=`paid_fee`.`' . FEE_ID . '` '
                    . ' AND `paid_fee`.`' . FEE_TYPE . '`="' . FEE_TYPE_MEMBERSHIP . '" '
                    . ' )) ';
            break;
        case PAID_ONLY:
            $sql .= ' INNER JOIN `' . FM_TABLE . '` AS `paid_fm` ON ('
                    . ' `paid_fm`.`' . FM_PAY_ID . '`!="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`paid_fm`.`' . FM_MEMEBER_ID . '` '
                    . ' AND (CURDATE() BETWEEN `paid_fm`.`' . FM_FROM . '` AND `paid_fm`.`' . FM_TO . '`))'
                    . ' INNER JOIN `' . FEE_TABLE . '` AS `paid_fee` ON ('
                    . ' `paid_fm`.`' . FM_FEE_ID . '`=`paid_fee`.`' . FEE_ID . '` '
                    . ' AND `paid_fee`.`' . FEE_TYPE . '`="' . FEE_TYPE_MEMBERSHIP . '" '
                    . ' )) ';
            break;
        default:
            $sql .= ' ) ';
            break;
    }
    switch ($selected_licence) {
        /* WHER feltetelbe ment, mert a MEMBER tablaban kell nezni
          case LICENCE_WITH:
          $sql .= ' INNER JOIN `' . MEMBER_TABLE . '` AS `mem_lic` ON ('
          . ' `mem_lic`.`' . MEMBER_LICENCE . '`!=" " )) ';
          break;
          case LICENCE_WITHOUT:
          $sql .= ' INNER JOIN `' . MEMBER_TABLE . '` AS `mem_lic` ON ('
          . ' `mem_lic`.`' . MEMBER_LICENCE . '`=" " )) ';
          break;
         */
        case LICENCE_PAID:
            $sql .= ' INNER JOIN `' . FM_TABLE . '` AS `licence_fm` ON ('
                    . ' `licence_fm`.`' . FM_PAY_ID . '`!="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`licence_fm`.`' . FM_MEMEBER_ID . '` '
                    . ' AND (CURDATE() BETWEEN `licence_fm`.`' . FM_FROM . '` AND `licence_fm`.`' . FM_TO . '`))'
                    . ' INNER JOIN `' . FEE_TABLE . '` AS `licence_fee` ON ('
                    . ' `licence_fm`.`' . FM_FEE_ID . '`=`licence_fee`.`' . FEE_ID . '` '
                    . ' AND `licence_fee`.`' . FEE_TYPE . '`="' . FEE_TYPE_ORGANIZATION . '" '
                    . ' AND `licence_fee`.`' . FEE_NAME . '`="0" ' // FAI
                    . ' )) ';
            break;
        case LICENCE_ALL:
        default:
            $sql .= ' ) ';
            break;
    }
    if (strlen($search) < 3 AND ! empty($search)) {
        warning('Legalább 3 karakter adj meg a szűréshez');
    }
    if (strlen($search) >= 3) {
        $sql .= ' WHERE (`' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '` LIKE "%' . $search . '%"  OR '
                . '`' . MEMBER_TABLE . '`.`' . MEMBER_KERNEV . '` LIKE "%' . $search . '%")';
    } else {
        $sql .= ' WHERE 1 ';
    }
    if ($selected_licence == LICENCE_WITH OR $selected_licence == LICENCE_WITHOUT) {
        $sql .= ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_LICENCE . '`' . (($selected_licence == LICENCE_WITH) ? "!" : "") . '="" ';
    }
    $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ';
    switch ($order_by) {
        case MEMBER_ID: $sql .= ' ORDER BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` ASC ';
            break;
        case MEMBER_VEZNEV:
        default:
            $sql .= ' ORDER BY `' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '` ASC ';
            break;
    }
}

$res = $mysqliLink->query($sql);
$total_member = $res ? $res->num_rows : 0;
$output .= '<div class="keret"><form  method="post"><div class="search"> 

<label for="search">Név: </label>
<input type="text" name="search"  id="search" value="' . $search . '"  size="10">'
        . '&nbsp;&nbsp;&nbsp;
            <label for="search">Azonosító : </label>
<input type="text" name="search_by_id"  id="search_by_id" value="' . $search_by_id . '" size="10">';
$output .= '&nbsp;&nbsp;&nbsp;Rendezés:';
$output .= '<input type="radio" name="order_by" value="' . MEMBER_ID . '" onChange="submit();" ';
if ($order_by == MEMBER_ID) {
    $output .= 'checked="checked"';
}
$output .= '>sorszám&nbsp;&nbsp;&nbsp;&nbsp;';
$output .= '<input type="radio" name="order_by" value="' . MEMBER_VEZNEV . '" onChange="submit();" ';
if ($order_by == MEMBER_VEZNEV) {
    $output .= 'checked="checked"';
}
$output .= '>név&nbsp;&nbsp;&nbsp;&nbsp;Találat :' . $total_member . ' tag';

if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $output .= '<br><br>';
    $output .= 'Egyesület: <select  name="selected_club" onChange="submit();">';
    $output .= '<option value="0">Válassz egyesületet !</option>';
    $clubs = getClubNameList();
    foreach ($clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }
    $output .= '</select>';
}
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Tagság: ';
$output .= '<select name="selected_active" onChange="submit();">';
$output .= '<option value="' . ACTIVE_ALL . '" ' . ($selected_active == ACTIVE_ALL ? "selected" : "") . '>Összes</option>';
$output .= '<option value="' . ACTIVE_ONLY . ' " ' . ($selected_active == ACTIVE_ONLY ? "selected" : "") . '>Aktívak</option>';
$output .= '<option value="' . ACTIVE_NOT . '" ' . ($selected_active == ACTIVE_NOT ? "selected" : "") . '>Nem aktívak</option>';
$output .= '</select>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Fizetett: ';
$output .= '<select name="selected_paid" onChange="submit();">';
$output .= '<option value="' . PAID_ALL . '" ' . ($selected_paid == PAID_ALL ? "selected" : "") . '>Összes</option>';
$output .= '<option value="' . PAID_ONLY . '" ' . ($selected_paid == PAID_ONLY ? "selected" : "") . '>Rendezett</option>';
$output .= '<option value="' . PAID_NOT . '"  ' . ($selected_paid == PAID_NOT ? "selected" : "") . '>Nem rendezett</option>';
$output .= '</select>';

$output .= '&nbsp;&nbsp;&nbsp;&nbsp;FAI licensz: ';
$output .= '<select name="selected_licence" onChange="submit();">';
$output .= '<option value="' . LICENCE_ALL . '" ' . ($selected_licence == LICENCE_ALL ? "selected" : "") . '>Összes</option>';
$output .= '<option value="' . LICENCE_WITH . '" ' . ($selected_licence == LICENCE_WITH ? "selected" : "") . '>Van</option>';
$output .= '<option value="' . LICENCE_WITHOUT . '" ' . ($selected_licence == LICENCE_WITHOUT ? "selected" : "") . '>Nincs</option>';
$output .= '<option value="' . LICENCE_PAID . '" ' . ($selected_licence == LICENCE_PAID ? "selected" : "") . '>Fizetve</option>';
$output .= '</select>';

if (isset($pager)) {
    switch ($pager) {
        case '<<':
            $listed_member = 0;
            break;
        case '<':
            $listed_member -= PAGE_MEMBERS;
            if ($listed_member < 0) {
                $listed_member = 0;
            }
            break;
        case '>':
            $listed_member += PAGE_MEMBERS;
            if ($listed_member + PAGE_MEMBERS >= $total_member) {
                $listed_member = $total_member - PAGE_MEMBERS;
            }
            break;
        case '>>':
            $listed_member = $total_member - PAGE_MEMBERS;
            break;

        default:
            break;
    }
}

$output .= '<br><br><input type="submit" name="search_submit" value="Szűrés">&nbsp;&nbsp;&nbsp;&nbsp';
$output .= '<input type="hidden" name="listed_member" value="' . $listed_member . '">    ';
$output .= '<input type="hidden" name="total_member" value="' . $total_member . '">    ';

if ($total_member > PAGE_MEMBERS) {
    $output .= 'Lapozás:<input type="submit" name="pager" value="<<">    ';
    $output .= '<input type="submit" name="pager" value="<">    ';
    $output .= $listed_member + 1 . '/' . $total_member;
    $output .= '<input type="submit" name="pager" value=">">    ';
    $output .= '<input type="submit" name="pager" value=">>">    ';
}
$output .= '&nbsp;&nbsp;&nbsp;&nbspNézet: ';
foreach ($all_columns as $view_chkindex => $view_chkname) {
    $output .= $view_chkname . ':<input type="checkbox" name="view_' . $view_chkindex . '" onChange="submit();"'
            . ((in_array($view_chkindex, $view_columns)) ? ' checked ' : '')
            . '>&nbsp;&nbsp;&nbsp;';
}
$output .= '</div></form>';
$output .= '</div>';
