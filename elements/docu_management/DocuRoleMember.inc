<?php

define("DOCU_ROLE_TABLE", 'docu_rele');
define("DOCU_ROLE_ID", 'id');
define("DOCU_ROLE_MEMBER_ID", 'member_id');
define("DOCU_ROLE_NAME", 'name');
define("DOCU_ROLE_VALUE", 'value');
define("DOCU_ROLE_FROM", 'from');
define("DOCU_ROLE_TO", 'to');
define("DOCU_ROLE_VARIABLES", 'variables');

/**
 * Description of DocuRole
 *   A dokumentum kezelő jogosultágai
 * @author Peaceful
 */
class DocuRole extends BasicElement {
   
    function __construct() {
        parent::__construct();
        $this->setTableName(DOCU_ROLE_TABLE);
        $this->setTableFields([
            DOCU_ROLE_ID,
            DOCU_ROLE_MEMBER_ID,
            DOCU_ROLE_NAME,
            DOCU_ROLE_VALUE,
            DOCU_ROLE_FROM,
            DOCU_ROLE_TO
        ]);
        $this->setVariablesName(DOCU_ROLE_VARIABLES);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . DOCU_ROLE_TABLE . '` (`'
                . DOCU_ROLE_ID . '` int NOT NULL AUTO_INCREMENT,`'
                . DOCU_ROLE_MEMBER_ID . '` int NOT NULL DEFAULT -1,`'
                . DOCU_ROLE_NAME . '` int NOT NULL DEFAULT -1,`'
                . DOCU_ROLE_VALUE . '` int NOT NULL DEFAULT -1,`'
                . DOCU_ROLE_FROM . '` DATE NULL DEFAULT NULL,`'
                . DOCU_ROLE_TO . '` DATE NULL DEFAULT NULL,'
                . 'UNIQUE KEY `' . DOCU_ROLE_ID_ID . '` (`' . DOCU_ROLE_ID . '`)) '
                . 'ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(DOCU_ROLE_ID);
    }

    public function deleteRole($id) {
        return $this->deleteElementById($id);
    }

    public function updateRole($id, $name, $value, $from, $to) {
        $this->updateElementById($id, DOCU_ROLE_NAME, $name);
        $this->updateElementById($id, DOCU_ROLE_VALUE, $value);
        $this->updateElementById($id, DOCU_ROLE_FROM, $from);
        $this->updateElementById($id, DOCU_ROLE_TO, $to);
    }

    public function getRoleById($id) {
        return $this->getItemById($id);
    }

    public function getItems() {
        
    }

    public function getNames() {
        
    }

    public function isDeletable($id) {
        return true;
    }

}
