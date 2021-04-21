<?php

if (!isset($congress_title)) {
    $congress_title = '';
}
if (!isset($congress_description)) {
    $congress_description = '';
}
$congress_title = $congressOnline->getCongressTitle();
$congress_description = $congressOnline->getCongressDescription();

$view->putPrevent();
$view->inputText('congress_title', 'Cím', 'A gyűlés címe');
$view->inputTextArea('congress_description', 'Leírás', 'A gyűlés leírása');
$view->submitSave('congress_congress_save', 0, 'Gyűlés szövegének mentése');

$view->putElement('<hr><h3>Beállítások</h3>');
$option_chk_byname = $congressOnline->isCongressByName() ? 'on' : 'off';
$comment_by_name = $congressOnline->isCongressByName() ? 'Nyilvános, hogy ki hogyan szavazott.' : 'Titkos szavazás, a rendszer nem tárolja, ki hogyan szavazott.';
$view->inputCheckboxCommented('option_chk_byname', 'Név szerinti szavazás', true, $comment_by_name);

$option_chk_public = $congressOnline->isCongressPublic() ? 'on' : 'off';
$comment_public = $congressOnline->isCongressPublic() ? 'A gyűlés minden részlete nyilvános.' : 'A gyűlést csak az abban résztvevők látják.';
$view->inputCheckboxCommented('option_chk_public', 'Mindenki láthatja', true, $comment_public);

$option_chk_byemail = $congressOnline->isCongressByEmail() ? 'on' : 'off';
$comment_by_email = $congressOnline->isCongressByEmail() ? 'Emailben kapott linken keresztül is lehet szavazni.' : 'Az emailben szavazás nem lehetséges.';
$view->inputCheckboxCommented('option_chk_byemail', 'Email szavazás', true,$comment_by_email );

$option_chk_appendable = $congressOnline->isCongressAppendable() ? 'on' : 'off';
$comment_appendable = $congressOnline->isCongressAppendable() ? 'Napirendek tárgyalása közben lehetőség van új napirend felvételére.' : 'Csak a kiírásban szereplő napirendek tárgyalása és szavazása.';
$view->inputCheckboxCommented('option_chk_appendable', 'Utólagos napirend', true, $comment_appendable);

$option_chk_auto_item = $congressOnline->isCongressAutoItem() ? 'on' : 'off';
$comment_auto_item = $congressOnline->isCongressAutoItem() ? 'A napirendek és hitelesítők elfogadásához. Minden más napirendet megelőz.' : 'Nem jön létre.';
$view->inputCheckboxCommented('option_chk_auto_item', 'Automatikus napirend', true,$comment_auto_item);

if (!$congressOnline->isCongressAutoItem()) {
    $option_chk_auth_mand = $congressOnline->isCongressAuthMandatory() ? 'on' : 'off';
    $comment_auth_mand = $congressOnline->isCongressAuthMandatory() ? 'Kötelező megadni két jegyzőkönyv hitelesítőt.' : 'Nem kötelező a jegyzőkönyvhitelesítő.';
    $view->inputCheckboxCommented('option_chk_auth_mand', 'Jegyzőkönyv hitelesítő', true, $comment_auth_mand);
}
$option_chk_base_voted = $congressOnline->isCongressBaseVoted() ? 'on' : 'off';
$comment_base_voted = $congressOnline->isCongressBaseVoted() ? 'A szavazók száma.': 'A gyűlésre meghívottak száma.';
$view->inputCheckboxCommented('option_chk_base_voted', 'Szavazat számlálás alapja', true, $comment_base_voted);

$mandate_radio = $congressOnline->getMandateType();
$view->putLabel('Szavazat számlálás módja');
foreach ($mandate_names as $type => $name) {
    $view->submitRadioInline('mandate_radio', $name['short'], $type, $name['desc']);
}


