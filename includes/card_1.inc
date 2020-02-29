<?php

if (isset($card_member)) {
    $_SESSION[SESSION_REQUIRED_MEMBER] = $card_member;
    $_SESSION[SESSION_REQUIRED_NOROBOT] = SESSION_REQUIRED_NOROBOT;
}
if (!isset($_SESSION[SESSION_REQUIRED_MEMBER])) { // Még nem adtunk meg tagszamot
    if (isset($member_id)) {
        $mem_id = $member_id;
    } else {
        $_SESSION[SESSION_REQUIRED_MEMBER] = 0;
    }
}
if (!isset($mem_id)) {
    $mem_id = $_SESSION[SESSION_REQUIRED_MEMBER];
}
if (!isset($chaptha_expected)) {
    $chaptha_expected = -1;
}
if (!isset($chaptha_entered)) {
    $chaptha_entered = -2;
}
if (!isset($member_id)) {
    $member_id = -1;
} else {
    $_SESSION[SESSION_REQUIRED_NOROBOT] = SESSION_REQUIRED_NOROBOT;
}

if (isset($require_member)) {
    if (isValidMemberId($mem_id)) {
        $_SESSION[SESSION_REQUIRED_MEMBER] = $mem_id;
    } else {
        warning("Adj meg érvényes azonosítót !");
    }
}
if (isset($download_member)) {
    downloadCardOfMember($mem_id);
//    logger($member_id, $mem_id, LOGGER_QUERY, "Tag MMSz kártyájának letöltése");
    exit();
}
$_SESSION[SESSION_REQUIRED_MEMBER] = $mem_id;
$output .= '<div class="torzs"><div class="card"><form method="post">';
if (!isset($_SESSION[SESSION_REQUIRED_NOROBOT])) {
    if ($chaptha_entered == $chaptha_expected) {
        $_SESSION[SESSION_REQUIRED_NOROBOT] = SESSION_REQUIRED_NOROBOT;
    } else {
        $output .= '<input type="hidden" name="chaptha_expected" value="' . $chaptha_number . '">
                <label for="chaptha_entered">Írd ide, mennyi ' . $chaptha_text . ': </label><input type="text" name="chaptha_entered" value="">
                <br>';
    }
}
$output .= '<label>Azonosító:</label><input min="0" max="9999" type="number" name="mem_id" value="' . $mem_id . '" onChange="submit();">';
$output .= ' <input type="submit" name="require_member" value="Lekérdezés">';
if (isValidMemberId($mem_id)) {
    $output .= '&nbsp;&nbsp;&nbsp;<input type="submit" name="download_member" value="Letöltés">';
}
$output .= '<br></form>';
if (isValidMemberId($mem_id)) {
    $mem = retriveMember($mem_id);
    logger($member_id, $mem_id, LOGGER_QUERY, "Tag MMSz kártyájának lekérdezése");
    $output .= '<label>Név:</label>' . getMemberName($mem_id) . '<br>';
    $output .= '<label>Tagság:</label>' . getMemberClubNames($mem_id) . '<br>';
    $club_id = getDelegateClub($mem_id);
    if ($club_id > 0) {
        $output .= '<label>Küldött:</label>' . getDelegateClubNames($mem_id)
                . ' ' . delegatesNumberToString($club_id) . '<br>';
    }
    $output .= '<label>Kategóriák: </label>' . getListMemberCategories($mem_id) . '<br>';
    $output .= '<label>MMSz tagdíj:</label>' . $this_year . ' évre ' . (isPaidMembership($mem_id) ? "befizetve" : "nincs rendezve") . '<br>';
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
$output .= '</div></div>';

