// This code configures an ESP32 device as an anchor in an Ultra-Wideband (UWB) system 
// using a RYUW122 module for distance measurement. The setup includes initializing serial 
// communication, resetting the hardware, and setting the UWB mode, network ID, device address 
// and encryption key. In the main loop, the anchor sends data to a specific tag, receives 
// the distance data, and prints the response to the serial monitor. The code uses a hardware 
// reset function and AT commands to configure the UWB module and manage communication.

#include <HardwareSerial.h>

HardwareSerial mySerial(2); // Use hardware serial port 1
#define NRST 23

void setup() {
  pinMode(NRST, OUTPUT);
  Serial.begin(115200);
  
  // Wait for serial port to connect. Necessary only for native USB
  while (!Serial) {
    ; 
  }
  
  // Set pins for RX and TX for mySerial (e.g., 16 for RX, 17 for TX)
  mySerial.begin(115200, SERIAL_8N1, 16, 17); // Set baud rate and pins for ESP32

  // Wait for everything to stabilize
  //delay(1000); 
  HW_RESET();
  delay(1000); 
  // Setting up as Anchor
  sendATCommand("AT+MODE=1\r\n"); // Set the mode to Anchor
  readResponse();
  delay(1000);
  
  sendATCommand("AT+NETWORKID=REYAX123\r\n"); // Set network ID
  readResponse();
  delay(1000);

  sendATCommand("AT+ADDRESS=REYAX003\r\n"); // Set device address
  readResponse();
  delay(1000);

  sendATCommand("AT+CPIN=FABC0002EEDCAA90FABC0002EEDCAA90\r\n"); // Set encryption key
  readResponse();
  delay(1000);
}

void loop() {
  // Transmit data to Tag and request distance
  sendATCommand("AT+ANCHOR_SEND=DAVID123,4,TEST\r\n");
  readResponse(); // Read the response which should include the distance
  delay(2000); // Delay between measurements, adjust as needed
}

void sendATCommand(String command) {
  mySerial.print(command); // Send the command to RYUW122
  delay(100);
}

void readResponse() {
  long startTime = millis();
  while (!mySerial.available() && millis() - startTime < 5000) { 
    // Wait for response for up to 5 seconds
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read()); // Echo the response to Serial monitor
  }
}

void HW_RESET(){
      digitalWrite(NRST, LOW );
      delay(500);
      digitalWrite(NRST, HIGH);
}
