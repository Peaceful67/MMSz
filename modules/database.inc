<?php

$mime = "application/x-gzip";
if (!isset($fai_mem_id)) {
    $fai_mem_id = 1508;
}

if (isset($cron_daily)) {
    $output .= 'Napi rutin lefuttatva';
    include_once INCLUDES . 'cron_daily.inc';
}
if (isset($create_tables)) {
    $output .= 'Táblák létrehozva<br>Eredmény a szerver logokban';
    include_once INCLUDES . 'create_tables.inc';
}

if (isset($integrity)) {
    $error = false;
    $output .= '<div class="keret">';
    include_once INCLUDES . 'db_integrity.inc';
    if (!$error) {
        logger($member_id, -1, LOGGER_DATABASE, "Az ellenőrzés sikeresen lefutott");
    }
    $output .= '</div>';
}
if (isset($system_error)) {
    mail_system_error("Próba rendszer hibaüzenet<br>Árvíztűrő tükörfúrógép");
    $output .= 'Hibaüzenet elküldve';
}
if (isset($phpinfo)) {
    $extensions = get_loaded_extensions();
    $output .= '<h2>Loaded PHP extensions:</h2>';
    foreach ($extensions as $key => $value) {
        $output .= $key . ' => ' . $value . ', ';
    }
    $output .= '<br><br>';
    ob_start();
    phpinfo();
    $output .= ob_get_contents();
    ob_clean();
}
if (isset($relative)) {
    include_once FUNCTIONS . 'docu_functions.inc';
// $docuFile = new DocuFile();
    $sql = 'SELECT * FROM `' . DOCU_FILE_TABLE . '`;';
    $res = $mysqliLink->query($sql);
    $changed = 0;
    while ($res AND $row = $res->fetch_assoc()) {
        $path = $row[DOCU_FILE_REAL_PATH];
        if ($path[0] == '/') {
            $changed++;
            $new_path = substr(strstr($path, LOCALFILE_PATH, false), strlen(LOCALFILE_PATH) + 1);
            error_log($new_path);
            $mysqliLink->query('UPDATE `' . DOCU_FILE_TABLE . '` SET `' . DOCU_FILE_REAL_PATH . '`="' . $new_path . '" WHERE `' . DOCU_FILE_ID . '`=' . $row[DOCU_FILE_ID] . ';');
        }
    }
    o('Átalakítva:' . $changed . ' tétel');
}
if (isset($anonim)) {
    include_once FUNCTIONS . 'vote_functions.inc';
    $congressPoll = new CongressPoll();
    $congressDelegates = new CongressDelegates();
    $congressPollVote = new CongressPollVote();
    $num_votes = $changed_votes = $num_polls = 0;
    $sql = 'SELECT * FROM `' . CONGRESS_POLL_TABLE . '` WHERE (`' . CONGRESS_POLL_OPTIONS . '`&' . CONGRESS_OPTIONS_BY_NAME . ')=0;';
    $poll_res = $mysqliLink->query($sql);
    while ($poll_res AND $row = $poll_res->fetch_assoc()) {
        $poll_id = $row[CONGRESS_POLL_ID];
        $congressDelegates->setCongressIdByPollId($poll_id);
        $num_polls++;
        $votes_of_options = array();
        $sql = 'SELECT * FROM `' . CONGRESS_POLL_VOTE_TABLE . '` WHERE `' . CONGRESS_POLL_VOTE_POLL_ID . '`=' . $poll_id . ' AND `' . CONGRESS_POLL_VOTE_DELEGATE . '`>0;';
        $res = $mysqliLink->query($sql);
        while ($res AND $row = $res->fetch_assoc()) {
            $option_id = $row[CONGRESS_POLL_VOTE_OPTION_ID];
            $num_votes++;
            if ($option_id > 0) {
                $changed_votes++;
                $mandat = $congressDelegates->getMandateNumber($row[CONGRESS_POLL_VOTE_DELEGATE]);
                if (isset($votes_of_options[$option_id])) {
                    $votes_of_options[$option_id] += max($mandat, 1);
                } else {
                    $votes_of_options[$option_id] = max($mandat, 1);
                }
//                $congressPollVote->updateAnonimCounter($poll_id, $option_id, max($mandat, 1));
                $congressPollVote->removeVotedOption($row[CONGRESS_POLL_VOTE_ID]);
            }
        }
        foreach ($votes_of_options as $option_id => $mandat) {
            $congressPollVote->createAnonimCounter($poll_id, $option_id, $mandat);
        }
    }
    output_label('Összes érintett napirend');
    o($num_polls);
    output_ln(2);
    output_label('Összes érintett szavazás');
    o($num_votes);
    output_ln(2);
    output_label('Módosított szavazás');
    o($changed_votes);
    output_ln(2);
}
if (isset($import)) {
    $sql = 'SELECT * FROM `TABLE 25` ';
    $eredm = $mysqliLink->query($sql);
    while ($eredm AND $row = $eredm->fetch_assoc()) {
        $club_id = 125; //getClubIdByClubName($row['egyesület']);
        /*
          $names = explode(' ', $row['Versenyző neve']);
          $veznev = $names[0];
          $kernev = $names[1];
          if (isset($names[2])) {
          $kernev .= ' ' . $names[2];
          }
         */
        $veznev = $row['Vezetéknév'];
        $kernev = $row['Keresztnév'];
        $birth_city = $row['Születési hely'];
        $mother = $row['Édesanyja neve'];
        $addr_city = $row['Város'];
        $addr_post = $row['Irányítószám'];
        $addr_street = $row['Lakcím'];
        $email = $row['E-mail'];
        $telefon = $row['Telefonszám'];
        $sex = ($row['Nem'] == 'nő') ? 'F' : 'M';
//        $born = DateTime::createFromFormat('Y.m.d', $row['Születési ideje'])->format('Y-m-d');
        $born = '2000-01-01';
        $base_data_id = createNewMember();
        $sql = 'UPDATE `' . MEMBER_TABLE . '` SET ';
        $sql .= ' `' . MEMBER_VEZNEV . '` ="' . $veznev . '" ,';
        $sql .= ' `' . MEMBER_KERNEV . '` ="' . $kernev . '" ,';
        $sql .= ' `' . MEMBER_BORN . '` ="' . $born . '" ,';
        $sql .= ' `' . MEMBER_BIRTH_CITY . '` ="' . $birth_city . '" ,';
        $sql .= ' `' . MEMBER_MOTHER . '` ="' . $mother . '" ,';

        $sql .= ' `' . MEMBER_ADDR_POST . '` ="' . $addr_post . '" ,';
        $sql .= ' `' . MEMBER_ADDR_CITY . '` ="' . $addr_city . '" ,';
        $sql .= ' `' . MEMBER_ADDR_STREET . '` ="' . $addr_street . '" ,';
        $sql .= ' `' . MEMBER_EMAIL . '` ="' . $email . '" ,';
        $sql .= ' `' . MEMBER_TEL . '` ="' . $telefon . '" ,';
        $sql .= ' `' . MEMBER_HOMEPAGE . '` ="" ,';
        $sql .= ' `' . MEMBER_NICKNAME . '` ="" ,';
        $sql .= ' `' . MEMBER_SEX . '` ="' . $sex . '" ';
        $sql .= ' WHERE `' . MEMBER_ID . '`="' . $base_data_id . '"';
        if ($club_id > 0) {
            setClubOfNewMember($base_data_id, $club_id);
        }
//        error_log($sql);
        $mysqliLink->query($sql);
    }
}

