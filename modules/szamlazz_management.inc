<?php

 if (getOptionValue(OPTIONS_NAME_SZAMLAZZ_ENA)<1) {
    warning('A számlázó modul nincs engedélyezve !');
    theEnd($output);
    exit(0);
}

if (!isset($list_from)) {
    $list_from = date('Y-m-d', strtotime($today . ' -1 month'));
}
if (!isset($list_to)) {
    $list_to = $today;
}

$output .= '<div class="keret"><form id="list_method" method="post">';

$sql = 'SELECT * FROM `' . SZAMLAZZ_TABLE . '` '
        . ' WHERE (`' . SZAMLAZZ_PREPARED . '` BETWEEN "' . $list_from . ' 00:00:00" AND "' . $list_to . ' 23:59:59")'
        . ' ORDER BY `' . SZAMLAZZ_PREPARED . '` DESC ';
$res = $mysqliLink->query($sql);
$output .= '<label >Időszak: </label>
            <input type="date" name="list_from"  value="' . $list_from . '" >
            <input type="date" name="list_to"  value="' . $list_to . '">'
        . '&nbsp;&nbsp;&nbsp;<input type="submit" name="submit" value="Keresés">';
$output .= '</div>';
if ($res->num_rows > 0) {
    $output .= '<div class="logger_list"> <br><table border="1"><caption>Elkészült számlák listázása</caption>';
    $output .= '<tr><th>Azonosító</th><th>Vevő</th><th>Összeg</th><th>Előkészítve</th><th>Kiállítva</th></tr>';

    while ($row = $res->fetch_assoc()) {
        $output .= '<tr><td>' . $row[SZAMLAZZ_BILL_NUMBER] . '</td>';
        $output .= '<td>' . $row[SZAMLAZZ_NAME] . '</td>';
        $output .= '<td>' . getSumOfBills($row[SZAMLAZZ_BILL_NUMBER]) . ' Ft</td>';
        $output .= '<td>' . $row[SZAMLAZZ_PREPARED] . '</td>';
        $output .= '<td>' . ($row[SZAMLAZZ_DONE]>0 ? $row[SZAMLAZZ_DONE]:"Nincs kiállítva!") . '</td>';
        $output .= '</tr>';
    }
    $output .= "</table></div>";
} else {
    warning('Nem készült számla ebben az időszakban !');
}
$output .= '</form>';