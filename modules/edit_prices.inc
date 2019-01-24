<?php

$cl_id = getRoleClubleader($member_id);
if (!isPermitted(ACCESS_ADMIN) && !isPermitted(ACCESS_MMSZ) && $cl_id >= 0) { // A klubvezető csak a saját klubját szerkesztheti
    $output .= '<div class="editor">';
    $club_limited = $cl_id;
    include_once INCLUDES . 'edit_club_fees.inc';
    $output .= '</div>';
    return;
}

if (isset($radio_edit_fee)) {
    $_SESSION[SESSION_FEE2EDIT] = $radio_edit_fee;
} else {
    $radio_edit_fee = isset($_SESSION[SESSION_FEE2EDIT]) ? $_SESSION[SESSION_FEE2EDIT] : "mmsz";
}

$output .= '<div class="editor"><form  method="post" >';
$output .= $fee_type[FEE_TYPE_MEMBERSHIP][FEE_TYPE_NAME] . ':<input type="radio" name="radio_edit_fee" value="mmsz"  onChange="submit();" ' . (($radio_edit_fee == "mmsz") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;' . $fee_type[FEE_TYPE_INSURANCE][FEE_TYPE_NAME] . ':<input type="radio" name="radio_edit_fee" value="insur"  onChange="submit();" ' . (($radio_edit_fee == "insur") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;' . $fee_type[FEE_TYPE_ORGANIZATION][FEE_TYPE_NAME] . ':<input type="radio" name="radio_edit_fee" value="org"  onChange="submit();" ' . (($radio_edit_fee == "org") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;' . $fee_type[FEE_TYPE_CLUB][FEE_TYPE_NAME] . ':<input type="radio" name="radio_edit_fee" value="club"  onChange="submit();" ' . (($radio_edit_fee == "club") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;' . $fee_type[FEE_TYPE_ASSOCIATION][FEE_TYPE_NAME] . ':<input type="radio" name="radio_edit_fee" value="assoc"  onChange="submit();" ' . (($radio_edit_fee == "assoc") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;' . $fee_type[FEE_TYPE_ADMINISTRATION][FEE_TYPE_NAME] . ':<input type="radio" name="radio_edit_fee" value="admin"  onChange="submit();" ' . (($radio_edit_fee == "admin") ? " checked " : "") . '>';
$output .= '</form>';

switch ($radio_edit_fee) {
    default:
    case "mmsz":
        $edited_type = FEE_TYPE_MEMBERSHIP;
        $edited_names = $member_types;
        include_once INCLUDES . 'edit_general_fees.inc';
        break;
    case "insur":
        $edited_type = FEE_TYPE_INSURANCE;
        $edited_names = $member_insurances;
        include_once INCLUDES . 'edit_general_fees.inc';
        break;
    case "org":
        $edited_type = FEE_TYPE_ORGANIZATION;
        $edited_names = $organizations;
        include_once INCLUDES . 'edit_general_fees.inc';
        break;
    case "admin":
        $edited_type = FEE_TYPE_ADMINISTRATION;
        $edited_names = $administrations;
        include_once INCLUDES . 'edit_general_fees.inc';
        break;
    case "assoc":
        $edited_type = FEE_TYPE_ASSOCIATION;
        $edited_names = $associations;
        include_once INCLUDES . 'edit_general_fees.inc';
        break;

    case "club":
        include_once INCLUDES . 'edit_club_fees.inc';
        break;
}

$output .= '</div><br><form method="post"><div class="fees">';
if (!isset($year_selector)) {
    $year_selector = $this_year;
}
$output .= 'Áttekintő: <select name="year_selector"  onchange="submit();">';

for ($y = YEAR_FROM; $y <= YEAR_TO; $y++) {
    $output .= '<option value="' . $y . '" ' . (($y == $year_selector) ? "selected" : "") . '>' . $y . '</option>';
}
$output .= '</select> évre<br><br>';
foreach ($fee_type as $ft => $fn) {
    if($ft==FEE_TYPE_LICENCES) {
        break;
    }
    $output .= '<table><caption>' . $fn[FEE_TYPE_NAME] . '</caption>'
            . '<tr><th>Id</th><th>Egyesület</th><th>Név</th>'
            . '<th>Kezdet</th><th>Vég</th><th>Szakág</th><th>Összeg</th></tr>';
    foreach (getFeesOfYear($ft, $year_selector) as $fee) {
        $output .= '<tr>';
        $output .= '<td>' . $fee[FEE_ID] . '</td>';
        $output .= '<td>' . (($fee[FEE_CLUB] >= 0) ? getClubName($fee[FEE_CLUB]) : "---") . '</td>';
        $output .= '<td>';
        $output .= printFeeName($fee);
        $output .= '</td>';
        $output .= '<td>' . $fee[FEE_FROM] . '</td>';
        $output .= '<td>' . $fee[FEE_TO] . '</td>';
        $output .= '<td>' . (($fee[FEE_BRANCH] >= 0) ? $branches[$fee[FEE_BRANCH]] : "---") . '</td>';
        $output .= '<td>' . $fee[FEE_SUM] . ' Ft</td>';
        $output .= '</tr>';
    }
    $output .= '</table><br>';
}
$output .= '</div></form>';
