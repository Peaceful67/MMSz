<?php

ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 1);

include_once 'params.inc';
include_once FUNCTIONS . 'init.inc';
include_once INCLUDES . 'crontab.inc';

session_gc();
