<?php

include_once FUNCTIONS . 'docu_functions.inc';

if (!isset($edu_base_select)) {
    $edu_base_select = -1;
}
output_hidden('edu_base_select');

o('<div class="tab">');
o('<button class=" ' . ($edu_base_select == 5 ? 'active' : '') . '" name="edu_base_select" title="A képzési szervezetek bizonytványai" value="5">Képzések</button>');
o('<button class=" ' . ($edu_base_select == 6 ? 'active' : '') . '" name="edu_base_select" value="6">Képzettségek</button>');
o('<button class=" ' . ($edu_base_select == 7 ? 'active' : '') . '" name="edu_base_select" title="Képzettségel vezethető légijármű kategóriák" value="7">Képzettség - LJK</button>');
o('<button class=" ' . ($edu_base_select == 3 ? 'active' : '') . '" name="edu_base_select" title="Szakágak légijármű kategóriái" value="3">LJK</button>');
o('<button class=" ' . ($edu_base_select == 1 ? 'active' : '') . '" name="edu_base_select" title="Azok a képzési vagy FLF szervezetek, amelyek egy-egy szakág számára szolgáltatást nyújtanak" value="1">Szervezet</button>');
o('<button class=" ' . ($edu_base_select == 2 ? 'active' : '') . '" name="edu_base_select" title="Az egyesülethethez tartozó szervezetek szerkesztése" value="2">Választott szervezetek</button>');
o('<button class=" ' . ($edu_base_select == 4 ? 'active' : '') . '" name="edu_base_select" title="Elnevezések szerkesztése" value="4">Elnevezések</button>');
o('</div>');

switch ($edu_base_select) {
    case 1: // Szervezetek
        include_once DOCU_INCLUDES . 'docu_edit_orgs.inc';
        break;
    case 2: // Egyesület választott szervezetei
        include_once DOCU_INCLUDES . 'docu_edit_evos.inc';
        break;
    case 3:// Szakágak légijármű kategóriái
        include_once DOCU_INCLUDES . 'docu_edit_ljks.inc';
        break;
    case 4: // Standard dokumentumok
        include_once DOCU_INCLUDES . 'docu_edit_sdns.inc';
        break;
    case 5: // Képzések szereksztése
        include_once DOCU_INCLUDES . 'docu_edit_dao.inc';
        break;
    case 6: // Képzettségek
        include_once DOCU_INCLUDES . 'docu_edit_jog.inc';
        break;
    case 7: // Képzettségel vezethető légijármű kategóriák
        include_once DOCU_INCLUDES . 'docu_edit_jog_ljk.inc';
        break;
    default:
        break;
}

