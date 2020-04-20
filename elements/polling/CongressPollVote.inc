<?php

define('CONGRESS_POLL_VOTE_TABLE', "congress_poll_vote");
define('CONGRESS_POLL_VOTE_ID', "id");
define('CONGRESS_POLL_VOTE_POLL_ID', "poll_id");
define('CONGRESS_POLL_VOTE_DELEGATE', "mem_id");
define('CONGRESS_POLL_VOTE_OPTION_ID', "option_id");
define('CONGRESS_POLL_VOTE_VOTED_TIME', "voted");

class CongressPollVote extends BasicElement {

    function __construct() {
        parent::__construct();
        $this->setTableName(CONGRESS_POLL_VOTE_TABLE);
        $this->setTableFields([
            CONGRESS_POLL_VOTE_ID,
            CONGRESS_POLL_VOTE_POLL_ID,
            CONGRESS_POLL_VOTE_DELEGATE,
            CONGRESS_POLL_VOTE_OPTION_ID,
            CONGRESS_POLL_VOTE_VOTED_TIME,
        ]);
        $this->createTableIfNotExists(
                'CREATE TABLE IF NOT EXISTS `' . CONGRESS_POLL_VOTE_TABLE . '` (`'
                . CONGRESS_POLL_VOTE_ID . '` int(8) NOT NULL AUTO_INCREMENT,`'
                . CONGRESS_POLL_VOTE_OPTION_ID . '` int(8) NOT NULL DEFAULT -1, `'
                . CONGRESS_POLL_VOTE_POLL_ID . '` int(8) NOT NULL DEFAULT -1, `'
                . CONGRESS_POLL_VOTE_DELEGATE . '` int(8) NOT NULL DEFAULT -1, `'
                . CONGRESS_POLL_VOTE_VOTED_TIME . '` timestamp(1) NOT NULL DEFAULT CURRENT_TIMESTAMP(1), '
                . 'UNIQUE KEY `' . CONGRESS_POLL_VOTE_ID . '` (`' . CONGRESS_POLL_VOTE_ID . '`))'
                . ' ENGINE=InnoDB DEFAULT CHARSET=utf8;');
        $this->setPrimaryKey(CONGRESS_POLL_VOTE_ID);
    }

}
