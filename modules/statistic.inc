<?php

include_once CLASSES . 'PHPExcel.php';
include_once CLASSES . 'PHPExcel/Writer/Excel2007.php';
include_once CLASSES . 'ExcelHelper.php';

$downloads = [
    0 => ["Válassz", "Válaszd ki, miről akarsz CSV/XLS formátumú fájlt letölteni."],
    1 => ["Tagok összetétele", "Tagsági forma, aktuális létszám, ebből fizetve"],
    2 => ["Tagok kategóriánként", "Kategória, Szakág, " . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " azonosító, Név, Született, Egyesület, Tagdíj, Rendezve, FAI befizetés, Email, Telefon, Egyesületi szerep, lejárata, Jogosítások, lejárata"],
    3 => ["Egyesületek vezetői", "Egyesület neve, Létszám, Vezetők"],
    4 => ["Egyesületek küldöttei", "Egyesület neve, Küldöttek száma, Maximális küldött, Küldöttek neve, Vezetők elérhetősége"],
    5 => ["Taglista", "Tag neve, Email, Születetett, Város, Egyesület"],
    6 => ["SiR versenyzők", "Tag neve, Egyesület, Születési idő, Születési hely, Nem, Anyja neve, Település, Irsz., Lakcím, Amatőr"],
    7 => ["Offline egyesületek", "Még egy tagnak sincs jelszava, tehát nem használják a tagnyilvántartót"],
//    8 =>  ["Összes FAI tag", "Minden olyan tag, akinek van FAI ID-je"],
//    9 =>  ["Fizetett FAI tag", "Minden olyan tag, akinek van FAI ID-je és befizette a díjat"],
    10 => ["Tavalyi tagok", "Egyesületenkénti kimutatás tagokról (xlsx)"],
];

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
            $filename = "member_category_" . date("Y-m-d_H-i-s") . ".xlsx";
            header('Content-type: application/vnd.ms-excel');
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            header('Cache-Control: max-age=0');
            $ex = new ExcelHelper("Tagok kategóriánként", $filename);
            $ex->insertRow(array("Kategória", "Szakág", getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " azonosító",
                "Név", "Született", "Egyesület", "Tagdíj", "Rendezve",
                "FAI befizetés", "Email", "Telefon",
                "Egyesületi szerep", "lejárata",
                $fee_type[FEE_TYPE_LICENCES][0], "lejárata"));
            $cat_res = $mysqliLink->query('SELECT * FROM `' . CATEGORIES_TABLE . '` ORDER BY `' . CATEGORIES_SHORT . '`');
            while ($cat_res AND $cat_row = $cat_res->fetch_assoc()) {
                $mem_res = $mysqliLink->query('SELECT * FROM `' . CM_TABLE . '` WHERE `' . CM_CATEGORY . '`=' . $cat_row[CATEGORIES_ID]);
                while ($mem_res AND $mem_row = $mem_res->fetch_assoc()) {
                    $mem_id = $mem_row[CM_MEMBER_ID];
                    $mem = retriveMember($mem_id);
                    $fm = getFeeOfMember($mem_id, $today, FEE_TYPE_MEMBERSHIP);
                    if (empty($fm)) {
                        $sum = '---';
                        $paid = '---';
                    } else {
                        $sum = getFeeById($fm[FM_FEE_ID])[FEE_SUM] . ' Ft';
                        $paid = $fm[FM_PAY_ID] == 0 ? "Rendezetlen" : "Fizetve";
                    }
                    $roles_of_mem = getRolesOfMember($mem_id);
                    $role_name = $role_date = '';
                    if (!empty($roles_of_mem)) {
                        foreach ($roles_of_mem as $role) {
                            $role_name .= $access[$role[ROLE_PERMISSION]] . ', ';
                            $role_date .= $role[ROLE_TO] . ', ';
                        }
                    } else {
                        $role_name = $role_date = '---';
                    }
                    $lic_name = $lic_date = '';
                    $licences_of_mem = getLicencesOfMember($mem_id);
                    if (!empty($licences_of_mem)) {
                        foreach ($licences_of_mem as $lic) {
                            $lic_name .= $licences[$lic[LM_TYPE_ID]] . ', ';
                            $lic_date .= $lic[LM_TO] . ', ';
                        }
                    } else {
                        $lic_name = $lic_date = '---';
                    }
                    $ex->insertRow(array($cat_row[CATEGORIES_SHORT], $branches[$cat_row[CATEGORIES_BRANCH]],
                        $mem_row[CM_MEMBER_ID], getMemberName($mem_row[CM_MEMBER_ID]),
                        $mem[MEMBER_BORN], getMemberClubNames($mem_row[CM_MEMBER_ID])
                        , $sum, $paid,
                        getFaiPaymentDate($mem_row[CM_MEMBER_ID]), $mem[MEMBER_EMAIL], $mem[MEMBER_TEL],
                        $role_name, $role_date,
                        $lic_name, $lic_date));
                }
            }

            $ex->close($filename);
            logger($member_id, -1, LOGGER_STATISTIC, "Tagok kategoriánként");
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
        case 8:
            $filename = TMP_DIR . "all_fai_members" . date("Y-m-d_H-i-s") . ".xls";
            header('Content-Type:   application/vnd.ms-excel; charset=utf-8');
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            /*
              $objPHPExcel = new PHPExcel();
              $objPHPExcel->getProperties()->setCreator(getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' Tagnyilvántartó');


              $objPHPExcel->setActiveSheetIndex(0);
              $objPHPExcel->getActiveSheet()->setTitle($mem[MEMBER_KERNEV] . ' ' . $mem[MEMBER_VEZNEV] . ' ' . $this_year);


              $objPHPExcel->getActiveSheet()->setCellValue('A1', 'FAI ID');
              $objPHPExcel->getActiveSheet()->setCellValue('A2', $mem[MEMBER_LICENCE]);

              $objPHPExcel->getActiveSheet()->setCellValue('B1', 'Licence Number');
              $objPHPExcel->getActiveSheet()->setCellValue('B2', 'HUN-' . $mem_id);

              $objPHPExcel->getActiveSheet()->setCellValue('C1', 'Validity From');
              $objPHPExcel->getActiveSheet()->setCellValue('C2', date('d.m.Y'));

              $objPHPExcel->getActiveSheet()->setCellValue('D1', 'Validity To');
              $objPHPExcel->getActiveSheet()->setCellValue('D2', '31.12.' . date('Y', time()));

              $objPHPExcel->getActiveSheet()->setCellValue('E1', 'Air Sport Discipline');
              $objPHPExcel->getActiveSheet()->setCellValue('E2', 'Aeromodelling and Spacemodelling');

              $objPHPExcel->getActiveSheet()->setCellValue('F1', 'First Name');
              $objPHPExcel->getActiveSheet()->setCellValue('F2', $mem[MEMBER_KERNEV]);

              $objPHPExcel->getActiveSheet()->setCellValue('G1', 'Last Name');
              $objPHPExcel->getActiveSheet()->setCellValue('G2', $mem[MEMBER_VEZNEV]);

              $objPHPExcel->getActiveSheet()->setCellValue('H1', 'Gender');
              $objPHPExcel->getActiveSheet()->setCellValue('H2', $mem[MEMBER_SEX]);

              $objPHPExcel->getActiveSheet()->setCellValue('I1', 'Date of Birth');
              $objPHPExcel->getActiveSheet()->setCellValue('I2', date('d.m.Y', strtotime($mem[MEMBER_BORN])));

              $objPHPExcel->getActiveSheet()->setCellValue('J1', 'Nationality');
              $objPHPExcel->getActiveSheet()->setCellValue('J2', 'HUN');

              $objPHPExcel->getActiveSheet()->setCellValue('K1', 'Country of Residence');
              $objPHPExcel->getActiveSheet()->setCellValue('K2', 'HUN');

              $objPHPExcel->getActiveSheet()->setCellValue('L1', 'Address1');
              $objPHPExcel->getActiveSheet()->setCellValue('L2', $mem[MEMBER_ADDR_STREET]);

              $objPHPExcel->getActiveSheet()->setCellValue('M1', 'Address2');
              $objPHPExcel->getActiveSheet()->setCellValue('M2', $mem[MEMBER_ADDR_POST]);

              $objPHPExcel->getActiveSheet()->setCellValue('N1', 'Address3');
              $objPHPExcel->getActiveSheet()->setCellValue('N2', $mem[MEMBER_ADDR_CITY]);

              $objPHPExcel->getActiveSheet()->setCellValue('O1', 'Country of addresse');
              $objPHPExcel->getActiveSheet()->setCellValue('O2', 'Hungary');

              $objPHPExcel->getActiveSheet()->setCellValue('P1', 'E-mail address');
              $objPHPExcel->getActiveSheet()->setCellValue('P2', $mem[MEMBER_EMAIL]);

              $objPHPExcel->getActiveSheet()->setCellValue('Q1', 'Phone (home)');
              $objPHPExcel->getActiveSheet()->setCellValue('Q2', '');

              $objPHPExcel->getActiveSheet()->setCellValue('R1', 'Phone (office)');
              $objPHPExcel->getActiveSheet()->setCellValue('R2', '');

              $objPHPExcel->getActiveSheet()->setCellValue('S1', 'Phone (mobile)');
              $objPHPExcel->getActiveSheet()->setCellValue('S2', '');

              $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);

              $objWriter->save($filename);
             */
            logger($member_id, -1, LOGGER_STATISTIC, "Összes FAI tag");

            exit(0);
        case 9:
            $filename = "paid_fai_members" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "Fizetett FAI tagok");
            exit(0);
        case 10:
            $filename = 'TavalyiTagok.xlsx';
            header("Content-type: application/vnd.ms-excel");
            header("Content-Disposition: attachment; filename=$filename");
            $last_year = intval($this_year) - 1;
            $last_year .= '-12-31';
            $objPHPExcel = new PHPExcel();
            $objPHPExcel->getProperties()->setCreator(getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' Tagnyilvántartó');
            $objPHPExcel->setActiveSheetIndex(0);
            $objPHPExcel->getActiveSheet()->setTitle('Tagadatok ' . $last_year . ' végén.');
            $row = 1; // 1-based index
            $total_mem = 0;
            $total_paid = 0;
            $total_fai = 0;
            $clubs = getClubNameList();
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(0, $row, 'Egyesület neve');
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(1, $row, 'Aktív tagok száma');
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(2, $row, 'Rendezett tagdíjak száma');
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(3, $row, 'FAI tagdíjat kért');
            $row++;
            foreach ($clubs as $club_id => $club_name) {
                $total_mem += $club_members = count(getClubMembers($club_id, $last_year));
                $total_paid += $paid_members = countPaidMembersOfClub($club_id, $last_year);
                $total_fai += $fai_members = countFaiMembersOfClub($club_id, $last_year);
                $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(0, $row, $club_name); // Egyesület neve
                $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(1, $row, $club_members); // Aktív tagok száma
                $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(2, $row, $paid_members); // Tagdíj rendezett tagok száma
                $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(3, $row, $fai_members); // FAI tagdíjat kért tagok
                $row++;
            }
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(0, $row, 'Összesen'); // Egyesület neve
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(1, $row, $total_mem); // Aktív tagok száma
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(2, $row, $total_paid); // Tagdíj rendezett tagok száma
            $objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow(3, $row, $total_fai); // FAI tagdíjat kért tagok
            $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
            $objWriter->save('php://output');
            logger($member_id, -1, LOGGER_STATISTIC, "Tavalyi egyesületi tagadatok");
            exit(0);
    }
}
$output .= '<div class="keret">';
$output .= '<h2>Letöltések</h2>';
$output .= '<form method="post">';
$output .= '<select name="selected_stat" onChange="submit();">';
foreach ($downloads as $download_id => $download_name) {
    $output .= '<option value="' . $download_id . '"'
            . (($selected_stat == $download_id) ? ' selected ' : '')
            . '>' . $download_name[0] . '</option>';
}

$output .= '</select>&nbsp;&nbsp;&nbsp;&nbsp;' . $downloads[$selected_stat][1];

$output .= '<br><br><input type="submit" name="download" value="Letöltés">';
$output .= '</form>';
$output .= '</div>';

