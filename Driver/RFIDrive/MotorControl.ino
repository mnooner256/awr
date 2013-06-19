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

int leftspeed = 150;
int rightspeed = 140;

void drive_motors(int dir) {
  if(abs(dir)>4)
    stop();
    
  else if(dir<0){
    left (leftspeed,rightspeed, -dir*1000);
  }
  else {
    right (leftspeed,rightspeed, dir*1000);
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
   digitalWrite(M1,HIGH);
   analogWrite (E2,b);
   digitalWrite(M2,LOW);
   while(millis() <= initTime+time){ }
   
   digitalWrite(M1,LOW);
}
void right (char a,char b, int time)
{  
   unsigned long initTime = millis();
   analogWrite (E1,a);
   digitalWrite(M1,LOW);
   analogWrite (E2,b);
   digitalWrite(M2,HIGH);
   while(millis() <= initTime+time){ }
   
   digitalWrite(M2,LOW);
}

