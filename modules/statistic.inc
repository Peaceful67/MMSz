<?php

include_once CLASSES . 'ExcelHelper.php';

$downloads = [
    0 => ["Válassz", "Válaszd ki, miről akarsz CSV/XLS formátumú fájlt letölteni."],
    1 => ["Tagok összetétele", "Tagsági forma, aktuális létszám, ebből fizetve"],
    2 => ["Tagok kategóriánként", "Kategória, Szakág, " . getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " azonosító, Név, Született, Egyesület, Tagdíj, Rendezve, FAI befizetés, Email, Telefon, Egyesületi szerep, lejárata, Jogosítások, lejárata"],
    3 => ["Egyesületek vezetői", "Egyesület neve, Létszám, Vezetők"],
    4 => ["Egyesületek küldöttei", "Egyesület neve, Küldöttek száma, Maximális küldött, Küldöttek neve, Vezetők elérhetősége"],
    5 => ["Taglista egyesületenként", "Tag neve, Email, Születetett, Város, Egyesület"],
    51 => ["Taglista szerepkörönként", "Tag neve, Email, Születetett, Város, Egyesület, Szerepkör, -tól, -ig"],
    6 => ["SiR versenyzők", "Tag neve, Egyesület, Születési idő, Születési hely, Nem, Anyja neve, Település, Irsz., Lakcím, Amatőr"],
    7 => ["Offline egyesületek", "Még egy tagnak sincs jelszava, tehát nem használják a tagnyilvántartót"],
    8 => ["Tavalyi taglista", "Tavaly év végi aktív tagok listája"],
//    8 =>  ["Összes FAI tag", "Minden olyan tag, akinek van FAI ID-je"],
//    9 =>  ["Fizetett FAI tag", "Minden olyan tag, akinek van FAI ID-je és befizette a díjat"],
    10 => ["Tavalyi tagok", "Egyesületenkénti kimutatás tagokról (xlsx)"],
    11 => ['Nov.15 aktív tagok száma', 'Tavaly nov. 15-én aktív tagok egyesületenkénti száma'],
    12 => ['Nov.15 aktív tagok részletesen', 'Tavaly nov. 15-én aktív tagok egyesületenként'],
];

