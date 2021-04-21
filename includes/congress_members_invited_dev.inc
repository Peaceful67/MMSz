<?php

/*  Mindenkit látó jogosultságnál:
 *      all[] - az összes látható tag
 *      
 *      checked_members - a kiválasztott tagok
 */
if (isset($changed_checkbox_add) AND $changed_checkbox_add > 0) {
    $congressInvited->alterMember($edit_congress_members, $changed_checkbox_add);
    }
$groups = getGroups();
$checked_members = $congressInvited->getMembersOfCongress($edit_congress_members);
$visible = getVisibleMembers();
foreach ($congressDelegates->getMembersOfCongress($edit_congress_members) as $mem_id => $mem_name) {
    $visible = array_diff($visible, [$mem_id]);
}
$visible = array_diff($visible, [$member_id]);

if (empty($visible)) {
    error_log('Empty Visible Members');
    warning('Nincsenek látható tagok');
    return;
}
if (!isset($selected_addr)) {
    $selected_addr = -1;
}
o('<div class="congress_members">');
output_post_onchange('selected_addr');
output_label('Egyedi kijelölés', true);


$output .= '<div class="congress_selected">';
$output .= '<b>Kiválasztottak száma: ' . count($checked_members) . '</b>';
output_ln();
$mem_num = 0;
foreach ($checked_members as $mem_id => $mem_name) {
    $mem = retriveMember($mem_id);
    $output .= '<div class="tooltip">' . $mem_name
            . '<span class="tooltiptext">'
            . 'Azonosító:' . $mem_id
            . '<br>Szül:' . $mem[MEMBER_BORN]
            . '<br>Email:' . $mem[MEMBER_EMAIL]
            . '</span></div>, ';
    if (++$mem_num > 4) {
        $mem_num = 0;
        output_ln();
    }
}
$output .= '</div>';

$output .= '<div id="congress_address" class="congress_address">';
$output .= '<ul>';
$selected_num = 0;
$congress_members = array();
$var_name = 'checkbox_add';
output_post_onchange_checkbox($var_name);
foreach ($visible as $id) {
    $mem = retriveMember($id);
    if (isset($checked_members[$id])) {
        $checked = 'checked';
        $selected_num++;
        $congress_members[$id] = $mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV];
    } else {
        $checked = '';
    }
    $output .= '<li><input id="chk_id_' . $id . '" onChange="submitOnchange_' . $var_name . '(' . $id . ');" type="checkbox" name="' . $var_name . '[]" id="' . $var_name . '[]" ' . $checked . ' " value="' . $id . '">'
            . '<a title="Azonosító: ' . $mem[MEMBER_ID] . '&#010;Szül:' . $mem[MEMBER_BORN] . '">'
            . $mem[MEMBER_VEZNEV] . ' ' . $mem[MEMBER_KERNEV] . '</a></li>';
}
if (isset($changed_checkbox_add) AND $changed_checkbox_add > 0) {
    o('<script type="text/javascript">'
            . 'var alteredElement = document.getElementById("chk_id_' . $changed_checkbox_add . '");'
            . 'alteredElement.scrollIntoView({behavior: "auto", block:"center"});'
            . '</script>');
}


$output .= '</ul>';
$output .= '</div>';
$output .= '</div>';

