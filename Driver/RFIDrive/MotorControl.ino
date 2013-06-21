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

void drive(int dir) {
  
  if(dir >= 8)
    stop();  
  else 
  {
    goal = compassPoint[dir];
    
    float drift = avgheading-goal;
    while( abs(drift) >= .08727){
     
      //if the relative heading differs from the goal by more than 5 degrees
      //if error is greater than pi (180 degrees), then it is shorter to turn the other way
      if( drift <= -.08727 && abs(drift) < M_PI ) 
        left (leftspeed,rightspeed);    //turn left
      else  
        right (leftspeed,rightspeed);   //turn right
        
      //update position to check heading
      avgheading = read_from_compass();
      drift = avgheading-goal;
    }
    //once heading is correct, continue forward
    forward (leftspeed,rightspeed); 
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
void left (char a,char b)
{
   analogWrite (E1,a);
   digitalWrite(M1,HIGH);
   analogWrite (E2,b);
   digitalWrite(M2,LOW);
}
void right (char a,char b)
{  
   analogWrite (E1,a);
   digitalWrite(M1,LOW);
   analogWrite (E2,b);
   digitalWrite(M2,HIGH);
}

