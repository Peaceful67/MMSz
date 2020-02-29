<?php

function mail_create_bill($msg) {
    send_mail("Fizetés indítása", $msg);
}

function mail_system_error($error) {
    global $member_id;
    logger($member_id, '---', LOGGER_SYSTEM_ERROR, $error);
    send_mail_to(getOptionValue(OPTIONS_NAME_ADMIN_EMAIL), 'Rendszerüzenet', $error);
}

function send_mail_via_phpmailer($addr, $subject, $body, $filename = "") {
    require_once CLASSES . 'PHPMailer/PHPMailer.php';
    require_once CLASSES . 'PHPMailer/SMTP.php';
    require_once CLASSES . 'PHPMailer/Exception.php';

    $my_mail = new \PHPMailer\PHPMailer\PHPMailer(true);
    // $my_mail->SMTPDebug = 4;
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
    $my_mail->AddAddress($addr);
    $my_mail->Encoding = 'base64';
    if (!empty($filename)) {
        $my_mail->AddAttachment($filename);
    }
    if (getOptionValue(OPTIONS_NAME_DEVELOPMENT) > 0) {
        $my_mail->AddAddress(getOptionValue(OPTIONS_NAME_ADMIN_EMAIL));
    }
    if (!$my_mail->Send()) {
        error_log('Error occurred during sending the email: ' . $my_mail->ErrorInfo);
    }
}

function send_mail_to($addr, $subject, $body, $filename = "") {
    global $member_id;
    $email_body = '<html><body style=\"font-family:Verdana, Verdana, Geneva, sans-serif; font-size:12px;\">';
    $email_body .= $body;
    $email_body .= "<br><br>Üdvözlettel:<br>" . getOptionValue(OPTIONS_NAME_EMAIL_SIGNATURE);
    $email_body .= "</body></html>\n";
    if (!isset($addr) OR empty($addr)) {
        mail_system_error('Nem létező emailcím !\r\nTárgy: ' . $subject . '\r\nTartalom: ' . $body);
        return;
    }
    $addr = trim($addr);
    send_mail_via_phpmailer($addr, $subject, $email_body, $filename);
    logger($member_id, -1, LOGGER_EMAIL_SENT, $addr . ' címre, ' . $subject . ' tárgyú levél kiküldve');
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
    $person_body = str_replace(REPLACE_NAME, getMemberName($id), $body);
    $formated_body = str_replace(NEWLINE, '<br>', $person_body);
    $addr = getMemberEmail($id);
    if (!isset($addr) OR empty($addr)) {
        return;
    }
    send_mail_to($addr, $subject, $formated_body);
}

function send_notification($id, $subject, $body) {
    global $member_id;
    $person_body = str_replace(REPLACE_NAME, getMemberName($id), $body);
    $formated_body = str_replace(NEWLINE, '<br>', $person_body);
    $addr = getMemberEmail($id);
    if(getOptionValue(OPTIONS_NAME_DEVELOPMENT)>0) {
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
    $max = $options[OPTIONS_NAME_MAX_NEWSLETTERS];
    updateOption(OPTIONS_NAME_LAST_NEWSLETTER, time());
    $sql = 'SELECT * FROM `' . NLM_TABLE . '` WHERE `' . NLM_SENT . '`= 0 '
            . 'LIMIT ' . $max . ';';
    $res = $mysqliLink->query($sql);
    if ($res AND $res->num_rows > 0) {
        while ($row = $res->fetch_assoc()) {
            $sql = 'SELECT *  FROM `' . NEWSLETTER_TABLE . '` WHERE `' . NEWSLETTER_ID . '`=' . $row[NLM_NEWSLETTER_ID] . ' ;';
            $nl_res = $mysqliLink->query($sql);
            if ($nl_res AND $nl_res->num_rows > 0) {
                $nl_row = $nl_res->fetch_assoc();
                $subject = $nl_row[NEWSLETTER_SUBJECT];
                $body = $nl_row[NEWSLETTER_BODY];
                send_newsletter($row[NLM_MEMBER], $subject, $body);
                $sql = 'UPDATE `' . NLM_TABLE . '` ' //Mark as sent                        
                        . 'SET `' . NLM_SENT . '`=CURRENT_TIMESTAMP '
                        . ' WHERE `' . NLM_ID . '`=' . $row[NLM_ID] . ';';
                $mysqliLink->query($sql);
            }
        }
    }
}
