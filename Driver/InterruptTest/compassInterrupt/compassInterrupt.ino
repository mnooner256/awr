#include <Wire.h>

#include <HMC5883L.h>

#include <TimerOne.h>

/*
 *  Timer1 library example
 *  June 2008 | jesse dot tane at gmail dot com
 */
 // Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur.
int error = 0;

const int sample = 5;
float avg[sample];
int initFlag=0;
float initHeading=0;

float heading;
float avgheading;
float goal;
volatile float headingDegrees;

void setup()
{
  //noInterrupts();
  pinMode(13, OUTPUT);
  
  Serial.begin(9600);
  Serial.flush();
  
  //Serial.println("\nStarting the I2C interface.");
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
    
  Timer1.initialize(5000000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(ISR_correction);  // attaches callback() as a timer overflow interrupt
  interrupts();
}
 
void ISR_correction()
{
  digitalWrite(13, digitalRead(13) ^ 1);
  
  
  //Serial.println(headingDegrees);
}
 
void loop()
{
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/ If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.01367;
  heading += declinationAngle;
  
  // Convert radians to degrees for readability.
  headingDegrees = heading * (180/M_PI); 
  
  // Check for initial reading
  if(initFlag==0) {
    initHeading = headingDegrees;
    Serial.print("heading: ");
    Serial.println(initHeading);
    initFlag = 1;
  }
  else
    // Convert all subsequent readings to be relative to the initial reading.
    headingDegrees -= initHeading;

  // Correct for when signs are reversed.
  if(headingDegrees < 0)
    headingDegrees += 360;
    
  // Check for wrap due to addition of declination.
  if(headingDegrees > 360)
    headingDegrees -= 360;
  
  avgheading = CalcAvg(headingDegrees);
  
  //if( abs(headingDegrees-avgheading*(180/M_PI))>=3 ) {
      Serial.println(headingDegrees);
  //}
}

float CalcAvg(float headingDegrees)
{
   float total= headingDegrees;
   
   for(int i= 0; i<sample-1; i++){
       total += avg[i];
       avg[i]=avg[i+1];
   }
   avg[sample-1] = headingDegrees;
   
   return (total/sample);
}
 
