<?php

if (!isset($club_name))
    $club_name = '';
if (!isset($club_postcode))
    $club_postcode = '';
if (!isset($club_city))
    $club_city = '';
if (!isset($club_street))
    $club_street = '';
if (!isset($club_email))
    $club_email = '';
if (!isset($club_comment))
    $club_comment = '';
if (!isset($club_homepage))
    $club_homepage = '';
if (!isset($club_act))
    $club_act = '';
if (!isset($club_bank))
    $club_bank = '';
if (!isset($club_tax))
    $club_tax = '';
if (!isset($club_mandat))
    $club_mandat = 0;
if (!isset($club_mandat2))
    $club_mandat2 = 0;

if (isset($cancel)) {
    unset($edit_club);
    return;
}
if (isset($save_club)) {
    if (empty($club_name)) {
        $view->danger('Add meg az egyesület nevét !');
    } else {

        if ($edit_club < 0) {
            $sql = 'INSERT INTO `' . CLUB_TABLE . '` SET '
                    . ' `' . CLUB_NAME . '`="' . $club_name . '", '
                    . ' `' . CLUB_ADDR_POST . '`="' . $club_postcode . '", '
                    . ' `' . CLUB_ADDR_CITY . '`="' . $club_city . '", '
                    . ' `' . CLUB_ADDR_STREET . '`="' . $club_street . '", '
                    . ' `' . CLUB_EMAIL . '`="' . $club_email . '", '
                    . ' `' . CLUB_HOMEPAGE . '`="' . $club_homepage . '", '
                    . ' `' . CLUB_BANK . '`="' . $club_bank . '", '
                    . ' `' . CLUB_TAX . '`="' . $club_tax . '", '
                    . ' `' . CLUB_MANDATUM . '`="' . $club_mandat . '", '
                    . ' `' . CLUB_MANDATUM2 . '`="' . $club_mandat2 . '", '
                    . ' `' . CLUB_COMMENT . '`="' . $club_comment . '"; ';
            $mysqliLink->query($sql);
            $edit_club = $mysqliLink->insert_id;
            logger($member_id, -1, LOGGER_CLUB_EDIT, $edit_club . ' azonosítójú egyesület létrehozva.');
        } else {
            $sql = 'UPDATE `' . CLUB_TABLE . '` SET '
                    . ' `' . CLUB_NAME . '`="' . $club_name . '", '
                    . ' `' . CLUB_ADDR_POST . '`="' . $club_postcode . '", '
                    . ' `' . CLUB_ADDR_CITY . '`="' . $club_city . '", '
                    . ' `' . CLUB_ADDR_STREET . '`="' . $club_street . '", '
                    . ' `' . CLUB_HOMEPAGE . '`="' . $club_homepage . '", '
                    . ' `' . CLUB_EMAIL . '`="' . $club_email . '", '
                    . ' `' . CLUB_BANK . '`="' . $club_bank . '", '
                    . ' `' . CLUB_TAX . '`="' . $club_tax . '", '
                    . ' `' . CLUB_MANDATUM . '`="' . $club_mandat . '", '
                    . ' `' . CLUB_MANDATUM2 . '`="' . $club_mandat2 . '", '
                    . ' `' . CLUB_COMMENT . '`="' . $club_comment . '", '
                    . ' `' . CLUB_ACT . '`="' . (($club_act == 'on') ? '1' : '0') . '" '
                    . ' WHERE `' . CLUB_ID . '`="' . $edit_club . '"';
            $mysqliLink->query($sql);
            logger($member_id, -1, LOGGER_CLUB_EDIT, $edit_club . ' azonosítójú egyesület adatai módosítva.');
            if (isset($_FILES["document_upload"]) AND isset($feltolt)) {
                if ($_FILES["document_upload"]["type"] == "application/pdf") {
                    $file = DOCUMENTS . 'club_' . $edit_club . uniqid('_') . '.pdf';
                    move_uploaded_file($_FILES["document_upload"]["tmp_name"], $file);
                    logger($member_id, $edit_club, LOGGER_CLUB_EDIT, "A " . $edit_club . " egyesülethez tartozó új dokumentum feltöltése : " . $file);
                } else {
                    $view->danger("A feltöltött fájl nem pdf !");
                }
            }
        }
    }
}
if (isset($delete_document)) {
    $file = DOCUMENTS . $delete_document;
    unlink($file);
    logger($member_id, $edit_club, LOGGER_CLUB_EDIT, "A " . $edit_club . " egyesülethez tartozó dokumentum (" . $delete_document . ") törlése");
}



