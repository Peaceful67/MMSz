<?php

// $cl_id = getRoleClubleader($member_id);
if (!isset($edit_club_price)) {
    $edit_club_price = -1;
}
if (isOnlyClubleader()) {
    $clubs = getClubsOfLeader($member_id);
    if (count($clubs) == 1) { // A klubvezető csak egy klubját szerkesztheti
        reset($clubs);
        $cl_id = key($clubs);
        $club_limited = $cl_id;
        include_once INCLUDES . 'edit_club_fees.inc';
    } else {

        $view->submitDropDownChooser('Egyesület', [-1 => 'Válassz!!!'] + $clubs, 'edit_club_price');
        if ($edit_club_price >= 0) {
            $club_limited = $edit_club_price;
            include_once INCLUDES . 'edit_club_fees.inc';
        }
    }
    return;
}
if (isset($radio_edit_fee)) {
    $_SESSION[SESSION_FEE2EDIT] = $radio_edit_fee;
} else {
    $radio_edit_fee = isset($_SESSION[SESSION_FEE2EDIT]) ? $_SESSION[SESSION_FEE2EDIT] : "mmsz";
}

$view->newElement('<ul class="nav nav-tabs">');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn navbar-btn btn-secondary '
        . ($radio_edit_fee == 'mmsz' ? 'active' : '') . '" name="radio_edit_fee"  value="mmsz">' . $fee_type_names[FEE_TYPE_MEMBERSHIP] . '</button>');
$view->endElement('</li>');
$view->putElement('<button class="btn navbar-btn btn-secondary '
        . ($radio_edit_fee == 'insur' ? 'active' : '') . '" name="radio_edit_fee"  value="insur">' . $fee_type_names[FEE_TYPE_INSURANCE] . '</button>');
$view->endElement('</li>');
$view->putElement('<button class="btn navbar-btn btn-secondary '
        . ($radio_edit_fee == 'org' ? 'active' : '') . '" name="radio_edit_fee"  value="org">' . $fee_type_names[FEE_TYPE_ORGANIZATION] . '</button>');
$view->endElement('</li>');
$view->putElement('<button class="btn navbar-btn btn-secondary '
        . ($radio_edit_fee == 'club' ? 'active' : '') . '" name="radio_edit_fee"  value="club">' . $fee_type_names[FEE_TYPE_CLUB] . '</button>');
$view->endElement('</li>');
$view->putElement('<button class="btn navbar-btn btn-secondary '
        . ($radio_edit_fee == 'assoc' ? 'active' : '') . '" name="radio_edit_fee"  value="assoc">' . $fee_type_names[FEE_TYPE_ASSOCIATION] . '</button>');
$view->endElement('</li>');
$view->putElement('<button class="btn navbar-btn btn-secondary '
        . ($radio_edit_fee == 'admin' ? 'active' : '') . '" name="radio_edit_fee"  value="admin">' . $fee_type_names[FEE_TYPE_ADMINISTRATION] . '</button>');
$view->endElement('</li>');
$view->putElement('<button class="btn navbar-btn btn-secondary '
        . ($radio_edit_fee == 'occasion' ? 'active' : '') . '" name="radio_edit_fee"  value="occasion">' . $fee_type_names[FEE_TYPE_OCCASION] . '</button>');
$view->endElement('</li>');
$view->endElement('</ul>');
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
    case "occasion":
        $edited_type = FEE_TYPE_OCCASION;
        $edited_names = $occasions;
        include_once INCLUDES . 'edit_general_fees.inc';
        break;
    case "club":
        include_once INCLUDES . 'edit_club_fees.inc';
        break;
}


if (!isset($year_selector)) {
    $year_selector = $this_year;
}

$view->submitDropDownChooser('Áttekintő', $years, 'year_selector', true);
foreach ($fee_type as $ft => $fn) {
    if ($ft == FEE_TYPE_LICENCES) {
        continue;
    }
    $view->newTable($fn[FEE_TYPE_NAME]);
    $view->putThead('', ['Id', 'Egyesület', 'Név', 'Kezdet', 'Vég', 'Szakág', 'Összeg']);
    foreach (getFeesOfYear($ft, $year_selector) as $fee) {
        $view->newTableTr();

        $view->putElement('<td>' . $fee[FEE_ID] . '</td>');
        $view->putElement('<td>' . (($fee[FEE_CLUB] >= 0) ? getClubName($fee[FEE_CLUB]) : "---") . '</td>');
        $view->putElement('<td>' . printFeeName($fee) . '</td>');
        $view->putElement('<td>' . $fee[FEE_FROM] . '</td>');
        $view->putElement('<td>' . $fee[FEE_TO] . '</td>');
        $view->putElement('<td>' . (($fee[FEE_BRANCH] >= 0) ? $branches[$fee[FEE_BRANCH]] : "---") . '</td>');
        $view->putElement('<td>' . $fee[FEE_SUM] . ' Ft</td>');
        $view->endTableTr();
    }
    $view->endTable();
}


