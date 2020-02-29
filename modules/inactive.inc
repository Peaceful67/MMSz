<?php

$output .= '<form method="post">';
$output .= '<div class="editor">';

if (isset($modify_role)) {
    $_SESSION[SESSION_FOUND_MEMBERS] = serialize(array($modify_role));
    $_SESSION[SESSION_SELECTED_ITEM] = 0;
    $_SESSION[SESSION_DATA2EDIT] = 'roles';
    redirect('edit_member');
    return;
}

$clubs = getClubNameList();
foreach ($clubs as $club_id => $club_name) {
    $members = getActiveMembersOfClub($club_id);
    foreach ($members as $mem) {
        if (isPaidMember($mem)) {
            if (($key = array_search($mem, $members)) !== false) {
                unset($members[$key]);
            }
        }
    }
    if (!empty($members)) {
        $first_line = true;
        $first_line = true;
        foreach ($members as $mem) {
            if ($first_line) {
                $first_line = false;
                $output .= '<br><b><label>' . $club_name . '</label></b>';
            } else {
                $output .= '<br><label> </label>';
            }
            $output .= '<button  name="modify_role" value="' . $mem . '" >' . getMemberName($mem);
            $output .= '</button>&nbsp;&nbsp;';
            $output .= getLastPaymentOfMember($mem);
        }
    }
}

$output .= '<br>';
$output .= '</div>';
$output .= '</form>';

function getActiveMembersOfClub($club_id) {
    global $mysqliLink;
    $sql = 'SELECT `members`.`ID` FROM (`members`  INNER JOIN `role_member` AS `active` ON (`active`.`permission`!=0  
                            AND `members`.`ID`=`active`.`member_id`
                            AND `active`.`club_id`=' . $club_id . ' 
                            AND (CURRENT_DATE BETWEEN `active`.`date_from` AND `active`.`date_to`)))
                            GROUP BY `members`.`ID`';
    $res = $mysqliLink->query($sql);
    $ret = array();
    if ($res AND $res->num_rows > 0) {
        while ($row = $res->fetch_assoc()['ID']) {
            $ret[] = $row;
        }
    }
    return $ret;
}

function isPaidMember($mem_id) {
    global $mysqliLink;
    $sql = 'SELECT  * FROM (`fee_member` 
                  INNER JOIN `fees` AS `membership` ON 
                            (`membership`.`type`=0 AND `fee_member`.`fee_id`=`membership`.`id`
                            	AND (
                                    (CURRENT_DATE BETWEEN `membership`.`fee_from` AND `membership`.`fee_to`) 
                            		OR (DATE_SUB(CURRENT_DATE, INTERVAL 1 YEAR) BETWEEN `membership`.`fee_from` AND `membership`.`fee_to`)
                                	)
                            )
                )
            WHERE `fee_member`.`payment_id`!=0 AND `fee_member`.`member_id`=' . $mem_id . '
            GROUP BY `fee_member`.`id`';
    $res = $mysqliLink->query($sql);
    return ($res AND $res->num_rows > 0) ? true : false;
}

function getLastPaymentOfMember($mem_id) {
    global $mysqliLink;
    $sql = 'SELECT  * FROM ((`payments` 
                 INNER JOIN `fee_member` AS `fm` ON
                			(`fm`.`member_id`=' . $mem_id . ' AND `payments`.`id`=`fm`.`payment_id`))
                  INNER JOIN `fees` AS `membership` ON 
                            (`membership`.`type`=0 AND `fm`.`fee_id`=`membership`.`id`)
                  )
            ORDER BY `payments`.`time` DESC
            LIMIT 1';
    $res = $mysqliLink->query($sql);
    $ret = '---';
    if ($res AND $res->num_rows > 0) {
        $ret = $res->fetch_assoc()[PAYMENT_TIME];
    }
    return $ret;
}
