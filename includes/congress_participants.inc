<?php

$edit_congress_members = $edit_participants;
if (isset($participant_exit)) {
    unset($edit_participants);
    return;
}
if (!isset($radio_participants)) {
    $radio_participants = 'invite';
}
$cOnline = new CongressOnline();
$cItem = new CongressItems();
$cOnline->setSelectedCongress($edit_participants);
$view->newKeret();
$view->putElement('<h3>Résztvevők szerkesztése</h3>');
$view->newDiv('row');
$view->submitBack('participant_exit', 'Visszatérés a részvevők szerkesztéséből');
$view->putText($cOnline->getCongressTitle(), 'Gyűlés címe', true);
$view->endDiv();
if ($cItem->getSelectionOfItemIdCongressClose($edit_participants)) {
    $view->warning('A gyűlésnek vannak lezárt tárgyalású napirendjei');
}

if ($radio_participants == 'delegate') {
    $view->info('(A tárgyaláson és a szavazáson is részt vesznek)');
    include_once INCLUDES . 'congress_members_delegate.inc';
} else {
    $view->info('(A meghívott személyek bármelyik még le nem zárt napirendi ponthoz hozzászólhatnak, de nem szavazhatnak)');
    include_once INCLUDES . 'congress_members_invited.inc';
}
$view->endKeret();
