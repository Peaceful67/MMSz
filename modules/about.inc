<?php

include_once FUNCTIONS . 'versions.inc';
if (!isset($selected_version)) {
    $selected_version = VERSION;
}

$view->newKeret();
$view->putElement(getOptionValue(OPTIONS_NAME_COMPANY_LONG_NAME) . ' online adminisztrációs felület<br>
    Készítette: Baksa Zsolt (2017-'.$this_year.') (peaceful.app@gmail.com)');
$view->putElement('<br><br>Aktuális verzió : <b>' . VERSION . '</b>, legutóbb frissítve: ' . date('Y-m-d, h:i', getOptionValue(OPTIONS_NAME_VERSION_UPDATED)));
$view->endKeret()->newKeret();
$view->newDiv('form-inline form-group my-1');
$view->putElement('<label class="col-form-label" for="' . $id_name . '">A frissítés összefoglalója: </label>');
$view->putElement('<select class="form-control col-md-6" id="selected_version" name="selected_version" id="selected_version" onChange="submit();">');
foreach ($update_versions as $version_id => $version_data) {
    $view->putElement('<option value="' . $version_id . '" ' . ($version_id == $selected_version ? ' selected ' : '') . '>');
    $view->putElement($version_id.' / '.$version_data[VERSION_TITLE]);
    $view->putElement('</option>');
}
$view->putElement('</select>');
$view->endDiv();
$view->putElement('A frissítés leírása: ');
$view->putElement($update_versions[$selected_version][VERSION_DESCRIPTION]);
$view->endKeret();
