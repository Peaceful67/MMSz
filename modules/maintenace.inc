<?php

$output .= '<h3>Az itt található tesztek esetleg hosszan futhatnak, légy türelemmel !</h3>';

$output .= '<form method="post">';
$output .= '<label>Név egyezőség: </label><input type="submit" name="multi_members" value="Futtat"></br>';
$output .= '</form>';

if(isset($multi_members)) {
    include_once INCLUDES.'multi_members.inc';
}
