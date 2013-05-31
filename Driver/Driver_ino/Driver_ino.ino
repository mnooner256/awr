/*Author:    Anna Shafer
Description: Controls and directs the Rover with received instructions (a single char).
             f=forward, r=turn right 90 deg, t=turn right 45? deg(for diagonal),
             l=turn left 90 deg, k=turn left 45 deg, b=reverse/backwards*/
             
/*Current notes:
 - Check out encoders for turning?
 - Check out onboard compass for correction (and turning?)
 -- Integrate with RFID for navigation output
*/

#include <SoftwareSerial.h>

SoftwareSerial id20(3,4); // virtual serial port(RX,TX)
char i;
int flag =0;              // flag variable for RFID interrupt

int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

void RFID_ISR(){
  flag =1;
}
void setup()
{
   for(int i=5; i<=8; i++)
     pinMode(i, OUTPUT);
   attachInterrupt(0, RFID_ISR, HIGH);
     
   Serial.begin(9600);
   id20.begin(9600); //
}
void loop()
{
  //for debugging purposes
  Serial.println("test");
 
  if(flag == 1) RFID();
  
   //while (Serial.available() < 1) {} // Wait until a character is received
   //char val = Serial.read();
   int leftspeed = 255; //255 is maximum speed 
   int rightspeed = 255;
//   switch(val) // Perform an action depending on the command
//   {
//     case 'f'://Move Forward
//       forward (leftspeed,rightspeed);
//     break;
//     case 'l'://Turn Left
//       left (leftspeed,rightspeed);
//     break;
//     case 'r'://Turn Right
//       right (leftspeed,rightspeed);
//     break;
//     case 'k'://Move Diagonally Left
//       dleft (leftspeed,rightspeed);
//     break;
//     case 't'://Move Diagonally Right
//       dright (leftspeed,rightspeed);
//     break;
//     case 'b'://Move Backwards
//       reverse (leftspeed,rightspeed);
//     break;
//     default:
//     stop();
//     break;
//   } 
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
 analogWrite (E1,a);
 digitalWrite(M1,HIGH);
 analogWrite (E2,b);
 digitalWrite(M2,LOW);
}
void dright (char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,LOW);
 analogWrite (E2,b);
 digitalWrite(M2,HIGH);
}
