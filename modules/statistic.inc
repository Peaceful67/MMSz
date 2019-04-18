<?php

$downloads = array(
    array(0, "Válassz", "Válaszd ki, miről akarsz CSV formátumú fájlt letölteni."),
    array(1, "Tagok összetétele", "Tagsági forma, aktuális létszám, ebből fizetve"),
    array(2, "Tagok kategóránként", "Kategória, Szakág, " . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " azonosító, Név, Született, Egyesület, Tagdíj, Rendezve"),
    array(3, "Egyesületek vezetői", "Egyesület neve, Létszám, Vezetők"),
    array(4, "Egyesületek küldöttei", "Egyesület neve, Küldöttek száma, Maximális küldött, Küldöttek neve, Vezetők elérhetősége"),
    array(5, "Taglista", "Tag neve, Email, Születetett, Város, Egyesület"),
    array(6, "SiR versenyzők", "Tag neve, Egyesület, Születési idő, Születési hely, Nem, Anyja neve, Település, Irsz., Lakcím, Amatőr"),
    array(7, "Offline egyesületek", "Még egy tagnak sincs jelszava, tehát nem használják a tagnyilvántartót"),    
);

if (!isset($selected_stat)) {
    $selected_stat = 0;
}
if (isset($download)) {
    switch ($selected_stat) {
        case 1:
            $filename = "membership_" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fees = getFeeIdsToday(FEE_TYPE_MEMBERSHIP);
            $data = conv_array(array("Tagsági forma", "Létszám", "Fizetett"));
            $fh = fopen('php://output', 'w');
            fputcsv($fh, $data, CSV_DELIMITER);
            foreach ($member_types as $mt_key => $mt_name) {
                if (isset($fees[$mt_key])) {
                    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
                            . ' `' . FM_FEE_ID . '`=' . $fees[$mt_key]
                            . ' AND (CURDATE() BETWEEN `' . FM_FROM . '` AND `' . FM_TO . '`) ';
                    $res = $mysqliLink->query($sql);
                    $num = $res ? $res->num_rows : 0;
                    $sql = 'SELECT * FROM `' . FM_TABLE . '` WHERE '
                            . ' `' . FM_FEE_ID . '`=' . $fees[$mt_key]
                            . ' AND `' . FM_BILL_ID . '`!=0'
                            . ' AND (CURDATE() BETWEEN `' . FM_FROM . '` AND `' . FM_TO . '`) ';
                    $res = $mysqliLink->query($sql);
                    $num_paied = $res ? $res->num_rows : 0;
                    fputcsv($fh, conv_array(array($mt_name, $num, $num_paied)), CSV_DELIMITER);
                }
            }
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "Tagok összetétele");
            exit(0);

        case 2:
            $filename = "member_category_" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            fputcsv($fh, conv_array(array("Kategória", "Szakág", getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " azonosító", "Név", "Született", "Egyesület", "Tagdíj", "Rendezve")), CSV_DELIMITER);
            $cat_res = $mysqliLink->query('SELECT * FROM `' . CATEGORIES_TABLE . '` ORDER BY `' . CATEGORIES_SHORT . '`');
            while ($cat_res AND $cat_row = $cat_res->fetch_assoc()) {
                $mem_res = $mysqliLink->query('SELECT * FROM `' . CM_TABLE . '` WHERE `' . CM_CATEGORY . '`=' . $cat_row[CATEGORIES_ID]);
                while ($mem_res AND $mem_row = $mem_res->fetch_assoc()) {
                    $mem = retriveMember($mem_row[CM_MEMBER_ID]);
                    $fm = getFeeOfMember($mem_row[CM_MEMBER_ID], $today, FEE_TYPE_MEMBERSHIP);
                    if (empty($fm)) {
                        $sum = '---';
                        $paid = '---';
                    } else {
                        $sum = getFeeById($fm[FM_FEE_ID])[FEE_SUM] . ' Ft';
                        $paid = $fm[FM_PAY_ID] == 0 ? "Rendezetlen" : "Fizetve";
                    }
                    fputcsv($fh, conv_array(array($cat_row[CATEGORIES_SHORT], $branches[$cat_row[CATEGORIES_BRANCH]],
                        $mem_row[CM_MEMBER_ID], getMemberName($mem_row[CM_MEMBER_ID]),
                        $mem[MEMBER_BORN], getMemberClubNames($mem_row[CM_MEMBER_ID])
                        , $sum, $paid)), CSV_DELIMITER);
                }
            }
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "Tagok kategóriánként");
            exit(0);

        case 3:
            $filename = "clubleaders_" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            $data = array(" Egyesület", "Létszám", "Vezetők");
            fputcsv($fh, conv_array($data), CSV_DELIMITER);
            $res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE  1  ORDER BY `' . CLUB_NAME . '` ');
            while ($res AND $row = $res->fetch_assoc()) {

                $name = " " . $row[CLUB_NAME];
                $num = " " . getNumMemberOfClub($row[CLUB_ID]);

                $sql = 'SELECT `' . ROLE_MEMBER . '` FROM `' . ROLE_TABLE . '` WHERE `'
                        . ROLE_PERMISSION . '`="' . ACCESS_CLUBLEADER . '" '
                        . 'AND `' . ROLE_CLUB . '`="' . $row[CLUB_ID] . '" '
                        . ' AND (CURDATE() BETWEEN `' . ROLE_FROM . '` AND `' . ROLE_TO . '`)';
                $er_res = $mysqliLink->query($sql);
                $leader = " ";
                while ($er_res AND $r = $er_res->fetch_assoc()) {
                    $mem = retriveMember($r[ROLE_MEMBER]);
                    $leader .= getMemberName($r[ROLE_MEMBER]) . '/';
                    $leader .= $mem[MEMBER_BORN] . '/';
                    $leader .= $mem[MEMBER_EMAIL] . ', ';
                }
                $data = array($name, $num, $leader);
                fputcsv($fh, conv_array($data), CSV_DELIMITER);
            }
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "Egyesületi vezetők");
            exit(0);

        case 4:
            $filename = "clubdelegates_" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            $data = array(" Egyesület", "Kijelölt", "Maximum", "Küldöttek", "Vezetők");
            fputcsv($fh, conv_array($data), CSV_DELIMITER);
            $res = $mysqliLink->query('SELECT * FROM `' . CLUB_TABLE . '` WHERE  1  ORDER BY `' . CLUB_NAME . '` ');
            while ($res AND $row = $res->fetch_assoc()) {

                $club = $row[CLUB_NAME];
                $leaders = '';
                foreach (getClubLeaderIds($row[CLUB_ID]) as $leader_id) {
                    $leaders .= getMemberName($leader_id) . ': ' . getMemberEmail($leader_id) . '/' . retriveMember($leader_id)[MEMBER_TEL] . '; ';
                }
                $act = " " . getNumberOfDelegateClub($row[CLUB_ID]);
                $max = " " . getMaxNumberOfDelegateClub($row[CLUB_ID]);
                $names = getClubDelegates($row[CLUB_ID]);
                $data = array($club, $act, $max, $names, $leaders);
                fputcsv($fh, conv_array($data), CSV_DELIMITER);
            }
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "Egyesületek küldöttei");
            exit(0);
        case 5:
            $filename = "members_" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            $data = array("Tag neve", "Email", "Született", "Város", "Egyesület");
            fputcsv($fh, conv_array($data), CSV_DELIMITER);
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '` ';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`!="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                $name = getMemberName($row[MEMBER_ID]);
                $born = $row[MEMBER_BORN];
                $email = $row[MEMBER_EMAIL];
                $city = $row[MEMBER_ADDR_CITY];
                $club = getMemberClubNames($row[MEMBER_ID]);
                $data = array($name, $email, $born, $city, $club);
                fputcsv($fh, conv_array($data), CSV_DELIMITER);
            }
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "Aktív tagok");
            exit(0);
        case 6:
            $filename = "Sir_Competitors" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            $data = array("Tag neve", "Egyesület", "Születési idő", "Születési hely", "Nem", "Anyja neve", "Település", "Irsz.", "Lakcím", "Amatőr");
            fputcsv($fh, conv_array($data), CSV_DELIMITER);
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '` ';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`!="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) '
                    . ' GROUP BY `' . MEMBER_ID . '` '
                    . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                $name = getMemberName($row[MEMBER_ID]);
                $born = $row[MEMBER_BORN];
                $city = $row[MEMBER_ADDR_CITY];
                $post = $row[MEMBER_ADDR_POST];
                $addr = $row[MEMBER_ADDR_STREET];
                $birth_city = $row[MEMBER_BIRTH_CITY];
                $mother = $row[MEMBER_MOTHER];
                $sex = ($row[MEMBER_SEX] != "F") ? "Férfi" : "Nő";
                $club = getMemberClubNames($row[MEMBER_ID]);
                $data = array($name, $club, $born, $birth_city, $sex, $mother, $city, $post, $addr, "Amatőr");
                fputcsv($fh, conv_array($data), CSV_DELIMITER);
            }
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "SiR Versenyzők");
            exit(0);
        case 7:
            $filename = "offline_clubs" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            $data = array("Egyesület neve");
            fputcsv($fh, conv_array($data), CSV_DELIMITER);
            $clubs = getClubNameList();
            $sql = 'SELECT * FROM `' . MEMBER_TABLE . '` WHERE '
                    . ' `' . MEMBER_PASSWORD . '`!="" ';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                unset($clubs[getMemberClubId($row[MEMBER_ID])]);
            }
            foreach ($clubs as $club) {
                fputcsv($fh, conv_array(array($club)), CSV_DELIMITER);
            }
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "SiR Versenyzők");
            exit(0);
    }
}
$output .= '<div class="keret">';
$output .= '<h2>Letöltések</h2>';
$output .= '<form method="post">';
$output .= '<select name="selected_stat" onChange="submit();">';
foreach ($downloads as $item) {
    $output .= '<option value="' . $item[0] . '"'
            . (($selected_stat == $item[0]) ? ' selected ' : '')
            . '>' . $item[1] . '</option>';
}

$output .= '</select>&nbsp;&nbsp;&nbsp;&nbsp;' . $downloads[$selected_stat][2];

$output .= '<br><br><input type="submit" name="download" value="Letöltés">';
$output .= '</form>';
$output .= '</div>';

