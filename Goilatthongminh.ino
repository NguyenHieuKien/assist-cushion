// #define L_PWM 4
// #define R_PWM 5
#define MQ135pin A1
#define voPin A2
#define btnlen 3
#define btnxuong 2
#define WaterSensor A3
#define buzzer 10
int ledPin = 9;
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "SharpGP2Y10.h"
#include "SoftwareSerial.h"

#define SIMRX 5
#define SIMTX 4
#define SIMBAUD 9600
unsigned long lastSendData = 0;
bool check = true, check1 = true, btnlenVal = true, btnxuongVal = true, dongColen = false, dongCoxuong = false;
SharpGP2Y10 dustSensor(voPin, ledPin);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
SoftwareSerial ESP_Serial(6, 7);
SoftwareSerial mysim(SIMRX, SIMTX);
float h, t, dustDensity;
int MQ135;
int WaterValue = 0;
char phoneNum[] = "+84327824568";  //change this

void setup() {
  // put your setup code here, to run once:
  pinMode(btnlen, INPUT_PULLUP);
  pinMode(btnxuong, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("SHT31 test");
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);

  if (!sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
  }


  ESP_Serial.begin(4800);

  mysim.begin(SIMBAUD);
  Serial.println("Initializing....");
  // initModule("AT", "OK", 1000);                 //Once the handshake test is successful, it will back to OK
  // initModule("ATE1", "OK", 1000);               //this command is used for enabling echo
  // initModule("AT+CPIN?", "READY", 1000);        //this command is used to check whether SIM card is inserted in GSM Module or not
  // initModule("AT+CMGF=1", "OK", 1000);          //Configuring TEXT mode
  // initModule("AT+CNMI=2,2,0,0,0", "OK", 1000);  //Decides how newly arrived SMS messages should be handled
  Serial.println("Initialized Successfully");
  sendSMS(phoneNum, "test");
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensor();
  btnlenVal = digitalRead(btnlen);
  btnxuongVal = digitalRead(btnxuong);
  if (btnlenVal == false) {
    dongColen = true;
  } else {
    dongColen = false;
  }
  if (btnxuongVal == false) {
    dongCoxuong = true;
  } else {
    dongCoxuong = false;
  }
  if (dustDensity > 10 || t > 45 || h > 80 || MQ135 > 800 || WaterValue > 500) {
    digitalWrite(buzzer, HIGH);
    if (check1 == true) {
      sendSMS(phoneNum, "Canh bao chi so moi truong!!");
      check1 = false;
    }

  } else {
    check1 = true;
    digitalWrite(buzzer, LOW);
  }


  if (millis() - lastSendData > 3000) {

    update_data();
    lastSendData = millis();
  }
}


// void motorControl(int len, int xuong) {
//   analogWrite(L_PWM, len);
//   analogWrite(R_PWM, xuong);
// }


void readSensor() {
  t = sht31.readTemperature();
  h = sht31.readHumidity();
  MQ135 = analogRead(MQ135pin);
  dustDensity = dustSensor.getDustDensity();
  WaterValue = analogRead(WaterSensor);

  // Serial.print("t:");
  // Serial.print(t);
  // Serial.print("\t h:");
  // Serial.print(h);
  // Serial.print("\t MQ135:");
  // Serial.print(MQ135);
  // Serial.print("\t dustDensity:");
  // Serial.print(dustDensity);
  // Serial.print("\t WaterValue:");
  // Serial.print(WaterValue);
  // Serial.println();
}




void update_data() {
  String data1[7] = { String(h), String(t), String(MQ135), String(dustDensity), String(WaterValue), String(dongColen), String(dongCoxuong) };

  String combinedData = "";  // Khởi tạo chuỗi kết quả

  // Kết hợp các dữ liệu vào chuỗi
  for (int i = 0; i < 7; i++) {
    combinedData += data1[i];
    if (i < 6) {
      combinedData += ",";  // Thêm dấu phẩy nếu không phải là phần tử cuối cùng
    }
  }
  combinedData += "\n";  // Thêm ký tự xuống dòng

  // Gửi chuỗi dữ liệu qua UART
  ESP_Serial.print(combinedData);
  Serial.print(combinedData);
  //delay(100);
  //nhan_dulieu();
}
void initModule(String cmd, char *res, int t) {
  while (1) {
    Serial.println(cmd);
    mysim.println(cmd);
    delay(100);
    while (mysim.available() > 0) {
      if (mysim.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}
void sendSMS(char *number, char *msg) {
  mysim.print("AT+CMGS=\"");
  mysim.print(number);
  mysim.println("\"\r\n");  // AT+CMGS=”Mobile Number” <ENTER> - Assigning recipient’s mobile number
  delay(500);
  mysim.println(msg);  // Message contents
  delay(500);
  mysim.write(byte(26));  // Ctrl+Z  send message command (26 in decimal).
  delay(3000);
}
