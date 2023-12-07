#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>
#include <string>

const char *ssid = "ESP32wifi";//SSID создаваемой сети
const char *password = "asd123asd";//Пароль от созданной сети
const int udpServerPort = 80;  // Порт получателя
const int motor1Dir = 4; //Порт скорости 1 мотора
const int motor2Dir = 5;//Порт скорости 2 мотора
const int motor3Dir = 18;//Порт скорости 3 мотора
const int motor4Dir = 19;//Порт скорости 4 мотора
const int Led_pin = 2;

const int freq = 490;
const int resolution = 8;

const int motor1Channel = 0;
const int motor2Channel = 2;
const int motor3Channel = 4;
const int motor4Channel = 6;

const int dir1m1 = 13;//1 пин направления 1 мотора
const int dir2m1 = 12;//2 пин направления 1 мотора
const int dir1m2 = 14;//1 пин направления 2 мотора
const int dir2m2 = 27;//2 пин направления 2 мотора
const int dir1m3 = 26;//1 пин направления 3 мотора
const int dir2m3 = 25;//2 пин направления 3 мотора
const int dir1m4 = 33;//1 пин направления 4 мотора
const int dir2m4 = 32;//2 пин направления 4 мотора

WiFiUDP udp;
String deviceName = "ESP32";

void setup() {
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
  ledcSetup(motor2Channel, freq, resolution);
  ledcSetup(motor3Channel, freq, resolution);
  ledcSetup(motor4Channel, freq, resolution);
  //
  ledcAttachPin(motor1Dir, motor1Channel);
  ledcAttachPin(motor2Dir, motor2Channel);
  ledcAttachPin(motor3Dir, motor3Channel);
  ledcAttachPin(motor4Dir, motor4Channel);
  //
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Serial.println("Server started");
  udp.begin(udpServerPort);  // Инициализация UDP для приема данных

  digitalWrite(Led_pin, HIGH);//Включение синего светодиода при успешном запуске всех систем
}

void loop() {
  // Прием данных по UDP
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
}