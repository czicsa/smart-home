// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

#define MY_BAUD_RATE 9600
#define MY_NODE_ID 3
#define CHILD_ID_TEMP 3
bool initialValueSent = false;

#include <MySensors.h>
#include <IRremote.h>

#define FADE_SPEED 5    // make this higher to slow down
#define IR_PIN 5        // input pin for IR sensor
#define LED_PIN 5       // pin number of the fet driver
#define LED_ON 1        // GPIO value to write to turn on attached relay
#define LED_OFF 0       // GPIO value to write to turn off attached relay

int actualValue; 				//actual value of the led strip output

#define MAX_VALUE 255
#define MIN_VALUE 0

MyMessage msg(CHILD_ID_TEMP, V_STATUS);
IRrecv irrecv(IR_PIN);
decode_results signals;


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
    
  irrecv.enableIRIn(); // enable input from IR receiver
}

void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Led strip", "1.0");

    // Register all sensors to gw (they will be created as child devices)
    present(CHILD_ID_TEMP, S_BINARY);
}
 
 
void loop() {
    if (irrecv.decode(&signals)) {
        Serial.println(signals.value, HEX);
        if(signals.value == 0xFFA857){
            fadeTo(actualValue + 50);
        }
        else if (signals.value == 0xFFE01F){
            fadeTo(actualValue - 50);
        }
        irrecv.resume(); // get the next signal
    }
} 

void receive(const MyMessage &message)
{
    // We only expect one type of message from controller. But we better check anyway.
    if (message.type==V_STATUS) {
        // Store state in eeprom
        saveState(message.sensor, message.getBool());
        // Write some debug info
        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", New status: ");
        Serial.println(message.getBool());
        
        // Change relay state
        fadeTo(message.getBool()?MAX_VALUE:MIN_VALUE);
    }
}

void fadeTo(int fadeToValue) {
  if(fadeToValue <= MIN_VALUE){
    send(msg.set(LED_OFF));
    fadeToValue = MIN_VALUE;
  }
  else if(fadeToValue > MAX_VALUE){
    fadeToValue = MAX_VALUE;
  }
  else {
    send(msg.set(LED_ON));
  }
  
  while(actualValue != fadeToValue){
      actualValue < fadeToValue ? actualValue++ : actualValue--;
      analogWrite(LED_PIN, actualValue);
      delay(FADE_SPEED);
  }

}

