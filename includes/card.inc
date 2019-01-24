<?php

if (isset($download_member)) {
    downloadCardOfMember($mem_id);
    logger($member_id, $mem_id, LOGGER_QUERY, "Tag ".COMPANY_NAME." kártyájának letöltése");
    exit();
}
$output .= '<div class="torzs"><div class="card"><form method="post">';

if (isPermitted(ACCESS_MMSZ | ACCESS_CLUBLEADER | ACCESS_ADMIN | ACCESS_MMSZ_PRESIDENT)) {
    if (!isset($mem_id)) {
        $mem_id = $member_id;
    }
    $output .= '<label>Azonosító:</label><input min="0" max="9999" type="number" name="mem_id" value="' . $mem_id . '" onChange="submit();">';
    $output .= ' <input type="submit" name="require_member" value="Lekérdezés">&nbsp;&nbsp;&nbsp;';
    if(isPaidMembership($mem_id)) {
        $output .= ' <input type="submit" name="download_member" value="Letöltés">';
    }
} else {
    $mem_id = $member_id;
}

if (isValidMemberId($mem_id)) {

    $output .= '<br>';
    $mem = retriveMember($mem_id);
    $output .= '<label>Név:</label>' . getMemberName($mem_id) . '<br>';
    $output .= '<label>Tagság:</label>' . getMemberClubNames($mem_id) . '<br>';
    $club_id = getDelegateClub($mem_id);
    if ($club_id > 0) {
        $output .= '<label>Küldött:</label>' . getDelegateClubNames($mem_id)
                . ' ' . delegatesNumberToString($club_id) . '<br>';
    }
    $output .= '<label>Kategóriák: </label>' . getListMemberCategories($mem_id) . '<br>';
    $output .= '<label>'.COMPANY_NAME.' tagdíj:</label>' . $this_year . ' évre ' . (isPaidMembership($mem_id) ? "befizetve" : "nincs rendezve") . '<br>';
    if ($mem[MEMBER_LICENCE] != "") {
        $output .= '<label>Licensz: </label>' . $mem[MEMBER_LICENCE] . '<br>';
    }
    $avatar = '.' . AVATAR . $mem_id . '.jpg';
    if (is_file($avatar)) {
        $mtime = filemtime($avatar);
        $output .= '<img name="avatar"  src="' . AVATAR_URL . $mem_id . '.jpg?' . $mtime . '"/><br>';
    } else {
        if (retriveMember($mem_id)[MEMBER_SEX] == "F") {
            $output .= '<img name="avatar"  src="' . IMAGES . 'female.jpg"/><br>';
        } else {
            $output .= '<img name="avatar" src="' . IMAGES . 'male.jpg"/><br>';
        }
    }
}
$output .= '</form>';
$output .= '</div></div>';

