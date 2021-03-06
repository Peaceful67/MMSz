<?php

function mail_create_bill($msg) {
    send_mail("Fizetés indítása", $msg);
}

function mail_system_error($error) {
    global $member_id;
    logger($member_id, '---', LOGGER_SYSTEM_ERROR, $error);
    send_mail_to(getOptionValue(OPTIONS_NAME_ADMIN_EMAIL), 'Rendszerüzenet', $error);
}

function hasMemberEmail($mem_id) {
    $email_addr = getMemberEmail($mem_id);
    return strlen($email_addr)>0 && filter_var($email_addr, FILTER_VALIDATE_EMAIL);
}

function send_mail_via_phpmailer($addr, $subject, $body, $filename = "") {
    require_once CLASSES . 'PHPMailer/PHPMailer.php';
    require_once CLASSES . 'PHPMailer/SMTP.php';
    require_once CLASSES . 'PHPMailer/Exception.php';

    $my_mail = new \PHPMailer\PHPMailer\PHPMailer(true);
//    $my_mail->SMTPDebug = \PHPMailer\PHPMailer\SMTP::DEBUG_SERVER;
    $my_mail->IsSMTP();
    $my_mail->SMTPOptions = array(
        'ssl' => array(
            'verify_peer' => false,
            'verify_peer_name' => false,
            'allow_self_signed' => true
        )
    );
//$my_mail->isSendmail();
    $my_mail->Host = "localhost";
    $my_mail->Port = 25;
    $my_mail->CharSet = 'utf-8';
    $my_mail->Username = SMTP_USER;
    $my_mail->Password = SMTP_PASSWD;
    if (empty($my_mail->Username)) {
        $my_mail->SMTPAuth = false;
    } else {
        $my_mail->SMTPAuth = true;
    }
    $my_mail->AddReplyTo(getOptionValue(OPTIONS_NAME_EMAIL_REPLY), getOptionValue(OPTIONS_NAME_EMAIL_SIGNATURE));
    $my_mail->SetFrom(getOptionValue(OPTIONS_NAME_EMAIL_SENDER), getOptionValue(OPTIONS_NAME_EMAIL_SIGNATURE));
    $my_mail->Subject = $subject;
    $my_mail->AltBody = 'Az üzenet megtekintéséhez használj HTML kompatibilis email klienst !';
    $my_mail->MsgHTML($body);
    $my_mail->Encoding = 'base64';
    if (!empty($filename)) {
        $my_mail->AddAttachment($filename);
    }
    if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
        $my_mail->AddAddress(getOptionValue(OPTIONS_NAME_ADMIN_EMAIL));
//        $my_mail->action_function = 'send_email_callback';
//    $my_mail->AddAddress($addr);
    } else {
        $my_mail->AddAddress($addr);
    }
    try {
        if (!$my_mail->Send()) {
            error_log('Error occurred during sending the email: ' . $my_mail->ErrorInfo);
        }
    } catch (Exception $e) {
        warning('Rendszer hiba: Levél küldés sikertelen(' . $e->getMessage() . ')');
        error_log('Mailer ErrorInfo:' . $my_mail->ErrorInfo);
    }
}

function send_email_callback($result, $to, $cc, $bcc, $subject) {
    error_log('send_email_callback: result:' . ($result ? 'sikeres' : 'sikertelen') . ', subject:' . $subject);
}

function send_mail_to($addr, $subject, $body, $filename = "") {
    global $member_id;
    global $email_member;
    if (!isset($email_member)) {
        $email_member = -1;
    }
    if (!filter_var($addr, FILTER_VALIDATE_EMAIL)) {
        $subject .= ' Hiás email cím :' . $addr;
        $addr = getOptionValue(OPTIONS_NAME_ADMIN_EMAIL);
    }
    $email_body = '<html><body style=\"font-family:Verdana, Verdana, Geneva, sans-serif; font-size:12px;\">';
    $email_body .= $body;
    $email_body .= "<br><br>Üdvözlettel:<br>" . getOptionValue(OPTIONS_NAME_EMAIL_SIGNATURE);
    $email_body .= "</body></html>\n";
    if (!isset($addr) OR empty($addr)) {
        mail_system_error('Nem létező emailcím :' . $addr . '<br>Tárgy: ' . $subject . '<br>Tartalom: ' . $body);
        return;
    }
    $addr = trim($addr);
    send_mail_via_phpmailer($addr, $subject, $email_body, $filename);
    logger($member_id, $email_member, LOGGER_EMAIL_SENT, $addr . ' címre, ' . $subject . ' tárgyú levél kiküldve');
}

function send_mail($subject, $body) {
    global $member_id;
    $addr = getMemberEmail($member_id);
    if (!isset($addr) OR empty($addr)) {
        error_log("Missing email address (send_mail)");
        return;
    }
    send_mail_to($addr, $subject, $body);
}

function send_newsletter($id, $subject, $body) {
    global $email_member;
    $person_body = str_replace(REPLACE_NAME, getMemberName($id), $body);
    $id_body = str_replace(REPLACE_ID, $id, $person_body);
    $formated_body = str_replace(NEWLINE, '<br>', $id_body);
    $addr = getMemberEmail($id);
    $email_member = $id;
    if (!isset($addr) OR empty($addr)) {
        return;
    }
    send_mail_to($addr, $subject, $formated_body);
}

