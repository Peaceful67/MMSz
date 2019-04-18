<?php

define("RADIO_CUSTOMER_SELF", "self");
define("RADIO_CUSTOMER_CLUB", "club");
define("RADIO_CUSTOMER_OWN", "own");
/*
  if (!isset($radio_customer)) {
  $radio_customer = RADIO_CUSTOMER_CLUB;
  $selected_club = getMemberClubId($member_id);
  }
 * 
 */
if (!isset($selected_club)) {
    $selected_club = -1;
}
if (!isset($invoice_name)) {
    $invoice_name = '';
}
if (!isset($invoice_addr_post)) {
    $invoice_addr_post = '';
}
if (!isset($invoice_addr_city)) {
    $invoice_addr_city = '';
}
if (!isset($invoice_addr_street)) {
    $invoice_addr_street = '';
}
if (!isset($invoice_tax)) {
    $invoice_tax = '';
}

$output .= '<div class="keret">';
$output .= '<h4>Számla Vevőadatok</h4>';
/*
  $output .= '<label>Saját név:</label><input type="radio" name="radio_customer" value="' . RADIO_CUSTOMER_SELF . '"  onChange="submit();" '
  . (($radio_customer == RADIO_CUSTOMER_SELF) ? ' checked ' : '') . '>';
  $output .= '<br>';
  $output .= '<label>Egyesület:</label><input type="radio" name="radio_customer" value="' . RADIO_CUSTOMER_CLUB . '"  onChange="submit();" '
  . (($radio_customer == RADIO_CUSTOMER_CLUB) ? ' checked ' : '') . '>';


  $output .= '&nbsp;&nbsp;&nbsp;&nbsp;<select id="selected_club" name="selected_club" onchange="submit();">';
  $output .= '<option value="-1">Válassz egyesületet !</option>';
  $clubs = getClubNameList();
  foreach ($clubs as $club_id => $club_name) {
  $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
  }
  $output .= '</select>';

  $output .= '<br>';
  $output .= '<label>Egyéni</label><input type="radio" name="radio_customer" value="' . RADIO_CUSTOMER_OWN . '"  onChange="submit();" '
  . (($radio_customer == RADIO_CUSTOMER_OWN) ? ' checked ' : '') . '>';
  $output .= '<br><hr>';
  switch ($radio_customer) {
  case RADIO_CUSTOMER_SELF:
  $invoice_name = getMemberName($member_id);
  $mem = getMemberById($member_id);
  $invoice_addr_post = $mem[MEMBER_ADDR_POST];
  $invoice_addr_city = $mem[MEMBER_ADDR_CITY];
  $invoice_addr_street = $mem[MEMBER_ADDR_STREET];
  $invoice_tax = '';
  break;
  case RADIO_CUSTOMER_CLUB:
  if ($selected_club >= 0) {
  $invoice_club = getClub($selected_club);
  $invoice_name = $invoice_club[CLUB_NAME];
  $invoice_addr_post = $invoice_club[CLUB_ADDR_POST];
  $invoice_addr_city = $invoice_club[CLUB_ADDR_CITY];
  $invoice_addr_street = $invoice_club[CLUB_ADDR_STREET];
  } else {
  $invoice_name = '';
  }
  break;
  case RADIO_CUSTOMER_OWN:
  default:
  break;
  }
 * 
 */
if (isPermitted(ACCESS_ADMIN) OR isPermitted(ACCESS_MMSZ)) {
    $clubs = getClubNameList();
} else {
    $clubs = getClubsMember($member_id);
}
if (count($clubs) > 1) {
    $output .= '<label>Egyesület:</label>'
            . '<select id="selected_club" name="selected_club" onchange="submit();">';
//    $output .= '<option value="-1">Válassz egyesületet !</option>';
    foreach ($clubs as $club_id => $club_name) {
        $output .= '<option value="' . $club_id . '" ' . (($club_id == $selected_club) ? "selected" : "") . '>' . $club_name . '</option>';
    }
    $output .= '</select><br>';
} else {
    $selected_club = key($clubs);
    if ($selected_club == NULL) {
        warning('Nincs kiválasztható egyesület a számlázáshoz !');
    }
}

if ($selected_club >= 0) {
    $invoice_club = getClub($selected_club);
    $invoice_name = $invoice_club[CLUB_NAME];
    $invoice_addr_post = $invoice_club[CLUB_ADDR_POST];
    $invoice_addr_city = $invoice_club[CLUB_ADDR_CITY];
    $invoice_addr_street = $invoice_club[CLUB_ADDR_STREET];
    $invoice_tax = $invoice_club[CLUB_TAX];
} else {
    $invoice_name = '';
}
$output .= '<label>Név:</label><input type="text" name="invoice_name" value="' . $invoice_name . '" ' . (empty($invoice_name) ? '' : 'readonly') . '><br>';
$output .= '<label>Irsz:</label><input type="text" name="invoice_addr_post" value="' . $invoice_addr_post . '" ' . (empty($invoice_addr_post) ? '' : 'readonly') . '><br>';
$output .= '<label>Város:</label><input type="text" name="invoice_addr_city" value="' . $invoice_addr_city . '" ' . (empty($invoice_addr_city) ? '' : 'readonly') . '><br>';
$output .= '<label>Cím:</label><input type="text" name="invoice_addr_street" value="' . $invoice_addr_street . '" ' . (empty($invoice_addr_street) ? '' : 'readonly') . '><br>';
$output .= '<label>Adószám:</label><input type="text" name="invoice_tax" value="' . $invoice_tax . '" ' . (empty($invoice_tax) ? '' : 'readonly') . '>';
$output .= '</div>';
$output .= '<br><br>';
