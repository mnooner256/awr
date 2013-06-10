<?php
$ma = $_REQUEST['myarray'];


  $mf = "map_data.txt";
  $fh = fopen($mf,'w');

  $w = max(array_map('count', $ma));
  $l = count($ma);

  fwrite($fh, $l.",".$w.".");

  for ($i = 0; $i<$l; $i++){
    for ($j = 0; $j<$w; $j++){

      $m = $i.",".$j.":";
      fwrite($fh, $m);
      $values = explode(";",$ma[$i][$j]);

      if( ($values[0]==2)||($values[0]==3) ){
        fwrite($fh, $values[0].";".$values[1].";".$values[2]);
      }
      else{
        if( $values[0]==16 ){
          fwrite($fh, $values[0].";".$values[1]);
        }
        else{
          fwrite($fh, $values[0]);
        }
      }

    fwrite($fh, ".");
    }
  }

fclose($fh);
?>