<?php


$sql = 'SELECT * FROM `' . SETUP_GROUP_TABLE . '`  ORDER BY `' . SETUP_GROUP_WEIGHT . '` ASC;';
$main_menu_group = array();
$result = $mysqliLink->query($sql);
if ($result) {
    while ($row = mysqli_fetch_assoc($result)) {
        $main_menu_group[] = $row;
    }
}

$sql = 'SELECT * FROM `' . SETUP_MOD_TABLE . '`     WHERE 1 ORDER BY `' . SETUP_MOD_WEIGHT . '` ;';
$admin_menu = array();
$result = $mysqliLink->query($sql);
if ($result) {
    while ($row = mysqli_fetch_assoc($result)) {
        $admin_menu[$row[SETUP_MOD_ID]] = $row;
    }
}

$branches = getBranches();

$sql = 'SELECT * FROM `' . TYPES_TABLE . '` ;';
$organizations = array();
$associations = array();
$member_insurances = array();
$member_types = array();
$administrations = array();
$club_categories = array();
$licences = array();
$occasions = array();

$res = $mysqliLink->query($sql);
if ($res && $res->num_rows > 0) {
    while ($row = mysqli_fetch_assoc($res)) {
        switch ($row[TYPES_TYPE]) {
            case FEE_TYPE_MEMBERSHIP:
                $member_types[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
            case FEE_TYPE_INSURANCE:
                $member_insurances[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
            case FEE_TYPE_ORGANIZATION;
                $organizations[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
            case FEE_TYPE_CLUB:
                $club_categories[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
            case FEE_TYPE_ADMINISTRATION:
                $administrations[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
            case FEE_TYPE_OCCASION:
                $occasions[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
            case FEE_TYPE_ASSOCIATION:
                $associations[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
            case FEE_TYPE_LICENCES:
                $licences[$row[TYPES_TYPE_ID]] = $row[TYPES_TYPE_NAME];
                break;
        }
    }
} else {
    error_log("Ures tabla");
}
if (empty($member_types)) {
    $member_types = fill_type($default_member_types, FEE_TYPE_MEMBERSHIP);
}
if (empty($member_insurances)) {
    $member_insurances = fill_type($default_member_insurances, FEE_TYPE_INSURANCE);
}
if (empty($organizations)) {
    $organizations = fill_type($default_organizations, FEE_TYPE_ORGANIZATION);
}
if (empty($associations)) {
    $associations = fill_type($default_associations, FEE_TYPE_ASSOCIATION);
}
if (empty($administrations)) {
    $administrations = fill_type($default_administrations, FEE_TYPE_ADMINISTRATION);
}
if (empty($occasions)) {
    $occasions = fill_type($default_occasions, FEE_TYPE_OCCASION);
}
if (empty($club_categories)) {
    $club_categories = fill_type($default_club_categories, FEE_TYPE_CLUB);
}
if (empty($licences)) {
    $licences = fill_type($default_licences, FEE_TYPE_LICENCES);
}

