/*Controls the movement of the robot based on single integer
commands received through serial communication with a PC.

 6 = forward
-2 = left 90 degrees, then forward
 2 = right 90 degrees, then forward
-1 = left 45 degrees, then forward
 1 = right 45 degrees, then forward
default = stop
*/
int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

void drive_motors(int cmd) {
  
  //fail safe to stop
  if(abs(cmd)>4){
    stop();
  }
  else if(cmd<0){
    //negative direction indicates turning left
    left (leftspeed,rightspeed, -cmd*915);
  }
  else {
    right (leftspeed,rightspeed, cmd*975);
  }
}

void stop(void) //Stop
{
 digitalWrite(E1,LOW);
 digitalWrite(E2,LOW);
}
void forward(char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,LOW);
 analogWrite (E2,b);
 digitalWrite(M2,LOW);
}
void left (char a,char b, int time)
{
   unsigned long initTime = millis();
   analogWrite (E1,a);
   analogWrite (E2,b);
   digitalWrite(M1,HIGH);
   //delay while turning
   while(millis() <= initTime+time){ }
   digitalWrite(M1,LOW);
}
void right (char a,char b, int time)
{  
   unsigned long initTime = millis();
   analogWrite (E1,a);
   analogWrite (E2,b);
   digitalWrite(M2,HIGH);
   //delay while turning
   while(millis() <= initTime+time){ }
   digitalWrite(M2,LOW);
}
