<?php

define("DEVELOPMENT", TRUE);
define("FUNCTIONS", "./functions/");
define("SIMPLE_PAY_KEY", "ThjuZeD4h1J1l16jP4T24x6kLqH463ju");
define("SIMPLE_PAY_MERCHANT", "S160501");



if (DEVELOPMENT) {
    define("URL", "http://mmsz-dev.peaceful.hu");
    define("DATABASE_NAME", "tagmodellsport");
    define("DATABASE_USER", "mmsz");
    define("DATABASE_PASSWORD", "CsaCsi67");
    define("DATABASE_ADDR", "localhost");
    define("SMTP_USER", "");
    define("SMTP_PASSWD", "");
    define("TMP_DIR" , "/tmp/");    
} else {
    define("DATABASE_ADDR", "mysql56");
    define("URL", "http://tag.modellsport.hu");
    define("DATABASE_NAME", "tagmodellsport");
    define("DATABASE_USER", "tagmodellsport");
    define("DATABASE_PASSWORD", "F8e2aaXX97H");
    define("SMTP_USER", "tagmsportweb");
    define("SMTP_PASSWD", "V7a63WPA5Xf");
    define("TMP_DIR" , "../tmp/");
}

define("COMPANY_NAME", "MMSz");
define("COMPANY_LONG_NAME", "Magyar Modelező Szövetség");
define("COMPANY_LOGO", 'http://modellsport.hu/sites/default/files/mmsz_logo_regi_orig.png');
