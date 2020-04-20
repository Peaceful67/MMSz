<?php

output_ln();
output_label('Cím');
$output .= '<input type="text" size="80" maxlength="80"  name="item_title" value="' . $item_title . '">';
output_ln();
output_label('Leírás');
$output .= '<textarea rows="5" cols="100" maxlength="10000" name="item_description">' . $item_description . '</textarea>';
output_ln();
output_label('Csak ezek után szavazható');
$selected_congress_item_dependencies = $congressItems->getDependency();
output_multiple_selector($congressItems->getSelectionOfCongress(), 'congress_item_dependencies');
output_ln();
output_label('Név szerinti szavazás');
output_set_checkbox('option_chk_byname', $congressItems->isItemByName());
output_spaces(4);
o('Emailben kapott linken keresztül is lehet szavazni.');
output_ln();
output_label('Mindenki láthatja');
output_set_checkbox('option_chk_public', $congressItems->isItemPublic());
output_spaces(4);
o('A napirendi pont kiírása és a szavazás végeredménye nyilvános.');
output_ln();
output_label('Emailben is lehet szavazni');
output_set_checkbox('option_chk_byemail', $congressItems->isItemByEmail());
output_spaces(4);
o('Emailben kapott linken keresztül is lehet szavazni.');
output_ln();
output_label('Több választási lehetőség');
output_set_checkbox('option_chk_multichoice', $congressItems->isItemMultichoice());
output_spaces(4);
o('A szavazás jellege megengedi több opció bejelölését is.');
output_ln();
output_label('Melléklet');

if (is_file($doc_file)) {
    output_spaces(5);
    $output .= '<a target="_blank" href="' . URL . $doc_file . '">Megtekintés</a>';
    output_spaces(4);
    $output .= '<button title="Melléklet törlése" name="delete_item_doc"><img src="' . IMAGES . 'delete.jpg"></button>';
} else {
    $output .= '<input type="file" name="congress_item_file"  id="congress_item_file" />';
}
output_ln(2);
$output .= '<button title="Napirendi pont mentése" name="save_item"><img src="' . IMAGES . 'save.jpg"></button>';
