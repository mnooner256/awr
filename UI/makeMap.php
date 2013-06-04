<?php
$ma = $_REQUEST['myarray'];

$con = mysqli_connect("localhost","root","","reu_wheelchair");

if(!mysqli_connect_errno($con)){



  $mf = "map_layout.txt";
  $fh = fopen($mf,'w');

  $w = max(array_map('count', $ma));
  $l = count($ma);

  fwrite($fh, $l." ".$w);
  fwrite($fh, "\r\n");

  for ($i = 0; $i<$l; $i++){
    for ($j = 0; $j<$w; $j++){

      $m = $i." ".$j." ";
      fwrite($fh, $m);
      $values = explode(";",$ma[$i][$j]);
      if($values[0] < 0){
        $values[0] = $values[1];
        $values[1] = $values[2];
      }
      if($values[0] == 0){
        fwrite($fh, "0");
      }
      else{
        $result = mysqli_query($con, "SELECT RFID_tag FROM rfid_values WHERE number = ".$values[0]);
  
        while($row = mysqli_fetch_array($result)){
        
          fwrite($fh, $row['RFID_tag']);
          if($values[1] != null){
            fwrite($fh, " ".$values[1]);
            $location = $l." ".$w;
            mysqli_query($con, "INSERT INTO room_locations VALUES ('$values[1]', '$location')");
          }
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