<?php

if (!isset($document_id)) {
    $document_id = -1;
}

if (isset($delete_document)) {
    $file = DOCUMENTS . $delete_document;
    unlink($file);
    logger($member_id, $document_id, LOGGER_MEMBER_MOD, "A " . $document_id . " taghoz tartozó dokumentum (" . $delete_document . ") törlése");
}

if (isset($_FILES["document_upload"]) AND isset($feltolt)) {
    if ($_FILES["document_upload"]["type"] == "application/pdf") {
        $file = DOCUMENTS . $document_id . uniqid('_') . '.pdf';
        move_uploaded_file($_FILES["document_upload"]["tmp_name"], $file);
        logger($member_id, $document_id, LOGGER_MEMBER_MOD, "A " . $document_id . " taghoz tartozó új dokumentum feltöltése : " . $file);
    } else {
        warning("A feltöltött fájl nem pdf !");
    }
}
$output .= '<div class="keret">';
$output .= '<h2>PDF Dokumentumok</h2>';
$output .= '<form method="post" enctype="multipart/form-data">';
$files = scandir(DOCUMENTS);
if (!empty($files)) {
    $needle = $document_id . '_';
    foreach ($files as $f) {
        if (substr($f, 0, strlen($needle)) == $needle) {
            $output .= '<a target="_blank" href="' . DOCUMENTS . $f . '"><img src="' . IMAGES . 'pdf.jpg"></a>';
        }
    }
    $output .= '<br>';
    foreach ($files as $f) {
        if (substr($f, 0, strlen($needle)) == $needle) {
            $output .= '<input alt="Törlés" type="image" name="delete_document" value="' . $f . '" src="'
                    . IMAGES . 'delete.jpg" style="margin-left: 18px; margin-right: 18px;" onClick="return confirm(\'Biztosan tölni akarod a dokumentumot ?\')">';
        }
    }
}

$output .= '<br><input type="file" name="document_upload"  />
          <input type="submit" name="feltolt" value="Feltöltés" />
          <input type="hidden" name="edit_member" value="' . $edit_member . '"><br>';
$output .= '</form>';
$output .= '</div>';
