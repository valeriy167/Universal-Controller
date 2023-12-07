/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>
#include <string>
//#include <cstdlib>

//#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "ESP32wifi";
const char *password = "asd123asd";
const int udpServerPort = 80;  // Порт получателя
const int motor1Dir = 4; //
const int motor2Dir = 5;
const int motor3Dir = 18;
const int motor4Dir = 19;
const int Led_pin = 2;

// const int motorPin3A = 25;
// const int motorPin3B = 26;
// const int motorPin4A = 32;
// const int motorPin4B = 33;


//Here 2 more pins for 2 motor

const int freq = 490;
const int resolution = 8;

const int motor1Channel = 0;
//const int motor1Channel2 = 1;
const int motor2Channel = 2;
//const int motor2Channel2 = 3;
const int motor3Channel = 4;
//const int motor3Channel2 = 5;
const int motor4Channel = 6;
//const int motor4Channel2 = 7;

const int dir1m1 = 13;//25
const int dir2m1 = 12;//33
const int dir1m2 = 14;//32
const int dir2m2 = 27;//13
const int dir1m3 = 26;//12
const int dir2m3 = 25;//14
const int dir1m4 = 33;//27
const int dir2m4 = 32;//26
// connect motors

//const int IA = 5;  //  pin 5 connected to pin IA
//const int IB = 6;  //  pin 6 connected to pin IB
//byte speed = 255;  // change this (0-255) to control the speed of the motor

// WiFiServer server(80);
WiFiUDP udp;
String deviceName = "ESP32";

void setup() {
  //pinMode(IA, OUTPUT); // set pins to output
  //pinMode(IB, OUTPUT);
  pinMode(Led_pin, OUTPUT);
  pinMode(dir1m1, OUTPUT);
  pinMode(dir2m1, OUTPUT);
  pinMode(dir1m2, OUTPUT);
  pinMode(dir2m2, OUTPUT);
  //
  pinMode(dir1m3, OUTPUT);
  pinMode(dir2m3, OUTPUT);
  pinMode(dir1m4, OUTPUT);
  pinMode(dir2m4, OUTPUT);
  //
  ledcSetup(motor1Channel, freq, resolution);
  //ledcSetup(motor1Channel2, freq, resolution);
  ledcSetup(motor2Channel, freq, resolution);
  //ledcSetup(motor2Channel2, freq, resolution);
  ledcSetup(motor3Channel, freq, resolution);
  //ledcSetup(motor3Channel2, freq, resolution);
  ledcSetup(motor4Channel, freq, resolution);
  //ledcSetup(motor4Channel2, freq, resolution);
  ledcAttachPin(motor1Dir, motor1Channel);
  ledcAttachPin(motor2Dir, motor2Channel);
  ledcAttachPin(motor3Dir, motor3Channel);
  ledcAttachPin(motor4Dir, motor4Channel);
  // ledcAttachPin(motorPin3A, motor3Channel1);
  // ledcAttachPin(motorPin3B, motor3Channel2);
  // ledcAttachPin(motorPin4A, motor4Channel1);
  // ledcAttachPin(motorPin4B, motor4Channel2);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1)
      ;
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  // server.begin();

  Serial.println("Server started");
  udp.begin(udpServerPort);  // Инициализация UDP для приема данных

  digitalWrite(Led_pin, HIGH);
}

void loop() {
  // WiFiClient client = server.available();   // listen for incoming clients

  // if (client) {                             // if you get a client,
  // Serial.println("New Client.");           // print a message out the serial port
  // String currentLine = "";                // make a String to hold incoming data from the client


  // Прием данных по UDP
  //int offsetX=525; //When the joystick is in the middle of the analog input value, different joysticks may have different deviations.

  int packetSize = udp.parsePacket();
  if (packetSize) {
    char buf[100];
    int len = udp.read(buf, 100);

  char* token;
  char strings[4][10]; //up to four strings to receive the parsed data (each 9 characters max)


  byte index = 0;  // index to index the strings array
  token = strtok(buf, " ");  // get the first part
  while (token != NULL )
  {
    strcpy(strings[index], token);  //safer: use strncpy() instead
    Serial.println( strings[index] ); //print each substring
    index++;                 // increment the array position
    token = strtok(NULL, " ");  // get the next part
  }


  {
      int num = atoi(strings[0]);
      int pwm = map(abs(num), 0, 1000, 0, 255);
      Serial.printf("Recieved pwm 1: %d\n", pwm);
      if (num > 0) {
        ledcWrite(motor1Channel, pwm);
        digitalWrite(dir1m1, HIGH);
        digitalWrite(dir2m1, LOW);
      } else {
        ledcWrite(motor1Channel, pwm);
        digitalWrite(dir1m1, LOW);
        digitalWrite(dir2m1, HIGH);
      }
  }

  {
      int num = atoi(strings[1]);
      int pwm = map(abs(num), 0, 1000, 0, 255);
      Serial.printf("Recieved pwm 2: %d\n", pwm);
      if (num > 0) {
        ledcWrite(motor2Channel, pwm);
        digitalWrite(dir1m2, HIGH);
        digitalWrite(dir2m2, LOW);
      } else {
        ledcWrite(motor2Channel, pwm);
        digitalWrite(dir1m2, LOW);
        digitalWrite(dir2m2, HIGH);
      }
  }

  {
      int num = atoi(strings[2]);
      int pwm = map(abs(num), 0, 1000, 0, 255);
      Serial.printf("Recieved pwm 1: %d\n", pwm);
      if (num > 0) {
        ledcWrite(motor3Channel, pwm);
        digitalWrite(dir1m3, HIGH);
        digitalWrite(dir2m3, LOW);
      } else {
        ledcWrite(motor3Channel, pwm);
        digitalWrite(dir1m3, LOW);
        digitalWrite(dir2m3, HIGH);
      }
  }

    {
      int num = atoi(strings[3]);
      int pwm = map(abs(num), 0, 1000, 0, 255);
      Serial.printf("Recieved pwm 1: %d\n", pwm);
      if (num > 0) {
        ledcWrite(motor4Channel, pwm);
        digitalWrite(dir1m4, HIGH);
        digitalWrite(dir2m4, LOW);
      } else {
        ledcWrite(motor4Channel, pwm);
        digitalWrite(dir1m4, LOW);
        digitalWrite(dir2m4, HIGH);
      }
  }
  
  }
  // } else {
  //   Serial.printf("Wi-Fi отключен");
  // }
  // }
  // // close the connection:
  // client.stop();
  // Serial.println("Client Disconnected.");
  // }
}