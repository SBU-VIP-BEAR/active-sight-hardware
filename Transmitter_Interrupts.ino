/* BME x FIT Software -- Transmitter
 * Developed by Steven Crimarco
 * Date: 1/22/2020
 * 
 * This iteration utilizes a hardware synchronization between the two devices using
 * interrupts. This is much more reliable that using a sensor as the trigger or timing with a RTC clock
 * because the latency is extremely low. Latency is a crucial detail here because any latency in
 * the system will cause a desynchronization between the receiver and transmitter's ultrasonic sensor.
 * 
 * Future development: 
 * -Check out system (basically remove interruptpin and recreate it with a new function to make state false)
 * -SD card data storage
 * -Velocity calculations
 */

#include <NewPing.h>  // Library used to control the ultrasonic sensor

bool state; //Trigger used to indicate that the interrupt has occurred

const int trig = 4;  // trig pin 
const int echo = 5; // echo pin
const int max_distance = 400; // Maximum distance (in cm)
int dist; 

NewPing sonar(trig, echo, max_distance); // NewPing setup of pins and maximum distance for ultrasonic sensor

void setup() {
  pinMode(3, INPUT_PULLUP); //Define pin 3(check-in) as an input with a pullup resistor
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(3), interrupt, FALLING); //When pin 3 is conencted to the receiver, the value "falls" to 0(GND) and the interrupt is triggered
  Serial.println("Startup Complete");
}

void loop() {
  while(state==true){ //Once the interrupt is triggered to sync the devices
    dist = sonar.ping_cm(); //Send out distance signal
    Serial.println(dist); 
    delay(29); //min delay of the ultrasonic sensors is 29ms
  }
}

void interrupt(){ //Function to ran once the interrupt is receieved
  state=true;
  detachInterrupt(digitalPinToInterrupt(3)); //Remove the interrupt pin to prevent latency issues
}

