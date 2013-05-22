void ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(30);
}

int main(void) {
  init();
  setup();
  
  while(true) {
    loop();
  }
}
