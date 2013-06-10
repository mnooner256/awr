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
         m[i][j] = "1";  
        }
    }

    $('#drawSpace')[0].width=Math.max(b*scaleFactor,500);
    $('#drawSpace')[0].height=Math.max(a*scaleFactor,500);

    drawToolBar();
    draw();


}

function drawToolBar(){
  var canvas = document.getElementById('toolBar');
  var l = canvas.width;
  var w = canvas.height;
  var la = scaleFactor;
  var wb = scaleFactor;
  
  if(canvas.getContext){

    var ctx = canvas.getContext("2d");

    canvas.addEventListener("click", toolBarClick);

    ctx.fillStyle="rgb(255,255,255)";
    ctx.fillRect(0,0,w,l);
    ctx.beginPath();
    drawFuns = new Array(new Array(drawDefault,drawWall,drawVertWall),new Array(drawHoriWall,drawHori,drawVert),new Array(drawBotRight,drawTopLeft,drawTopRight),new Array(drawBotLeft,drawColumn,drawTopT),new Array(drawBottomT,drawLeftT,drawRightT),new Array(drawCross,drawRFID));

    loadedFunction = drawFuns[0][0];

    for(var i = 0; i<6; i++){
      for(var j = 0; j<3; j++){
        
        ctx.strokeStyle="grey";
        ctx.lineWidth=(la/4);
        if(i*j!=10){
          drawFuns[i][j](j, i, wb, la, ctx);
        }
      }
    }
    ctx.stroke();

    for(var i = 0; i<6; i++){
      for(var j = 0; j<3; j++){
      
        if(i*j!=10){
          ctx.strokeStyle="black";
          ctx.lineWidth=1;
          ctx.strokeRect(j*wb,i*la,wb,la);
        }
      }
    }
  }
}

function draw(){

  var canvas = document.getElementById('drawSpace');
  var l = canvas.width;
  var w = canvas.height;
  var la = scaleFactor;
  var wb = scaleFactor;
  var arrayValue;
  var holdString;


  if(canvas.getContext) {

    var ctx = canvas.getContext("2d");

    canvas.addEventListener("click", mapClick);

    for(var i = 0; i < a; i++){

      for(var j = 0; j < b; j++){
        ctx.strokeRect(j*wb,i*la,wb,la);
        holdString = m[i][j];

        if(holdString.indexOf(";") == -1){
          arrayValue = (holdString-1)+1;
        }
        else{
          arrayValue = holdString.slice(0,holdString.indexOf(";"));
        }
          arrayValue = (arrayValue-1)+1;

        ctx.fillStyle="rgb(255,255,255)";
        ctx.fillRect(j*wb,i*la,wb,la);
        ctx.strokeStyle="grey";
        ctx.lineWidth=(la/4);
        ctx.beginPath();  

        drawFuns[Math.floor(arrayValue/3)][arrayValue%3](j, i, wb, la, ctx);

        ctx.stroke();
        ctx.lineWidth="1";
        ctx.strokeStyle="black";
        ctx.fillStyle="rgb(255,255,255)";

        ctx.strokeRect(j*wb,i*la,wb,la);
      }
    }
  }
}

function toolBarClick(event){
  var xCod = event.clientX;
  var yCod = event.clientY;
  canvas = document.getElementById('toolBar');
  var rect = canvas.getBoundingClientRect();
  xCod -= rect.left;
  yCod -= rect.top;

  var l = canvas.width;
  var w = canvas.heigth;
  var la = scaleFactor;
  var wb = scaleFactor;

  var yBox = yCod/la;
  yBox = Math.floor(yBox);
  var xBox = xCod/wb;
  xBox = Math.floor(xBox);

  if(yBox==5 && xBox==2){
    loadedFunction = drawFuns[0][0];
    loadedFunctionId = "0";
  }
  else{
    loadedFunction = drawFuns[yBox][xBox]; 
    loadedFunctionId = ((yBox*3)+xBox)+"";

  }
}

function mapClick(event){
  var xCod = event.clientX;
  var yCod = event.clientY;
  canvas = document.getElementById('drawSpace');
  var rect = canvas.getBoundingClientRect();
  xCod -= rect.left;
  yCod -= rect.top;
  change(xCod, yCod);

}

function change(x, y){

  var canvas = document.getElementById('drawSpace');
  var l = canvas.width, w = canvas.height, la = scaleFactor, wb = scaleFactor;

  var ctx = canvas.getContext("2d");

  var yBox = y/la;
  yBox = Math.floor(yBox);
  var xBox = x/wb;
  xBox = Math.floor(xBox);

  var initVal = m[yBox][xBox];

  if(initVal.indexOf(";") == -1){
    initVal = (initVal-1)+1;
  }
  var value=null;
  var value2=null;
  var value3=null;
  if(loadedFunctionId==2 || loadedFunctionId==3){
     if(initVal == 1){
       initVal = "0;0;0";
     }
     var holdInit = initVal.slice(initVal.indexOf(";")+1, initVal.length);

     value2 = prompt("Enter the door RFID Value. Old value: "+holdInit.slice(0, holdInit.indexOf(";")));  
     value3 = prompt("Enter the room number. Old value: "+holdInit.slice(holdInit.indexOf(";")+1, holdInit.length));
     value = loadedFunctionId+";"+value2+";"+value3;
  }
  else{
    if(loadedFunctionId==16){
     if(initVal == 1){
       initVal = "0;0";
     }
       value2 = prompt("Enter the RFID value. Old value: "+initVal.slice(initVal.indexOf(";")+1, initVal.length));
       value = 16+";"+value2;
    }
    else{
      value = loadedFunctionId+"";
    }
  }

  m[yBox][xBox] = value;
  draw();
}


