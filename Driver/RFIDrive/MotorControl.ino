
int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

int leftspeed = 100;
int rightspeed = 100;

void drive_motors(char dir) {

   switch(dir) // Perform an action depending on the command
   {
     case 'f'://Move Forward
       forward (leftspeed,rightspeed);
     break;
     case 'l'://Turn Left
       left (leftspeed,rightspeed);
     break;
     case 'r'://Turn Right
       right (leftspeed,rightspeed);
     break;
     case 'k'://Move Diagonally Left
       dleft (leftspeed,rightspeed);
     break;
     case 't'://Move Diagonally Right
       dright (leftspeed,rightspeed);
     break;
     case 'b'://Move Backwards
       reverse (leftspeed,rightspeed);
     break;
     default:
     stop();
     break;
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
void reverse (char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,HIGH);
 analogWrite (E2,b);
 digitalWrite(M2,HIGH);
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
void dleft (char a,char b)
{
}
void dright (char a,char b)
{
}


