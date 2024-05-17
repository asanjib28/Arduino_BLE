/*
  Bluetooth® Low Energy Motor Control

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control two motors to drive a robotic cart and control its direction.

  The circuit:
  - Arduino Nano RP2040 Connect board
  - TA6586 Based Motor Driver Breakout Board
  ===================================
  | Arduino Nano  | TA6586 Breakout |
  | RP2040 connect| Board           |
  ===================================
  |       A0      |       D0        |
  |       A1      |       D1        |
  |       A2      |       D2        |
  |       A3      |       D3        |
  |       GND     |       GND       |
  |     VIN=9V    |     3V3=NC      |
  ===================================
        
  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

*/

#include <ArduinoBLE.h>

BLEService driveMotorService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy Motor Control Service

// Bluetooth® Low Energy motor drive direction Characteristic - custom 128-bit UUID, read and writable by central
BLECharCharacteristic drvDirectionCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// Declare the motor pins
const int motor1_p = A0;
const int motor1_m = A1;
const int motor2_p = A3;
const int motor2_m = A2;

// Declare functions

// Functions to control the direction of the motor
void motor_forward(){
  // Motor 1 drive
    analogWrite(motor1_p, 255);
    analogWrite(motor1_m, 0);
  // Motor 2 drive
    analogWrite(motor2_p, 255);
    analogWrite(motor2_m, 0);
  }
void motor_stop(){
  // Motor 1 drive
    analogWrite(motor1_p, 0);
    analogWrite(motor1_m, 0);
  // Motor 2 drive
    analogWrite(motor2_p, 0);
    analogWrite(motor2_m, 0);
  }
void motor_reverse(){
  // Motor 1 drive
    analogWrite(motor1_p, 0);
    analogWrite(motor1_m, 255);
  // Motor 2 drive
    analogWrite(motor2_p, 0);
    analogWrite(motor2_m, 255);
  }
void motor_left(){
  // Motor 1 drive
    analogWrite(motor1_p, 128);
    analogWrite(motor1_m, 0);
  // Motor 2 drive
    analogWrite(motor2_p, 255);
    analogWrite(motor2_m, 0);
  }
void motor_right(){
  // Motor 1 drive
    analogWrite(motor1_p, 255);
    analogWrite(motor1_m, 0);
  // Motor 2 drive
    analogWrite(motor2_p, 128);
    analogWrite(motor2_m, 0);
  }

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  // Declare the pins as outputs
  pinMode(motor1_p, OUTPUT);
  pinMode(motor1_m, OUTPUT);
  pinMode(motor2_p, OUTPUT);
  pinMode(motor2_m, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("BLEMotorDrive");
  BLE.setAdvertisedService(driveMotorService);

  // add the characteristic to the service
  driveMotorService.addCharacteristic(drvDirectionCharacteristic);

  // add service
  BLE.addService(driveMotorService);

  // set the initial value for the characeristic:
  drvDirectionCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE Motor Drive Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the motors:
      if (drvDirectionCharacteristic.written()) {
        switch (drvDirectionCharacteristic.value()) {   // any value other than 0
          
          case 'D': //ASCII HEX 0x44
            motor_forward();
            Serial.println("Moving forward...");
            break;
          case 'd': //ASCII HEX 0x64
            motor_forward();
            Serial.println("Moving forward...");
            break;  
          case 'B': //ASCII HEX 0x42
            motor_reverse();
            Serial.println("Moving backward...");
            break;
          case 'b': //ASCII HEX 0x62
            motor_reverse();
            Serial.println("Moving backward...");
            break;
          case 'S': //ASCII HEX 0x53
            motor_stop();
            Serial.println("Motors halted...");
            break;
          case 's': //ASCII HEX 0x73
            motor_stop();
            Serial.println("Motors halted...");
            break;
          case 'L': //ASCII HEX 0x4C
            motor_left();
            Serial.println("Turning left...");
            break;
          case 'l': //ASCII HEX 0x6C
            motor_left();
            Serial.println("Turning left...");
            break;
          case 'R': //ASCII HEX 0x52
            motor_right();
            Serial.println("Turning right...");
            break;
          case 'r': //ASCII HEX 0x72
            motor_right();
            Serial.println("Turning right...");
            break;
          default:
            motor_stop();
            Serial.println("Motors halted...");
            break;
                
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