function lock(){
    $.post("makeMapData.php", {myarray : m});
    $.post("makeMap.php", {myarray : m}, function(){ 
    isLock = 1;
    alert("done"); });
}

function resetMap(){
  for(var i = 0; i < a; i++){
    for(var j = 0; j < b; j++){
      m[i][j] = "1";
    }
  }
  draw();
}

function load(){
  var holder1;
  var holder2;
  var counter = 0;
  jQuery.get('http://localhost/Map/map_data.txt', function(data){
  
  while(data.length > 0){
    holder1 = data.slice(0,data.indexOf("."));
    data = data.slice(data.indexOf(".")+1, data.length);
    if(counter==0){
      a = holder1.slice(0,holder1.indexOf(","));
      b = holder1.slice(holder1.indexOf(",")+1, holder1.length);   
      m = new Array(a);
      $('#drawSpace')[0].width=Math.max(b*scaleFactor,500);
      $('#drawSpace')[0].height=Math.max(a*scaleFactor,500);

      for (var x = 0; x<a;x++){
        m[x] = new Array(b);
      }

      for (var i = 0; i < a; i++) {
        for (var j = 0; j < b; j++){
         m[i][j] = "0";  
        }
      }
    }
    else{
      var i = holder1.slice(0,holder1.indexOf(","));
      var j = holder1.slice(holder1.indexOf(",")+1, holder1.indexOf(":"));
      i = i-1+1;
      j = j-1+1;
      var value = holder1.slice(holder1.indexOf(":")+1, holder1.length);
      value = value.concat("");
      m[i][j] = value;
    }
    counter++;

  }
    drawToolBar();
    draw();  
});

}

function drawVertWall(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(j*wb-((j-1)*wb))/2), ((i*la)+(((i+1)*la)-(i*la))/1.5), ctx);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ((j*wb)+(j*wb-((j-1)*wb))/2), ((i*la)+(((i+1)*la)-(i*la))/3), ctx);
}

function drawHoriWall(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(((j+1)*wb)-(j*wb))/1.5), ((i*la)+(i*la-((i-1)*la))/2), ctx);
  drawLineSegment( (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(((j+1)*wb)-(j*wb))/3), ((i*la)+(i*la-((i-1)*la))/2), ctx);
}

function drawVert(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ctx);
}

function drawHori(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ctx);
}

function drawTopLeft(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/1.6), ctx);
  drawLineSegment( (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(j*wb-((j-1)*wb))/1.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawTopRight(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+(i*la-((i-1)*la))/1.6), ctx);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb)+ ((j*wb-((j-1)*wb))/2.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawBotLeft(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/2.6), ctx);
  drawLineSegment( (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(j*wb-((j-1)*wb))/1.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawBotRight(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2),((i+1)*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/2.6), ctx);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+ (j*wb-((j-1)*wb))/2.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawTopT(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/1.6), ctx);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ctx);
}

function drawBottomT(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ctx);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2),((i+1)*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/2.6), ctx);
}

function drawLeftT(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ctx);
  drawLineSegment( (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(j*wb-((j-1)*wb))/1.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawRightT(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ctx);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb)+ ((j*wb-((j-1)*wb))/2.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawCross(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ctx);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ctx);
}

function drawDefault(j, i, wb, la, ctx){
  ctx.lineWidth=1;
  ctx.fillStyle="rgb(255,255,255)";
  ctx.strokeRect(j*wb,i*la,wb,la);
}

function drawWall(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  ctx.fillStyle="grey";
  ctx.fillRect(j*wb,i*la,wb,la);
}

function drawRFID(j, i, wb, la, ctx){
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  ctx.fillStyle="rgb(0,0,255)";
  ctx.globalAlpha=0.1;
  ctx.fillRect(j*wb,i*la,wb,la);
  ctx.globalAlpha=1;
}

function drawColumn(j, i, wb, la, ctx){
  ctx.stroke();

  ctx.beginPath();
  ctx.fillStyle="rgb(255,255,255)";
  ctx.fillRect(j*wb,i*la,wb,la);
  ctx.arc( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i*la)+(i*la-((i-1)*la))/2), wb/2, 0, 2*Math.PI);
  ctx.fillStyle="grey";
  ctx.fill();

  ctx.beginPath();
}

function drawLineSegment(start_x, start_y, end_x, end_y, ctx){
  ctx.moveTo(start_x, start_y);
  ctx.lineTo(end_x, end_y);
}
