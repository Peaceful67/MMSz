<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of View
 *
 * @author Peaceful
 * 
 */
class View {

    private $div_counter = 0;
    private $readable = true;
    private $label_width = 2;
    private $field_width = 10;

    function __construct() {
        $this->readable = getOptionValue(OPTIONS_NAME_HTML_TABULATED) > 0;
    }

    function viewEyeLink($file_name, $title) {
        $this->putElement('<a title = "' . $title . '" target = "_blank" href = "' . $file_name . '"><i class="fa fa-eye" aria-hidden="true"></i></a>');
    }

    function viewEyeSlash($title = '') {
        $this->putElement('<i title="' . $title . '" class="fa fa-eye-slash"></i>');
    }

    function submitButton($name, $text, $value = -1, $title = '', $confirm = '') {
        $this->putElement('<button type="submit" class="btn btn-primary m-1" name="' . $name . '" value="' . $value . '"');
        if (strlen($title) > 0) {
            $this->put(' title="' . $title . '"');
        }
        if (strlen($confirm) > 0) {
            $this->put(' onclick="saved=true;return confirm(\'' . $confirm . '\');"');
        } else {
            $this->put(' onclick="saved=true;"');
        }
        $this->put('>' . $text . '</button>');
        return $this;
    }

    function submitAlert($text, $title, $alert) {
        $this->putElement('<button type="submit" class="btn btn-primary m-1" ');
        if (strlen($title) > 0) {
            $this->put(' title="' . $title . '"');
        }
        if (strlen($alert) > 0) {
            $this->put(' onClick="alert(\'' . $alert . '\');"');
        }
        $this->put('>' . $text . '</button>');
        return $this;
    }

    function submitDelete($name, $value = -1, $title = 'Törlés', $confirm = 'Biztosan törölni akarod ?') {
        $onclick = (empty($confirm)) ? '' : ' onclick="saved=true; return confirm(\'' . $confirm . '\');"';
        $this->putElement('<button class="btn btn-danger btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '" ' . $onclick . '>'
                . '<i class="fa fa-trash" onclick="saved=true;"></i></button>');
        return $this;
    }

    function submitEmail($name, $value = -1, $title = 'Email küldése', $confirm = 'Biztosan el akarod küldeni az emailt?') {
        $onclick = (empty($confirm)) ? '' : ' onclick="saved=true; return confirm(\'' . $confirm . '\');"';
        $this->putElement('<button class="btn btn-primary btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '" ' . $onclick . '>'
                . '<i class="fa fa-envelope-o" onclick="saved=true;"></i></button>');
        return $this;
    }

    function submitEdit($name, $value = -1, $title = 'Szerkesztés') {
        $this->putElement('<button class="btn btn-info btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '">'
                . '<i class="fa fa-pencil" onclick="saved=true;"></i></button>');
        return $this;
    }

    function submitSave($name, $value = -1, $title = 'Mentés') {
        $this->putElement('<button class="btn btn-success btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '">'
                . '<i class="fa fa-floppy-o" onclick="saved=true;"></i></button>');
        return $this;
    }

    function submitUp($name, $value = -1, $title = 'Mozgatás fel') {
        $this->putElement('<button class="btn btn-success btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '">'
                . '<i class="fa fa-arrow-up" onclick="saved=true;"></i></button>');
        return $this;
    }

    function submitDown($name, $value = -1, $title = 'Mozgatás le') {
        $this->putElement('<button class="btn btn-success btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '">'
                . '<i class="fa fa-arrow-down" onclick="saved=true;"></i></button>');
        return $this;
    }

    function submitBack($name, $title = 'Visszatérés az előző oldalra') {
        $this->putElement('<button class="btn btn-secondary btn-sm rounded-1 m-1" type="submit" name="' . $name . '"  title="' . $title . '">'
                . '<i class="fa fa-arrow-left" ></i></button>');
        return $this;
    }

    function submitRefresh($name, $value = -1, $title = 'Frissítés') {
        $this->putElement('<button class="btn btn-secondary btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '">'
                . '<i class="fa fa-refresh" aria-hidden="true" onclick="saved=true;"></i></button>');
        return $this;
    }

    function submitReset($name, $title = '') {
        $this->newElement('<button type="submit" name="' . $name . '" class="close" aria-label="Close" title="' . $title . '">');
        $this->putElement('<span aria-hidden="true">&times;</span>');
        $this->endElement('</button>');
    }

