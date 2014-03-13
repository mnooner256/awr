<?php

$room = $_POST['location'];


$dbhandle = mysqli_connect("localhost","root","","reu_wheelchair");

if(!mysqli_connect_errno($dbhandle)){

  $fn = "end_location.txt";
  $fh = fopen($fn, 'w');

  $result = mysqli_query($dbhandle, "SELECT location FROM room_locations WHERE room = ".$room);
  while($row = mysqli_fetch_array($result)){

    fwrite($fh, $row['location']);
  }

  fclose($fh);
  mysqli_close($dbhandle);
}
else{
  mysqli_close($dbhandle);
}

?>