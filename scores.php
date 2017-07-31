<?php

$file = "scores.data";

$u = $_GET["username"];
$s = $_GET["score"];

$addLine = $u ."\t".$s."\n";

$fh = file_put_contents($file, $addLine, FILE_APPEND | LOCK_EX);

$fileLines = file_get_contents($file);

$array = explode("\n", $fileLines);

$kva = array();

foreach($array as $line) {

    $keyValue = explode("\t", $line);
  
    $key = $keyValue[0];
    $val = $keyValue[1];

    $storedValue = $kva[$key];
    if(!$storedValue || $storedValue < val) {
        $kva[$key] = $val;
    }
}

arsort($kva, SORT_NUMERIC);

$arraySize = min(count($kva),3);
$counter = 0;

foreach($kva as $key => $val) {
    echo "$key\t$val\n";
    $counter++;
    if($counter >= $arraySize) {
        break; //stop printing
    }
}

?>