    function submitAdd($name, $value = -1, $title = 'Új tétel') {
        $this->putElement('<button class="btn btn-info btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="'
                . $value . '" title="' . $title . '" onclick="saved=true;">'
                . '<i class="fa fa-plus-circle"></i></button>');
        return $this;
    }

    function submitPrint($name, $value = -1, $title = 'A tétel PDF fájlba nyomtatása') {
        $this->putElement('<button class="btn btn-secondary btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  title="' . $title . '">'
                . '<i class="fa fa-print" aria-hidden="true" onclick="saved=true;"></i></button>');
        return $this;
    }

    function downloadPdf($name, $value = -1, $title = 'A PDF dokumentum letöltése') {
        $this->putElement('<button class="btn btn-default btn-lg rounded-1 m-1" type="submit" name="' . $name . '" value="'
                . $value . '" title="' . $title . '" >'
                . '<i class="fa fa-file-pdf-o"></i></button>');
        return $this;
    }

    function submitExpand($name, $text, $value = -1, $title = 'Kibontás') {
        $this->putElement('<button class="btn btn-primary btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  '
                . 'title="' . $title . '">'
                . '<i class="fa fa-expand" aria-hidden="true"></i>' . $text . '</button>');
        return $this;
    }

    function submitCollapse($name, $text, $value = -1, $title = 'Összecsukás') {
        $this->putElement('<button class="btn btn-primary btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="' . $value . '"  '
                . 'title="' . $title . '">'
                . '<i class="fa fa-collaps" aria-hidden="true"></i>' . $text . '</button>');
        return $this;
    }

    function submitAwesome($name, $text, $icon, $value = -1, $title = '', $confirm = '') {
        $onclick = (empty($confirm)) ? '' : ' onclick="saved=true; return confirm(\'' . $confirm . '\');"';
        $this->putElement('<button class="btn btn-danger btn-sm rounded-1 m-1" type="submit" name="' . $name . '" value="'
                . $value . '"  title="' . $title . '" ' . $onclick . '>'
                . '<i class="fa fa-' . $icon . '"></i>' . $text . '</button>');
        return $this;
    }

    function inputSelector($label, $arr, $name, $submit, $default = 'Válassz !!!') {
        $id_name = 'selected_' . $name;
        global $$id_name;
        if (!isset($$id_name)) {
            $$id_name = -1;
        }
        $sel_arr = (strlen($default) > 0) ? ([-1 => $default] + $arr) : $arr;
        $this->newDiv('form-inline form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $id_name);
        }
        $this->newElement('<select class="form-control mx-2" name="' . $id_name . '" '
                . ' id="' . $id_name . '" ' . ($submit ? 'onChange="submit();"' : 'onchange="saved=false;"') . '>');
        foreach ($sel_arr as $key => $value) {
            $this->putElement('<option value="' . $key . '" '
                    . ($$id_name == $key ? ' selected ' : '') . '>' . $value . '</option>');
        }
        $this->endElement('</select>');
        return $this->endDiv();
    }

    function inputRadioButton($name, $label, $value, $title = '') {
        global $$name;
        if (isset($$name)) {
            $checked = ($$name == $value);
            $this->newDiv('form-check my-3');
            $this->newElement('<label class="form-check-label' . ($checked ? " active" : "") . '" title="' . $title . '">');
            $this->putElement('<input  type="radio" onChange="saved=false;" class="form-check-input" value="' . $value . '" name="' . $name . '" id="' . $name . '" autocomplete="off" ' . ($checked ? "checked" : "") . '>' . $label);
            $this->endElement('</label>');
        } return $this->endDiv();
    }

    function submitRadioInline($name, $label, $value, $title) {
        global $$name;
        if (isset($$name)) {
            $checked = ($$name == $value);
            $this->newDiv('form-check-inline');
            $this->newElement('<label class="form-check-label' . ($checked ? ' active' : '') . '" title="' . $title . '">');
            $this->putElement('<input  type="radio" onChange="submit();" class="form-check-input mx-2" value="' . $value . '" name="' . $name . '" id="' . $name . '" autocomplete="off" ' . ($checked ? "checked" : "") . '>' . $label);
            $this->endElement('</label>');
            $this->endDiv();
        } else {
            error_log($name . ' variable is not set!');
        }
        return $this;
    }

