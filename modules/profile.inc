<?php

if (isset($radio_edit_member)) {
    $_SESSION[SESSION_DATA2EDIT] = $radio_edit_member;
} else {
    $radio_edit_member = isset($_SESSION[SESSION_DATA2EDIT]) ? $_SESSION[SESSION_DATA2EDIT] : "base";
}

$output .= '<div class="member"><form  method="post" enctype="multipart/form-data">';
$output .= 'Tagsági szám: ' . $member_id . '&nbsp;&nbsp;&nbsp;&nbsp;';
$output .= 'Alap:<input type="radio" name="radio_edit_member" value="base"  onChange="submit();" ' . (($radio_edit_member == "base") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Díjak:<input type="radio" name="radio_edit_member" value="fees" onChange="submit();" ' . (($radio_edit_member == "fees") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Jelszó:<input type="radio" name="radio_edit_member" value="passwd" onChange="submit();" ' . (($radio_edit_member == "passwd") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Kategóriák:<input type="radio" name="radio_edit_member" value="categories" onChange="submit();" ' . (($radio_edit_member == "categories") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;' . $fee_type[FEE_TYPE_LICENCES][0] . ':<input type="radio" name="radio_edit_member" value="licences" onChange="submit();" ' . (($radio_edit_member == "licences") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Arckép<input type="radio" name="radio_edit_member" value="avatar" onChange="submit();" ' . (($radio_edit_member == "avatar") ? " checked " : "") . '>';
$output .= '&nbsp;&nbsp;&nbsp;&nbsp;Fájlok: <input type="radio" name="radio_edit_member" value="documents" onChange="submit();" ' . (($radio_edit_member == "documents") ? " checked " : "") . '>';
$output .= '</form>';
switch ($radio_edit_member) {
    case "base":
    default:
        $base_data_id = $member_id;
        include_once INCLUDES . 'base_data.inc';
        break;
    case "fees":
        $sf_member = $member_id;
        $deletable_fm = false;
        include_once INCLUDES . 'show_fees_member.inc';
        break;
    case "passwd":
        include_once INCLUDES . 'change_passwd.inc';
        break;
    case "categories":
        $category_member_id = $member_id;
        include_once INCLUDES . 'choose_categories.inc';
        break;
    case "licences":
        $licences_member_id = $member_id;
        include_once INCLUDES . 'choose_licences.inc';
        break;
    case "avatar":
        if (!isset($avatar_id)) {
            $avatar_id = $member_id;
        }
        include_once INCLUDES . 'avatar.inc';
        break;
    case "documents":
        $document_id = $member_id;
        include_once INCLUDES . 'documents.inc';
        break;
}
$output .= '</div>';
?>
