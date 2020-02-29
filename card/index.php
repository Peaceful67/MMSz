<?php

chdir('..');
include_once 'params.inc';
redirect("card");
/*
if(isset($_GET)) {
    foreach($_GET as $get => $v) {
        if(isValidMemberId($get)) {
            $card_member = $get;
            break;
        }
    }
}
$output = '<!DOCTYPE  html>
<head>        <meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1">
      <link type="text/css" rel="stylesheet" media="screen" href="'.URL.'/design/horizontal.css">
        <title>MMSz kártyafelület</title>
        </script>        
</head>
<body>
';
include_once INCLUDES . 'card.inc';
$output .= '</body></html>';
theEnd($output);
?>
 * 
 */