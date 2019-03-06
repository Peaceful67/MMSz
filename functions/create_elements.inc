<?php

foreach (glob(ELEMENTS . "/*.php") as $filename) {
    require_once $filename;
}



