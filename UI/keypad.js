
function showNumber(num){
   var tbox = document.getElementById("rooms");
   tbox.value = tbox.value+num;
}
 
function showTotal(){
   var tbox = document.getElementById("rooms");
   
   alert(tbox.value);
   tbox.value = "";
}

function clearBox() {
	var tbox = document.getElementById("rooms");
	tbox.value = "";
}