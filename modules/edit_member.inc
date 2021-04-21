<?php

$found_member_ids = unserialize($_SESSION[SESSION_FOUND_MEMBERS]);
if (!isset($selected_item)) {
    $selected_item = (isset($_SESSION[SESSION_SELECTED_ITEM])) ? $_SESSION[SESSION_SELECTED_ITEM] : 0;
}
if ($selected_item != $_SESSION[SESSION_SELECTED_ITEM]) {
    unset($_SESSION[SESSION_SELECTED_CLUB]);
    unset($selected_club);
    unset($selected_branch);
}
if (isset($exit_edit_member)) {
    unset($_SESSION[SESSION_FOUND_MEMBERS]);
    redirect(get_back());
}
if (isset($radio_edit_member)) {
    $_SESSION[SESSION_DATA2EDIT] = $radio_edit_member;
} else {
    $radio_edit_member = isset($_SESSION[SESSION_DATA2EDIT]) ? $_SESSION[SESSION_DATA2EDIT] : "base";
}


$found_total = count($found_member_ids);
$edit_member = $found_member_ids[$selected_item];
$_SESSION[SESSION_SELECTED_ITEM] = $selected_item;
if (isset($found_member_ids) && $found_total > 1) {
    $view->newElement('<nav aria-label="search_members">');
    $view->newElement('<ul class="pagination m-2">');
    $view->newElement('<li class="page-item">');
    $view->putElement('<button class="page-link" name="selected_item" id="selected_item" value="0" title="Első oldal"><i class="fa fa-angle-double-left" aria-hidden="true"></i><span class="sr-only">Első</span></button>');
    $view->endElement('</li>');
    $view->newElement('<li class="page-item">');
    $view->putElement('<button class="page-link" name="selected_item" id="selected_item" value="' . max(0, $selected_item - 1) . '" title="Balra"><i class="fa fa-angle-left" aria-hidden="true"></i><span class="sr-only">Balra</span></button>');
    $view->endElement('</li>');
    $view->newElement('<li class="page-item">');
    $view->putElement('<p class="page-link">' . ($selected_item + 1) . '/' . count($found_member_ids) . '</p>');
    $view->endElement('</li>');
    $view->newElement('<li class="page-item">');
    $view->putElement('<button class="page-link" name="selected_item" id="selected_item" value="' . min($found_total - 1, $selected_item + 1) . '" title="Jobbra"><i class="fa fa-angle-right" aria-hidden="true"></i><span class="sr-only">Jobbra</span></button>');
    $view->endElement('</li>');
    $view->newElement('<li class="page-item">');
    $view->putElement('<button class="page-link" type="submit" name="selected_item" id="selected_item" value="' . ($found_total - 1) . '" title="Balra"><i class="fa fa-angle-double-right" aria-hidden="true"></i><span class="sr-only">Utolsó</span></button>');
    $view->endElement('</li>');
    $view->newElement('<li class="page-item">');
    $view->submitButton('exit_edit_member', 'Kilépés');
    $view->endElement('</li>');
    $view->endElement('</ul>');
    $view->endElement('</nav>');
}

$view->info(getMemberName($edit_member) . ' (' . $edit_member . ')');
$view->putHidden('edit_member');

$view->newElement('<ul class="nav nav-tabs">');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'base' ? 'active' : '') . '" name="radio_edit_member" title="Tag alapadatai" value="base">Alap</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'fees' ? 'active' : '') . '" name="radio_edit_member" title="A nyitott és rendezett díjak áttekintése" value="fees">Díjak</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'roles' ? 'active' : '') . '" name="radio_edit_member" title="Betöltött szerepek áttekintése, módosítása" value="roles">Szerepek</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'categories' ? 'active' : '') . '" name="radio_edit_member" title="Szakágak és azon belüli kategóriák beállítása" value="categories">Kategóriák</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'licences' ? 'active' : '') . '" name="radio_edit_member" title="Különböző jogosítások beállítása" value="licences">' . $fee_type[FEE_TYPE_LICENCES][0] . '</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'avatar' ? 'active' : '') . '" name="radio_edit_member" title="Profilhoz tartozó arckép feltöltése" value="avatar">Arckép</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'documents' ? 'active' : '') . '" name="radio_edit_member" title="Publikus PDF állaományok feltöltése" value="documents">Fájlok</button>');
$view->endElement('</li>');
$view->endElement('</ul>');

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

