function showNumber(num){
   var tbox = document.getElementById("rooms");
   tbox.value = tbox.value+num;
}
 
function showTotal(){
   var tbox = document.getElementById("rooms");
   var room_num = tbox.value;   
   
   $.post("set_end.php", { location: room_num } );

}

function clearBox() {
	var tbox = document.getElementById("rooms");
	tbox.value = "";
}

