<?php

define("HELP_TABLE", 'help');
define("HELP_MODULE", 'module');
define("HELP_TITLE", 'title');
define("HELP_DESCRIPTION", 'desc');
define("HELP_UPDATED", 'updated');
define("HELP_SUBPAGES", 'subpages');

class Help extends BasicElement {

    function __construct($mod_name) {
        parent::__construct();
        $this->setTableName(HELP_TABLE);
        $this->setTableFields([
            HELP_MODULE,
            HELP_TITLE,
            HELP_DESCRIPTION,
            HELP_UPDATED]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . HELP_TABLE . '` (`'
                . HELP_MODULE . '` varchar(16) NOT NULL, `'
                . HELP_SUBPAGES . '` varchar(256) NOT NULL, `'
                . HELP_UPDATED . '` timestamp(1) ON UPDATE CURRENT_TIMESTAMP() NOT NULL DEFAULT CURRENT_TIMESTAMP() , `'
                . HELP_TITLE . '` varchar(256) NOT NULL, `'
                . HELP_DESCRIPTION . '` text NOT NULL, '
                . 'UNIQUE KEY `' . HELP_MODULE . '` (`' . HELP_MODULE . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8; '
        );
        $this->setPrimaryKey(HELP_MODULE);
        $this->setItemId($mod_name);
        $this->getItemById($mod_name);
    }

    function getTitle() {
        return $this->getValue(HELP_TITLE);
    }

    function getUpdated() {
        return $this->getValue(HELP_UPDATED);
    }

    function setTitle($title) {
        if ($this->isHelpExiting()) {
            $this->updateValue(HELP_TITLE, $title);
        } else {
            $this->insertElement([
                HELP_MODULE => $this->itemId,
                HELP_TITLE => $title
            ]);
        }
    }

    function getDescription() {
        return $this->getValue(HELP_DESCRIPTION);
    }

    function setDescription($desc) {
        if ($this->isHelpExiting()) {
            $this->updateValue(HELP_DESCRIPTION, $desc);
        } else {
            $this->insertElement([
                HELP_MODULE => $this->itemId,
                HELP_DESCRIPTION => $desc
            ]);
        }
    }

    function isHelpExiting() {
        return !empty($this->tableRow);
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        
    }

}
