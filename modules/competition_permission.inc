<?php

if (isset($download_member)) {
    if (!isset($mem_id)) {
        $mem_id = $member_id;
    }
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
    $born = strlen($mem[MEMBER_BIRTH_CITY]) > 0 ? ($mem[MEMBER_BIRTH_CITY] . ', ') : '';
    $born .= $mem[MEMBER_BORN] > 0 ? $mem[MEMBER_BORN] : 'Nincs kitöltve';
    $branch = getBranchesNameOfMember($mem_id);
    $stylesheet = file_get_contents('design/comp_perm.css');
    $body = '<body>';
    $body .= '<div class="avatar">';
    $body .= '<img src="' . $temp_file . '"></div>';
    $body .= '<div class="branch">Sportág:' . $branch . '</div>';
    $body .= '<div class="mmsz_hun"><b>' . getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . '</b><br>Versenyengedély</div>';
    // $body .= '<div class="mmsz_id">Azonosító:' . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' - ' . $mem_id . '</b></div>';
    $body .= '<div class="born">Szül:' . $born . '</div>';
    $body .= '<div class="valid">Érvényes:' . $endOfYear . '</div>';
    $body .= '<div class="name">';
    $body .= getMemberName($mem_id) . ' :' . getId($mem_id);
    $body .= '</div><div class="club">';
    $body .= getMemberClubNames($mem_id);
    $body .= '</div><div class="created">Kiállítva:' . $today . '</div>';
    if (defined('COMPANY_COMP_PERM_EXTENSION')) {
        $body .= '<div class="extension">' . COMPANY_COMP_PERM_EXTENSION . '</div>';
    }
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
    $pdf->Output('comp_perm_' . $mem_id . '_' . date("Y-m-d_H-i-s") . '.pdf', 'D');
    unlink($temp_file);
    logger($member_id, $mem_id, LOGGER_QUERY, "Tag " . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " versenyengedélyének letöltése");
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

if (isValidMemberId($mem_id)) {
    $view->submitButton('download_member', 'Letöltés');
    $mem = retriveMember($mem_id);
    $view->putText(getMemberName($mem_id), 'Név', true);
    $view->putText((strlen($mem[MEMBER_BIRTH_CITY]) > 0 ? ($mem[MEMBER_BIRTH_CITY] . ', ') : '') . ($mem[MEMBER_BORN] > 0 ? $mem[MEMBER_BORN] : 'Nincs kitöltve'), 'Született', true);
    $view->putText(getMemberClubNames($mem_id), 'Tagság', true);
    $view->putText(getListMemberCategories($mem_id) . (defined('COMPANY_COMP_PERM_EXTENSION') ? COMPANY_COMP_PERM_EXTENSION : ''), 'Sportág', true);
    $view->putText($endOfYear, 'Érvénes', true);
    $view->putText(getId($mem_id), 'Azonosító', true);
    $view->putText($today, 'Kiállítva', true);
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
} else {
    $view->danger('Érvénytelen azonosító!');
}
$view->endKeret();

