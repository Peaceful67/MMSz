<?php

o('<div class="lists">');
output_label('Választható képzés');
$pidao_daos = $docuDao->getActiveNames();
output_selector($pidao_daos, 'pidao_dao_id', false);
output_spaces(5);
output_button('docu_pidao_new', -1, 'add.jpg', 'Új képzés felvétele', 'Biztosan fel akarod venni ezt a képzést ?');

$pidaos = $docuPidao->getPIDAOsOfMember($member_pidao);
if (empty($pidaos)) {
    o('</div>');
    return;
}

o('<table><tr><th>Képzés</th><th>Művelet</th></tr>');
foreach ($pidaos as $pidao_id => $pidao) {
    o('<tr><td>' . $docuDao->getName($pidao[DOCU_PIDAO_DAO_ID]) . '</td>');
    o('<td>');
    output_button('docu_pidao_edit', $pidao_id, 'edit.jpg', 'Képzés adatainak szerkesztése');
    if ($docuPidao->isDeletable($pidao_id)) {
        output_button('docu_pidao_delete', $pidao_id, 'delete.jpg', 'Képzés törlése a tag képzései közül', 'Biztosan törölni akarod a képzést');
    }
    o('</td></tr>');
}

