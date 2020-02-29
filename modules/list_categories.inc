<?php

if (!isset($selected_cat)) {
    $selected_cat = -1;
}

$output .= '<br><div class = "lists"><form id="edit_categories" method="post" >';
$output .= '<h2>Kategóriák</h2><table><tr>';
$output .= '<th><select id = "selected_act" name = "selected_cat" onChange = "submit();">';
$output .= '<option value = "-1">Összes</option>';
foreach ($branches as $index => $branch) {
    $output .= '<option value = "' . $index . '" ' . ($index == $selected_cat ? 'selected' : '') . '>' . $branch . '</option>';
}
$output .= '</select></th>';
$output .= '<th>Hosszú név</th>';
$output .= '<th>Vezető</th>';
$output .= '<th>Rövid név</th>';
$output .= '<th>Szabályzatok</th></tr>';
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
    $output .= "</td>";
    $output .= '<td>' . $row[CATEGORIES_LONG] . "</td>";
    $output .= "<td>" . getNameOfCatLeaders($row[CATEGORIES_ID]) . "</td>";
    $output .= "<td>" . $row[CATEGORIES_SHORT] . "</td>";
    $output .= '<td>';
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

