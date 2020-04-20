<?php

if (isset($save_category)) {
    if ($cat_branch < 0) {
        warning("Válassz szakágat !");
    } elseif (empty($short_name)) {
        warning("Legalább a rövid nevet add meg !");
    } else {
        if (isCategoryNameUsed($short_name, $cat_branch, $category_id)) {
            warning("Ebben a szakágban ez a név már használatban van !");
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
            warning("A feltöltött fájl nem pdf !");
        }
    }
    if (isset($_FILES["rule_en_upload"])AND ! empty($_FILES["rule_en_upload"]["tmp_name"])) {
        if ($_FILES["rule_en_upload"]["type"] == "application/pdf") {
            $file = RULES . '_' . $category_id . '_EN.pdf';
            move_uploaded_file($_FILES["rule_en_upload"]["tmp_name"], $file);
            logger($member_id, -1, LOGGER_CATEGORY_MOD, "A " . $category_id . " azonosítójú kategóriához tartozó angol szabályzat feltöltése");
        } else {
            warning("A feltöltött fájl nem pdf !");
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

$output .= '<div class="keret"><div class="editor"><form id="edit_categories" method="post" enctype="multipart/form-data">';
if (isset($category_id)) {
    $output .= '<input type="hidden" name="category_id" value="' . $category_id . '">';
}
$leaded_brances = getBranchesOfLeader($member_id);
if(isPermitted(ACCESS_ADMIN) OR isPermitted(ACCESS_MMSZ) OR isPermitted(ACCESS_MMSZ_PRESIDENT)) {
    $leaded_brances = $branches;
}
if (empty($leaded_brances)) {
    warning('Jogosulatlan hozzáférés !');
    return;
} elseif (count($leaded_brances) == 1) {
    reset($leaded_brances);
    $cat_branch = key($leaded_brances);
    $output .= '<input type="hidden" name="cat_branch" value="' . $cat_branch . '">';
    $output .= '<label>Szakág: </label>' . $branches[$cat_branch] . '<br>';
} else {
    $output .= '<label>Szakág: </label><select name="cat_branch">';
    $output .= '<option value = "-1">Válassz !</option>';
    foreach ($branches as $index => $branch) {
        if (isset($leaded_brances[$index])) {
            $output .= '<option value="' . $index . '" ' . ($index == $cat_branch ? 'selected' : '') . '>' . $branch . '</option>';
        }
    }
    $output .= '</select><br>';
    $output .= '<label>Szervezet: </label><select name="cat_org">';
    $output .= '<option value = "-1">Válassz !</option>';
    foreach ($organizations as $index => $org) {
        $output .= '<option value="' . $index . '" ' . ($index == $cat_org ? 'selected' : '') . '>' . $org . '</option>';
    }
    $output .= '</select><br>';
}
$output .= '<label for="short_name">Rövid név :</label>';
$output .= '<input type="text" size="8" name="short_name" value="' . $short_name . '"><br>';
$output .= '<label for="long_name">Hosszú név :</label>';
$output .= '<input type="text" size="32" name="long_name" value="' . $long_name . '"><br>';
$output .= '<label>Szabályzat magyar:</label>';
if (isset($category_id) AND is_file(RULES . '_' . $category_id . '_HU.pdf')) {
    $output .= '<button  type="submit" name="delete_rule_hu" value="' . $category_id . '" onClick="return confirm(\'Biztosan tölni akarod a szabályzatot ?\')"><img src="' . IMAGES . 'delete.jpg" title="Törlés"></button>';
}

$output .= '<input type="file" name="rule_hu_upload"/>';
$output .= '<br><label>Szabályzat angol:</label>';
if (isset($category_id) AND is_file(RULES . '_' . $category_id . '_EN.pdf')) {
    $output .= '<button  type="submit" name="delete_rule_en" value="' . $category_id . '" onClick="return confirm(\'Biztosan tölni akarod a szabályzatot ?\')"><img src="' . IMAGES . 'delete.jpg" title="Törlés"></button>';
}

$output .= '<input type="file" name="rule_en_upload"/>';
$output .= '<br>';
if (isset($edit_category)) {
    $output .= '<input type = "submit" name = "save_category" value = "Módosítás">';
    $output .= '&nbsp;&nbsp;&nbsp;<input type = "submit" name = "new_category" value = "Új kategória">';
} else {
    $output .= '<input type = "submit" name = "save_category" value = "Felvitel">';
}
$output .= '</div></div>';
$output .= '<br><div class = "lists">';
$output .= '<h2>Kategóriák</h2><table><tr>';
$output .= '<th>';
if (empty($leaded_brances)) {
    warning('Jogosulatlan hozzáférés');
} elseif (count($leaded_brances) == 1) {
    reset($leaded_brances);
    $selected_cat = key($leaded_brances);
    $output .= $branches[$selected_cat];
} else {
    $output .= '<select name = "selected_cat" onChange = "submit();">';
    $output .= '<option value = "-1">Összes</option>';
    foreach ($branches as $index => $branch) {
        if (isset($leaded_brances[$index])) {
            $output .= '<option value = "' . $index . '" ' . ($index == $selected_cat ? 'selected' : '') . '>' . $branch . '</option>';
        }
    }
    $output .= '</select>';
}
$output .= '</th>';
$output .= '<th>Hosszú név</th>';
$output .= '<th>Rövid név</th>';
$output .= '<th>Műveletek</th></tr>';
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
    $output .= "<tr>";
    $output .= '<td>' . $branches[$row[CATEGORIES_BRANCH]];
    if ($row[CATEGORIES_ORG] >= 0) {
        $output .= ', ';
        if (isset($organizations[$row[CATEGORIES_ORG]])) {
            $output .= $organizations[$row[CATEGORIES_ORG]];
        }
    }
    $output .= '</td>';
    $output .= '<td>' . $row[CATEGORIES_LONG] . "</td>";
    $output .= "<td>" . $row[CATEGORIES_SHORT] . "</td>";
    $output .= '<td>';
    if (!isCategoryInUse($row[CATEGORIES_ID])) {
        $output .= '<button type = "submit" name = "delete_category" value = "' . $row[CATEGORIES_ID] . '" onClick = "return confirm(\'Biztosan tölni akarod a kategóriát ?\')"><img src = "' . IMAGES . 'delete.jpg" title = "Törlés"></button>';
    }
    $output .= '<button type = "submit" name = "edit_category" value = "' . $row[CATEGORIES_ID] . '"><img src = "' . IMAGES . 'edit.jpg" title = "Szerkesztés"></button>';
    $file_en = RULES . '_' . $cat_id . '_EN.pdf';
    $file_hu = RULES . '_' . $cat_id . '_HU.pdf';
    if (is_file($file_en)) {
        $output .= '<a title = "Angol nyelvű szabályzat" target = "_blank" href = "' . $file_en . '"><img src = "' . IMAGES . 'pdf.jpg"></a>';
    } else {
        $output .= '<img title = "Nincs feltöltve angol nyelvű szabályzat" src = "' . IMAGES . 'empty.jpg">';
    }
    if (is_file($file_hu)) {
        $output .= '<a title = "Magyar nyelvű szabályzat" target = "_blank" href = "' . $file_hu . '"><img src = "' . IMAGES . 'pdf.jpg"></a>';
    } else {
        $output .= '<img title = "Nincs feltöltve magyar nyelvű szabályzat" src = "' . IMAGES . 'empty.jpg">';
    }
    $output .= "</td></tr>";
}
$output .= '</table></form></div>';
