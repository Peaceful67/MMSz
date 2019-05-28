<?php

$file = '.' . AVATAR . $avatar_id . '.jpg';
$sized_file = '.' . AVATAR_SIZED . $avatar_id . '.jpg';
$original_file = '.' . AVATAR_ORIGINAL . $avatar_id . '.jpg';

if (isset($delete_avatar)) {
    logger($member_id, $avatar_id, LOGGER_MEMBER_MOD, " tag avatar törlése");
    unlink($file);
    unlink($sized_file);
    unlink($original_file);
}
if (isset($download_sized)) {
    $output_name = $avatar_id . '_sized.jpg';
    $sized_filename = './' . AVATAR_SIZED . $avatar_id . '.jpg';
    header('Content-Description: File Transfer');
    header('Content-Type: image/jpg');
    header('Content-Disposition: attachment; filename="' . $output_name . '"');
    header('Expires: 0');
    header('Cache-Control: must-revalidate');
    header('Pragma: public');
    header('Content-Length: ' . filesize($sized_filename));
    readfile($sized_filename);
}
if (isset($download_original)) {
    $output_name = $avatar_id . '_original.jpg';
    $original_filename = './' . AVATAR_ORIGINAL . $avatar_id . '.jpg';
    header('Content-Description: File Transfer');
    header('Content-Type: image/jpg');
    header('Content-Disposition: attachment; filename="' . $output_name . '"');
    header('Expires: 0');
    header('Cache-Control: must-revalidate');
    header('Pragma: public');
    header('Content-Length: ' . filesize($original_filename));
    readfile($original_filename);
}
if (isset($_FILES["avatar_upload"]) AND isset($feltolt_avatar)) {
    if (!empty($_FILES["avatar_upload"]["tmp_name"])) {

        $check = getimagesize($_FILES["avatar_upload"]["tmp_name"]);
        if ($check !== false OR $_FILES["document_upload"]["type"] == "image/jpg") {
            list($width, $height) = $check;
            if (!isAvatarRatio($width, $height)) {
                warning(AVATAR_SIZED_WIDTH . "(széles)/" . AVATAR_SIZED_HEIGHT . " (magas) képarányú (&#177;5%) kép tölthető csak fel !");
                unset($_FILES);
            } elseif ($width >= AVATAR_SIZED_WIDTH AND $height >= AVATAR_SIZED_HEIGHT) {
                $thumb = imagecreatetruecolor(AVATAR_WIDTH, AVATAR_HEIGHT);
                $sized = imagecreatetruecolor(AVATAR_SIZED_WIDTH, AVATAR_SIZED_HEIGHT);
                $source = imagecreatefromjpeg($_FILES["avatar_upload"]["tmp_name"]);
                imagecopyresized($thumb, $source, 0, 0, 0, 0, AVATAR_WIDTH, AVATAR_HEIGHT, $width, $height);
                imagejpeg($thumb, $file);
                imagecopyresized($sized, $source, 0, 0, 0, 0, AVATAR_SIZED_WIDTH, AVATAR_SIZED_HEIGHT, $width, $height);
                imagejpeg($sized, $sized_file);
                rename($_FILES["avatar_upload"]["tmp_name"], $original_file);
                logger($member_id, $avatar_id, LOGGER_MEMBER_MOD, " tag avatar feltöltése");
            } else {
                warning(" A képnek legalább " . AVATAR_SIZED_WIDTH . " képpont szélesnek és "
                        . AVATAR_SIZED_HEIGHT . " képpont magasnak kell lennie.");
                unset($_FILES);
            }
        } else {
            warning("A feltöltött fájl nem kép !");
            unset($_FILES);
        }
    }
}
$output .= '<div class="keret">';
$output .= '<H1>Arckép feltöltés</H1>';
if (isset($avatar_id)) {
    if (is_file($file)) {
        $mtime = filemtime($file);
        $output .= '<img height="' . AVATAR_HEIGHT . '" width="' . AVATAR_WIDTH . '" src="' . AVATAR_URL . $avatar_id . '.jpg?' . $mtime . '"/>';
    } else {
        $output .= "Nincs feltöltött kép";
    }
}
$output .= '<form method="post" enctype="multipart/form-data"><label>Feltöltés:</label>
            <input type="hidden" name="avatar_id" value="' . $avatar_id . '">
                        <input type="file" name="avatar_upload"  ><br>';
//if (isset($_FILES["avatar_upload"])) {
$output .= '<input type="submit" name="feltolt_avatar" value="Feltöltés" >';
//}
$output .= '<input type="hidden" name="edit_member" value="' . $edit_member . '">';
if (is_file($file)) {
    $output .= '<br><label>Törlés:</label><input type="submit" name="delete_avatar" value="Törlés" onClick="return confirm(\'Biztosan törölni akarod a képet ?\')"/>';
}
if (is_file($sized_file) OR is_file($original_file)) {
    $output .= '<br><label>Letöltés:</label>';
}
if (is_file($sized_file)) {
    list($width, $height) = getimagesize($sized_file);
    $output .= '<input title="Méretes" type="submit" name="download_sized" value="' . $width . ' x ' . $height . '" >';
}
if (is_file($original_file)) {
    list($width, $height) = getimagesize($original_file);
    $output .= '&nbsp;&nbsp;&nbsp;<input title="Eredeti" type="submit" name="download_original" value="' . $width . ' x ' . $height . '" >';
}

$output .= '</form>';
$output .= '</div>';

function isAvatarRatio($width, $height) {
    if ((floatval($height * AVATAR_SIZED_WIDTH) / floatval($width*AVATAR_SIZED_HEIGHT)) > AVATAR_TOLERANCE) {
        return false;
    }
    if ((floatval($height*AVATAR_SIZED_WIDTH) / floatval($width*AVATAR_SIZED_HEIGHT)) < (1 / AVATAR_TOLERANCE)) {
        return false;
    }
    return true;
}
