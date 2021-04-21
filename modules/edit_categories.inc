<?php


if (isset($save_category)) {
    if ($cat_branch < 0) {
        $view->danger("Válassz szakágat !");
    } elseif (empty($short_name)) {
        $view->danger("Legalább a rövid nevet add meg !");
    } else {
        if (isCategoryNameUsed($short_name, $cat_branch, $category_id)) {
            $view->danger("Ebben a szakágban ez a név már használatban van !");
        } else {
            if (isset($category_id)) {
                $sql = 'UPDATE ' . CATEGORIES_TABLE . ' SET ';
                $sql .= ' `' . CATEGORIES_SHORT . '` ="' . $short_name . '" ,';
                $sql .= ' `' . CATEGORIES_LONG . '` ="' . $long_name . '" ,';
                $sql .= ' `' . CATEGORIES_BRANCH . '` ="' . $cat_branch . '",';
                $sql .= ' `' . CATEGORIES_ORG . '` ="' . $cat_org . '"';
                $sql .= ' WHERE `' . CATEGORIES_ID . '`=' . $category_id;
                $mysqliLink->query($sql);
                logger($member_id, -1, LOGGER_CATEGORY_MOD, $category_id . " azonosítójú kategória adatmódosítása: ");
            } else {
                $sql = 'INSERT INTO ' . CATEGORIES_TABLE . ' ( ';
                $sql .= CATEGORIES_SHORT . ', ' . CATEGORIES_LONG . ', ' . CATEGORIES_BRANCH . ', ' . CATEGORIES_ORG . ') ';
                $sql .= ' VALUES ("' . $short_name . '", "' . $long_name . '", "' . $cat_branch . '", "' . $cat_org . '");';
                $mysqliLink->query($sql);
                $category_id = $mysqliLink->insert_id;
                logger($member_id, -1, LOGGER_CATEGORY_MOD, "Új kategória (" . $short_name . ") felvitele. Azonosító: " . $category_id);
            }
            $short_name = '';
            $long_name = '';
            $cat_branch = -1;
            $cat_org = -1;
        }
    }
    if (isset($_FILES["rule_hu_upload"]) AND ! empty($_FILES["rule_hu_upload"]["tmp_name"])) {
        if ($_FILES["rule_hu_upload"]["type"] == "application/pdf") {
            $file = RULES . '_' . $category_id . '_HU.pdf';
            move_uploaded_file($_FILES["rule_hu_upload"]["tmp_name"], $file);
            logger($member_id, -1, LOGGER_CATEGORY_MOD, "A " . $category_id . " azonosítójú kategóriához tartozó magyar szabályzat feltöltése");
        } else {
            $view->warning("A feltöltött fájl nem pdf !");
        }
    }
    if (isset($_FILES["rule_en_upload"])AND ! empty($_FILES["rule_en_upload"]["tmp_name"])) {
        if ($_FILES["rule_en_upload"]["type"] == "application/pdf") {
            $file = RULES . '_' . $category_id . '_EN.pdf';
            move_uploaded_file($_FILES["rule_en_upload"]["tmp_name"], $file);
            logger($member_id, -1, LOGGER_CATEGORY_MOD, "A " . $category_id . " azonosítójú kategóriához tartozó angol szabályzat feltöltése");
        } else {
            $view->warning("A feltöltött fájl nem pdf !");
        }
    }
    unset($category_id);
}

if (isset($new_category)) {
    $short_name = '';
    $long_name = '';
    $cat_branch = -1;
    $cat_org = -1;
    unset($category_id);
}

if (isset($delete_rule_hu)) {
    unlink(RULES . '_' . $delete_rule_hu . '_HU.pdf');
    logger($member_id, -1, LOGGER_CATEGORY_MOD, "A " . $category_id . " azonosítójú kategóriához tartozó magyar szabályzat törlése");
}
if (isset($delete_rule_en)) {
    unlink(RULES . '_' . $delete_rule_en . '_EN.pdf');
    logger($member_id, -1, LOGGER_CATEGORY_MOD, "A " . $category_id . " azonosítójú kategóriához tartozó angol szabályzat törlése");
}

if (isset($delete_category)) {
    $mysqliLink->query('DELETE FROM ' . CATEGORIES_TABLE . ' WHERE ' . CATEGORIES_ID . '=' . $delete_category);
    logger($member_id, -1, LOGGER_CATEGORY_MOD, "Kategória (" . $delete_category . ") törlése");
}

if (isset($edit_category)) {
    $res = $mysqliLink->query('SELECT * FROM ' . CATEGORIES_TABLE . ' WHERE ' . CATEGORIES_ID . '=' . $edit_category . ' LIMIT 1');
    if ($res) {
        $cat = $res->fetch_assoc();
        $short_name = $cat[CATEGORIES_SHORT];
        $long_name = $cat[CATEGORIES_LONG];
        $cat_branch = $cat[CATEGORIES_BRANCH];
        $cat_org = $cat[CATEGORIES_ORG];
        $category_id = $edit_category;
    }
}
if (!isset($short_name))
    $short_name = '';
