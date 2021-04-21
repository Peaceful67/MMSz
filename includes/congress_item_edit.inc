<?php

$view->submitSave('save_item', 0, "Napirendi pont szövegének mentése");
$item_title = $congressItems->getCongressItemTitle();
$view->inputText('item_title', 'Cím', 'Napirend címe');
$item_description = $congressItems->getCongressItemDescription();
$view->inputTextArea('item_description', 'Leírás', 'Napiren részletes leírása');
/*
  output_ln();
  output_label('Melléklet');
  if (is_file($doc_file)) {
  output_spaces(5);
  $output .= '<a target="_blank" href="' . URL . $doc_file . '">Megtekintés</a>';
  output_spaces(4);
  $output .= '<button  title="Melléklet törlése" name="delete_item_doc"><img src="' . IMAGES . 'delete.jpg"></button>';
  } else {
  $output .= '<input  type="file" name="congress_item_file"  id="congress_item_file" />';
  }
 * 
 */

$view->putElement('<hr><h4>Beállítások:</h4>');
$selection_possible_dependencies = $congressItems->getSelectionOfCongress();
unset($selection_possible_dependencies[$selected_congress_item]); // Saját magát kiveszem
if (!empty($selection_possible_dependencies)) {
    $multichk_item_dependencies = $congressItems->getDependency();
    $view->putLabel('Csak ezek után szavazható');
    foreach ($selection_possible_dependencies as $dep_item_id => $dep_item_name) {
        $view->inputMultiCheckboxInline('multichk_item_dependencies', $dep_item_id, substr($dep_item_name, 0, 20) . '...', true, $dep_item_name);
    }
}
$option_chk_byname = $congressItems->isItemByName() ? 'on' : 'off';
$comment_byname = $congressItems->isItemByName() ? 'Nyilvános, hogy ki hogyan szavazott.' : 'Titkos szavazás, a rendszer nem tárolja, ki hogyan szavazott.';
$view->inputCheckboxCommented('option_chk_byname', 'Név szerinti szavazás', true, $comment_byname);
$option_chk_byemail = $congressItems->isItemByEmail() ? 'on' : 'off';
$comment_by_email = $congressItems->isItemByEmail() ? 'Emailben kapott linken keresztül is lehet szavazni.' : 'Az emailben szavazás nem lehetséges.';
$view->inputCheckboxCommented('option_chk_byemail', 'Email szavazás', true, $comment_by_email);
$option_chk_yes_no = $congressItems->isItemYesNo() ? 'on' : 'off';
$comment_yes_no = $congressItems->isItemYesNo() ? 'Eldöntendő tipusú szavazás' : 'Több választási lehetőség.';
$view->inputCheckboxCommented('option_chk_yes_no', 'Típus', true, $comment_yes_no);

if (!$congressItems->isItemYesNo()) {
    $option_chk_multichoice = $congressItems->isItemMultichoice() ? 'on' : 'off';
    $comment_multichoice = $congressItems->isItemMultichoice() ? 'A szavazás jellege megengedi több opció bejelölését is.' : 'A lehetőségek közül csak egyet lehet választani';
    $view->inputCheckboxCommented('option_chk_multichoice', 'Több választási lehetőség', true, $comment_multichoice);
}
$option_chk_abstain = $congressItems->isItemAbstain() ? 'on' : 'off';
$comment_abstain = $congressItems->isItemAbstain() ? 'Lehetőség van tartózkodásra a szavazás során.' : 'A tartózkodás nem választható.';
$view->inputCheckboxCommented('option_chk_abstain', 'Tartózkodás', true, $comment_abstain);
$view->putLabel('Határozati többség');
$item_option = $congressItems->getItemOption();
$decision_radio = CONGRESS_OPTIONS_DECISION_50;
foreach ($decision_options as $decision_opt => $decision_name) {
    if (($item_option & $decision_opt) > 0) {
        $decision_radio = $decision_opt;
    }
    $view->submitRadioInline('decision_radio', $decision_name, $decision_opt, '');
}




