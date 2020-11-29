//Initializing LED Pin
int led_pin = 3;
int i = 0;
void setup() {
  Serial.begin(9600);
  //Declaring LED pin as output
  pinMode(led_pin, OUTPUT);

}

void loop() {
  //Fading the LED

  i = 10;
  analogWrite(led_pin, i);
  Serial.print(i);
  Serial.print("\n");
  delay(8000);
  i = 180;
  analogWrite(led_pin, i);
  Serial.print(i);
  Serial.print("\n");
  delay(8000);
  i = 255;
  analogWrite(led_pin, i);
  Serial.print(i);
  Serial.print("\n");
  delay(8000);
  /*
  for(i=0; i<255; i++){
    analogWrite(led_pin, i);
    Serial.print(i);
    Serial.print("\n");
    delay(20);
  }
  for(i=255; i>0; i--){
    analogWrite(led_pin, i);
    Serial.print(i);
    Serial.print("\n");
    delay(20);
  }
  */
}
