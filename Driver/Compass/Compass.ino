/*

*/

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
float initHeading=0;
float avg[sample];

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  // Initialize the serial port.
  Serial.begin(9600);
  Serial.flush();
  
  Serial.println("\nStarting the I2C interface.");
  Wire.begin(); // Start the I2C interface.

  Serial.println("Constructing new HMC5883L");
  compass = HMC5883L(); // Construct a new HMC5883 compass.
    
  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
  
  Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
    
    
}

// Our main program loop.
void loop()
{
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/ If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.01367;
  heading += declinationAngle;
  
  // Convert radians to degrees for readability.
  float headingDegrees = heading * (180/M_PI); 
  
  // Check for initial reading
  if(initFlag==0) {
    initHeading = headingDegrees;
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
   
  // Output the data via the serial port.
  Output(headingDegrees);

  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However, since we have a long serial out (104ms at 9600), we will let 
  // it run at its natural speed.
  delay(500);
}

// Output the data down the serial port.
void Output(float headingDegrees)
{
   Serial.print("   \tHeading:\t");
   Serial.print(headingDegrees);
   Serial.print(" Degrees   \t");
   
   int pos = CalcAvg(headingDegrees);
   Serial.print("   \tAverage:\t");
   Serial.print(pos);
   Serial.println(" Degrees   \t");
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
