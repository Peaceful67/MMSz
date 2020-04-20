<?php

include_once CLASSES . 'PHPExcel.php';
include_once CLASSES . 'PHPExcel/Writer/Excel2007.php';

class ExcelHelper {

    protected $filename;
    protected $currentRow;
    protected $numCol;
    protected $objPHPExcel;

    function __construct($title, $fn) {
        $this->filename = $fn;
        $this->currentRow = 1;
        $this->numCol = 0;
        $this->objPHPExcel = new PHPExcel();
        $this->objPHPExcel->getProperties()->setCreator(getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' Tagnyilvántartó');
        $this->objPHPExcel->setActiveSheetIndex(0);
        $this->objPHPExcel->getActiveSheet()->setTitle($title);
    }

    function insertHeaderRow($arr) {
        $this->numCol = count($arr);        
        $this->insertRow($arr);
    }

    function insertRow($arr) {
        if (empty($arr)) {
            error_log('ExcelHelper: Empty array');
            return false;
        }
        if ($this->numCol != count($arr)) {
            error_log('Wrong number of Columns in ExcelSheet');
        }
        if ($this->numCol != count($arr)) {
            error_log('ExcelHelper: Wrong length of array');
            return false;
        }
        for ($index = 0; $index < count($arr); $index++) {
            $this->objPHPExcel->getActiveSheet()->getColumnDimensionByColumn($index)->setAutoSize(true);
            $this->objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow($index, $this->currentRow, $arr[$index]);
        }
        $this->currentRow++;
        return true;
    }

    function save() {
        $this->sendHeader();
        $objWriter = new PHPExcel_Writer_Excel2007($this->objPHPExcel);
        $objWriter->save('php://output');
        exit;
    }

    function close($filename) {
        $filePath = $filename;
        $objWriter = new PHPExcel_Writer_Excel2007($this->objPHPExcel);
        $objWriter->save($filePath);
        readfile($filePath);
        unlink($filePath);
    }

    protected function sendHeader() {
        ob_clean();
        header('Content-Type: application/vnd.ms-excel; charset=utf-8');
        header('Content-Disposition: attachment; filename="' . $this->filename . '"');
//    header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
//    header('Content-Length: ' . filesize($filename));
//    header('Content-Transfer-Encoding: BINARY');
//    header('Cache-Control: no-cache');
//    header('Pragma: public');
    }

}
