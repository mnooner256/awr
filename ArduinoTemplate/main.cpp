#include <Arduino.h>

int ledPin =  13;


void setup()
{
  Serial.begin(115200);
}

void loop()
{
  Serial.println("Hello, World!");
  delay(1000);
}

int main(void)
{
  init();
  setup();

  while(true) {
    loop();
  }
}
