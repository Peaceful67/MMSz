<?php


define("ADDR_CHK", "addr_chk_");
$selected_group = array(
    -1 => array("Válassz", "Kiknek az email címeit kéred"),
    0 => array("Egyesületi tisztek", "Vezetők, küldöttek, elnökök"),
    1 => array("Egyesületi vezetők", "Az egyesületek vezetői"),
    2 => array("Egyesületi küldöttek", "Csak küldöttek"),
    3 => array("Elnökségi tagok", getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " elnökségi tagok"),
    4 => array("Aktív tagok", "Akiknek van valamilyen szerepük bármely egyesületben"),
    5 => array("Rendezetlen tagok", "Aktív tagok, akiknek van előírt, de ki nem fizetett díjtételük, és van emailcímük"),
);
if (isset($_SESSION[SESSION_FILTERED_NEWSLETTER]) AND ! empty($_SESSION[SESSION_FILTERED_NEWSLETTER])) {
    $filtered = unserialize($_SESSION[SESSION_FILTERED_NEWSLETTER]);
    unset($_SESSION[SESSION_FILTERED_NEWSLETTER]);
} else {
    $filtered = array();
}

if (!isset($selected_addr)) {
    $selected_addr = -1;
}
if (!isset($selected_newsletter)) {
    $selected_newsletter = -1;
}
if (!isset($old_selected_addr)) {
    $old_selected_addr = $selected_addr;
}
if (!isset($email_subject)) {
    $email_subject = '';
}
if (!isset($email_body)) {
    $email_body = 'Tisztelt   ' . REPLACE_NAME . ' !';
}
$sql = 'SELECT * FROM `' . NEWSLETTER_TABLE . '` ;';
$res = $mysqliLink->query($sql);
$newsletters = array();
$newsletter_total = array();
while ($res AND $row = $res->fetch_assoc()) {
    $newsletters[$row[NEWSLETTER_ID]] = $row[NEWSLETTER_SUBJECT];
    $newsletters_total[$row[NEWSLETTER_ID]] = $row;
}
if (isset($resend)) {
    $email_subject = $newsletters_total[$resend][NEWSLETTER_SUBJECT];
    $email_body = $newsletters_total[$resend][NEWSLETTER_BODY];
    $selected_newsletter = -1;
    $radio_news_letter = "off";
}
$view->newDiv('row');
$view->submitDropDownChooser('Hírlevelek', [-1 => 'Új hírlevél készítése'] + $newsletters, 'selected_newsletter');
if ($selected_newsletter < 0) {
    $view->submitDropDownChooser('Címzettek', $selected_group, 'selected_addr');
    $view->putModalToggler('modal_addr', 'Egyéni', 'A címzett tagok egyéni kijelölése, megtekintése');
    $view->endDiv();
    $view->submitSwitchCheckbox('chk_only_addresses', 'Címek lekérdezése', 'Körlevél küldés', 'Művelet');
    $sql = 'SELECT * FROM `' . MEMBER_TABLE . '` WHERE `' . MEMBER_EMAIL . '`!="" ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`';
    $res = $mysqliLink->query($sql);
    $all = array();
    while ($res AND $row = $res->fetch_assoc()) {
        $all[] = $row[MEMBER_ID];
        if (in_array($row[MEMBER_ID], $filtered)) {
            $chk = ADDR_CHK . $row[MEMBER_ID];
            $$chk = 'on';
        }
    }
    if ($old_selected_addr != $selected_addr) {
        foreach ($all as $id) {
            $chk = ADDR_CHK . $id;
            unset($$chk);
        }
        $old_selected_addr = $selected_addr;
        $res = retriveAddresses($selected_addr);
        if ($res) {
            while ($row = $res->fetch_assoc()) {
                $chk = ADDR_CHK . $row[MEMBER_ID];
                $$chk = 'on';
            }
        }
    }
    $view->newModalMembers('modal_addr', 'A kijelölt címzett tagok');
    $view->newElement('<ul>');
    $selected_num = 0;
    foreach ($all as $id) {
        $mem = retriveMember($id);
        $chk = ADDR_CHK . $id;
        if (isset($$chk) AND $ $chk == 'on') {
            $checked = 'checked';
            $selected_num++;
        } else {
            $checked = '';
        }
        $view->newElement('<li class="list-group-item">');
        $view->putElement('<input type="checkbox" name="' . $chk . '" ' . $checked . ' "  onChange="submit();" class="mx-1">'
                . '<a data-toogle="tooltip" title="Szül:' . $mem[MEMBER_BORN] . '">'
                . $mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV] . '  (' . $mem[MEMBER_ID] . ') / ' . $mem[MEMBER_EMAIL] . '</a>');
        $view->endElement('</li>');
    }
    $view->endElement('</ul>');
    $view->endModal();
    $view->info('Címzettek száma: ' . $selected_num);
    if ($chk_only_addresses == "on") {
        foreach ($all as $id) {
            $chk = ADDR_CHK . $id;
            if (isset($$chk) AND $ $chk == 'on') {
                $mem = retriveMember($id);
                $view->putElement($mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV] . ' < ' . trim($mem[MEMBER_EMAIL]) . ' >;');
            }
        }
        $view->putHidden('email_body');
        $view->putHidden('email_subject');
    } else {
        $view->newDiv('row');
        $view->inputText('email_subject', '', 'Körlevél tárgya...');
        if ($selected_num > 0) {
            $view->submitButton('send_letter', 'Küldés', -1, 'Körlevelek kiküldése', 'Biztosan ki akarsz küldeni  ' . $selected_num . ' darab emailt ?');
        }
        $view->endDiv();
        $view->inputTextArea('email_body', '');
        $view->info(REPLACE_NAME.' : a címzett neve<br> '.REPLACE_ID.' : a címzett '. getOptionValue(OPTIONS_NAME_COMPANY_NAME).' azonosítója.');
    }
    $view->putHidden('old_selected_addr');
    if (isset($send_letter)) {
        unset($send_letter);
        if (empty($email_subject)) {
            $view->danger("Nem adtad meg a körlevél tárgyát !");
        } else {
            $sent_num = 0;
            $news_id = create_newsletter($email_subject, $email_body);
            foreach ($all as $id) {
                $chk = ADDR_CHK . $id;
                if (isset($$chk) AND $ $chk == 'on') {
                    push_newsletter($id, $news_id);
                    // send_newsletter($id, $email_subject, $email_body);
                    $sent_num++;
                }
            }
            start_sending_newsletters();
            $view->success($sent_num . ' darab körlevél kiküldése elindítva !');
        }
    }
} else { // List newsletters
    $view->submitButton('resend', 'Újra küldés', $selected_newsletter, 'A már egyszer kiküldött email ismételt elküldése');
    $view->endDiv();
    $view->putText(getMemberName($newsletters_total[$selected_newsletter][NEWSLETTER_CREATOR]), 'Küldő');
    $view->putText($newsletters_total[$selected_newsletter][NEWSLETTER_SUBJECT], 'Tárgy');
    $view->putText($newsletters_total[$selected_newsletter][NEWSLETTER_BODY], 'Tartalom');
    $prev_addr = '';
    $sql = 'SELECT * FROM `' . NLM_TABLE . '` WHERE `' . NLM_NEWSLETTER_ID . '`=' . $selected_newsletter . ' ;';
    $res = $mysqliLink->query($sql);
    while ($res AND $row = $res->fetch_assoc()) {
        $mem_id = $row[NLM_MEMBER];
        $prev_addr .= getMemberName($row[NLM_MEMBER]) . ' / ' . getMemberEmail($row[NLM_MEMBER]) . ' : ' . (0 == strtotime($row[NLM_SENT]) ? 'Várakozik' : $row[NLM_SENT]) . '<br>';
    }
    $view->putText($prev_addr, 'Címzettek');
}

