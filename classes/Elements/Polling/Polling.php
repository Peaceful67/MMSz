<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Polling
 *
 * @author Peaceful
 */
include_once 'Poll.php';
include_once 'PollMember.php';
include_once 'PollOption.php';
include_once 'PollVote.php';

class Polling {
private $poll, $pollMember, $pollOption, $pollVote;
      function __construct() {
          $this->poll = new Poll();
          $this->pollMember = new PollMember();
          $this->pollOption = new PollOption();
          $this->pollVote = new PollVote();
      }
        
}
