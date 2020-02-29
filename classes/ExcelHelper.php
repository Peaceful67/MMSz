<?php

include_once CLASSES . 'PHPExcel.php';
include_once CLASSES . 'PHPExcel/Writer/Excel2007.php';

class ExcelHelper {

    protected $filename;
    protected $currentRow;
    protected $numCol = 0;
    protected $objPHPExcel;

    function __construct($title, $fn) {
        $this->filename = $fn;
        $this->currentRow = 1;
        $this->objPHPExcel = new PHPExcel();
        $this->objPHPExcel->getProperties()->setCreator(getOptionValue(OPTIONS_NAME_COMPANY_NAME) . ' Tagnyilvántartó');
        $this->objPHPExcel->setActiveSheetIndex(0);
        $this->objPHPExcel->getActiveSheet()->setTitle($title);
    }

    function insertRow($arr) {
        if (empty($arr)) {
            error_log('ExcelHelper: Empty array');
            return false;
        }
        if ($this->numCol == 0) {
            $this->numCol = count($arr);
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

    function close($filename) {
        $filePath = TMP_DIR . $filename;
        $objWriter = new PHPExcel_Writer_Excel2007($this->objPHPExcel);
        $objWriter->save($filePath);
        readfile($filePath);
        unlink($filePath);
    }


}
