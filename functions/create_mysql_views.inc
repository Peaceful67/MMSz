<?php
/*
define("MYSQL_VIEW_ALL_ACTIVE_CLUBS", 'act_clubs');

$created_mysql_views = [
    MYSQL_VIEW_ALL_ACTIVE_CLUBS => 'SELECT * FROM `'.CLUB_TABLE.'` WHERE `'.CLUB_ACT.'`=1'
];


//$sql = 'SHOW FULL TABLES FROM `'.DATABASE_NAME.'` WHERE `Table_type`="VIEW"';

foreach ($created_mysql_views as $key => $sql) {
   $mysqliLink->query('CREATE OR REPLACE `'.$key.'` AS '.$sql);  
}
 
 */