if (!isset($long_name))
    $long_name = '';
if (!isset($cat_org))
    $cat_org = -1;
if (!isset($cat_branch))
    $cat_branch = -1;
if (!isset($selected_cat)) {
    $selected_cat = -1;
}
$leaded_brances = getBranchesOfLeader($member_id);
if (isPermitted(ACCESS_ADMIN) OR isPermitted(ACCESS_MMSZ) OR isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $leaded_brances = $branches;
}
if (empty($leaded_brances)) {
    $view->danger('Jogosulatlan hozzáférés !');
    return;
}
$view->putHidden('category_id');
$view->newKeret();
if (count($leaded_brances) == 1) {
    reset($leaded_brances);
    $cat_branch = key($leaded_brances);
    $view->putHidden('cat_branch');
    $view->putText($branches[$cat_branch], 'Szakág');
} else {
    $view->submitDropDownChooser('Szakág', [-1 => 'Válassz !!!'] + $branches, 'cat_branch', true);
    $view->submitDropDownChooser('Szervezet', [-1 => 'Nincs megadva'] + $organizations, 'cat_org', true);
}
$view->inputText('short_name', 'Rövid név');
$view->inputText('long_name', 'Hosszú név');

$view->inputFile('Magyar szabályzat', 'rule_hu_upload');
if (isset($category_id) AND is_file(RULES . '_' . $category_id . '_HU.pdf')) {
    $view->submitDelete('delete_rule_hu', $category_id, 'Magyar szabályzat törlése', 'Biztosan törölni akarod a szabályzatot ?');
}

$view->inputFile('Angol szabályzat', 'rule_en_upload');
if (isset($category_id) AND is_file(RULES . '_' . $category_id . '_EN.pdf')) {
    $view->submitDelete('delete_rule_en', $category_id, 'Angol szabályzat törlése', 'Biztosan törölni akarod a szabályzatot ?');
}

if (isset($edit_category)) {
    $view->submitButton('save_category', 'Módosítás');
    $view->submitButton('new_category', 'Új kategória');
} else {
    $view->submitButton('save_category', 'Felvitel');
}
$view->endDiv();
$view->putElement('<h2>Kategóriák</h2>');
$view->newDiv('table-responsive-sm');
$view->newElement('<table class="table table-striped table-bordered">');
$view->newTableTr();
$view->putElement('<th scope="col">');
if (count($leaded_brances) == 1) {
    reset($leaded_brances);
    $selected_cat = key($leaded_brances);
    $view->put($branches[$selected_cat]);
} else {
    $view->submitDropDownChooser('', [-1 => 'Összes'] + $leaded_brances, 'selected_cat', true);
}
$view->endElement('</th>');
$view->putElement('<th scope="col">Hosszú név</th>');
$view->putElement('<th scope="col">Rövid név</th>');
$view->putElement('<th scope="col">Műveletek</th>');
$view->endTableTr();
$sql = 'SELECT * FROM ' . CATEGORIES_TABLE . ' WHERE ';
if ($selected_cat < 0) {
    $sql .= ' 1 ';
} else {
    $sql .= ' `' . CATEGORIES_BRANCH . '` = "' . $selected_cat . '" ';
}
$sql .= ' ORDER BY ' . CATEGORIES_SHORT;
$res = $mysqliLink->query($sql);
while ($res AND $row = $res->fetch_assoc()) {
    $cat_id = $row [CATEGORIES_ID];
    $view->newTableTr();
    $view->putElement('<td>' . $branches[$row[CATEGORIES_BRANCH]]);
    if ($row[CATEGORIES_ORG] >= 0) {
        $view->put(', ');
        if (isset($organizations[$row[CATEGORIES_ORG]])) {
            $view->put($organizations[$row[CATEGORIES_ORG]]);
        }
    }
    $view->put('</td>');
    $view->putElement('<td>' . $row[CATEGORIES_LONG] . "</td>");
    $view->putElement("<td>" . $row[CATEGORIES_SHORT] . "</td>");
    $view->newElement('<td>');
    if (!isCategoryInUse($row[CATEGORIES_ID])) {
        $view->submitDelete('delete_category', $row[CATEGORIES_ID], 'Kategória törlése', 'Biztosan tölni akarod a kategóriát ?');
    }
    $view->submitEdit('edit_category', $row[CATEGORIES_ID]);
    $file_en = RULES . '_' . $cat_id . '_EN.pdf';
    $file_hu = RULES . '_' . $cat_id . '_HU.pdf';
    if (is_file($file_en)) {
        $view->viewEyeLink($file_en, 'Angol nyelvű szabályzat');
    } else {
        $view->viewEyeSlash('Nincs feltöltve angol nyelvű szabályzat');
    }
    if (is_file($file_hu)) {
        $view->viewEyeLink($file_hu, 'Magyar nyelvű szabályzat');
    } else {
        $view->viewEyeSlash('Nincs feltöltve magyar nyelvű szabályzat');
    }
    $view->endElement('</td>');
    $view->endTableTr();
}
$view->endElement('</table>');
$view->endDiv();
$view->endKeret();