    function inputDropDownSelector($label, $chk_prefix, $arr_all, $arr_checked, $submit) {
        $this->newDiv('form-inline form-group m-1');
        $this->newDiv('dropdown dropdown-hover');
        $this->putElement('<button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown">' . $label . '</button>');
        $this->newDiv('dropdown-menu checkbox-menu allow-focus');
        foreach ($arr_all as $key => $value) {
            $chk_name = $chk_prefix . $key;
            $checked = in_array($key, $arr_checked);
            $this->newDiv('dropdown-item')->newDiv('form-check float-left');
            $this->newElement('<label for="' . $chk_name . '" class = "label form-check-label checkbox-inline">');
            $this->putElement('<input type="checkbox" class="form-check-input"  name="' . $chk_name . '" id="' . $chk_name . '" '
                    . ($checked ? " checked " : "") . ($submit ? ' onChange="submit();"' : '') . '>' . $value);
            $this->endElement('</label>');
            $this->endDiv()->endDiv();
        }
        $this->endDiv()->endDiv()->endDiv();
        return $this;
    }

    function inputDropDownChooser($label, $arr, $name) {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        } else {
            $this->putHidden($name);
        }
        if (!isset($arr[$$name])) {
            error_log($name . ' variable has invalid value: ' . $$name);
            return $this;
        }
        $this->newDiv('row form-group m-1');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->putScript("$(document).ready(function() {
                            $('.dropdown_" . $name . " button').on('click', function(){
                                $('#show_" . $name . "').text($(this).text());
                                var new_value = $(this).val();
                                $('#" . $name . "').val(new_value);
                            });
                        });");
        $this->newDiv('col-sm-4');
        $this->newDiv('dropdown dropdown-hover');
        $this->putElement('<button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown" title="Válassz !!!">'
                . '<span id="show_' . $name . '">' . $arr[$$name] . '</span></button>');
        $this->newDiv('dropdown-menu checkbox-menu allow-focus dropdown_' . $name)->newDiv('dropdown-hover');
        foreach ($arr as $key => $value) {
            $this->putElement('<button  type="button" class="dropdown-item' . ($key == $$name ? ' active' : '') . '" value="' . $key . '" name="' . $name . '" id="' . $name . '"  '
                    . ' ' . ($key == $$name ? "checked" : "") . '>' . $value . '</button>');
        }
        $this->endDiv()->endDiv()->endDiv();
        $this->endDiv()->endDiv();
        return $this;
    }

    function submitDropDownChooser($label, $arr, $name) {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        } else {
            $this->putHidden($name);
        }
        if (!isset($arr[$$name])) {
            error_log($name . ' variable has invalid value: ' . $$name);
            return $this;
        }
        $this->newDiv('form-inline form-group m-1');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->newDiv('dropdown dropdown-hover');
        $this->putElement('<button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown" title="Válassz !!!">'
                . '<span id="show_' . $name . '">' . (is_array($arr[$$name]) ? $arr[$$name][0] : $arr[$$name]) . '</span></button>');
        $this->newDiv('dropdown-menu checkbox-menu allow-focus dropdown_' . $name)->newDiv('dropdown-hover');
        foreach ($arr as $key => $value) {
            if (is_array($value)) {
                $this->putElement('<button  type="submit" class="dropdown-item' . ($key == $$name ? ' active' : '') . '" value="' . $key . '" name="' . $name . '" id="' . $name . '"  '
                        . ' ' . ($key == $$name ? "checked" : "") . '>' . $value[0] . '</button> ');
            } else {
                $this->putElement('<button  type="submit" class="dropdown-item' . ($key == $$name ? ' active' : '') . '" value="' . $key . '" name="' . $name . '" id="' . $name . '"  '
                        . ' ' . ($key == $$name ? "checked" : "") . '>' . $value . '</button>');
            }
        }
        $this->endDiv()->endDiv();
        $this->endDiv()->endDiv();
        return $this;
    }

    function submitDropDownChooserOnChange($label, $arr, $name) {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        } else {
            $this->putHidden($name);
        }
        if (!isset($arr[$$name])) {
            error_log($name . ' variable has invalid value: ' . $$name . ', valid values: ' . print_r($arr, true));
            return $this;
        }
        $old_var_name = 'old_' . $name;
        global $$old_var_name;
        $$old_var_name = $$name;
        $this->putHidden($old_var_name);
        $this->newDiv('row form-group m-1');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->newDiv('col-sm-4');
        $this->newDiv('dropdown dropdown-hover');
        $this->putElement('<button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown" title="Válassz !!!">' . $arr[$$name] . '</button>');
        $this->newDiv('dropdown-menu checkbox-menu allow-focus')->newDiv('dropdown-hover');
        foreach ($arr as $key => $value) {
            $this->putElement('<button  type="submit" class="dropdown-item' . ($key == $$name ? ' active' : '') . '" value="' . $key . '" name="' . $name . '" id="' . $name . '" '
                    . ($key == $$name ? "checked" : "") . '>' . $value . '</button>');
        }
        $this->endDiv()->endDiv()->endDiv();
        $this->endDiv()->endDiv();
        return $this;
    }

    function inputText($name, $label = '', $placeholder = '', $type = 'text') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        $this->newDiv('form-inline form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->putElement('<input type="' . $type . '" onchange="saved=false;" class="form-control mx-2" id="'
                . $name . '" name="' . $name . '" value="' . $$name . '" placeholder="' . $placeholder . '">');
        return $this->endDiv();
    }

    function inputSearch($name, $label = '') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        $this->newDiv('form-inline form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->newDiv('d-flex');
        $this->putElement('<input type="search" class="form-control py-2 bg-transparent" value="'.$$name.'" id="'.$name.'" name="'.$name.'" >');
        $this->endDiv();
        return $this->endDiv();
    }

    function inputTextArea($name, $label = '', $placeholder = '') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        $this->newDiv('form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->putElement('<textarea rows="5" onchange="saved=false;" class="form-control mx-2" style="min-width: 100%" id="'
                . $name . '" name="' . $name . '" placeholder="' . $placeholder . '">' . $$name . '</textarea>');
        return $this->endDiv();
    }

    function inputFileUpload($name, $upload_button, $label = 'Válasz ki a fájlt!') {
        $this->newDiv('input-group')->newDiv('custom-file');
        $this->putElement('<input type="file" class="custom-file-input" name="' . $name . '" id="' . $name . '" aria-describedby="' . $upload_button . '">');
        $this->putElement('<label class="custom-file-input" for="'.$name.'">'.$label.'</label>');
        $this->endDiv()->newDiv('input-group-append');
        $this->putElement('<button class="btn btn-primary" type="button" name="' . $upload_button . '" id="' . $upload_button . '">Feltöltés</button>');
        $this->endDiv();
        return $this->endDiv();
    }

    function inputFile($label, $name) {
        $this->newDiv('form-inline my-1');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->putElement('<input type="file" class="form-control-file mx-2" id="' . $name . '" name="' . $name . '" />');
        return $this->endDiv();
    }

    function inputDate($name, $label = '') {
        return $this->inputText($name, $label, '', 'date');
    }

    function inputTime($name, $label = '') {
        return $this->inputText($name, $label, '', 'time');
    }

    function inputCheckbox($name, $label, $submit = false) {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        $this->newDiv('form-inline-sm form-group my-1');
        if (!empty($label)) {
            $this->putElement('<label ' . (empty($for) ? '' : 'for="' . $for . '"') . ' class="col-form-label form-check-label"> ' . $label . ': </label>');
        }
        $this->putElement('<input id = "' . $name . '"  type = "checkbox" name = "' . $name . '"  class="form-check-input m-2" ' .
                ((isset($$name) AND $ $name == "on") ? "checked" : "")
                . ($submit ? ' onChange="submit();" ' : ' onChange="saved=false;"')
                . ' >');
        return $this->endDiv();
    }

    function inputMultiCheckboxInline($name, $value, $label, $submit = false, $title = '') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        if (isset($$name) AND ( in_array($value, $$name))) {
            $checked = ' checked ';
        } else {
            $checked = '';
        }
        $this->newDiv('form-check-inline my-2');
        $this->newElement('<label class="form-check-label' . ($checked ? ' active' : '') . '" title="' . $title . '">');
        $this->putElement('<input  type="checkbox" class="form-check-input mx-1"  name="' . $name . '[]" id="' . $name . '" autocomplete="off" '
                . $checked . 'value="' . $value . '" '
                . ($submit ? ' onChange="submit();" ' : ' onChange="saved=false;"')
                . '>' . $label);
        $this->endElement('</label>');
        return $this->endDiv();
    }

    function inputMultiCheckbox($name, $value, $label, $submit = false, $title = '') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        if (isset($$name) AND ( in_array($value, $$name))) {
            $checked = ' checked ';
        } else {
            $checked = '';
        }
        $this->newDiv('form-check my-2');
        $this->newElement('<label class="form-check-label' . ($checked ? ' active' : '') . '" title="' . $title . '">');
        $this->putElement('<input  type="checkbox" class="form-check-input mx-1"  name="' . $name . '[]" id="' . $name . '" autocomplete="off" '
                . $checked . 'value="' . $value . '" '
                . ($submit ? ' onChange="submit();" ' : ' onChange="saved=false;"')
                . '>' . $label);
        $this->endElement('</label>');
        return $this->endDiv();
    }

    function inputCheckboxCommented($name, $label, $submit = false, $comment = '') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        $this->newDiv('row m-2');
        $this->newDiv('col-3')->putElement($label . ':')->endDiv();
        $this->newDiv('col-1')->putElement('<input id = "' . $name . '"  type = "checkbox" name = "' . $name . '"  class="form-check-input m-1" ' .
                ((isset($$name) AND $ $name == "on") ? "checked" : "")
                . ($submit ? ' onChange="submit();" ' : ' onChange="saved=false;"')
                . ' >')->endDiv();
        $this->newDiv('col-7')->putElement($comment)->endDiv();
        return $this->endDiv();
    }

    function inputRole($chk, $from, $to, $label) {
        global $$chk, $$from, $$to, $today;
        if (!isset($$chk)) {
            error_log($chk . ' variable not set');
            return $this;
        }
        if (!isset($$from)) {
            error_log($from . ' variable not set');
            return $this;
        }
        if (!isset($$to)) {
            error_log($to . ' variable not set');
            return $this;
        }
        $this->newDiv('form-inline form-group m-2');
        $this->putLabel($label, $chk);
        $this->putElement('<input id = "' . $chk . '"  onchange="saved=false;" type = "checkbox" name = "' . $chk . '"  class = "form-check-input form-control-sm mx-1" ' . (( $ $chk == "on") ? "checked" : "") . ' >');
        $this->putElement('<input type="date" onchange="saved=false;" class="form-control-md mx-2" id="' . $from . '" name="' . $from . '" value="' . $$from . '" >');
        $this->putElement('<input type="date" onchange="saved=false;" class="form-control-md mx-2" id="' . $to . '" name="' . $to . '" value="' . $$to . '" >');
        if ($$chk == 'on' AND $today > $$to) {
            $this->warning('Lejárt');
        }
        $this->endDiv();
    }

    function inputClub($label, $arr, $name, $submit) {
        return $this->submitDropDownChooser($label, $arr, $name);
    }

    function inputMemberId($name, $label, $tail = '') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        $this->newDiv('form-inline form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->putElement('<input type="number" min="0" max="9999" size="4" style="width:5em;" onchange="saved=false;" class="form-control mx-2" id="' . $name . '" name="' . $name . '" value="' . $$name . '">');
        $this->put($tail);
        $this->endDiv();
        return $this;
    }

    function inputNumber($name, $label, $tail = '') {
        global $$name;
        if (!isset($$name)) {
            error_log($name . ' variable not set');
            return $this;
        }
        $this->newDiv('form-inline form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $this->putElement('<input type="number"  style="width:5em;" onchange="saved=false;" class="form-control mx-2" id="' . $name . '" name="' . $name . '" value="' . $$name . '">');
        $this->put($tail);
        return $this->endDiv();
    }

    function inputPassword($name, $label) {
        return $this->inputText($name, $label, 'Jelszó', 'password');
    }

    function inputEmail($name, $label) {
        return $this->inputText($name, $label, 'E-mail cím', 'email');
    }

    function submitSwitchCheckbox($name, $text_on, $text_off, $label = '') {
        global $$name;
        $this->newDiv('form-inline form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $checked = (isset($$name) AND $ $name === 'on') ? ' checked ' : '';
        $disabled = (isset($$name) AND $ $name === 'disabled') ? ' disabled' : '';
        $this->putElement('<input class="form-check-input" type="checkbox" ' . $checked . ' ' . $disabled . ' name="' . $name . '" id="' . $name . '" onChange="submit();" '
                . 'data-toggle="toggle" data-size="sm" data-on="' . $text_on . '" data-off="' . $text_off . '" data-onstyle="primary" data-offstyle="primary">');
        return $this->endDiv();
    }

    function inputSwitchCheckbox($name, $text_on, $text_off, $label = '') {
        global $$name;
        $this->newDiv('form-inline form-group m-2');
        if (strlen($label) > 0) {
            $this->putLabel($label, $name);
        }
        $checked = (isset($$name) AND $ $name === 'on') ? ' checked ' : '';
        $disabled = (isset($$name) AND $ $name === 'disabled') ? ' disabled' : '';
        $this->putElement('<input class="form-check-input" type="checkbox" ' . $checked . ' ' . $disabled . ' name="' . $name . '" id="' . $name . '" '
                . 'data-toggle="toggle" data-size="sm" data-on="' . $text_on . '" data-off="' . $text_off . '" data-onstyle="primary" data-offstyle="primary">');
        return $this->endDiv();
    }

    function putLabel($label, $for = '') {
        $this->newDiv('label-md');
        $this->putElement('<label ' . (empty($for) ? '' : 'for="' . $for . '"') . ' class="col-form-label"> ' . $label . ': </label>');
        return $this->endDiv();
    }

    function putHidden($name, $value = false) {
        global $$name;
        if ($value === false AND isset($$name)) {
            $this->putElement('<input type="hidden" name="' . $name . '" id="' . $name . '"  value="' . $$name . '">');
        } elseif ($value != false) {
            $this->putElement('<input type="hidden" name="' . $name . '" id="' . $name . '"  value="' . $value . '">');
        }
        return $this;
    }

    function putModalToggler($modal_name, $text, $title = '') {
        $this->newElement('<button type="button" class="btn btn-primary" data-toggle="modal"  data-target="#' . $modal_name . '"  title="' . $title . '">');
        $this->putElement($text);
        return $this->endElement('</button>');
    }

    function newModal($modal_name, $title, $size = 'md') {
        $this->newDiv('modal fade', $modal_name)->newDiv('modal-dialog modal-' . $size)->newDiv('modal-content');
        $this->newDiv('modal-header');
        $this->putElement('<h4 class="modal-title">' . $title . '</h4>');
        $this->putElement('<button type="button" class="close" data-dismiss="modal">&times;</button>');
        $this->endDiv();
        return $this->newDiv('modal-body')->newDiv('input-group input-group-md');
    }

    function newModalMembers($modal_name, $title) {
        $this->newDiv('modal fade', $modal_name)->newDiv('modal-dialog modal-dialog-scrollable')->newDiv('modal-content');
        $this->newDiv('modal-header');
        $this->putElement('<h4 class="modal-title">' . $title . '</h4>');
        $this->putElement('<button type="button" class="close" data-dismiss="modal">&times;</button>');
        $this->endDiv();
        return $this->newDiv('modal-body')->newDiv('input-group input-group-md');
    }

    function newTab() {
        $this->newElement('<ul class="nav nav-tabs">');
    }

    function putTabItem($name, $value, $text, $title) {
        global $$name;
        if (!isset($$name)) {
            error_log('Undefined valiable: ' . $name);
        }
        $this->newElement('<li class="nav-item">');
        $this->putElement('<button class="btn m-1 navbar-btn btn-secondary ' . ($$name == $value ? 'active' : '')
                . '" name="' . $name . '" id="' . $name . '" title="' . $title . '" value="' . $value . '">' . $text . '</button>');
        $this->endElement('</li>');
    }

    function endTab() {
        $this->endElement('</ul>');
    }

    function endModal() {
        $this->endDiv()->endDiv();
        return $this->endDiv()->endDiv()->endDiv();
    }

    function putPrevent() {
        $this->putScript('var saved=true; '
                . 'function confirmNavigate() { '
                . '     if(!saved) {'
                . '         return confirm("Biztosan el akarja dobni a változásokat?"); '
                . '     } else {'
                . '         return;'
                . '     } '
                . '} '
                . 'window.onbeforeunload = confirmNavigate;');
        return $this;
    }

    function putPostOnchange($var_name) {
        $changed_var_name = 'changed_' . $var_name;
        global $$changed_var_name;
        if (!isset($$changed_var_name)) {
            $$changed_var_name = -1;
        }
        $this->putHidden($changed_var_name);
        $this->newElement('<script type="text/javascript">');
        $this->putElement('function submitOnchange_' . $var_name . '() {'
                . 'var element = document.getElementById("' . $var_name . '");'
                . 'var posted_element = document.getElementById("' . $changed_var_name . '");'
                . 'posted_element.value=element.value;'
                . 'posted_element.form.submit();'
                . '}');
        $this->endElement('</script>');
    }

    function putText($text, $label = '', $bold_label = false) {
        $this->newDiv('form-inline m-2');
        if (strlen($label) > 0) {
            if ($bold_label) {
                $this->putLabel('<strong>' . $label . '</strong>');
            } else {
                $this->putLabel($label);
            }
        }
        $this->putElement($text);
        return $this->endDiv();
    }

    function info($text) {
        $this->newDiv('alert alert-info m-1');
        $this->putElement('<strong>' . $text . '</strong>');
        $this->endDiv();
        return $this;
    }

    function warning($text) {
        $this->newDiv('alert alert-warning  my-1');
        $this->putElement('<strong>' . $text . '</strong>');
        $this->endDiv();
        return $this;
    }

    function danger($text) {
        $this->newDiv('alert alert-danger my-1');
        $this->putElement('<strong>' . $text . '</strong>');
        $this->endDiv();
        return $this;
    }

    function success($text) {
        $this->newDiv('alert alert-success my-1');
        $this->putElement('<strong>' . $text . '</strong>');
        $this->endDiv();
        return $this;
    }

    function newKeret($add_on = '') {
        $this->newDiv('container');
        $this->newDiv('shadow p-1 m-2 rounded');
        $this->newDiv('keret ' . $add_on);
        return $this;
    }

    function endKeret() {
        $this->endDiv()->endDiv()->endDiv();
        return $this;
    }

    function newTable($caption = '') {
        $this->newDiv('table-responsive-sm');
        return $this->newElement('<table class="table table-striped table-bordered" >' . (empty($caption) ? "" : "<caption>" . $caption . "</caption>"));
    }

    function endTable() {
        $this->endElement('</tbody>');
        $this->endElement('</table>');
        return $this->endDiv();
    }

    function putThead($class, $heads) {
        $this->newElement('<thead' . (empty($class) ? '' : ' class="' . $class . '"') . '>');
        $this->newTableTr();
        foreach ($heads as $head) {
            $this->putElement('<th scope="col">' . $head . '</th>');
        }
        $this->endTableTr();
        $this->endElement('</thead>')->newElement('<tbody>');
        return $this;
    }

    function putScript($js) {
        //     error_log('Script:'.$js);
        $this->newElement('<script type="text/javascript">');
        $this->putElement($js);
        $this->endElement('</script>');
    }

    function putTooltipTrigger($toggle = "tooltip") {
        $this->putScript('$(document).ready(function(){$(\'[data-toggle="' . $toggle . '"]\').tooltip();})');
    }

    function newDiv($classes, $id = '') {
        return $this->newElement('<div class = "' . $classes . '"' . (strlen($id) > 0 ? ' id="' . $id . '"' : '') . '>');
    }

    function tab() {
        if ($this->readable AND $this->div_counter > 0) {
            o(str_repeat('  ', $this->div_counter));
        }
        return $this;
    }

    function newline() {
        if ($this->readable) {
            o('
        ');
        }
        return $this;
    }

    function endDiv() {
        $this->endElement('</div>');
        return $this;
    }

    function incTab() {
        $this->div_counter++;
        return $this;
    }

    function decTab() {
        $this->div_counter--;
        return $this;
    }

    function newElement($element) {
        $this->putElement($element);
        if ($this->readable) {
            $this->put('<!-- ' . $this->div_counter . ' -->');
        }
        $this->incTab();
        return $this;
    }

    function endElement($element) {
        $this->decTab()->newline()->tab();
        $this->put($element);
        if ($this->readable) {
            $this->put('<!-- ' . $this->div_counter . ' -->');
        }
        return $this;
    }

    function newTableTr($class = '') {
        if (empty($class)) {
            $this->newElement('<tr>');
        } else {
            $this->newElement('<tr class="' . $class . '">');
        }
        return $this;
    }

    function endTableTr() {
        return $this->endElement('</tr>');
    }

    function putElement($element) {
        $this->newline()->tab();
        $this->put($element);
        return $this;
    }

    function put($str) {
        global $output;
        $output .= $str;
        return $this;
    }

    function setLabelWidth($width) {
        $this->label_width = $width;
        return $this;
    }

    function setFieldWidth($width) {
        $this->field_width = $width;
        return $this;
    }

}
