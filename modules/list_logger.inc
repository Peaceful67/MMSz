<?php

if (!isset($member)) {
    $member = 0;
}
if (!isset($related)) {
    $related = 0;
}
if (!isset($selected_act)) {
    $selected_act = -1;
}
if (!isset($logger_from)) {
    $logger_from = date('Y-m-d', strtotime($today.' -1 month'));
}
if (!isset($logger_to)) {
    $logger_to = $today;
}

$output .= '<div class="keret"><form id="list_method" method="post">';
$output .= '<label >Elkövető: </label>
            <input type="number" name="member"  size="5" value="' . $member . '"><br><br>';
$output .= '<label >Érintet: </label>
            <input type="number" name="related" value="' . $related . '"><br><br>';
$output .= '<label >Művelet: </label><select id="selected_act" name="selected_act" onChange="submit();">';
$output .= '<option value="-1">Válassz műveletet !</option>';
foreach ($logger_categories as $cat_id => $cat_name) {
    $output .= '<option value="' . $cat_id . '" ' . (($selected_act == $cat_id) ? " selected " : "") . '>' . $cat_name . '</option>';
}
$output .= '</select><br><br>';
$output .= '<label >Időszak: </label>
            <input type="date" name="logger_from"  value="' . $logger_from . '" >
            <input type="date" name="logger_to"  value="' . $logger_to . '">'
        . '&nbsp;&nbsp;&nbsp;<input type="submit" name="submit" value="Keresés">';
$output .= '</div><div class="logger_list"> <br><table border="1"><caption>Aktivitás listázása</caption>';
$output .= '<tr><th>Elkövető</th><th>Érintett tag</th><th>Művelet</th><th>Időpont</th></tr>';
$sql = 'SELECT * FROM `logs` WHERE '
        . '(`time_stamp` BETWEEN "'.$logger_from.' 00:00:00" AND "'.$logger_to.' 23:59:59" ) ';
if ($member > 0) {
    $sql .= ' AND`member`="' . $member . '" ';
}
if ($related > 0) {
    $sql .= ' AND `related_id`="' . $related . '" ';
}
if ($selected_act > 0) {
    $sql .= ' AND `log_category`="' . $selected_act . '" ';
}
$sql .= ' ORDER BY `time_stamp` DESC LIMIT 300';
$res = $mysqliLink->query($sql);
while ($res AND $row = $res->fetch_assoc()) {
    $mid = $row["member"];
    $output .= "<tr>";

    $output .= '<td><a title="' . getMemberName($row["member"]) . '">' . $row["member"] . '</a> </td>';
    if ($row["related_id"] > 0) {
        $output .= '<td><a title="' . getMemberName($row["related_id"]) . '">' . $row["related_id"] . '</a> </td>';
    } else {
        $output .= '<td>---</td>';
    }
    $output .= '<td><a title="'.$logger_categories[$row["log_category"]].'">'.$row["activity"] . '</a></td>';
    $output .= '<td><small>' . $row["time_stamp"] . '</small></td>';
    $output .= "</tr>";
}
$output .= "</table></div></form>";
