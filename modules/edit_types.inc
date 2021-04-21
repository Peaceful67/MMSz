<?php


if (isset($select_edit_fee_type)) {
    $_SESSION[SESSION_FEETYPE2EDIT] = $select_edit_fee_type;
} else {
    $select_edit_fee_type = isset($_SESSION[SESSION_FEETYPE2EDIT]) ? $_SESSION[SESSION_FEETYPE2EDIT] : -1;
}
$view->newKeret();
$view->submitDropDownChooser('Típus', [-1 => 'Válassz!!!'] + $fee_type_names, 'select_edit_fee_type', true);
$et_type = $select_edit_fee_type;

if (isset($delete)) {
    $sql = 'DELETE FROM `' . TYPES_TABLE . '` WHERE '
            . ' `' . TYPES_TYPE . '`=' . $et_type
            . ' AND `' . TYPES_TYPE_ID . '`=' . $delete . ';';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_TYPES, $delete . ' azonosítójó tipus törölve.');
    include INCLUDES . 'setup_load.inc';
}

if (isset($modify)) {
    $mod_name = 'name_' . $modify;
    $sql = 'UPDATE `' . TYPES_TABLE . '` SET '
            . '`' . TYPES_TYPE_NAME . '`="' . $$mod_name . '" '
            . ' WHERE `' . TYPES_TYPE . '`=' . $et_type
            . ' AND `' . TYPES_TYPE_ID . '`=' . $modify . ';';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_TYPES, $modify . ' azonosítójó tipus módosítva.');
    include INCLUDES . 'setup_load.inc';
}

if (isset($add_new)) {
    $new_id = getNewId($et_type);
    $sql = 'INSERT INTO `' . TYPES_TABLE . '` '
            . '(`' . TYPES_TYPE_ID . '`, `' . TYPES_TYPE_NAME . '`, `' . TYPES_TYPE . '`) '
            . ' VALUES (' . $new_id . ', "Nevezd át !!!", ' . $et_type . ');';
    $mysqliLink->query($sql);
    logger($member_id, -1, LOGGER_TYPES, $add_new . $mysqliLink->insert_id . ' azonosítójó tipus létrehozva.');
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
    $view->submitAdd('add_new');

    foreach ($et_list as $key => $value) {
        $var_name = 'name_' . $key;
        $$var_name = $value;
        $view->newDiv('form-inline');
        $view->inputText($var_name, $key);
        $view->submitSave('modify', $key, 'Módosít');
        switch ($et_type) {
            case FEE_TYPE_LICENCES:
                if (isLicenceDeleteable($key)) {
                    $view->submitDelete('delete', $key);
                }
                break;
            default:
                if (isFeeTypeDeleteable($et_type, $key)) {
                    $view->submitDelete('delete', $key);
                }
                break;
        }
        $view->endDiv();
    }
}
$view->endKeret();
