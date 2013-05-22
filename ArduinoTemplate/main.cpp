#include <Arduino.h>

int ledPin =  13;    // LED connected to digital pin 13

// The setup() method runs once, when the sketch starts
void setup()
{
  pinMode(ledPin, OUTPUT);
}

// the loop() method runs over and over again,
// as long as the Arduino has power
void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}

// just like any application, this is the entry point
// init is required to initialize the Arduino library
// we then just call setup(), followed loop() inside
// an infinite loop.
int main(void)
{
  init();
  setup();

  while(true) {
    loop();
  }
}
