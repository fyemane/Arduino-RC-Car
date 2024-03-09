#include <IRremote.h> // include the IRremote library

// for motor outputs
#define MOTORS 2
#define LMOTOR_A 3
#define LMOTOR_B 4
#define RMOTOR_A 5
#define RMOTOR_B 6

#define ftrigPin 9
#define fechoPin 10
#define btrigPin 11
#define bechoPin 12
double backDistance, frontDistance;

// for IR receiver
const int RECV_PIN = 8; // define the IR receiver pin
IRrecv irrecv(RECV_PIN); // create a receiver object of the IRrecv class
decode_results results; // create a results object of the decode_results class
unsigned long key_value = 0; // variable to store the key value

/*
ethan remote - REMOTE FOR PROJECT
forward 0x9cb47
left 0xdcb47
right 0x3cb47
back 0x5cb47
stop 0xbcb47
on 0x16b47
off 0xe6b47

faniel remote
FORWARD = 0xff629d;
LEFT = 0xff22dd;
RIGHT = 0xffc23d;
BACKWARD = 0xffa857;
STOP = 0xff38c7;

tinkercad
forward - 0xfd8877
left - 0xfd28d7
right - 0xfd6897
back - 0xfd9867
pause - 0xfda857
on - 0xfd00ff
off - 0xfd807f

*/

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


void setup(){
  // for IR remote
  Serial.begin(9600); // begin serial communication with a baud rate of 9600
  irrecv.enableIRIn(); // enable the receiver
  irrecv.blink13(true); // enable blinking of the built-in LED when an IR signal is received
  
  pinMode(ftrigPin, OUTPUT);
  pinMode(fechoPin, INPUT);
  pinMode(btrigPin, OUTPUT);
  pinMode(bechoPin, INPUT);
  
}

void loop(){
  
  frontDistance = 0.01723 * readUltrasonicDistance(ftrigPin, fechoPin);
  backDistance = 0.01723 * readUltrasonicDistance(btrigPin, bechoPin);
	
  // go backward if front too close
  if (frontDistance < 5) {
      digitalWrite(LMOTOR_A, LOW);
      digitalWrite(LMOTOR_B, HIGH);
      digitalWrite(RMOTOR_A, LOW);
      digitalWrite(RMOTOR_B, HIGH);
  }
  
   // go forward if front too close
  if (backDistance < 5) {
      digitalWrite(LMOTOR_A, HIGH);
      digitalWrite(LMOTOR_B, LOW);
      digitalWrite(RMOTOR_A, HIGH);
      digitalWrite(RMOTOR_B, LOW);
  }
  
  if (irrecv.decode(&results)){ // decode the received signal and store it in results
    Serial.println(results.value, HEX);
    
    
    Serial.print("front ");
    Serial.println(frontDistance);
    Serial.print("back ");
    Serial.println(backDistance);
    

    // TURN ON
    if (results.value == 0x16b47) {
      digitalWrite(MOTORS, HIGH);
    }

    // TURN OFF
    if (results.value == 0xe6b47) {
      digitalWrite(MOTORS, LOW);
      digitalWrite(LMOTOR_A, LOW);
      digitalWrite(LMOTOR_B, LOW);
      digitalWrite(RMOTOR_A, LOW);
      digitalWrite(RMOTOR_B, LOW);
    }

    // stop
    if (results.value == 0xbcb47) {
      digitalWrite(LMOTOR_A, LOW);
      digitalWrite(LMOTOR_B, LOW);
      digitalWrite(RMOTOR_A, LOW);
      digitalWrite(RMOTOR_B, LOW);
    }

    // MOVE FORWARD
    if (results.value == 0x9cb47) {
      digitalWrite(LMOTOR_A, HIGH);
      digitalWrite(LMOTOR_B, LOW);
      digitalWrite(RMOTOR_A, HIGH);
      digitalWrite(RMOTOR_B, LOW);
    }

    // MOVE BACKWARD
    if (results.value == 0x5cb47) {
      digitalWrite(LMOTOR_A, LOW);
      digitalWrite(LMOTOR_B, HIGH);
      digitalWrite(RMOTOR_A, LOW);
      digitalWrite(RMOTOR_B, HIGH);
    }

    // MOVE LEFT
    if (results.value == 0xdcb47) {
      digitalWrite(LMOTOR_A, LOW);
      digitalWrite(LMOTOR_B, HIGH);
      digitalWrite(RMOTOR_A, HIGH);
      digitalWrite(RMOTOR_B, LOW);
    }

    // MOVE RIGHT
    if (results.value == 0x3cb47) {
      digitalWrite(LMOTOR_A, HIGH);
      digitalWrite(LMOTOR_B, LOW);
      digitalWrite(RMOTOR_A, LOW);
      digitalWrite(RMOTOR_B, HIGH);
    }
  
    irrecv.resume(); // reset the receiver for the next code
  }
}
