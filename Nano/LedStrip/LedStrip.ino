// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

#define MY_BAUD_RATE 9600
#define MY_NODE_ID 3
#define CHILD_ID_TEMP 3
bool initialValueSent = false;

#include <MySensors.h>

#define FADE_SPEED 5     // make this higher to slow down
#define LED_PIN 3         // pin number of the fet driver
#define LED_ON 1          // GPIO value to write to turn on attached relay
#define LED_OFF 0         // GPIO value to write to turn off attached relay

MyMessage msg(CHILD_ID_TEMP, V_STATUS);

int actualValue;

void before()
{
}

void setup() {
    
  // set led driver pin to output mode
  pinMode(LED_PIN, OUTPUT);
  // Set relay to last known state (using eeprom storage)
  digitalWrite(LED_PIN, LED_OFF);
  actualValue = 0;
  
  Serial.println("Sending initial value");
  send(msg.set(LED_OFF));
  Serial.println("Requesting initial value from controller");
  request(CHILD_ID_TEMP, V_STATUS);
  wait(2000, C_SET, V_STATUS);
}

void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Led strip", "1.0");

    // Register all sensors to gw (they will be created as child devices)
    present(CHILD_ID_TEMP, S_BINARY);
}
 
 
void loop() {
} 

void receive(const MyMessage &message)
{
    // We only expect one type of message from controller. But we better check anyway.
    if (message.type==V_STATUS) {
         // Change relay state
        message.getBool()?fadeTo(255):fadeTo(0);
        // Store state in eeprom
        saveState(message.sensor, message.getBool());
        // Write some debug info
        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", New status: ");
        Serial.println(message.getBool());
        send(msg.set(message.getBool()?LED_ON:LED_OFF));
    }
}

void fadeTo(int level) {
  while(actualValue != level){
      actualValue < level ? actualValue++ : actualValue--;
      analogWrite(LED_PIN, actualValue);
      delay(FADE_SPEED);
  }

}

