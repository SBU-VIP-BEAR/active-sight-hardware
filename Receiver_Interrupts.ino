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
 
#include <NewPing.h>

bool state1;

const int trig = 4;  // trig pin
const int echo = 5; // echo pin
const int max_distance = 400; // Maximum distance (in cm)
int dist;
int store[7];
int index;
int average;

NewPing sonar(trig, echo, max_distance);

void setup() {
  pinMode(3,INPUT_PULLUP);
  pinMode(9, OUTPUT); 
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(3), fall, FALLING);
  Serial.println("Startup Complete");
}

void loop() {
  while(state1==true){ //Once the interupt has activated
    dist = sonar.ping_cm(); //Send out distance signal/receive signal

    /*To ensure that there isn't a sudden spike in vibration just because of one incorrect
    * datapoint, we take a running average of the distance values. This is fine because with 
    * such a small delay of 29ms, it takes a brief amount of time to make a drastic change to the 
    * average value, but a single spiked value will not change the average greatly
    * 
    * Note, for future velocity calculations you will probably still want to store all data
    */
    store[index]=dist; //Store the curent value
    index++; //Index to the next array spot
    if(index==7){index=0;} //We only want 7 values stored, if we're at index 7 move back to index 0
    for(int i=0;i<7;i++){
      average+=store[i]; //Sum up the 7 stored values
    }
    average=average/7; //Average the values
    Serial.println(average);

    //Map the distance to the proper range for the buzzer
    map(average,0,200,255,45);
    analogWrite(9,average);
    delay(29);
    average=0;
  }
}

void fall(){ //function to run when the interrupt is received
  state1=true;
  detachInterrupt(digitalPinToInterrupt(3));
}

