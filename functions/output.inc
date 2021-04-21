<?php

$headerAlready = false;

function theEnd($output) {
    global $redirected, $headerAlready;
 
    if (!$headerAlready AND ! $redirected) {
        header("Content-type:text/html; charset=UTF-8;");
        header('Cache-Control: no-cache, no-store, must-revalidate');
        header('Pragma: no-cache');
        header('Expires: 0');
        $headerAlready = true;
    }
    echo $output;
}

function output_spaces($num) {
    global $output;
    $output .= str_repeat('&nbsp;', $num);
}

function o($str) {
    global $output;
    $output .= $str;
}

function output_ln($num = 1) {
    global $output;
    $output .= str_repeat('<br>', $num);
}

function output_label($lab = NULL, $bold = false, $right = false, $width = 0) {
    global $output;
    if (is_null($lab)) {
        $output .= '<label>  </label>';
    } else {
        $output .= '<label style="'
                . ($right ? ' text-align: right; margin-right:10px; ' : '')
                . ($width > 0 ? ' width: ' . $width . 'px;";' : '')
                . '">' . ($bold ? "<b>" : "") . $lab . ($bold ? "</b>" : "") . ': </label>';
    }
}

function o_div($classes) {
    o('<div class="' . $classes . '">');
}

function o_div_end() {
    o('</div>');
}

function output_reload() {
    o('<script type="text/javascript"> window.onload = formAutoSubmit;</script>');
}

/**
 * Description of output_prevent()
 * Ezt meghívni a vedendo oldal elejen és onchange="saved=false;" a védendő inputmezokbe, onclick="saved=true;" a mentő gombaba
 * @author Peaceful
 */
function output_prevent() { // Ezt meghívni a vedendo oldal elejen és onchange="saved=false;" a védendő inputmezokbe, onclick="saved=true;" a mentő gombaba
    o('<script type="text/javascript"> saved=true; function confirmNavigate() { if(!saved) {return confirm("Biztosan el akarja dobni a változásokat?"); } else {return;} } </script>');
    o('<script type="text/javascript"> window.onbeforeunload = confirmNavigate;</script>');
}

function output_not_saved() {
    o('<script type="text/javascript"> saved=false; </script>');
}

function output_hidden($var_name) {
    global $output, $$var_name;
    if (isset($$var_name)) {
        $output .= '<input type="hidden" name="' . $var_name . '" id="' . $var_name . '" value="' . $$var_name . '">';
    }
}

function output_time($time) {
    return ((is_null($time) OR empty($time)) ? 'Nincs meghatározva' : date('Y-m-d H:i', strtotime($time)));
}

function set_default_value($var_name, $def_value) {
    global $$var_name;
    if (!isset($$var_name)) {
        $$var_name = $def_value;
    }
    output_hidden($var_name);
}

function output_save($var_name, $value = 0, $title = "Módosítások mentése") {
    global $output;
    $output .= '<button onclick="saved=true;" title="' . $title . '" name="' . $var_name . '" value="' . $value . '">'
            . '<img src="' . IMAGES . 'save.jpg">'
            . '</button>';
}

function output_cancel($var_name, $value = 0, $title = "A módosítások elvetése") {
    global $output;
    $output .= '<button onclick="saved=true;" title="' . $title . '" name="' . $var_name . '" value="' . $value . '">'
            . '<img src="' . IMAGES . 'back.jpg">'
            . '</button>';
}

function output_delete($var_name, $value = 0, $title = "") {
    global $output;
    $output .= '<button onclick="saved=true;" title="' . $title . '" name="' . $var_name . '" value="' . $value . '">'
            . '<img src="' . IMAGES . 'delete.jpg">'
            . '</button>';
}

function output_button_text($name, $text, $value = -1) {
    o('<button type="submit" class="btn btn-primary" name="' . $name . '" value="' . $value . '">' . $text . '</button>');
}

function output_add($var_name, $title = "") {
    output_button($var_name, 0, 'add.jpg', $title);
}

/**
 * Description of output_button()
 *  paraméterek: ($var_name, $value, $image, $title = "", $confirm_message = ""
 * Ha az image üres vagy null, akkor a value szöveget jelenít meg
 * @author Peaceful
 */
function output_button($var_name, $value, $image, $title = "", $confirm_message = "") {
    global $output;
    $onclick = (empty($confirm_message)) ? '' : ' onclick="return confirm(\'' . $confirm_message . '\');"';
    $output .= '<button title="' . $title . '" id="' . $var_name . '"  name="' . $var_name . '" value="' . $value . '" ' . $onclick . '>';
    if (!is_null($image) AND ! empty($image)) {
        $output .= '<img  src="' . IMAGES . $image . '">';
    } else {
        $output .= $value;
    }
    $output .= '</button>';
}

