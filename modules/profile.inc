<?php

if (isset($radio_edit_member)) {
    $_SESSION[SESSION_DATA2EDIT] = $radio_edit_member;
} else {
    $radio_edit_member = isset($_SESSION[SESSION_DATA2EDIT]) ? $_SESSION[SESSION_DATA2EDIT] : "base";
}

$view->info('<b>Tagsági szám: ' . $member_id . '</b>');
$view->newElement('<ul class="nav nav-tabs">');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'base' ? 'active' : '') . '" name="radio_edit_member" title="Tag alapadatai" value="base">Alap</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'fees' ? 'active' : '') . '" name="radio_edit_member" title="A nyitott és rendezett díjak áttekintése" value="fees">Díjak</button>');
$view->endElement('</li>');
$view->newElement('<li class="nav-item">');
$view->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($radio_edit_member == 'passwd' ? 'active' : '') . '" name="radio_edit_member" title="Jelszó modosítása" value="passwd">Jelszó</button>');
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
        $base_data_id = $member_id;
        include_once INCLUDES . 'base_data.inc';
        break;
    case "fees":
        $fee_member_id = $member_id;
        $deletable_fm = true;
        $editable_fees = array(FEE_TYPE_ORGANIZATION, FEE_TYPE_CLUB,
            FEE_TYPE_OCCASION);
        include_once INCLUDES . 'choose_fee_type.inc';
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

?>
