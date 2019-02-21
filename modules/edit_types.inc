<?php

if (isset($select_edit_fee_type)) {
    $_SESSION[SESSION_FEETYPE2EDIT] = $select_edit_fee_type;
} else {
    $select_edit_fee_type = isset($_SESSION[SESSION_FEETYPE2EDIT]) ? $_SESSION[SESSION_FEETYPE2EDIT] : -1;
}

$output .= '<div class="editor"><form method="POST">';
$output .= '<label>Típus: </label><select name="select_edit_fee_type" onChange="submit();">';
$output .= '<option value=-1 ' . (($select_edit_fee_type == -1) ? ' selected ' : '') . '>Válassz !</option>';
foreach ($fee_type as $key => $value) {
    $output .= '<option value=' . $key . '  ' . (($select_edit_fee_type == $key) ? ' selected ' : '') . '>'
            . $value[0] . '</option>';
}
$output .= '</select>';
$output .= '<br><br>';
$et_type = $select_edit_fee_type;

if (isset($delete)) {
    $sql = 'DELETE FROM `' . TYPES_TABLE . '` WHERE '
            . ' `' . TYPES_TYPE . '`=' . $et_type
            . ' AND `' . TYPES_TYPE_ID . '`=' . $delete . ';';
    $mysqliLink->query($sql);
    include INCLUDES . 'setup_load.inc';
}

if (isset($modify)) {
    $mod_name = 'name_' . $modify;
    $sql = 'UPDATE `' . TYPES_TABLE . '` SET '
            . '`' . TYPES_TYPE_NAME . '`="' . $$mod_name . '" '
            . ' WHERE `' . TYPES_TYPE . '`=' . $et_type
            . ' AND `' . TYPES_TYPE_ID . '`=' . $modify . ';';
    $mysqliLink->query($sql);
    include INCLUDES . 'setup_load.inc';
}

if (isset($add_new)) {
    $new_id = getNewId($et_type);
    $sql = 'INSERT INTO `' . TYPES_TABLE . '` '
            . '(`' . TYPES_TYPE_ID . '`, `' . TYPES_TYPE_NAME . '`, `' . TYPES_TYPE . '`) '
            . ' VALUES (' . $new_id . ', "Nevezd át !!!", ' . $et_type . ');';
    $mysqliLink->query($sql);
    include INCLUDES . 'setup_load.inc';
}


switch ($select_edit_fee_type) {
    case FEE_TYPE_MEMBERSHIP:
        $et_list = $member_types;
        break;
    case FEE_TYPE_INSURANCE:
        $et_list = $member_insurances;
        break;
    case FEE_TYPE_ORGANIZATION:
        $et_list = $organizations;
        break;
    case FEE_TYPE_CLUB:
        $et_list = $club_categories;
        break;
    case FEE_TYPE_ASSOCIATION:
        $et_list = $associations;
        break;
    case FEE_TYPE_ADMINISTRATION:
        $et_list = $administrations;
        break;
    case FEE_TYPE_OCCASION:
        $et_list = $occasions;
        break;
    case FEE_TYPE_LICENCES:
        $et_list = $licences;
        break;
    default:
        $et_list = array();
        break;
}
if ($et_type >= 0) {
    $output .= '<input type="submit" name="add_new" value="Új felvétele">&nbsp;&nbsp;&nbsp;&nbsp;';
    $output .= '<br><br>';

    foreach ($et_list as $key => $value) {
        $output .= $key . ':&nbsp;&nbsp;&nbsp;&nbsp;';
        $output .= '<input type="text" name="name_' . $key . '" value="' . $value . '">&nbsp;&nbsp;&nbsp;';
        $output .= '<button  type="submit" name="modify" value="' . $key . '" >'
                . '<img src="' . IMAGES . 'edit.jpg" title="Módosít"></button>';
        switch ($et_type) {
            case FEE_TYPE_LICENCES:
                if (isLicenceDeleteable($key)) {
                    $output .= '&nbsp;&nbsp;&nbsp;<button  type="submit" name="delete" value="' . $key . '" >'
                            . '<img src="' . IMAGES . 'delete.jpg" title="Töröl"></button>';
                }
                break;
            default:
                if (isFeeTypeDeleteable($et_type, $key)) {
                    $output .= '&nbsp;&nbsp;&nbsp;<button  type="submit" name="delete" value="' . $key . '" >'
                            . '<img src="' . IMAGES . 'delete.jpg" title="Töröl"></button>';
                }
                break;
        }
        $output .= '<br>';
    }
}

$output .= '</form></div>';
