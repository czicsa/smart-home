 
#define LED_PIN 3
 
#define FADE_SPEED 10     // make this higher to slow down

int actualValue;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  actualValue = 0;
}
 
 
void loop() {
  fadeTo(255);
  delay(1000);
  fadeTo(10);
  delay(1000);
  fadeTo(70);
  delay(1000);
  fadeTo(15);
  delay(1000);
  fadeTo(170);
  delay(1000);
  fadeTo(0);
  delay(1000);
} 

void fadeTo(int level) {
  while(actualValue != level){
      actualValue < level ? actualValue++ : actualValue--;
      analogWrite(LED_PIN, actualValue);
      delay(FADE_SPEED);
  }

}

