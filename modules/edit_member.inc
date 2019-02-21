<?php

$found_member_ids = unserialize($_SESSION[SESSION_FOUND_MEMBERS]);
$selected_item = $_SESSION[SESSION_SELECTED_ITEM];
if (isset($exit_edit_member)) {
    unset($_SESSION[SESSION_FOUND_MEMBERS]);
    redirect(get_back());
}
if (isset($radio_edit_member)) {
    $_SESSION[SESSION_DATA2EDIT] = $radio_edit_member;
} else {
    $radio_edit_member = isset($_SESSION[SESSION_DATA2EDIT]) ? $_SESSION[SESSION_DATA2EDIT] : "base";
}
$output .= '<form method="post">';

$found_total = count($found_member_ids);
if (isset($found_member_ids) && $found_total > 1) {
    if (isset($pager)) {
        switch ($pager) {
            case '<<':
                $selected_item = 0;
                break;
            case '<':
                $selected_item--;
                if ($selected_item < 0) {
                    $selected_item = 0;
                }
                break;
            case '>':
                $selected_item++;
                if ($selected_item >= $found_total) {
                    $selected_item--;
                }
                break;
            case '>>':
                $selected_item = $found_total - 1;
                break;

            default:
                break;
        }
    }

    $output .= '<label>Lapozás:</label><input type="submit" name="pager" value="<<">    ';
    $output .= '<input type="submit" name="pager" value="<">    ';
    $output .= ($selected_item + 1) . '/' . count($found_member_ids);
    $output .= '<input type="submit" name="pager" value=">">    ';
    $output .= '<input type="submit" name="pager" value=">>">    ';
}
$edit_member = $found_member_ids[$selected_item];
$_SESSION[SESSION_SELECTED_ITEM] = $selected_item;
$output .= '<div class="member">';
$output .= '<input type="submit" name="exit_edit_member" value="Kilépés">&nbsp;&nbsp;&nbsp;&nbsp;';
$output .= getMemberName($edit_member) . ' (' . $edit_member . ')';
$output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
$output .= '<br>Alap:<input type="radio" name="radio_edit_member" value="base"  onChange="submit();" ' . (($radio_edit_member == "base") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Díjak:<input type="radio" name="radio_edit_member" value="fees" onChange="submit();" ' . (($radio_edit_member == "fees") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Szerepek:<input type="radio" name="radio_edit_member" value="roles" onChange="submit();" ' . (($radio_edit_member == "roles") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Kategóriák:<input type="radio" name="radio_edit_member" value="categories" onChange="submit();" ' . (($radio_edit_member == "categories") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;' . $fee_type[FEE_TYPE_LICENCES][0] . ':<input type="radio" name="radio_edit_member" value="licences" onChange="submit();" ' . (($radio_edit_member == "licences") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Arckép<input type="radio" name="radio_edit_member" value="avatar" onChange="submit();" ' . (($radio_edit_member == "avatar") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Fájlok: <input type="radio" name="radio_edit_member" value="documents" onChange="submit();" ' . (($radio_edit_member == "documents") ? " checked " : "") . '>';
$output .= '</form>';
switch ($radio_edit_member) {
    case "base":
    default:
        $base_data_id = $edit_member;
        include_once INCLUDES . 'base_data.inc';
        break;
    case "fees":
        $fee_member_id = $edit_member;
        $editable_fees = array(FEE_TYPE_MEMBERSHIP, FEE_TYPE_INSURANCE, FEE_TYPE_ORGANIZATION, FEE_TYPE_CLUB, 
            FEE_TYPE_ADMINISTRATION, FEE_TYPE_ASSOCIATION, FEE_TYPE_LICENCES, FEE_TYPE_OCCASION);
        include_once INCLUDES . 'choose_fee_type.inc';
        break;
    case "roles":
        $role_member_id = $edit_member;
        include_once INCLUDES . 'choose_role.inc';
        break;
    case "categories":
        $category_member_id = $edit_member;
        include_once INCLUDES . 'choose_categories.inc';
        break;
    case "licences":
        $licences_member_id = $edit_member;
        include_once INCLUDES . 'choose_licences.inc';
        break;
    case "avatar":
        if (!isset($avatar_id)) {
            $avatar_id = $edit_member;
        }
        include_once INCLUDES . 'avatar.inc';
        break;
    case "documents":
        $document_id = $edit_member;
        include_once INCLUDES . 'documents.inc';
        break;
}
$output .= '</div>';

