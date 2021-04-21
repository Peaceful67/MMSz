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

$view->newKeret('editor');
$view->putElement('<h4>PDF Dokumentumok</h4>');

$files = scandir(DOCUMENTS);
if (!empty($files)) {
    $needle = $document_id . '_';
    $view->newDiv('row from-inline');
    foreach ($files as $f) {
        if (substr($f, 0, strlen($needle)) == $needle) {
            $view->newDiv('col-md-1 align-middle');
            $view->putElement('<a target="_blank" href="' . DOCUMENTS . $f . '"><img style="width: 60px;" src="' . IMAGES . 'pdf.jpg"></a><br>');
            $view->submitDelete('delete_document', $f, 'Dokumentum törlése', 'Biztosan tölni akarod a dokumentumot ?');
            $view->endDiv();
        }
    }

    $view->endDiv();
}

$view->newDiv('file-upload-wrapper');
$view->putElement('<label class="control-label">File feltöltés</label>');
$view->putElement('<input type="file" id="document_upload" name="document_upload" class="file-upload" />');
$view->endDiv();
$view->submitButton('feltolt', 'Feltöltés');
$view->endKeret();
$view->putHidden('edit_member');
