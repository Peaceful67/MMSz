<?php

function theEnd($output) {
    global $redirected;
    if (!$redirected) {
        header("Content-type:text/html; charset=UTF-8;");
    }
//    error_log($output);
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

function output_label($lab, $bold = false) {
    global $output;
    $output .= '<label>' . ($bold ? "<b>" : "") . $lab . ($bold ? "<b>" : "") . ': </label>';
}

function output_hidden($var_name) {
    global $output, $$var_name;
    if (isset($$var_name)) {
        $output .= '<input type="hidden" name="' . $var_name . '" value="' . $$var_name . '">';
    }
}

function output_save($var_name) {
    output_button($var_name, 0, 'save.jpg');
}

function output_add($var_name) {
    output_button($var_name, 0, 'add.jpg');
}

function output_button($var_name, $value, $image, $title = "") {
    global $output;
    $output .= '<button title="' . $title . '" name="' . $var_name . '" value="' . $value . '">'
            . '<img style="height: 15px; width:15px; padding: 0px;" src="' . IMAGES . $image . '">'
            . '</button>';
}

function output_selector($arr, $name, $submit = true) {
    $id_name = 'selected_' . $name;
    global $output, $$id_name;
    if (!isset($$id_name)) {
        $$id_name = -1;
    }
    $output .= '<select name="selected_' . $name . '" '
            . ' id="selected_' . $name . '" ' . ($submit ? 'onChange="submit();"' : '') . '>';

    $output .= '<option value="-1" '
            . (-1 == $$id_name ? ' selected ' : '') . '>Válassz !!!</option>';
    foreach ($arr as $key => $value) {

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

function output_checkbox($name) {
    global $output, $$name;
    if (isset($$name) AND ( $$name == 'on')) {
        $checked = 'checked';
    } else {
        $checked = '';
    }
    $output .= '<input type="checkbox" name="' . $name . '" ' . $checked . '  >';
}

function output_set_checkbox($name, $bool) {
    global $output, $$name;
    $checked = ($bool ? ' checked ' : '');
    $output .= '<input type="checkbox" name="' . $name . '" ' . $checked . '  >';
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
    $pdf->Output($filename, $toFile ? 'F':'D');
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