function output_post_onchange_checkbox($var_name) {
    $changed_var_name = 'changed_' . $var_name;
    o('<input type="hidden" name="' . $changed_var_name . '" id="' . $changed_var_name . '">');
    o('<script type="text/javascript">');
    o('function submitOnchange_' . $var_name . '(chk_id) {'
            . 'var posted_element = document.getElementById("' . $changed_var_name . '");'
            . 'posted_element.value=chk_id;'
            . 'posted_element.form.submit();'
            . '}');
    o('</script>');
}

function output_post_onchange($var_name) {
    $changed_var_name = 'changed_' . $var_name;
    global $$changed_var_name;
    if (!isset($$changed_var_name)) {
        $$changed_var_name = -1;
    }
    o('<input type="hidden" name="' . $changed_var_name . '" id="' . $changed_var_name . '" >');
    o('<script type="text/javascript">');
    o('function submitOnchange_' . $var_name . '() {'
            . 'var element = document.getElementById("' . $var_name . '");'
            . 'var posted_element = document.getElementById("' . $changed_var_name . '");'
            . 'posted_element.value=element.value;'
            . 'posted_element.form.submit();'
            . '}');
    o('</script>');
}

function output_switch_checkbox($var_name, $text_on, $text_off) {
    global $$var_name;
    $checked = (isset($$var_name) AND $ $var_name === 'on');
    $disabled = (isset($$var_name) AND $ $var_name === 'disabled');
    o('<script type="text/javascript">'
            . 'function onClick_' . $var_name . '() {'
            . 'var checkBox = document.getElementById("' . $var_name . '");'
            . 'var text = document.getElementById("text_' . $var_name . '");'
            . 'if (checkBox.checked == true){
                     text.innerHTML = "' . $text_on . '";
                } else {
                     text.innerHTML = "' . $text_off . '";
                }'
            . '}'
            . '</script>');
    o('<label class="switch-checkbox"><input type="checkbox" onchange="saved=false;"  name="' . $var_name . '" id="' . $var_name . '" ');
    if ($disabled) {
        o('disabled');
    } else {
        o(($checked ? 'checked' : '') . ' onclick="onClick_' . $var_name . '()"');
    }
    o('><span class="slider round"></span></label>');
    output_spaces(3);
    o('<label class="switch-checkbox-message" id="text_' . $var_name . '">' . ($checked ? $text_on : $text_off) . '</label>');
}

function output_switch_radio($var_name, $value_selected) {
    global $$var_name;
    o('<label class="switch-radio"><input value="' . $value_selected . '" type="radio" onchange="saved=false;" name="' . $var_name . '"');
    if (isset($$var_name) AND $ $var_name === $value_selected) {
        o(' checked="checked"');
    }
    o(' ><span class="checkmark"></span></label>');
}

function output_selector($arr, $name, $submit, $default = 'Válassz !!!') {
    $id_name = 'selected_' . $name;
    global $output, $$id_name;
    if (!isset($$id_name)) {
        $$id_name = -1;
    }
    $sel_arr = [-1 => $default] + $arr;
    $output .= '<select name="' . $id_name . '" '
            . ' id="selected_' . $name . '" ' . ($submit ? 'onChange="submit();"' : 'onchange="saved=false;"') . '>';
    foreach ($sel_arr as $key => $value) {

        $output .= '<option value="' . $key . '" '
                . ($$id_name == $key ? ' selected ' : '') . '>' . $value . '</option>';
    }
    $output .= '</select>';
}

function output_chooser($arr, $name, $submit) {
    $id_name = 'selected_' . $name;
    global $output, $$id_name;
    if (!isset($$id_name)) {
        $$id_name = end($arr);
    }
    $output .= '<select name="' . $id_name . '" '
            . ' id="selected_' . $name . '" ' . ($submit ? 'onChange="submit();"' : 'onchange="saved=false;"') . '>';
    foreach ($arr as $key => $value) {

        $output .= '<option value="' . $key . '" '
                . ($$id_name == $key ? ' selected ' : '') . '>' . $value . '</option>';
    }
    $output .= '</select>';
}

