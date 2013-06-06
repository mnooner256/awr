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
         m[i][j] = "-9";  
        }
    }

    $('#drawSpace')[0].width=Math.max(b*scaleFactor,500);
    $('#drawSpace')[0].height=Math.max(a*scaleFactor,500);

    draw();


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
    
    ctx.fillStyle="rgb(255,255,255)";
    ctx.fillRect(0,0,w,l);

    canvas.addEventListener("click", q);

    for(var i = 0; i < a; i++){

      for(var j = 0; j < b; j++){

        holdString = m[i][j];
        if(holdString.indexOf(";") == -1){
          arrayValue = (holdString-1)+1;
        }
        else{
          arrayValue = holdString.slice(0,holdString.indexOf(";"));
        }
          

        if(arrayValue>0){
          ctx.fillStyle="rgb(255,255,255)";
          ctx.fillRect(j*wb,i*la,wb,la);
          ctx.strokeRect(j*wb,i*la,wb,la);
        }
        else{

        ctx.fillStyle="rgb(255,255,255)";
        ctx.fillRect(j*wb,i*la,wb,la);
        ctx.strokeStyle="grey";
        ctx.lineWidth=(la/4);
        ctx.beginPath();  
       


          if(arrayValue==-1){
drawVertWall(j,i,wb,la,ctx);
          }
          else{
            if(arrayValue==-2){
drawHoriWall(j,i,wb,la,ctx);          
            }
            else{
              if(arrayValue==-3){
drawHori(j,i,wb,la,ctx);
              }
              else{
                if(arrayValue==-4){
drawVert(j, i, wb, la, ctx);               
                }
                else{
                  if(arrayValue==-5){
drawBotRight(j,i,wb,la,ctx);

                  }
                  else{
                    if(arrayValue==-6){
drawTopLeft(j,i,wb,la,ctx);
                    }
                    else{
                      if(arrayValue==-7){
drawTopRight(j,i,wb,la,ctx);
                      }
                      else{
                        if(arrayValue==-8){
drawBotLeft(j,i,wb,la,ctx);
                        }
                        else{
                          if(arrayValue==-9){
drawColumn(j,i,wb,la,ctx);
                          }
                          else{
                            if(arrayValue==-9999){
                              ctx.fillStyle="rgb(255,0,0)";
                              ctx.fillRect(j*wb,i*la,wb,la);
                            }
                            else{
                              ctx.fillStyle="grey";
                              ctx.fillRect(j*wb,i*la,wb,la);
			    }
			  }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          ctx.stroke();
          ctx.lineWidth="1";
          ctx.strokeStyle="black";
          ctx.fillStyle="rgb(255,255,255)";
          ctx.strokeRect(j*wb,i*la,wb,la);
        }
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
  var l = canvas.width, w = canvas.height, la = scaleFactor, wb = scaleFactor;

  var yBox = y/la;
  yBox = Math.floor(yBox);
  var xBox = x/wb;
  xBox = Math.floor(xBox);
  var initVal = m[yBox][xBox];

  if(initVal.indexOf(";") == -1){
    initVal = (initVal-1)+1;
  }
  else{
   // initVal = initVal.slice(0,initVal.indexOf(";"));
  }


  var value = prompt("Enter the RFID Value.(Empty or 0 for walls. -1 & -2 for doors)",initVal);
  var value2 = null;
  var value3 = null;
 

  if(value!=null){
    if(value < 0){
      if(value == -1){
        var value2 = prompt("Enter the wall RFID Value.");
        var value3 = prompt("Enter the room number.");
      }
      else{
        if(value == -2){
          var value2 = prompt("Enter the wall RFID Value.");
          var value3 = prompt("Enter the room number.");
        }
        else{
      //    value="0";
        }
      }
    }
  }
  else{
    value = "0";
  }

//    value = value-1+1;

    if(value2!=null){
      if(value3!=null){
        var hold = value2.concat(";");
        hold = hold.concat(value3);
        value = value.concat(";");
        value = value.concat(hold);
      }
    }

    m[yBox][xBox] = value;
  
  draw();
}

function showmatrix(){
 for(var x = 0; x<a; x++){
   document.writeln(m[x]+"<br>");
 }
}


function lock(){
  var canLock = 0;
  var holdString;
  var arrayValue;

  for(var i = 0; i < a; i++){

    for(var j = 0; j < b; j++){
      holdString = m[i][j];
      if(holdString.indexOf(";") == -1){
        arrayValue = (holdString-1)+1;
      }
      else{
        arrayValue = holdString.slice(0,holdString.indexOf(";"));
      }
      if(arrayValue<-2){
        canLock = 1;
      }
    }
  }
 
  if(canLock == 0){
    
    $.post("makeMapData.php", {myarray : m});
    $.post("makeMap.php", {myarray : m}, function(){ 
    isLock = 1;
    alert("done");
});

  }
  else{
    alert("There must be no more teal tiles");
  }
}

function reset(){
  isEnd = 0;
  isLock = 0;
  isEndSet = 0;
  for(var i = 0; i < a; i++){
    for(var j = 0; j < b; j++){
      m[i][j] = "0";
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
    draw();  
});

}

function drawVertWall(j, i, wb, la, ctx){
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(j*wb-((j-1)*wb))/2), ((i*la)+(((i+1)*la)-(i*la))/1.5), ctx);
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ((j*wb)+(j*wb-((j-1)*wb))/2), ((i*la)+(((i+1)*la)-(i*la))/3), ctx);
//  ctx.lineWidth=(la/8);
//  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2.5), ((i*la)+(((i+1)*la)-(i*la))/1.5), (j*wb)+(j*wb-((j-1)*wb)), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawHoriWall(j, i, wb, la, ctx){
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(((j+1)*wb)-(j*wb))/1.5), ((i*la)+(i*la-((i-1)*la))/2), ctx);
  drawLineSegment( (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(((j+1)*wb)-(j*wb))/3), ((i*la)+(i*la-((i-1)*la))/2), ctx);
//  ctx.lineWidth=(la/8);
//  drawLineSegment( ((j*wb)+(((j+1)*wb)-(j*wb))/1.2), ((i*la)+(i*la-((i-1)*la))/2.5), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+(i*la-((i-1)*la))), ctx);
}

function drawVert(j, i, wb, la, ctx){
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ctx);
}

function drawHori(j, i, wb, la, ctx){
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ctx);
}

function drawTopLeft(j, i, wb, la, ctx){
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/1.6), ctx);
  drawLineSegment( (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(j*wb-((j-1)*wb))/1.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawTopRight(j, i, wb, la, ctx){
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), (i*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+(i*la-((i-1)*la))/1.6), ctx);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), (j*wb)+ ((j*wb-((j-1)*wb))/2.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawBotLeft(j, i, wb, la, ctx){
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i+1)*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/2.6), ctx);
  drawLineSegment( (j*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+(j*wb-((j-1)*wb))/1.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);
}

function drawBotRight(j, i, wb, la, ctx){
  drawLineSegment( ((j*wb)+(j*wb-((j-1)*wb))/2),((i+1)*la), ((j*wb)+(((j+1)*wb)-(j*wb))/2), ((i*la)+ (i*la-((i-1)*la))/2.6), ctx);
  drawLineSegment( ((j+1)*wb), ((i*la)+(i*la-((i-1)*la))/2), ((j*wb)+ (j*wb-((j-1)*wb))/2.6), ((i*la)+(((i+1)*la)-(i*la))/2), ctx);

}


function drawColumn(j, i, wb, la, ctx){
  ctx.stroke();
  ctx.arc( ((j*wb)+(j*wb-((j-1)*wb))/2), ((i*la)+(i*la-((i-1)*la))/2), wb/2, 0, 2*Math.PI);
  ctx.fillStyle="grey";
  ctx.fill();
  ctx.beginPath();
}



function drawLineSegment(start_x, start_y, end_x, end_y, ctx){
  ctx.moveTo(start_x, start_y);
  ctx.lineTo(end_x, end_y);
}
