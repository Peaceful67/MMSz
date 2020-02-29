<?php

$chapthas = array(
    array(12, 'háromszor négy'),
    array(25, 'ötször öt'),
    array(42, 'hétszer hat'),
    array(32, 'négyszer nyolc'),
    array(27, 'háromszor kilenc'),
    array(42, 'hatszor hét'),
    array(63, 'hétszer kilenc'),
    array(36, 'hatszor hat'),
    array(36, 'négyszer kilenc'),
    array(72, 'nyolcszor kilenc'),
    array(24, "négyszer hat"),
    array(54, 'hatszor kilenc'),
    array(56, 'nyolcszor hét'),
);
$item = rand(0, count($chapthas) - 1);
$chaptha_text = $chapthas[$item][1];
$chaptha_number = $chapthas[$item][0];

