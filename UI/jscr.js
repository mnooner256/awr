function generate(){

    a = document.getElementById('rows').value;
    b = document.getElementById('column').value;
    a = a-1+1;
    b = b-1+1;    
    m = new Array(a);

    for (var x = 0; x<a;x++){
        m[x] = new Array(b);
    }

    for (var i = 0; i < a; i++) {
        for (var j = 0; j < b; j++){
         m[i][j] = 0;  
        }
    }
    draw();
}

function draw(){

  var canvas = document.getElementById('drawSpace');
  var l = canvas.width;
  var w = canvas.height;
  var la = l/a;
  var wb = w/b;

  if(canvas.getContext) {

    var ctx = canvas.getContext("2d");

    canvas.addEventListener("click", q);

    for(var i = 0; i < a; i++){

      for(var j = 0; j < b; j++){
        
        if(m[i][j]>0){
          ctx.fillStyle="rgb(255,255,255)";
          ctx.fillRect(j*wb,i*la,wb,la);
          ctx.strokeRect(j*wb,i*la,wb,la);
        }
        else{
          if(m[i][j]==-1){
//ctx.fillStyle="rgb(120,255,120)";
//ctx.fillRect(j*wb,i*la,wb,la);
ctx.fillStyle="rgb(255,255,255)";
ctx.fillRect(j*wb,i*la,wb,la);

ctx.strokeStyle="grey";
ctx.lineWidth="30";
ctx.beginPath();
ctx.moveTo( (j*wb)+ (j*wb-((j-1)*wb))/2,((i+1)*la));
ctx.lineTo( (j*wb)+ (j*wb-((j-1)*wb))/2, (i*la)+(((i+1)*la)-(i*la))/1.5);
ctx.stroke();

ctx.beginPath();
ctx.moveTo((j*wb)+ (j*wb-((j-1)*wb))/2, (i*la));
ctx.lineTo( (j*wb)+ (j*wb-((j-1)*wb))/2,(i*la)+(((i+1)*la)-(i*la))/3);
ctx.stroke();

ctx.lineWidth="15";

ctx.beginPath();
ctx.moveTo( (j*wb)+ (j*wb-((j-1)*wb))/2.5, (i*la)+(((i+1)*la)-(i*la))/1.2);
ctx.lineTo( (j*wb)+ (j*wb-((j-1)*wb)), (i*la)+(((i+1)*la)-(i*la))/2);
ctx.stroke();
ctx.lineWidth="1";
ctx.strokeStyle="black";

    //        ctx.fillStyle="rgb(100,80,255)";
    //        ctx.fillRect(j*wb,i*la,wb,la);
          }
          else{
            if(m[i][j]==-999){
            ctx.fillStyle="rgb(255,0,0)";
            ctx.fillRect(j*wb,i*la,wb,la);
            }
            else{
              ctx.fillStyle="grey";
              ctx.fillRect(j*wb,i*la,wb,la);
            }
          }
        }
          ctx.fillStyle="rgb(255,255,255)";
          ctx.strokeRect(j*wb,i*la,wb,la);
      }
    }
  }
}

function q(event){
  var xCod = event.pageX;
  var yCod = event.pageY;
  canvas = document.getElementById('drawSpace');
  xCod -= canvas.offsetLeft;
  yCod -= canvas.offsetTop;
  if(isEnd == 0){
    change(xCod, yCod);
  }
  else{
    changeEnd(xCod, yCod);
  }
}

function change(x, y){

  var canvas = document.getElementById('drawSpace');
  var l = canvas.width, w = canvas.height, la = l/a, wb = w/b;
  var yBox = y/la;
  yBox = Math.floor(yBox);
  var xBox = x/wb;
  xBox = Math.floor(xBox);
  var initVal = m[yBox][xBox];
  //if(initVal == -1){
 //   initVal = 0;
 // }

  var value = prompt("Enter the RFID Value.(Empty or negative for walls)",initVal);

  if(value!=null){
    if(value < -1){
          value = 0;
    }
    value = value-1+1;



    m[yBox][xBox] = value;

  }
  
  draw();
}

function showmatrix(){
 for(var x = 0; x<a; x++){
   document.writeln(m[x]+"<br>");
 }
}

function changeEnd(x, y){

  for(var i = 0; i < a; i++){
    for(var j = 0; j < b; j++){
      if(m[i][j] == -999){
        m[i][j] = 0;
      }
    }
  }
  
  var canvas = document.getElementById('drawSpace');
  var l = canvas.width, w = canvas.height, la = l/a, wb = w/b;
  var yBox = y/la;
  yBox = Math.floor(yBox);
  var xBox = x/wb;
  xBox = Math.floor(xBox);
  if(m[yBox][xBox] > 0){
    m[yBox][xBox] = -999;
    isEndSet=1;
    isEnd=0;
  }
  else{
    alert("The end must be in an empty space");
  }
  draw();

}

function lock(){
  var canLock = 0;
  for(var i = 0; i < a; i++){

    for(var j = 0; j < b; j++){

      if(m[i][j]<0){
        canLock = 1;
      }
    }
  }
 
  if(canLock == 0){
    $.post("makeMap.php", {myarray : m}, function(){ 
    isLock = 1;
    alert("done");
});

  }
  else{
    alert("There must be no more teal tiles");
  }
}

function setEnd(){
  if(isLock==1){
    isEnd = 1;
    alert("Click the tile to make as the end");
  }
  else{
    alert("You must lock the layout first");
  }
}

function save(){
  if(isEndSet==1){
    $.post("makeEnd.php", {myarray : m}, function(){
    alert("done");
    });
  }
  else{
    alert("You must set an end point first");
  }
}


function reset(){
  isEnd = 0;
  isLock = 0;
  isEndSet = 0;
  for(var i = 0; i < a; i++){
    for(var j = 0; j < b; j++){
      m[i][j] = -1;
    }
  }
  draw();
}

