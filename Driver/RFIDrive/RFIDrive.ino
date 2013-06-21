#include <SoftwareSerial.h>
// Reference the I2C Library
#include <Wire.h>
// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

// Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur.
int error = 0;

const int sample = 5;
int initFlag=0;
float initHeading;
float avg[sample];
float heading;
volatile float avgheading;
float goal;

//8 points of the compass starting with E = 0 (degrees) 0, 315, 270, 225, 180, 135, 90, 45
const float compassPoint[8] = {0, PI*7/4, PI*3/2, PI*5/4,
                          PI, PI*3/4, PI/2, PI/4};

SoftwareSerial id20(3,4); // virtual serial port(RX,TX) for RFID reader
char tag[100];
int dir = 6;
char* hand_var = "H\n"; //Serial comm handshake variable
int hand_flag = 0; //flag for successful handshake/setup

void setup()
{
  //set pins 5-8 to output for motors
  for(int i=5; i<=8; i++)
     pinMode(i, OUTPUT);
     
  //initialize the serial communication baudrates
  Serial.begin(9600);
  id20.begin(9600);
  Wire.begin(); // Start the I2C interface.

  //Serial.println("Constructing new HMC5883L");
  compass = HMC5883L(); // Construct a new HMC5883 compass.
    
  //Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
  //Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));

  //initialize the LED for confirmation
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  //perform handshake with PC via serial and then light LED
  Serial.flush();
  Serial.print(hand_var);
  char confirm[2];
  Serial.readBytes(confirm,1);
  if(confirm[0] == 'H') {
    digitalWrite(13, HIGH);
    hand_flag = 1; //if handshake was successful, set flag
  }
}

void loop()
{
  //begin program only if serial communication setup was successful
  //if(hand_flag ==  1){
    //scan_rfid();
    avgheading = read_from_compass();
    read_from_controller();
  //}
}

void scan_rfid() 
{
  int bytes = 0;
  if (id20.available() > 0) {
    bytes = id20.readBytesUntil(13, tag, sizeof(tag));
    
    Serial.print('R');
    Serial.print(':');
    for ( int i = 1; i < bytes; i++) {
      Serial.print(tag[i]);
    }
    Serial.print('\n');
    Serial.flush();
    
    //Read off the closing two bytes constant of the RFID
    id20.readBytes(tag,sizeof(tag));
  }
}

void read_from_controller()
{
  if(Serial.available() >0){
    //read in the new direction command and convert to int
    char temp = Serial.read();
    dir = temp-'0';
   
    drive(dir);
      
  }
}

float read_from_compass()
{
  // Retrieve the scaled values from the compass
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/ If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.01367;
  heading += declinationAngle;

  // Convert radians to degrees for readability.
  //heading = heading * (180/M_PI); 
  
  // Check for initial reading
  if(initFlag==0) {
    initHeading = heading;
    initFlag = 1;
  }
  else {
    // Convert all subsequent readings to be relative to the initial reading.
    heading -= initHeading;
  }
  
  // Correct for when signs are reversed.
  if(heading < 0) 
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
      
  //keep a running average of the last several reads to eliminate spikes
  return avgheading = CalcAvg(heading);
}

float CalcAvg(float heading)
{
   float total= heading;
   
   for(int i= 0; i<sample-1; i++){
       total += avg[i];
       avg[i]=avg[i+1];
   }
   avg[sample-1] = heading;
   
   return (total/sample);
}
 
