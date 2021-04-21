<?php

/*  Mindenkit látó jogosultságnál:
 *      all[] - az összes látható tag
 *      
 *      checked_members - a kiválasztott tagok
 */


$selectable_roles = [];
$roles_of_member = [];
$all_roles_of_member = getRolesOfMember($member_id);
foreach ($all_roles_of_member as $role) {
    if (in_array($role[ROLE_PERMISSION], $all_selectable_roles)) {
        $selectable_roles[$role[ROLE_ID]] = $access[$role[ROLE_PERMISSION]] . ' / ' . getClubName($role[ROLE_CLUB]);
        $roles_of_member[$role[ROLE_ID]] = [
            ROLE_PERMISSION => $role[ROLE_PERMISSION],
            ROLE_CLUB => $role[ROLE_CLUB],
        ];
    }
}
if (empty($selectable_roles)) {
    $view->danger('Nincs meghívható csoport');
    return;
}
$selected_role = $congressOnline->getCongressByRole();
$selected_addr = $congressOnline->getCongressByGroup();


$checked_members = $congressDelegates->getMembersOfCongress($selected_open_congress);

if (count($selectable_roles) == 1) {
    $selected_role = key($selectable_roles);
    $view->putText('Szerepkör', $selectable_roles[$selected_role]);
} else {
    if (!isset($roles_of_member[$selected_role])) {
        reset($selectable_roles);
        $selected_role = key($selectable_roles);
    }
    $view->newDiv('row');
    $view->submitDropDownChooserOnChange('Szerepkör', $selectable_roles, 'selected_role');
    $view->endDiv();
}
$_SESSION[SESSION_SELECTED_ROLE] = $selected_role;
$role = $roles_of_member[$selected_role];
if (!empty(getClubSubclubs($role[ROLE_CLUB])) AND $role[ROLE_PERMISSION] == ACCESS_CLUBLEADER) {
    $visible = getDelegatesOfSubclubs($role[ROLE_CLUB]);
    $role[ROLE_PERMISSION] = ACCESS_SUBCLUB_LEADER;
} else {
    $visible = getVisibleMembersOfRole($selected_role);
}
if (empty($visible)) {
    $view->danger('Nincsenek látható tagok');
    return;
}

$groups = getGroups($role[ROLE_PERMISSION]);
$view->newDiv('row');
$view->submitDropDownChooserOnChange('Kijelölés', [-1 => 'Egyéni'] + $groups, 'selected_addr');
$view->putElement('<button type="button" class="btn btn-primary mx-2 my-1 px-4" data-toggle="modal" data-target="#sel_addr" >Egyedi</button>');
$view->endDiv();
$selected_auth1 = $congressOnline->getCongressAuth1();
$selected_auth2 = $congressOnline->getCongressAuth2();
asort($checked_members);
$view->newDiv('row');
$view->submitDropDownChooserOnChange('Hitelesítők', [-1 => 'Válassz!!!'] + $checked_members, 'selected_auth1');
$view->submitDropDownChooserOnChange('', [-1 => 'Válassz!!!'] + $checked_members, 'selected_auth2');
$view->endDiv();
$view->info('Kiválasztottak száma: ' . count($checked_members));
$view->putTooltipTrigger();
$mem_num = 0;
foreach ($checked_members as $mem_id => $mem_name) {
    $mem = retriveMember($mem_id);
    $view->newElement('<a  data-toogle="tooltip" title="Azonosító: ' . $mem_id . ', Szül: ' . $mem[MEMBER_BORN] . ', Email: ' . $mem[MEMBER_EMAIL] . '">');
    $view->putElement($mem_name);
    $view->endElement('</a>, ');
}
$view->newElement('<div class="modal fade" id="sel_addr" tabindex="-1" aria-labelledby="sel_addr_label" aria-hidden="true" role="dialog">');
$view->newDiv('modal-dialog modal-dialog-scrollable')->newDiv('modal-content');
$view->newDiv('modal-header');
$view->putElement('<h5 class="modal-title" id="sel_addr_label">Egyedi kiválasztás</h5>');
$view->newElement('<button class="close" data-dismiss="modal" aria-label="Bezár">');
$view->putElement('<span aria-hidden="true">&times;</span>');
$view->endElement('</button>');
$view->endDiv()->newDiv('modal-body', 'sel_members');
$selected_num = 0;
$congress_members = array();
$var_name = 'checkbox_add';
output_post_onchange_checkbox($var_name);
foreach ($visible as $mem_data) {
    $id = $mem_data[MEMBER_ID];
    if (isset($checked_members[$id])) {
        $checked = 'checked';
        $selected_num++;
        $congress_members[$id] = $mem_data[MEMBER_VEZNEV] . ' ' . $mem_data[MEMBER_KERNEV];
    } else {
        $checked = '';
    }
    $view->newElement('<li class="list-group-item">');
    $view->putElement('<input id="chk_id_' . $id . '" onChange="submitOnchange_' . $var_name . '(' . $id . ');" type="checkbox" class="mx-1" '
            . 'name="' . $var_name . '[]" id="' . $var_name . '[]" ' . $checked . ' " value="' . $id . '">'
            . '<a data-toogle="tooltip" title="Azonosító: ' . $id . '&#010;Szül:' . $mem_data[MEMBER_BORN] . '">'
            . $mem_data[MEMBER_VEZNEV] . ' ' . $mem_data[MEMBER_KERNEV] . ' / ' . $mem_data[CLUB_NAME] . '</a>');
    $view->endElement('</li>');
}
if (isset($changed_checkbox_add) AND $changed_checkbox_add > 0) {
    $view->putScript('$(document).ready(function() {'
            . ' $(\'#sel_addr\').modal(\'show\');});'
    );
    $view->putScript(" var addr_changed = $('#chk_id_" . $changed_checkbox_add . ');"
           . " var changed_pos = addr_changed.position(); "
            . " $('#sel_addr').on('shown', function() {"
            . " $('#sel_members).scrollTop(changed_pos);"
            . "});");
}
$view->endElement('</ul>');
$view->endDiv();
$view->endDiv()->endDiv();
$view->endElement('</div>');