function send_notification($id, $subject, $body) {
    $person_body = str_replace(REPLACE_NAME, getMemberName($id), $body);
    $formated_body = str_replace(NEWLINE, '<br>', $person_body);
    $addr = getMemberEmail($id);
    if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
        $addr = getOptionValue(OPTIONS_NAME_ADMIN_EMAIL);
    }
    if (!isset($addr) OR empty($addr)) {
        error_log("Missing email address (send_notification)");
        return;
    }
    send_mail_to($addr, $subject, $formated_body);
}

function push_newsletter($mem_id, $news_id) {
    global $mysqliLink;
    $sql = 'INSERT INTO `' . NLM_TABLE . '` '
            . '(`' . NLM_MEMBER . '`, `' . NLM_NEWSLETTER_ID . '`, `' . NLM_SENT . '`)'
            . ' VALUES (' . $mem_id . ', ' . $news_id . ', "0") ;';
    $mysqliLink->query($sql);
}

function create_newsletter($subject, $body) {
    global $mysqliLink, $member_id;
    $sql = 'INSERT INTO `' . NEWSLETTER_TABLE . '` '
            . '(`' . NEWSLETTER_CREATOR . '`, `' . NEWSLETTER_SUBJECT . '`, `' . NEWSLETTER_BODY . '`)'
            . ' VALUES ("' . $member_id . '", "' . $subject . '", "' . $body . '")';
    $mysqliLink->query($sql);
    return $mysqliLink->insert_id;
}

function start_sending_newsletters() {
    global $options;
    global $mysqliLink;
    global $member_id;
    $max = $options[OPTIONS_NAME_MAX_NEWSLETTERS];
    $sql = 'SELECT * FROM `' . NLM_TABLE . '` WHERE `' . NLM_SENT . '`= 0 '
            . 'LIMIT ' . $max . ';';
    $current = $member_id;
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        updateOption(OPTIONS_NAME_LAST_NEWSLETTER, time());
        while ($row = $res->fetch_assoc()) {
            $sql = 'SELECT *  FROM `' . NEWSLETTER_TABLE . '` WHERE `' . NEWSLETTER_ID . '`=' . $row[NLM_NEWSLETTER_ID] . ' ;';
            $nl_res = $mysqliLink->query($sql);
            if ($nl_res AND $nl_res->num_rows > 0) {
                $nl_row = $nl_res->fetch_assoc();
                $subject = $nl_row[NEWSLETTER_SUBJECT];
                $body = $nl_row[NEWSLETTER_BODY];
                $member_id = $nl_row[NEWSLETTER_CREATOR]; // A tenyleges kuldo legyen a logban az elkövető
                send_newsletter($row[NLM_MEMBER], $subject, $body);
                $sql = 'UPDATE `' . NLM_TABLE . '` ' //Mark as sent                        
                        . 'SET `' . NLM_SENT . '`=CURRENT_TIMESTAMP '
                        . ' WHERE `' . NLM_ID . '`=' . $row[NLM_ID] . ';';
                $mysqliLink->query($sql);
            }
        }
    }
    $member_id = $current;
}

function sending_emails() {
    global $options;

    $max = $options[OPTIONS_NAME_MAX_NEWSLETTERS];
    $emailMember = new EmailMember();
    $email = new Email(EMAIL_CONTEXT_DUMMY, -1);
    $comm = new Command(Command::EMAIL_VOTE);
    for ($i = 1; $i < $max; $i++) {
        if (!$emailMember->retriveUnsentEmail()) {
            break;
        }
        updateOption(OPTIONS_NAME_LAST_NEWSLETTER, time());
        $mem_id = $emailMember->getMemberId();
        $addr = getMemberEmail($mem_id);
        $email->getEmailById($emailMember->getEmailId());
        $filename = $email->getFilename();
        $subject = $email->getEmailSubject();
        $body = $email->getEmailBody();
        $email_body = '<html><body style=\"font-family:Verdana, Verdana, Geneva, sans-serif; font-size:12px;\">';
        $email_body .= $email->changeNewLines($body);
        $email_body .= "<br><br>" . getOptionValue(OPTIONS_NAME_EMAIL_SIGNATURE);
        $email_body .= "</body></html>\n";
        $person_body = str_replace(REPLACE_NAME, getMemberName($mem_id), $email_body);
        $id_body = str_replace(REPLACE_ID, $mem_id, $person_body);
        $formated_body = str_replace(NEWLINE, '<br>', $id_body);
        $matches = [];
        preg_match_all(REPLACE_COMMAND_REGEXP, $formated_body, $matches);
        if (!empty($matches)) {
            foreach ($matches[0] as $match) {
                $pattern_code = substr($match, 3, -3);
                $replace_code = $comm->putItemByPattern($mem_id, $pattern_code);
                $formated_body = str_replace($match, $replace_code, $formated_body);
            }
        }
        if (!isset($addr) OR empty($addr)) {
            error_log("Missing email address (send_mail)");
            mail_system_error('Nem létező emailcím !\r\nTárgy: ' . $subject . '\r\nTartalom: ' . $formated_body);
            continue;
        }
        send_mail_via_phpmailer($addr, $subject, $formated_body, $filename);
        $emailMember->setSent();
        logger(-1, $mem_id, LOGGER_EMAIL_SENT, $addr . ' címre, ' . $subject . ' tárgyú levél kiküldve');
    }
}
