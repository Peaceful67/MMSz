<?php

define("ADDR_CHK", "addr_chk_");
$selected_group = array(
    -1 => array("Válassz", "Kiknek az email címeit kéred"),
    0 => array("Egyesületi tisztek", "Vezetők, küldöttek, elnökök"),
    1 => array("Egyesületi küldöttek", "Csak küldöttek"),
    2 => array("Elnökségi tagok", getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " elnökségi tagok"),
    3 => array("Aktív tagok", "Akiknek van valamilyen szerepük bármely egyesületben"),
    4 => array("Rendezetlen tagok", "Aktív tagok, akiknek van előírt, de ki nem fizetett díjtételük, és van emailcímük"),
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
if (!isset($radio_news_letter)) {
    $radio_news_letter = "addresses";
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
while ($res AND $row = $res->fetch_assoc()) {
    $newsletters[$row[NEWSLETTER_ID]] = $row;
}
if (isset($resend)) {
    $email_subject = $newsletters[$resend][NEWSLETTER_SUBJECT];
    $email_body = $newsletters[$resend][NEWSLETTER_BODY];
    $selected_newsletter = -1;
    $radio_news_letter = "email";
}

$output .= '<form method="post">';
$output .= '<label>Hírlevelek: </label><select name="selected_newsletter" onChange="submit();">';
$output .= '<option value=-1 ' . (-1 == $selected_newsletter ? 'selected' : '') . '>Új hírlevél készítése</option>';
foreach ($newsletters as $key => $letter) {
    $output .= '<option value="' . $key . '" ' . ($selected_newsletter == $key ? "selected" : "") . '>'
            . $letter[NEWSLETTER_TIME] . ' / ' . $letter[NEWSLETTER_SUBJECT] . '</option>';
}
$output .= '</select>';
if ($selected_newsletter < 0) {
    $output .= '</br></br><label>Címzettek: </label><select name="selected_addr" onChange="submit();">';
    foreach ($selected_group as $key => $opt) {
        $output .= '<option value="' . $key . '" ' . ($selected_addr == $key ? "selected" : "") . '>' . $opt[0] . '</option>';
    }
    $output .= '</select>&nbsp;&nbsp;&nbsp;&nbsp;' . $selected_group[$selected_addr][1] . '</br>';
    $output .= 'Csak címek:<input type="radio" name="radio_news_letter" value="addresses" onChange="submit();" ' . (($radio_news_letter == "addresses") ? " checked " : "") . '>';
    $output .= '&nbsp;&nbsp;&nbsp;&nbsp;Email:<input type="radio" name="radio_news_letter" value="email" onChange="submit();" ' . (($radio_news_letter == "email") ? " checked " : "") . '><br>';
    $output .= '<br>';

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
    $output .= '<div class="email_address">';
    $output .= '<ul>';
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
        $output .= '<li><input type="checkbox" name="' . $chk . '" ' . $checked . ' "  onChange="submit();">'
                . '<a title="(' . $mem[MEMBER_ID] . ')  ' . $mem[MEMBER_EMAIL] . '">'
                . $mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV] . '</a></li>';
    }
    $output .= '</ul>';
    $output .= '</div><div = class="email_body">';
    $output .= 'Címzettek száma: ' . $selected_num . '<br><br>';
    if ($radio_news_letter == "addresses") {
        foreach ($all as $id) {
            $chk = ADDR_CHK . $id;
            if (isset($$chk) AND $ $chk == 'on') {
                $mem = retriveMember($id);
                $output .= $mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV] . ' < ' . trim($mem[MEMBER_EMAIL]) . ' >;';
            }
        }
        $output .= '<input type="hidden" name="email_body" value="' . $email_body . '">';
        $output .= '<input type="hidden" name="email_subject" value="' . $email_subject . '">';
    } else {
        $output .= '<input placeholder="Tárgy..." size="64" max_length="128" type="text" name="email_subject" value="' . $email_subject . '">&nbsp;&nbsp;&nbsp;&nbsp;';
        if ($selected_num > 0) {
            $output .= '<input type="submit" name="send_letter" value="Küldés" '
                    . 'onClick="return confirm(\'Biztosan ki akarsz küldeni  ' . $selected_num . ' darab emailt ?\')">';
        }
        $output .= '<br><br><textarea name="email_body" rows="36" cols="100">' . $email_body . '</textarea>';
    }
    $output .= '</div>';
    $output .= '<input type="hidden" name="old_selected_addr" value="' . $old_selected_addr . '">';
    $output .= '</form>';

    if (isset($send_letter)) {
        unset($send_letter);
        if (empty($email_subject)) {
            warning("Nem adtad meg a körlevél tárgyát !");
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
            warning($sent_num . ' darab körlevél kiküldése elindítva !');
        }
    }
} else { // List newsletters
    $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<button  name="resend" value=' . $selected_newsletter . '>Újra küldés</button></br></br>';
    $sql = 'SELECT * FROM `' . NLM_TABLE . '` WHERE `' . NLM_NEWSLETTER_ID . '`=' . $selected_newsletter . ' ;';
    $res = $mysqliLink->query($sql);
    $output .= '<label>Küldő:</label>' . getMemberName($newsletters[$selected_newsletter][NEWSLETTER_CREATOR]) . '<br><br>';
    $output .= '<label>Tárgy:</label>' . $newsletters[$selected_newsletter][NEWSLETTER_SUBJECT] . '<br><br>';
    $output .= '<label>Tartalom:</label>' . $newsletters[$selected_newsletter][NEWSLETTER_BODY] . '<br><br>';
    $output .= '<label>Címzettek:</label><br>';
    while ($res AND $row = $res->fetch_assoc()) {
        $mem_id = $row[NLM_MEMBER];
        $output .= getMemberName($row[NLM_MEMBER]) . ' / ' . getMemberEmail($row[NLM_MEMBER]) . ' : ' . (0 == strtotime($row[NLM_SENT]) ? 'Várakozik' : $row[NLM_SENT]) . '<br>';
    }
}

