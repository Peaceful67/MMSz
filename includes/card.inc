<?php


if (isset($download_member)) {
    $mem_id = $download_member;
    $file = '.' . AVATAR . $mem_id . '.jpg';
    $mem = retriveMember($mem_id);
    if (!is_file($file)) {
        $file = "./" . ((retriveMember($mem_id)[MEMBER_SEX] == "F") ?
                PATH_IMAGES . 'female.jpg' :
                PATH_IMAGES . 'male.jpg');
    }

    $avatar = imagecreatetruecolor(CARD_AVATAR_WIDTH, CARD_AVATAR_HEIGHT);
    $source = imagecreatefromjpeg($file);
    $temp_file = tempnam(TMP_DIR, "_") . ".jpg";
    $check = getimagesize($file);
    list($width, $height) = $check;
    imagecopyresized($avatar, $source, 0, 0, 0, 0, CARD_AVATAR_WIDTH, CARD_AVATAR_HEIGHT, $width, $height);
    imagejpeg($avatar, $temp_file, 100);

    $stylesheet = file_get_contents('design/card.css');
    $body = '<body>';
    $body .= '<div class="avatar">';
    $body .= '<img src="' . $temp_file . '"></div>';

    $body .= '<div class="mmsz_hun"><b>MAGYAR MODELLEZŐ SZÖVETSÉG</b></div>'
            . '<div class="mmsz_eng">HUNGARIAN ASSOCIATION OF MODELLERS</div>';

    $body .= '<div class="name">';
    $body .= getMemberName($mem_id) . ' : ' . getId($mem_id);
    $body .= '</div><div class="data">';
    $body .= getMemberClubNames($mem_id);
    if ($mem[MEMBER_LICENCE] != '') {
        $body .= '&nbsp;&nbsp;&nbsp;FAI:&nbsp;' . $mem[MEMBER_LICENCE];
    }
    $body .= '</div><div class="year">' . $this_year . '</div>';
    $body .= '</body>';

    require_once MPDF . 'vendor/autoload.php';
    $pdf = new \Mpdf\Mpdf(
            [
        'tempDir' => TMP_DIR,
        'mode' => '+aCJK',
        'margin_left' => 1,
        'margin_right' => 1,
        'margin_top' => 1,
        'margin_bottom' => 1,
        'margin_header' => 1,
        'margin_footer' => 1
            ]
    );
    $pdf->img_dpi = $pdf->dpi = CARD_IMG_DPI;
    $pdf->nonPrintMargin = '0';
    $pdf->WriteHTML($stylesheet, 1);
    $pdf->WriteHTML($body);
    $pdf->Output('card_' . $mem_id . '_' . date("Y-m-d_H-i-s") . '.pdf', 'D');
    unlink($temp_file);
    logger($member_id, $mem_id, LOGGER_QUERY, "Tag " . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " kártyájának letöltése");
    exit();
}
$view->newKeret();
if (isPermitted(ACCESS_MMSZ | ACCESS_CLUBLEADER | ACCESS_ADMIN | ACCESS_MMSZ_PRESIDENT)) {
    if (!isset($mem_id)) {
        $mem_id = $member_id;
    }
    $view->newDiv('row');
    $view->inputMemberId('mem_id', 'Azonosító');
    $view->submitButton('require_member', 'Lekérdezés');
    $view->endDiv();
} else {
    $mem_id = $member_id;
}
if (isPaidMembership($mem_id)) {
    $view->submitButton('download_member', 'Letöltés', $mem_id);
}
if (isValidMemberId($mem_id)) {
    $mem = retriveMember($mem_id);
    $view->putText(getMemberName($mem_id),'Név', true );
    $view->putText(($mem[MEMBER_BORN] > 0 ? $mem[MEMBER_BORN] : 'Nincs kitöltve'), 'Született', true);
    $view->putText(getMemberClubNames($mem_id), 'Tagság', true );
    $club_id = getDelegateClub($mem_id);
    if ($club_id > 0) {
        $view->putText(getDelegateClubNames($mem_id)
                . ' ' . delegatesNumberToString($club_id), 'Küldött', true);
    }
    $view->putText(getListMemberCategories($mem_id), 'Kategóriák', true);
    $view->putText($this_year . ' évre ' . (isPaidMembership($mem_id) ? "befizetve" : "nincs rendezve"), getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' tagdíj' , true);
    if ($mem[MEMBER_LICENCE] != "") {
        $view->putText($mem[MEMBER_LICENCE], 'Licensz', true);
    }
    $avatar = '.' . AVATAR . $mem_id . '.jpg';
    if (is_file($avatar)) {
        $mtime = filemtime($avatar);
        $view->putElement('<img class="show_card" name="avatar"  src="' . AVATAR_URL . $mem_id . '.jpg?' . $mtime . '"/>');
    } else {
        if (retriveMember($mem_id)[MEMBER_SEX] == "F") {
            $view->putElement('<img class="show_card" name="avatar"  src="' . IMAGES . 'female.jpg"/>');
        } else {
            $view->putElement('<img class="show_card" name="avatar" src="' . IMAGES . 'male.jpg"/>');
        }
    }
}
$view->endKeret();
