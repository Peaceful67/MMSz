<?php

if (!isset($add_invited)) {
    $add_invited = 0;
}
$cInvited = new CongressInvited();
$cEmailMember = new EmailMember();
if (isset($send_invite_email)) {
    $cInviteEmail = new Email(EMAIL_CONTEXT_ONLINE_INVITE, $edit_participants);
    $cInviteEmail->insertToBody(1, $congressOnline->getCongressTitle($edit_participants));
    $cInviteEmail->insertToBody(2, getMemberName($member_id));
    $cInviteEmail->sendEmail([$send_invite_email => getMemberName($send_invite_email)]);
    $view->success('Meghívó email kiküldve.');
}
if (isset($add_mem)) {
    if (isset((new CongressDelegates())->getMembersOfCongress($edit_participants)[$add_invited])) {
        $view->danger(getMemberName($add_invited) . ' (' . $add_invited . ') küldött a gyűlésen');
    } elseif (isset((new CongressInvited ())->getMembersOfCongress($edit_participants)[$add_invited])) {
        $view->danger(getMemberName($add_invited) . ' (' . $add_invited . ') már meghívott a gyűlésen');
    } elseif (!isValidMemberId($add_invited)) {
        $view->danger('Ezt a tagot nem lehet meghívni!');
    } elseif (!hasMemberEmail($add_invited)) {
        $view->danger('Nem érvényes a tag email címe ('.getMemberEmail($add_invited).'), nem lehet a gyűlésre meghívni!');
    } else {
        $cInvited->addMember($edit_participants, $add_invited);
        logger($member_id, $add_invited, LOGGER_CONGRESS_ONLINE, 'Tag meghívottként felvéve');
    }
}
if (isset($delete_invited)) {
    $cInvited->deleteMember($edit_participants, $delete_invited);
    logger($member_id, $delete_invited, LOGGER_CONGRESS_ONLINE, 'Tag meghívottak közül törölve');
}
$view->newDiv('row');
$view->inputMemberId('add_invited', 'Azonosító');
$view->submitSave('add_mem', 0, 'Tag felvétele meghívottként.');
$view->endDiv();
foreach ($cInvited->getMembersOfCongress($edit_participants) as $mem_id => $mem_name) {
    $view->newDiv('row');
    $view->submitDelete('delete_invited', $mem_id, $mem_name . ' részvételének törlése');
    if (!$cEmailMember->isAlreadySent(EMAIL_CONTEXT_ONLINE_INVITE, $edit_participants, $mem_id)) {
        $view->submitEmail('send_invite_email', $mem_id, 'Meghívó email kiküldése a résztvevőnek a a tárgyalásra.');
    }
    $view->putText($mem_name, $mem_id);
    $view->endDiv();
}
