<?php

o('<div class="lists">');
output_label('Választható képzettség');
$pidjog_jogs = $docuJog->getNames();
$pidjogDocus = new DocuPIDJOGDocus();
output_selector($pidjog_jogs, 'pidjog_jog_id', false);
output_spaces(5);
output_button('docu_pidjog_new', -1, 'add.jpg', 'Új képzettség felvétele', 'Biztosan fel akarod venni ezt a képzettséget ?');

$pidjogs = $docuPidjog->getPIDJOGsOfMember($member_pidjog);
if (empty($pidjogs)) {
    o('</div>');
    return;
}

o('<table><tr><th>Képzettség</th><th>Oktatói</th><th>Állapot</th><th>Művelet</th></tr>');
foreach ($pidjogs as $pidjog_id => $pidjog) {
    o('<tr><td>' . $docuJog->getName($pidjog[DOCU_PIDJOG_JOG_ID]) . '</td>');
    o('<td>');
    o($docuJog->isTrainer($pidjog[DOCU_PIDJOG_JOG_ID]) ? "Igen" : "Nem");
    o('</td>');

    $docus = $pidjogDocus->getPIDJOGDocusOfPidjog($pidjog_id);
    $valid = 0;
    foreach ($docus as $docu) {
        if ($pidjogDocus->isValid($docu[DOCU_PIDJOG_DOCUS_ID])) {
            $valid++;
        }
    }
    o('<td style="color:' . (count($docus) == $valid ? 'green' : 'red') . ';">');
    o($valid . '/' . count($docus));
    o('</td>');
    o('<td>');
    output_button('docu_pidjog_edit', $pidjog_id, 'edit.jpg', 'Képzettség adatainak szerkesztése');
    if ($docuPidjog->isDeletable($pidjog_id)) {
        output_button('docu_pidjog_delete', $pidjog_id, 'delete.jpg', 'Képzettség törlése a tag képzései közül', 'Biztosan törölni akarod a képzést');
    }
    o('</td></tr>');
}

