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
      if( ($values[0]==2)||($values[0]==3) ){
        $values[0] = $values[1];
        $values[1] = $values[2];
      }
      else{
        if($values[0] == 16){
          $values[0] = $values[1];
          $values[1] = null;
        }
        else{
          $values[0] = 0;
        }
      }



      if($values[0] == 0){
        fwrite($fh, "w");
      }
      else{
        $result = mysqli_query($con, "SELECT RFID_tag FROM rfid_values WHERE number = ".$values[0]);
  
        while($row = mysqli_fetch_array($result)){
        
          fwrite($fh, $row['RFID_tag']);
          if($values[1] != null){
            $location = $i." ".$j;
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