function retriveAddresses($sel) {
    global $mysqliLink;
    $ret = false;
    switch ($sel) {
        case 0:

            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' (`active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
                    . ' OR `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_PRESIDENT . '")'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '` ';
            $ret = $mysqliLink->query($sql);
            break;
        case 1:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '`';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' ( `active`.`' . ROLE_PERMISSION . '`="' . ACCESS_CLUB_DELEGATE . '" '
                    . ' )'
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '` ';
            $ret = $mysqliLink->query($sql);
            break;
        case 2:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM `' . MEMBER_TABLE . '`, `' . ROLE_TABLE . '` WHERE '
                    . '`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`'
                    . ' AND `' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`="' . ACCESS_MMSZ_PRESIDENT . '"'
                    . ' AND (CURDATE() BETWEEN `' . ROLE_TABLE . '`.`' . ROLE_FROM . '` AND `' . ROLE_TABLE . '`.`' . ROLE_TO . '`)';
            $ret = $mysqliLink->query($sql);
            break;
        case 3:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '` '
                    . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`!="' . ACCESS_PUBLIC . '" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '`!="" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '` ';
            $ret = $mysqliLink->query($sql);
            break;
        case 4:
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM ((`' . MEMBER_TABLE . '` '
                    . ' INNER JOIN `' . FM_TABLE . '` AS `paid_fm` ON ('
                    . ' `paid_fm`.`' . FM_PAY_ID . '`="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`paid_fm`.`' . FM_MEMEBER_ID . '` '
                    . ' AND (CURDATE() BETWEEN `paid_fm`.`' . FM_FROM . '` AND `paid_fm`.`' . FM_TO . '`))'
                    . ' INNER JOIN `' . FEE_TABLE . '` AS `paid_fee` ON ('
                    . ' `paid_fm`.`' . FM_FEE_ID . '`=`paid_fee`.`' . FEE_ID . '` '
                    . ' AND `paid_fee`.`' . FEE_TYPE . '`="' . FEE_TYPE_MEMBERSHIP . '" )'
                    . ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '` != "' . ACCESS_PUBLIC . '" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` = `active`.`' . ROLE_MEMBER . '` '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '` != "" '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`)))) ';
            $sql .= ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_EMAIL . '` ';
            $ret = $mysqliLink->query($sql);
            break;
    }
    return $ret;
}