if (!isset($selected_stat)) {
    $selected_stat = 0;
}
if (isset($download)) {
    switch ($selected_stat) {
        case 1:
            $filename = "membership_" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("Tagok összetétele", $filename);
            $ex->insertHeaderRow(["Tagsági forma", "Létszám", "Fizetett"]);
            $fees = getFeeIdsToday(FEE_TYPE_MEMBERSHIP);
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
                    $ex->insertRow([$mt_name, $num, $num_paied]);
                }
            }
            $ex->save();
            logger($member_id, -1, LOGGER_STATISTIC, "Tagok összetétele");
            break;

        case 2:
            $filename = "member_category_" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("Tagok kategóriánként", $filename);
            $ex->insertHeaderRow(["Kategória", "Szakág", getOptionValue(OPTIONS_NAME_COMPANY_NAME) . " azonosító",
                "Név", "Született", "Egyesület", "Tagdíj", "Rendezve",
                "FAI befizetés", "Email", "Telefon",
                "Egyesületi szerep", "lejárata",
                $fee_type[FEE_TYPE_LICENCES][0], "lejárata"]);
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
                    $ex->insertRow([$cat_row[CATEGORIES_SHORT], $branches[$cat_row[CATEGORIES_BRANCH]],
                        $mem_row[CM_MEMBER_ID], getMemberName($mem_row[CM_MEMBER_ID]),
                        $mem[MEMBER_BORN], getMemberClubNames($mem_row[CM_MEMBER_ID])
                        , $sum, $paid,
                        getFaiPaymentDate($mem_row[CM_MEMBER_ID]), $mem[MEMBER_EMAIL], $mem[MEMBER_TEL],
                        $role_name, $role_date,
                        $lic_name, $lic_date]);
                }
            }

            $ex->save();
            logger($member_id, -1, LOGGER_STATISTIC, "Tagok kategoriánként");
            breał;

        case 3:
            $filename = "clubleaders_" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("Egyesületek vezetői", $filename);
            $ex->insertHeaderRow([" Egyesület", "Létszám", "Vezetők"]);
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
                $ex->insertRow([$name, $num, $leader]);
            }
            $ex->save();
            logger($member_id, -1, LOGGER_STATISTIC, "Egyesületi vezetők");
            break;

        case 4:
            $filename = "clubdelegates_" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("Egyesületek küldöttei", $filename);
            $ex->insertHeaderRow([" Egyesület", "Kijelölt", "Maximum", "Küldöttek", "Vezetők"]);
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
                $ex->insertRow([$club, $act, $max, $names, $leaders]);
            }
            $ex->save();
            logger($member_id, -1, LOGGER_STATISTIC, "Egyesületek küldöttei");
            break;
        case 5:
            logger($member_id, -1, LOGGER_STATISTIC, "Aktív tagok");
            $filename = "members_" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("Tagok kategóriánkent", $filename);
            $ex->insertHeaderRow(["Tag neve", "Email", "Született", "Város", "Egyesület", 'club_cnt']);
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* FROM (`' . MEMBER_TABLE . '` ';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' `active`.`' . ROLE_PERMISSION . '`!="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) ';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                $club_cnt = 0;
                $name = getMemberName($row[MEMBER_ID]);
                $born = $row[MEMBER_BORN];
                $email = $row[MEMBER_EMAIL];
                $city = $row[MEMBER_ADDR_CITY];
                $clubs = getClubsMember($row[MEMBER_ID]);
                foreach ($clubs as $club) {
                    $ex->insertRow([$name, $email, $born, $city, $club, $club_cnt]);
                    $club_cnt--;
                }
//                $club = getMemberClubNames($row[MEMBER_ID]);
            }
            $ex->save();
            break;
        case 51:
            logger($member_id, -1, LOGGER_STATISTIC, "Aktív tagok szerkörönként");
            $filename = "members_by_roles_" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("Tagok szerepenként", $filename);
            $ex->insertHeaderRow(["Tag neve", "Email", "Született", "Város", "Egyesület", 'Szerepkör', 'Ettől', 'Eddig']);
            $searched_role = ACCESS_ADMIN | ACCESS_MMSZ | ACCESS_MMSZ_PRESIDENT | ACCESS_CLUBLEADER | ACCESS_CLUB_DELEGATE | ACCESS_CLUB_MEMBER | ACCESS_BRANCH_LEADER | ACCESS_USER | ACCESS_SUBCLUB_LEADER;
            $sql = 'SELECT `' . MEMBER_TABLE . '`.*, `active`.*  FROM (`' . MEMBER_TABLE . '` ';
            $sql .= ' INNER JOIN `' . ROLE_TABLE . '` AS `active` ON ('
                    . ' (`active`.`' . ROLE_PERMISSION . '`&' . $searched_role . ')!="0" '
                    . ' AND `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '`=`active`.`' . ROLE_MEMBER . '` '
                    . ' AND (CURDATE() BETWEEN `active`.`' . ROLE_FROM . '` AND `active`.`' . ROLE_TO . '`))) '
                    . ' ORDER BY `' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '`, `' . MEMBER_TABLE . '`.`' . MEMBER_VEZNEV . '`;';
            $res = $mysqliLink->query($sql);
//            error_log($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                $club_cnt = 0;
                $name = getMemberName($row[MEMBER_ID]);
                $born = $row[MEMBER_BORN];
                $email = $row[MEMBER_EMAIL];
                $city = $row[MEMBER_ADDR_CITY];
                $club = $row[ROLE_CLUB];
                $club_name = $club > 0 ? getClubName($club) : '---';
                $from = $row[ROLE_FROM];
                $to = $row[ROLE_TO];
                switch ($row[ROLE_PERMISSION]) {
                    case ACCESS_ADMIN :
                    case ACCESS_USER:
                    case ACCESS_CLUBLEADER:
                    case ACCESS_MMSZ:
                    case ACCESS_CLUB_DELEGATE:
                    case ACCESS_MMSZ_PRESIDENT:
                    case ACCESS_CLUB_PRESIDENT:
                        $role_name = $access[$row[ROLE_PERMISSION]];
                        $ex->insertRow([$name, $email, $born, $city, $club_name, $role_name, $from, $to]);
                        break;
                    default:
                        break;
                }
            } $ex->save();
            break;
        case 6:
            $filename = "Sir_Competitors" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("SiR versenyzők", $filename);
            $ex->insertHeaderRow(["Tag neve", "Egyesület", "Születési idő", "Születési hely", "Nem", "Anyja neve", "Település", "Irsz.", "Lakcím", "Amatőr"]);
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
                $ex->insertRow([$name, $club, $born, $birth_city, $sex, $mother, $city, $post, $addr, "Amatőr"]);
            }
            $ex->save();
            logger($member_id, -1, LOGGER_STATISTIC, "SiR Versenyzők");
            break;
        case 7:
            $filename = "offline_clubs" . date("Y-m-d_H-i-s") . ".xlsx";
            $ex = new ExcelHelper("Offline egyesületek", $filename);
            $ex->insertHeaderRow(["Egyesület neve"]);
            $clubs = getClubNameList();
            $sql = 'SELECT * FROM `' . MEMBER_TABLE . '` WHERE '
                    . ' `' . MEMBER_PASSWORD . '`!="" ';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                unset($clubs[getMemberClubId($row[MEMBER_ID])]);
            }
            foreach ($clubs as $club) {
                $ex->insertRow([$club]);
            }
            $ex->save();
            logger($member_id, -1, LOGGER_STATISTIC, "Offline egyesületek");
            break;
        case 8:
            logger($member_id, -1, LOGGER_STATISTIC, "Tavalyi taglista");
            $filename = 'TavalyiTaglista_' . date("Y-m-d_H-i-s") . '.xlsx';
            $helper = new ExcelHelper('Tavalyi taglista', $filename);
            $last_year = intval($this_year) - 1;
            $last_year .= '-12-31';
            $helper->insertHeaderRow([
                'Tag neve',
                'Egyesület',
            ]);
            $sql = 'SELECT `' . MEMBER_TABLE . '`.* '
                    . ' FROM `' . MEMBER_TABLE . '`,`' . ROLE_TABLE . '` WHERE '
                    . '`' . ROLE_TABLE . '`.`' . ROLE_MEMBER . '`=`' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` AND '
                    . '`' . ROLE_TABLE . '`.`' . ROLE_PERMISSION . '`!=0 AND '
                    . '("' . $last_year . '" BETWEEN `' . ROLE_TABLE . '`.`' . ROLE_FROM . '` AND `' . ROLE_TABLE . '`.`' . ROLE_TO . '`) '
                    . ' GROUP BY `' . MEMBER_TABLE . '`.`' . MEMBER_ID . '` '
                    . ' ORDER BY `' . MEMBER_VEZNEV . '`, `' . MEMBER_KERNEV . '`;';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                $helper->insertRow([
                    $row[MEMBER_VEZNEV] . ' ' . $row[MEMBER_KERNEV],
                    getMemberClubNamesByDate($row[MEMBER_ID], $last_year),
                ]);
            }
            $helper->save();
            exit(0);
        case 9:
            $filename = "paid_fai_members" . date("Y-m-d_H-i-s") . ".csv";
            header('Content-Type: ' . MIME_CSV);
            header('Content-Disposition: attachment; filename="' . $filename . '";');
            $fh = fopen('php://output', 'w');
            fclose($fh);
            logger($member_id, -1, LOGGER_STATISTIC, "Fizetett FAI tagok");
            break;
        case 10:
            logger($member_id, -1, LOGGER_STATISTIC, "Tavalyi egyesületi tagadatok");
            $filename = 'TavalyiTagok_' . date("Y-m-d_H-i-s") . '.xlsx';
            $helper = new ExcelHelper('Tagadatok', $filename);
            $last_year = intval($this_year) - 1;
            $last_year .= '-12-31';
            $total_mem = 0;
            $total_paid = 0;
            $total_fai = 0;
            $clubs = getClubNameList();
            $helper->insertHeaderRow([
                'Egyesület neve',
                'Aktív tagok száma',
                'Rendezett tagdíjak száma',
                'Rendezett tagdíjak száma'
            ]);
            foreach ($clubs as $club_id => $club_name) {
                $total_mem += $club_members = count(getClubMembers($club_id, $last_year));
                $total_paid += $paid_members = countPaidMembersOfClub($club_id, $last_year);
                $total_fai += $fai_members = countFaiMembersOfClub($club_id, $last_year);
                $helper->insertRow([
                    $club_name,
                    $club_members,
                    $paid_members,
                    $fai_members
                ]);
            }
            $helper->insertRow([
                'Összesen',
                $total_mem,
                $total_paid,
                $total_fai
            ]);
            $helper->save();
            break;
        case 11:
            logger($member_id, -1, LOGGER_STATISTIC, "Nov.15 tagok egyesületenkénti száma");
            $filename = 'Nov15_tagok_' . date("Y-m-d_H-i-s") . '.xlsx';
            $helper = new ExcelHelper('Nov15 aktivtagok', $filename);
            $last_year = intval($this_year) - 1;
            $last_year .= '-11-15';
            $helper->insertHeaderRow([
                'Egyesület', 'Létszám'
            ]);
            $sql = 'SELECT `clubs`.`KlubNev`, COUNT(*) AS number FROM `role_member` '
                    . ' INNER JOIN `clubs` ON `clubs`.`IDK` = `role_member`.`club_id` AND `role_member`.`club_id` > 0 '
                    . ' WHERE `date_from` < "' . $last_year . '" AND `permission` = 1 GROUP BY `club_id` ORDER BY `clubs`.`KlubNev`';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                $helper->insertRow([
                    $row[CLUB_NAME],
                    $row['number'],
                ]);
            }
            $helper->save();
            break;
        case 12:
            logger($member_id, -1, LOGGER_STATISTIC, "Nov.15 tagok egyesületenként részletesen");
            $filename = 'Nov15_tagok_reszletes_' . date("Y-m-d_H-i-s") . '.xlsx';
            $helper = new ExcelHelper('Nov15 aktivtagok részletesen', $filename);
            $last_year = intval($this_year) - 1;
            $last_year .= '-11-15';
            $helper->insertHeaderRow([
                'Egyesület', 'Azonosító', 'Név', 'Email', 'Született', 'GDPR',
            ]);
            $sql = 'SELECT `clubs`.`KlubNev`, `members`.* FROM `role_member` '
                    . ' INNER JOIN `clubs` ON (`clubs`.`IDK`=`role_member`.`club_id` AND `role_member`.`club_id`>0 )'
                    . ' INNER JOIN `members` ON (`role_member`.`member_id`=`members`.`ID`) '
                    . ' WHERE `role_member`.`date_from` < "2020-11-15" AND `role_member`.`permission` = 1'
                    . ' ORDER by `clubs`.`KlubNev`, `members`.`VezNev`, `members`.`KerNev`';
            $res = $mysqliLink->query($sql);
            while ($res AND $row = $res->fetch_assoc()) {
                $helper->insertRow([
                    $row[CLUB_NAME],
                    $row[MEMBER_ID],
                    $row[MEMBER_VEZNEV] . ' ' . $row[MEMBER_KERNEV],
                    $row[MEMBER_EMAIL],
                    $row[MEMBER_BORN],
                    $row[MEMBER_GDPR],
                ]);
            }
            $helper->save();
            break;
    }
}
$view->newKeret() ;
$view->submitDropDownChooser('Letöltések', $downloads, 'selected_stat');
$view->info($downloads[$selected_stat][1]);
$view->submitButton('download', 'Letöltés');
$view->endKeret();
