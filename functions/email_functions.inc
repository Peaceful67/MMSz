<?php

function mail_create_bill($msg) {
    send_mail("Fizetés indítása", $msg);
}

function mail_system_error($error) {
    global $member_id;
    logger($member_id, '---', LOGGER_SYSTEM_ERROR, $error);
    send_mail_to(ADMIN_EMAIL, 'Rendszerüzenet', $error);
}

function send_mail_via_phpmailer($addr, $subject, $body, $filename = "") {

    require_once CLASSES . 'PHPMailer/SMTP.php';
    require_once CLASSES . 'PHPMailer/PHPMailer.php';

    $my_mail = new \PHPMailer\PHPMailer\PHPMailer(false);
    $my_mail->IsSMTP();
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
    $my_mail->AddReplyTo('', '');
    $my_mail->SetFrom(EMAIL_SENDER, EMAIL_SIGNATURE);
    $my_mail->Subject = $subject;
    $my_mail->AltBody = 'Az üzenet megtekintéséhez használj HTML kompatibilis email klienst !';
    $my_mail->MsgHTML($body);
    $my_mail->AddAddress($addr);
    $my_mail->Encoding = 'base64';
    if (!empty($filename)) {
        $my_mail->AddAttachment($filename);
    }
    if (DEVELOPMENT) {
        $my_mail->AddAddress(ADMIN_EMAIL);
    }
    $my_mail->Send();
}

function send_mail_to($addr, $subject, $body, $filename = "") {
    global $member_id;
    $email_body = '<html><body style=\"font-family:Verdana, Verdana, Geneva, sans-serif; font-size:12px;\">';
    $email_body .= $body;
    $email_body .= "<br><br>Üdvözlettel:<br>" . EMAIL_SIGNATURE;
    $email_body .= "</body></html>\n";
    if (!isset($addr) OR empty($addr)) {
        mail_system_error('Nem létező emailcím !\r\nTárgy: ' . $subject . '\r\nTartalom: ' . $body);
        return;
    }
    $addr = trim($addr);
    send_mail_via_phpmailer($addr, $subject, $email_body, $filename);
    logger($member_id, $addr, LOGGER_EMAIL_SENT, $addr . ' címre, ' . $subject . ' tárgyú levél kiküldve');
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
    global $member_id;
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
    if (!isset($addr) OR empty($addr)) {
        error_log("Missing email address (send_notification)");
        return;
    }
    send_mail_to($addr, $subject, $formated_body);
}
