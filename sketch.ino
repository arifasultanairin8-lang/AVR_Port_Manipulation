void setup() {
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
  DDRB &= ~(1 << DDB4); 

  PORTB |= (1 << PORTB4);

  Serial.begin(9600);
}

void loop() {
  static uint8_t counter = 0;          
  static bool lastButtonState = HIGH;   

  bool currentButtonState = (PINB & (1 << PINB4));

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50); 

    counter = (counter + 1) % 16; 

    PORTB = (PORTB & 0xF0) | (counter & 0x0F);

    Serial.print("Counter: ");
    Serial.println(counter);
  }

  lastButtonState = currentButtonState;
}