if (isset($new_year)) {  // Letrehozza az uj evben is a tavalyi dijtetel rekordokat korkorrekcioval
    $current_year = $this_year . '-01-01';
    $last_year = $this_year - 1;
    $last_year .= '-12-31';

    if ($new_year == 'Nyitás') {
        $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE  '
                . '("' . $last_year . '" BETWEEN `' . FM_FROM . '` AND `' . FM_TO . '` )'
                . ' AND`' . FM_PAY_ID . '`>0;';
        $last_res = $mysqliLink->query($sql);
        while ($row = $last_res->fetch_assoc()) {
            $fm_member_id = $row[FM_MEMEBER_ID];
            $fee_last = getFeeById($row[FM_FEE_ID]);
            $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
                    . '`' . FEE_TYPE . '`="' . $fee_last[FEE_TYPE] . '" AND '
                    . '`' . FEE_CLUB . '`="' . $fee_last[FEE_CLUB] . '" AND '
                    . '`' . FEE_NAME . '`="' . $fee_last[FEE_NAME] . '" AND '
                    . ' ("' . $current_year . '" BETWEEN `' . FEE_FROM . '` AND `' . FEE_TO . '` );';
            $this_res = $mysqliLink->query($sql);
            if ($this_res AND $this_fm = $this_res->fetch_assoc()) {
                $fee_current = getFee($fee_last[FEE_TYPE], $fee_last[FEE_NAME], $current_year);
                if (!empty($fee_current)) {
                    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE  '
                            . ' `' . FM_MEMEBER_ID . '`=' . $fm_member_id . ' '
                            . ' AND `' . FM_FEE_ID . '`=' . $fee_current[FEE_ID] . ';';
                    $is_res = $mysqliLink->query($sql);
                    if ($is_res AND $is_res->num_rows == 0) {
                        $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`,'
                                . ' `' . FM_FEE_ID . '` , `' . FM_FROM . '`, `' . FM_TO . '`)'
                                . ' VALUES ("' . $fm_member_id . '", "' . $fee_current[FEE_ID] . '",'
                                . ' "' . $fee_current[FEE_FROM] . '", "' . $fee_current[FEE_TO] . '");';

                        logger($member_id, $fm_member_id, LOGGER_FM_EDIT, 'Évnytás keretében '
                                . $fee_type[$fee_current[FEE_TYPE]][0] . '/ ' . get_fee_name($fee_current[FEE_TYPE], $fee_current[FEE_NAME]) .
                                ' díjtétel beállítva');
                        $mysqliLink->query($sql);
                    } else {
                        warning("Egy tagnak már volt idei díjtétele, nem jött létre új !");
                    }
                } else {
                    warning("Az ellenőrzés ellenére nincs meg egy idei díjtétel");
                }
            }
        }
    } elseif ($new_year == "Ellenőrzés") {
        $fees_created = true;
        foreach ($fee_type as $type => $type_name) {
            $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE `' . FEE_TYPE . '`=' . $type
                    . ' AND ("' . $last_year . '" BETWEEN `' . FEE_FROM . '` AND `' . FEE_TO . '` );';
            $res = $mysqliLink->query($sql);
            while ($row = $res->fetch_assoc()) {
                $club = $row[FEE_CLUB];
                $name = $row[FEE_NAME];
                $fee_id = $row[FEE_ID];
                $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE `' . FM_FEE_ID . '`=' . $fee_id
                        . ' AND `' . FM_PAY_ID . '`>0;';
                $fm_res = $mysqliLink->query($sql);
                if ($fm_res AND $fm_res->num_rows > 0) {
                    $output .= $fee_type[$type][0];
                    if ($type == FEE_TYPE_CLUB) {
                        $output .= '(' . getClubName($club) . ')';
                    }
                    $output .= ' / ' . get_fee_name($type, $name) . ': ' . $fm_res->num_rows . ' db&nbsp;&nbsp;&nbsp;&nbsp;';
                    $sql = 'SELECT * FROM `' . FEE_TABLE . '` WHERE '
                            . '`' . FEE_TYPE . '`="' . $type . '" AND '
                            . '`' . FEE_CLUB . '`="' . $club . '" AND '
                            . '`' . FEE_NAME . '`="' . $name . '" AND '
                            . ' ("' . $current_year . '" BETWEEN `' . FEE_FROM . '` AND `' . FEE_TO . '` );';
                    $this_res = $mysqliLink->query($sql);
                    if ($this_res AND $this_res->num_rows > 0) {
                        $output .= 'Idei évi díjtétel létrehozva.<br>';
                    } else {
                        $output .= '<font color="red">Nincs az idei díjtétel létrehozva!</font><br>';
                        $fees_created = false;
                    }
                }
            }
        }
    }
}
if (isset($test)) {
    emailExcelMember($fai_mem_id, getOptionValue(OPTIONS_NAME_ADMIN_EMAIL), 'Teszt email');
    $output .= 'Test email elküldve';
}
if (isset($script)) {
    /*
      $sql = 'SELECT * FROM `members` WHERE 1';
      $eredm = mysql_query($sql);
      while ($row = mysql_fetch_assoc($eredm)) {
      if ($row["FAI"] != "") {
      $sql = 'INSERT INTO `' . FM_TABLE . '` (`' . FM_MEMEBER_ID . '`,`' . FM_FEE_ID . '`, '
      . '`' . FM_FROM . '`, `' . FM_TO . '`, `' . FM_LICENCE . '` ) VALUES ('
      . ' "' . $row[MEMBER_ID] . '", "60", "2018-01-01", "2018-12-31", "' . $row["FAI"] . '")';          error_log($sql);
      mysql_query($sql);
      }
      }
     */
    $output .= 'A konverzió sikeres';
}
if (isset($licence)) {
    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE `' . FM_FEE_ID . '`=60 ';
    $res = $mysqliLink->query($sql);
    $num = 0;
    while ($row = $res->fetch_assoc()) {
        $mysqliLink->query('UPDATE `' . MEMBER_TABLE . '` SET `' . MEMBER_LICENCE . '`="' . $row[FM_LICENCE] . '"'
                . ' WHERE `' . MEMBER_ID . '`="' . $row[FM_MEMEBER_ID] . '"');
        $num++;
    }
    $output .= 'Az áthelyezés sikeres ' . $num . 'esetben';
}

if (isset($_FILES["database_upload"]) AND isset($load_database)) {
    if ($_FILES["database_upload"]["type"] == $mime) {
        $filename = $_FILES["database_upload"]["tmp_name"];
        error_log($filename);
    } else {
        warning("Hibás fájltípus!<br> Zippelt állományt várok !");
    }
}
$output .= '<div class="editor"><form method="post" enctype="multipart/form-data">';
$output .= '<label>Test:</label><input type="submit" value="FAI Email" name="test">'
        . '&nbsp;&nbsp;&nbsp;&nbsp;Id:<input type="number" name="fai_mem_id" value="' . $fai_mem_id . '"><br>';
$output .= '<label>Importálás:</label><input type="submit" value="Import" name="import"><br>';
$output .= '<label>Integritás:</label><input type="submit" value="Ellenőrzés" name="integrity"><br>';
$output .= '<label>Rendszer hiba küldés:</label><input type="submit" value="Rendszer hiba" name="system_error"><br>';
$output .= '<label>PHP Info:</label><input type="submit" value="PHP Info" name="phpinfo"><br>';
$output .= '<label>Konverzió:</label><input type="submit" value="Futtatás" name="script"><br>';
$output .= '<label>Napi rutin:</label><input type="submit" value="Futtatás" name="cron_daily"><br>';
$output .= '<label>Adattablak létrehozása:</label><input type="submit" value="Indít" name="create_tables"><br>';
$output .= '<label>FAI teszt:</label><input type="submit" value="Indít"  name="fai_test">';
$output .= '<br>';
$output .= '<label>Szavazások anonimizálása:</label><input type="submit" value="Indít" name="anonim"><br>';
$output .= '<label>Fájlelérés relatív:</label><input type="submit" value="Indít" name="relative"><br>';
$output .= '<label>Évnyitás:</label>';
if (isset($fees_created) AND $fees_created) {
    $output .= '<input type="submit" value="Nyitás" name="new_year">';
} else {
    $output .= '<input type="submit" value="Ellenőrzés" name="new_year">';
}
$output .= '<br></form>';
$output .= '</div>';