function output_selector_change($arr, $name, $submit, $default = 'Válassz !!!') {
    $id_name = 'selected_' . $name;
    global $output, $$id_name;
    if (!isset($$id_name)) {
        $$id_name = -1;
    }
    $sel_arr = [-1 => $default] + $arr;

    $output .= '<select name="' . $id_name . '" '
            . ' id="selected_' . $name . '" ' . ($submit ? 'onChange="submitOnchange_' . $id_name . '();"' : '') . '>';
    foreach ($sel_arr as $key => $value) {

        $output .= '<option value="' . $key . '" '
                . ($$id_name == $key ? ' selected ' : '') . '>' . $value . '</option>';
    }
    $output .= '</select>';
}

function output_multiple_selector($arr, $name) {
    $id_name = 'selected_' . $name;
    global $output, $$id_name;
    if (!isset($$id_name)) {
        $$id_name = array();
    }
    $output .= '<select name="selected_' . $name . '[]"   multiple >';
    foreach ($arr as $key => $value) {
        $output .= '<option value="' . $key . '" '
                . (in_array($key, $$id_name) ? ' selected ' : '') . '>' . $value . '</option>';
    }
    $output .= '</select>';
}

function output_checkbox($name, $submit = false) {
    global $output, $$name;
    if (isset($$name) AND ( $$name == 'on')) {
        $checked = 'checked';
    } else {
        $checked = '';
    }
    $output .= '<input type="checkbox" name="' . $name . '" ' . $checked . '  ' . ($submit ? 'onClick="submit();" ' : '') . '>';
}

function output_multicheckbox($name, $value, $submit = false) {
    global $output, $$name;
    if (isset($$name) AND ( in_array($value, $$name))) {
        $checked = 'checked';
    } else {
        $checked = '';
    }
    $output .= '<input type="checkbox" name="' . $name . '[]" value="' . $value . '" ' . $checked . ' ' . ($submit ? 'onClick="submit();" ' : '') . ' >';
}

function output_set_checkbox($name, $bool, $submit = false) {
    global $output, $$name;
    $checked = (($bool) ? ' checked ' : '');
    $output .= '<input type="checkbox" name="' . $name . '" ' . $checked . ' ' . ($submit ? 'onClick="submit();" ' : '') . ' >';
}

function output_radio($name, $value, $submit = false, $title = '') {
    global $$name;
    o('<input type="radio" name="' . $name . '" title="' . $title . '" value="' . $value . '" ' . ($$name == $value ? " checked " : "") . '  ' . ($submit ? 'onClick="submit();" ' : '') . '>');
}

function php2jsDate($time_str, $date) {
    $time = strtotime($time_str, strtotime($date));
    $year = date('Y', $time);
    $month = date('m', $time) - 1;
    $day = date('d', $time);
    return $year . ', ' . $month . ', ' . $day;
}

function downloadPDF($filename, $body, $toFile = false) {
    require_once CLASSES . '/vendor/autoload.php';
    $pdf = new \Mpdf\Mpdf(
            ['tempDir' => TMP_DIR], ['mode' => '+aCJK'], ['format' => 'A4'], ['orientation' => 'L']);
    $pdf->WriteHTML(iconv("UTF-8", "UTF-8", $body));
    $pdf->SetFooter('{PAGENO}');
    $pdf->Output($filename, $toFile ? 'F' : 'D');
    if (!$toFile) {
        global $redirected;
        $redirected = true;  // Ne kuldjuk a headert ujra
    }
}

function outputAvatarOfMember($mem_id) {
    $ret = '';
    $file = '.' . AVATAR . $mem_id . '.jpg';
    if (is_file($file)) {
        $mtime = filemtime($file);
        $ret .= '<img src="' . AVATAR_URL . $mem_id . '.jpg?' . $mtime . '"/>';
    }
    return $ret;
}

function downloadFile($real_path, $file_name) {
    if (file_exists($real_path)) {
        header('Content-Description: File Transfer');
        header('Content-Type: application/octet-stream');
        header('Content-Disposition: attachment; filename=' . $file_name);
        header('Content-Transfer-Encoding: binary');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Pragma: public');
        header('Content-Length: ' . filesize($real_path));
        ob_clean();
        flush();
        readfile($real_path);
        exit;
    }
}

function viewFile($real_path, $orig_filename) {
    $mime = mime_content_type($real_path);
    if (file_exists($real_path) AND $mime) {
        header('Content-Type: ' . $mime);
        header('Content-Disposition: inline; filename="' . $orig_filename . '"');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Pragma: public');
        header('Content-Length: ' . filesize($real_path));
        ob_clean();
        flush();
        readfile($real_path);
        exit;
    }
}

function output_back_history() {
    if (is_history()) {
        output_button('back_to_history', 0, 'back.jpg', 'Visszatérés az előző szerkesztés ablakba');
        output_spaces(5);
    }
}
