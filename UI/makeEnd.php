<?php
$ma = $_REQUEST['myarray'];


$mf = "end_point.txt";
$fh = fopen($mf,'w');

$w = max(array_map('count', $ma));
$l = count($ma);


for ($i = 0; $i<$l; $i++){
  for ($j = 0; $j<$w; $j++){

    if($ma[$i][$j] == -999){
      $m = $i.",".$j." ";
      fwrite($fh, $m);
      fwrite($fh, "\r\n");
    }
  }
}

fclose($fh);
?>