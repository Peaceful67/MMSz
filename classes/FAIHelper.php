<?php

include_once CLASSES . 'PHPExcel.php';
include_once CLASSES . 'PHPExcel/Writer/Excel2007.php';

class FAIHelper {

    protected $objPHPExcel;
    protected $rowExcel;

    public function createExcel($filename, $title) {
        $this->rowExcel = 1;
        $obj = new PHPExcel();
        $obj->getProperties()->setCreator(getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' Tagnyilvántartó');


        $obj->setActiveSheetIndex(0);
        $obj->getActiveSheet()->setTitle($title);

        $obj->getActiveSheet()->setCellValue('A1', 'FAI ID');
        $obj->getActiveSheet()->setCellValue('B1', 'Licence Number');
        $obj->getActiveSheet()->setCellValue('C1', 'Validity From');
        $obj->getActiveSheet()->setCellValue('D1', 'Validity To');
        $obj->getActiveSheet()->setCellValue('E1', 'Air Sport Discipline');
        $obj->getActiveSheet()->setCellValue('F1', 'First Name');
        $obj->getActiveSheet()->setCellValue('G1', 'Last Name');
        $obj->getActiveSheet()->setCellValue('H1', 'Gender');
        $obj->getActiveSheet()->setCellValue('I1', 'Date of Birth');
        $obj->getActiveSheet()->setCellValue('J1', 'Nationality');
        $obj->getActiveSheet()->setCellValue('K1', 'Country of Residence');
        $obj->getActiveSheet()->setCellValue('L1', 'Address1');
        $obj->getActiveSheet()->setCellValue('M1', 'Address2');
        $obj->getActiveSheet()->setCellValue('N1', 'Address3');
        $obj->getActiveSheet()->setCellValue('O1', 'Country of addresse');
        $obj->getActiveSheet()->setCellValue('P1', 'E-mail address');
        $obj->getActiveSheet()->setCellValue('Q1', 'Phone (home)');
        $obj->getActiveSheet()->setCellValue('R1', 'Phone (office)');
        $obj->getActiveSheet()->setCellValue('S1', 'Phone (mobile)');
        $this->objPHPExcel = $obj;
    }

    public function writeRow($id, $licence, $from, $to, $disc, $first, $last, $gender, $birth, $addr1, $addr2, $addr3, $email, $tel) {
        $this->rowExcel++;
        $act = $this->objPHPExcel->getActiveSheet();
        $act->setCellValue('A'.$this->rowExcel, $id);
    }

}