if ($edit_club > 0) {
    $res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE `' . CLUB_ID . '`=' . $edit_club . ' LIMIT 1');
    if ($res) {
        $club = $res->fetch_assoc();
        $club_name = $club[CLUB_NAME];
        $club_postcode = $club[CLUB_ADDR_POST];
        $club_city = $club[CLUB_ADDR_CITY];
        $club_street = $club[CLUB_ADDR_STREET];
        $club_email = $club[CLUB_EMAIL];
        $club_comment = $club[CLUB_COMMENT];
        $club_homepage = $club[CLUB_HOMEPAGE];
        $club_bank = $club[CLUB_BANK];
        $club_act = $club[CLUB_ACT] == 1 ? 'on' : 'off';
        $club_tax = $club[CLUB_TAX];
        $club_mandat = $club[CLUB_MANDATUM];
        $club_mandat2 = $club[CLUB_MANDATUM2];
        $club_leader = getClubLeaders($edit_club);
        $club_president = getClubPresidents($edit_club);
        $club_delegate = getClubDelegates($edit_club);
        $subclubs = unserialize($club[CLUB_SUBCLUBS]);
    }
}
$view->newKeret();
$view->inputText('club_name', 'Név');
$view->inputText('club_postcode', 'Irányítósz.');
$view->inputText('club_city', 'Város');
$view->inputText('club_street', 'Utca');
$view->inputText('club_email', 'Klub email');
$view->inputText('club_comment', 'Megjegyzés');
$view->inputText('club_homepage', 'Honlap');
$view->inputText('club_bank', 'Bankszámla');
$view->inputText('club_tax', 'Adószám');
$view->inputMemberId('club_mandat', 'Mandátum 1');
$view->inputMemberId('club_mandat2', 'Mandátum 2');
$view->inputCheckbox('club_act', 'Aktív');
if (isset($club_leader)) {
    $view->putText((strlen($club_leader) > 1) ? $club_leader : 'Nincs megadva !', 'Vezető');
}
if (isset($club_president)) {
    $view->putText((strlen($club_president) > 1) ? $club_president : 'Nincs megadva !', 'Elnök');
}
if (isset($club_delegate)) {
    $view->putText(((strlen($club_delegate) > 1) ? $club_delegate : 'Nincs megadva !') . delegatesNumberToString($edit_club), 'Küldött');
}
if (isPermitted(ACCESS_ADMIN) OR isPermitted(ACCESS_MMSZ_PRESIDENT) OR isPermitted(ACCESS_MMSZ) OR amILeaderOfClub($edit_club)) {
    $clubs = getClubNameList();
    $view->submitButton('edit_subclubs', 'Alszövetség', $edit_club, ((isset($subclubs)AND ! empty($subclubs)) ? "Szerkesztése" : "Egyesületek alásorolásával alszövetséggé alakítjuk ezt az egyesületet."));
    output_spaces(5);
    if (isset($subclubs) AND ! empty($subclubs)) {
        $view->putText(count($subclubs) . ' tagegyesület');
    }
}

$files = scandir(DOCUMENTS);
if (!empty($files)) {
    $needle = 'club_' . $edit_club . '_';
    $found = false;
    $view->newDiv('row');
    foreach ($files as $f) {
        if (substr($f, 0, strlen($needle)) == $needle) {
            $found = true;
            $view->putElement('<a target="_blank" href="' . DOCUMENTS . $f . '"><img class="m-1" style="width:30px;" src="' . IMAGES . 'pdf.jpg"></a>');
        }
    }
    if ($found) {
        $view->endDiv()->newDiv('row');
    }
    foreach ($files as $f) {
        if (substr($f, 0, strlen($needle)) == $needle) {
            $view->submitDelete('delete_document', $f, 'Törlés', 'Biztosan tölni akarod a dokumentumot ?');
        }
    }
    $view->endDiv();
}
$view->submitButton('save_club', 'Rögzít');
$view->submitButton('cancel', 'Vissza');

$view->newDiv('file-upload-wrapper');
$view->putElement('<label class="control-label">File feltöltés</label>');
$view->putElement('<input type="file" id="document_upload" name="document_upload" class="file-upload" />');
$view->endDiv();
$view->submitButton('feltolt', 'Feltöltés');
$view->endKeret();
