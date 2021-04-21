<?php

define("LOCALFILE_TABLE", 'files');

define("LOCALFILE_ID", 'id');
define("LOCALFILE_TYPE", 'type');
define("LOCALFILE_NAME", 'name');
define("LOCALFILE_OWNER_ID", 'owner');
define("LOCALFILE_PERMISSION", 'perm');
define("LOCALFILE_ORIGIN", 'origin');
define("LOCALFILE_LOCALNAME", 'local_name');
define("LOCALFILE_UPLOADED", 'uploaded');

define("LOCALFILE_DEFAULT_PATH", '../files/'); 

define("LOCALFILE_TYPE_PROFILE", 1);


class LocalFile extends BasicElement {
    private $directory; 
    function __construct() {
        parent::__construct();
        $this->setTableName(LOCALFILE_TABLE);
        $this->setTableFields([
            LOCALFILE_ID,
            LOCALFILE_TYPE,
            LOCALFILE_NAME,
            LOCALFILE_OWNER_ID,
            LOCALFILE_PERMISSION,
            LOCALFILE_ORIGIN,
            LOCALFILE_LOCALNAME,
            LOCALFILE_UPLOADED,
        ]);
        $this->createTableIfNotExists(
           'CREATE TABLE IF NOT EXISTS `' . LOCALFILE_TABLE . '` (
             `' . LOCALFILE_ID . '` int NOT NULL AUTO_INCREMENT,
            `' . LOCALFILE_TYPE . '` int NOT NULL,
            `' . LOCALFILE_TYPE . '` int NOT NULL,
            `' . LOCALFILE_TYPE . '` int NOT NULL,
            `' . LOCALFILE_TYPE . '` int NOT NULL,                
            `' . LOCALFILE_ORIGIN . '` varchar(64) NOT NULL,
            `' . LOCALFILE_LOCALNAME . '` varchar(32) NOT NULL,
               `' . LOCALFILE_UPLOADED . '` timestamp DEFAULT CURRENTTIMESTAMP(),
                UNIQUE KEY `' . LOCALFILE_ID . '` (`' . LOCALFILE_ID . '`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->key = LOCALFILE_ID;
        if(defined(LOCALFILE_PATH)) {
            $this->directory = LOCALFILE_PATH;
        } else {
            $this->directory = LOCALFILE_DEFAULT_PATH;
        }
        if(is_dir($this->directory)) {
            if(!is_writeable($this->directory)) {
                 error_log('A megadott lokális fájlkönyvtár nem írható: '.$this->directory);
            }
        } else {
            error_log('Hibásan megadott lokális fájlkönyvtár');
        }
    }

}
