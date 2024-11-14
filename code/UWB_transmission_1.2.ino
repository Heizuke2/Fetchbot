#include <HardwareSerial.h>

HardwareSerial mySerialRight(2); // Use hardware serial port 2 for Right
HardwareSerial mySerialLeft(1);  // Use hardware serial port 1 for Left

#define NRST 23
float distanceValueRight = 0.0; // Global variable to store the numeric distance for Right
float distanceValueLeft = 0.0;  // Global variable to store the numeric distance for Left
float threshold = 25.40; //Set this to be the distance between the UWB Sensors in centimeters;

String atCommand = "AT+ANCHOR_SEND=DAVID123,4,TEST\r\n";




// SETUP
//-----------------------------------------------------------------

void setup() {
  pinMode(NRST, OUTPUT);
  Serial.begin(115200);
  
  mySerialRight.begin(115200, SERIAL_8N1, 16, 17); // TX: 16
  mySerialLeft.begin(115200, SERIAL_8N1, 27, 17);  // TX: 27

  HW_RESET();
  delay(100);
}






// MAIN LOOP
//-----------------------------------------------------------------

void loop() {
  sendATCommand("AT+ANCHOR_SEND=DAVID123,4,TEST\r\n");

  // Read data from both serials and store the values
  String responseRight = readResponse(mySerialRight);
  String distanceStrRight = extractDistance(responseRight);

  

  // If there is a RIGHT distance value, set the value and convert it to a string
  if (distanceStrRight != "") {
    distanceValueRight = distanceStrRight.toFloat();
  }

  String responseLeft = readResponse(mySerialLeft);
  String distanceStrLeft = extractDistance(responseLeft);


  // If there is a LEFT distance value, set teh value and convert it to a string
  if (distanceStrLeft != "") {
    distanceValueLeft = distanceStrLeft.toFloat();
  }

  //Find and set difference between right and left distance readings
  float distanceDifference = fabs(distanceValueRight - distanceValueLeft);


  // If the strings aren't empty and the differences are within the set threshold, send the data to the fetchbot
  if (distanceStrRight != "" && distanceStrLeft != "" && distanceDifference <= threshold) { // Send data to fetchbot if distances are within threshold range

    // // Print Values for debugging
    // Serial.print("Right Distance: ");
    // Serial.println(distanceValueRight);
    // Serial.print("Left Distance: ");
    // Serial.println(distanceValueLeft);

    // Format and send the data as a string of right and left values separated by a ","
    String dataToSend = String(distanceValueRight) + "," + String(distanceValueLeft) + "\n";
    Serial.print(dataToSend); // Sending data over the ESP32's serial interface
  }
  // If differences are outside of threshold, print them to the serial monitor
  else {
    Serial.print("Difference Outside Threshhold: ");
    Serial.print(distanceDifference);
  }

  delay(100);
  
}







// FUNCTIONS
//-----------------------------------------------------------------

void sendATCommand(String command) {
  mySerialRight.print(command);
  mySerialLeft.print(command);
  delay(100);
}

String readResponse(HardwareSerial &serial) {
  long startTime = millis();
  String response = "";
  while (!serial.available() && millis() - startTime < 5000) {}
  while (serial.available()) {
    response += (char)serial.read();
  }
  return response;
}

String extractDistance(String resp) {
  int index = resp.indexOf("cm");
  if (index > 0) {
    int startIndex = resp.lastIndexOf(',', index) + 1;
    String distance = resp.substring(startIndex, index);
    distance.trim();
    return distance;
  }
  return "";
}

void HW_RESET() {
  digitalWrite(NRST, LOW);
  delay(500);
  digitalWrite(NRST, HIGH);
}
