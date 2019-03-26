<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Szamlazz
 *
 * @author Peaceful
 */
define("SZAMLAZZ_TABLE", "szamlazz");
define("SZAMLAZZ_ID", "id");
define("SZAMLAZZ_BILL_NUMBER", "bill_number");
define("SZAMLAZZ_NAME", "name");
define("SZAMLAZZ_EMAIL", "email");
define("SZAMLAZZ_ADDR_POST", "addr_post");
define("SZAMLAZZ_ADDR_CITY", "addr_city");
define("SZAMLAZZ_ADDR_STREET", "addr_street");
define("SZAMLAZZ_TAX", "tax");
define("SZAMLAZZ_PREPARED", "prepared");
define("SZAMLAZZ_DONE", "done");

class Szamlazz extends BasicElement {

    public $bill_number;

    function __construct() {
        parent::__construct();
        $this->setTableName(SZAMLAZZ_TABLE);
        $this->setTableFields([SZAMLAZZ_ID, SZAMLAZZ_BILL_NUMBER, SZAMLAZZ_NAME, SZAMLAZZ_EMAIL,
            SZAMLAZZ_ADDR_POST, SZAMLAZZ_ADDR_CITY, SZAMLAZZ_ADDR_STREET,
            SZAMLAZZ_TAX, SZAMLAZZ_PREPARED, SZAMLAZZ_DONE]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `szamlazz` (
                    `id` int(8) NOT NULL AUTO_INCREMENT,
                    `bill_number` varchar(16) NOT NULL,
                    `name` varchar(64) NOT NULL,
                    `email` varchar(31) NOT NULL,
                    `addr_post` varchar(4) NOT NULL,
                    `addr_city` varchar(22) NOT NULL,
                    `addr_street` varchar(32) NOT NULL,
                    `tax` varchar(16) NOT NULL,
                    `prepared` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1),
                    `done` timestamp(1) NULL DEFAULT NULL,
                    PRIMARY KEY (`id`),
                    KEY `bill_number` (`bill_number`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8; ');
    }

    public function getElementByBillNumber($bn) {
        $ret = array();
        if ($this->getElementBy(SZAMLAZZ_BILL_NUMBER, $bn)) {
            $this->bill_number = $this->getValue(SZAMLAZZ_BILL_NUMBER);
            error_log("Bill Number: " . $this->bill_number);
            $ret = $this->tableRow;
        }
        return $ret;
    }

    public function deleteSzamlaByBillNumber($bn) {
        $this->deleteElementBy(SZAMLAZZ_BILL_NUMBER, $bn);
    }

    public function insertSzamla($number, $name, $post, $city, $street, $tax) {
        global $member_id;
        if (!$this->insertElement([
                    SZAMLAZZ_BILL_NUMBER => $number,
                    SZAMLAZZ_NAME => $name,
                    SZAMLAZZ_ADDR_POST => $post,
                    SZAMLAZZ_ADDR_CITY => $city,
                    SZAMLAZZ_ADDR_STREET => $street,
                    SZAMLAZZ_TAX => $tax
                ])) {
            error_log('Nem sikerült a szamlat elokesziteni');
        }
        logger($member_id, -1, LOGGER_SZAMLAZZ, $number . ' számla előkészítése');
    }

}
