<?php
$ma = $_REQUEST['myarray'];

$con = mysqli_connect("localhost","root","","reu_wheelchair");

if(!mysqli_connect_errno($con)){



  $mf = "map_layout.txt";
  $fh = fopen($mf,'w');

  $w = max(array_map('count', $ma));
  $l = count($ma);


  for ($i = 0; $i<$l; $i++){
    for ($j = 0; $j<$w; $j++){

      $m = $i.",".$j." ";
      fwrite($fh, $m);
      if($ma[$i][$j] == 0){
        fwrite($fh, "0");
      }
      else{
        $result = mysqli_query($con, "SELECT RFID_tag FROM rfid_values WHERE number = ".$ma[$i][$j]);
  
        while($row = mysqli_fetch_array($result)){
        
          fwrite($fh, $row['RFID_tag']);
        
        }
      }
    fwrite($fh, "\r\n");
    }
  }
}
else{
  mysqli_close($con);
}

fclose($fh); 
?>