
<?php

if (!isset($listed_member)) {
    $listed_member = 0;
}
if (!isset($search)) {
    $search = '';
}
if (!isset($only_active)) {
    $only_active = '';
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
        $list_clubs = getClubsOfLeader($member_id);
        reset($list_clubs);
        $selected_club = key($list_clubs);
    }
}
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

if ($search_by_id > 0) {
    $search = '';
    $sql = "SELECT * FROM `" . MEMBER_TABLE . "` WHERE `id`=" . $search_by_id . " LIMIT 1";
    $res = $mysqliLink->query($sql);
    $list_members_sql = $sql;
    $total_member = $res ? $res->num_rows : 0;
} else {
    if (strlen($search) < 3 AND ! empty($search)) {
        warning('Legalább 3 karakter adj meg a szűréshez');
    }

    if ($selected_club > 0) {
        $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM `' . MEMBER_TABLE . '`,`' . ROLE_TABLE . '` '
                . 'WHERE `' . ROLE_TABLE . '`.`' . ROLE_CLUB . '`="' . $selected_club . '" '
                . '   AND `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`="' . ACCESS_USER . '"'
                . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '` '
                . ' AND(CURDATE() BETWEEN `' . ROLE_TABLE . '`.`' . ROLE_FROM . '` AND `' . ROLE_TABLE . '`.`' . ROLE_TO . '`) '
                . ' AND ';
    } else {
        $sql = 'SELECT * FROM `' . MEMBER_TABLE . '` WHERE ';
    }
    $sql .= (strlen($search) < 3) ? " 1 " :
            "`" . MEMBER_KERNEV . "` LIKE '%" . $search . "%' OR "
            . "`" . MEMBER_VEZNEV . "` LIKE '%" . $search . "%' ";

    $sql .= 'GROUP BY `' . MEMBER_ID . '` ORDER BY `' . $order_by . '` ';
    $list_members_sql = $sql;
    $totalres = $mysqliLink->query($sql);
    $res = $mysqliLink->query($sql . ' LIMIT ' . $listed_member . ', ' . PAGE_MEMBERS);
    $total_member = $totalres ? $totalres->num_rows : 0;
    $output .= '<script>
    function searchChanges() {
        var searchField = document.getElementById("search");
        this.select();
        submitSearchForm();
    }
    function submitSearchForm() {
        alert("haho");
        document.getElementById("listed_member").value = 0;
        document.getElementById("list_method").submit();
    }
    function areSure() {
        if(confirm("Biztosan törölni akarod a tagot ?") {
            document.getElementById("list_method").submit();
        } 
    }

</script>';
}

$output .= '<div class="keret"><form id="list_method" method="post"> 

<label for="search">Név: </label>
<input type="text" name="search"  id="search" value="' . $search . '"  size="10">'
        . '&nbsp;&nbsp;&nbsp;
            <label for="search">azonosító : </label>
<input type="text" name="search_by_id"  id="search_by_id" value="' . $search_by_id . '" size="10">'
        . '&nbsp;&nbsp;<input type="submit" name="search_submit" value="Szűrés">&nbsp;A feltételnek ' . $total_member . ' tag felel meg.'
        . '<br><br>';
if (isPermitted(ACCESS_ADMIN) || isPermitted(ACCESS_MMSZ) || isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $output .= 'Egyesület: <select  name="selected_club" onChange="submit();">';
    $output .= '<option value="0">Válassz egyesületet !</option>';
    $clubs = getClubNameList();
    foreach ($clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }
    $output .= '</select><br><br>';
}
$list_clubs = getClubsOfLeader($member_id);
error_log('list_clubs: '. array_r($list_clubs, true));
if(count($list_clubs)>1) {
    $output .= 'Egyesület: <select  name="selected_club" onChange="submit();">';
    $output .= '<option value="0">Válassz egyesületet !</option>';
    foreach ($list_clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }    
    $output .= '</select><br><br>';    
}
$output .= 'Sorszám:';
$output .= '<input type="radio" name="order_by" value="' . MEMBER_ID . '" onChange="submit();" ';
if ($order_by == MEMBER_ID) {
    $output .= ' checked="checked" ';
}
$output .= '>&nbsp;&nbsp;&nbsp;&nbsp;Név:';
$output .= '<input type="radio" name="order_by" value="' . MEMBER_VEZNEV . '" onChange="submit();" ';
if ($order_by == MEMBER_VEZNEV) {
    $output .= ' checked="checked" ';
}
$output .= '>&nbsp;&nbsp;&nbsp;';
//$output .= 'Csak aktív tagok: <input type="checkbox" name="only_active" ' . ($only_active == "on" ? 'checked' : '') . '><br><br>';
$output .= '<input type="hidden" name="listed_member" value="' . $listed_member . '">    ';
$output .= '<input type="hidden" name="total_member" value="' . $total_member . '">    ';
if ($total_member > PAGE_MEMBERS) {
    $output .= '<input type="submit" name="pager" value="<<">    ';
    $output .= '<input type="submit" name="pager" value="<">    ';
    $output .= '<input type="submit" name="pager" value=">">    ';
    $output .= '<input type="submit" name="pager" value=">>">    ';
}
$output .= '</form>';
$output .= '</div>';

