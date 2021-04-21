<?php

if (!isset($selected_cat)) {
    $selected_cat = -1;
}

$view->newDiv('table-responsive-sm');
$view->newElement('<table class="table table-striped table-bordered">');
$view->newTableTr();
$view->newElement('<th>')->submitDropDownChooser('', [-1 => 'Összes'] + $branches, 'selected_cat', true)->endElement('</th>');
$view->putElement('<th>Hosszú név</th>');
$view->putElement('<th>Vezető</th>');
$view->putElement('<th>Rövid név</th>');
$view->putElement('<th>Szabályzatok</th>');
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
    $view->put("</td>");
    $view->putElement('<td>' . $row[CATEGORIES_LONG] . "</td>");
    $view->putElement("<td>" . getNameOfCatLeaders($row[CATEGORIES_ID]) . "</td>");
    $view->putElement("<td>" . $row[CATEGORIES_SHORT] . "</td>");
    $view->newElement('<td>');
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
$view->endElement('</table>')->endDiv();

