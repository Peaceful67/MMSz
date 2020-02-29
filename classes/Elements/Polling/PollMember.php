<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of PollMember
 *
 * @author Peaceful
 */
define("POLL_MEMBER_TABLE", 'poll_member');
define("POLL_MEMBER_ID", 'id');
define("POLL_MEMBER_POLL_ID", 'poll_id');
define("POLL_MEMBER_MEM_ID", 'mem_id');

class PollMember extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(POLL_MEMBER_TABLE);
        $this->setTableFields([POLL_MEMBER_ID, POLL_MEMBER_POLL_ID, POLL_MEMBER_MEM_ID]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `poll_member` (
                        `id` int(8) NOT NULL AUTO_INCREMENT,
                        `poll_id` int(8) NOT NULL,
                        `mem_id` int(8) NOT NULL,
                    PRIMARY KEY (`id`)
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
    }

}
