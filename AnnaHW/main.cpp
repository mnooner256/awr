//Testing
#include <Arduino.h>

int ledPin =  13;


void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);
  delay(5000);
}

int main(void)
{
  init();
  setup();

  while(true) {
    loop();
  }
}
