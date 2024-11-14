

//NOTES:
/* 
 *  1 wheel revolution at 25 speed = 1350ms
 *  1 wheel revolution = 22in or 55.88cm
 *  1ms = .0162963in or 0.0414cm
 *  
 *  41.4ms = 1in or 2.54 cm
 *  55.88cm = 1 wheel revolution
 *  55.88cm/1350ms = .0414cm/ms
 *  
 *  wheelbase diameter = 14in
 *  wheelbase circumpherance = 44in or 111.76cm
 *  1in or 2.54cm = .0326s or 32.6ms
 *  
 *  UWB Anchor Diameter = 
*/



#include <SoftwareSerial.h>

// SoftwareSerial RX and TX pins
const int RX_PIN = 13;
const int TX_PIN = -1;  // TX not used

// Set up the SoftwareSerial port
SoftwareSerial mySerial(RX_PIN, TX_PIN);

const int PIN_DIR1 = 2;
const int BRAKE = 3;
const int PIN_DIR2 = 4;    // Motor direction signal
const int PIN_PWM = 9;    // 490Hz pins 3, 9, 10, 11
const int DELAY = 20;     // Amount of time to delay between increments

// Variables
int _pwmCtrl = 0;         // PWM control signal 0-255
int _pwmInc = 1;          // Increment the PWM by this amount 
bool _dir = 0;            // Direction of the motor

float rightDistance = 0.0;
float leftDistance = 0.0;
float threshold = 8.0; // adjust this value based on your robot's accuracy and responsiveness
float noValue = NAN; // Using NaN to represent an 'invalid' state







// SETUP
//-----------------------------------------------------------------
void setup() 
{
    // Start the hardware serial communication
    Serial.begin(115200);
    
    // Start the software serial communication
    mySerial.begin(115200);
  
    // Set the RX pin as input
    pinMode(RX_PIN, INPUT);
  
    // Set DIR pin to output
      pinMode(PIN_DIR1, OUTPUT);
      pinMode(PIN_DIR2, OUTPUT);
      
    // Set PWM pin to output
    pinMode(PIN_PWM, OUTPUT);

    Serial.print("Right Distance: ");
    Serial.println(rightDistance);
    Serial.print("Left Distance: ");
    Serial.println(leftDistance);


    //Test the motors with a twist
    delay(1000);
    right(25, 1350);
    left(25, 1350);
}







// MAIN LOOP
//-----------------------------------------------------------------

void loop() 
{
  
  // Get UWB Data
  if (mySerial.available()) {
    processUWBData();
  }
  
  delay(100);
  
 Serial.println("UWB Data Read");


   // There are values for the the distances, maneuver the fetchbot
  if (!isnan(rightDistance) && !isnan(leftDistance) {
  
      // Calculate and set the difference in between right and left distances
      float difference = (abs(rightDistance - leftDistance));
      
      // Print the calculated difference between left and right distances
      Serial.println("Difference = ");
      Serial.println(difference);
  
      //Move FORWARD if difference is within threshold
      if (difference < threshold) {
  
        float timeToMove = rightDistance / .0414;
        
        // Move forward if the distances are approximately equal
        // Serial.print("Moving Forward ");
        // Serial.print(timeToMove);
  
        
        forward(25, timeToMove-200); // Adjust speed and time as necessary
  
        delay(500);
        clearSerialBuffer();
      } 
  
      
  
    //Turn LEFT if right distance is greater than left distance
      else if (rightDistance > leftDistance) {
        // Turn left if the right distance is greater
        left(25, 100); // Adjust speed and time as necessary
        clearSerialBuffer();
      } 
  
      
  
    //Turn RIGHT if left distance is greater than right distance
      else {
        // Turn right if the left distance is greater
        right(25, 100); // Adjust speed and time as necessary
        clearSerialBuffer();
      }
    }
  
    //Reset the distance values to have no value
    rightDistance = noValue;
    leftDistance = noValue;
    delay(250);

  }
}
    


// FUNCTIONS
//-----------------------------------------------------------------

// Read the incoming UWB data and set the distance values
void processUWBData() {
  // Read the incoming string until newline character
  String receivedData = mySerial.readStringUntil('\n');

  // Split the data into right and left distances
  int commaIndex = receivedData.indexOf(',');
  if (commaIndex != -1) {
  String rightDistanceStr = receivedData.substring(0, commaIndex);
  String leftDistanceStr = receivedData.substring(commaIndex + 1);

  // Convert the split strings into float values
  rightDistance = rightDistanceStr.toFloat();
  leftDistance = leftDistanceStr.toFloat();

  // Print the distances for debugging
  // Serial.print("Right Distance: ");
  // Serial.println(rightDistance);
  // Serial.print("Left Distance: ");
  // Serial.println(leftDistance);
  }
}



// Clear any extra data in the buffer
void clearSerialBuffer() {
  // Clear the serial buffer
  while(mySerial.available() > 0) {
    mySerial.read();
  }
}


// Move FORWARD
void forward(int speed,float time){
  
      // disengage brake
      digitalWrite(BRAKE, 0);
      
       // Set the direction
      digitalWrite(PIN_DIR1, 1);
      
       // Set the direction
      digitalWrite(PIN_DIR2, 0);
  
      // Set the PWM output
      analogWrite(PIN_PWM, speed);

      delay(time);
  
      //Brake
      analogWrite(BRAKE, 255);

      delay(500);

}



//Move BACKWARD
void backward(int speed,int time){

        // disengage brake
      digitalWrite(BRAKE, 0);
  
      // Set the direction
      digitalWrite(PIN_DIR1, 0);
      // Set the direction
      digitalWrite(PIN_DIR2, 1);

      // Set the PWM output
      analogWrite(PIN_PWM, speed);

      delay(time);
  
      // BRAKE
      analogWrite(BRAKE, 255);

      delay(500);
}



//Move RIGHT
void right(int speed,int time){

        // disengage brake
      digitalWrite(BRAKE, 0);
      
       // Set the direction
      digitalWrite(PIN_DIR1, 0);
      
       // Set the direction
      digitalWrite(PIN_DIR2, 0);
  
      // Set the PWM output
      analogWrite(PIN_PWM, speed);
  
      delay(time);
      
      //Brake
      analogWrite(BRAKE, 255);

      delay(500);
}



//Move LEFT
void left(int speed,int time){

      // disengage brake
      digitalWrite(BRAKE, 0);
      
       // Set the direction
      digitalWrite(PIN_DIR1, 1);
      
       // Set the direction
      digitalWrite(PIN_DIR2, 1);
  
      // Set the PWM output
      analogWrite(PIN_PWM, speed);
  
      delay(time);
      
      //Brake
      analogWrite(BRAKE, 255);

      delay(500);
}
