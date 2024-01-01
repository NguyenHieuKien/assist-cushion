#define BLYNK_TEMPLATE_ID "TMPL6Nmo5lgbt"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "rL3JE2n1IVg-YymC0WBoRdEnCzkiT2aX"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include "Arduino.h"
#include <PCF8574.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define L_PWM D7
#define R_PWM D8
SoftwareSerial mySerial(D6, D5);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

PCF8574 pcf8574(0x20);
int lastBtn1State = HIGH;
int btn1State, btn2State, btn3State, btn4State, btn5State, btn6State;
int lastBtn2State = HIGH, lastBtn3State = HIGH, lastBtn4State = HIGH, lastBtn5State = HIGH, lastBtn6State = HIGH;
unsigned long lastsendData = 0, btn1PressTime = 0, btn2PressTime = 0, btn3PressTime = 0, btn4PressTime = 0, btn5PressTime = 0, btn6PressTime = 0;
const unsigned long longPressDuration = 100;
int nut1 = 0, nut2 = 0, nut3 = 0, nut4 = 0, nut5 = 0, nut6 = 0;
int value1, value2, value3, value4, value5, value6, speed, dir, IsOnOff;
BlynkTimer timer;
WiFiManager wifiManager;

String h, t, MQ135, Dust, chiSo, Water_value, dongColen = "0", dongCoxuong = "0";
int Water;
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V1) {
  // Set incoming value from pin V0 to a variable
  value1 = param.asInt();
}
BLYNK_WRITE(V2) {
  // Set incoming value from pin V0 to a variable
  value2 = param.asInt();
}
BLYNK_WRITE(V3) {
  // Set incoming value from pin V0 to a variable
  value3 = param.asInt();
}
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  value4 = param.asInt();
}
BLYNK_WRITE(V4) {
  // Set incoming value from pin V0 to a variable
  value5 = param.asInt();
}
BLYNK_WRITE(V5) {
  // Set incoming value from pin V0 to a variable
  value6 = param.asInt();
}
BLYNK_WRITE(V8) {
  // Set incoming value from pin V0 to a variable
  speed = param.asInt();
}
BLYNK_WRITE(V6) {
  // Set incoming value from pin V0 to a variable
  dir = param.asInt();
}
BLYNK_WRITE(V9) {
  // Set incoming value from pin V0 to a variable
  IsOnOff = param.asInt();
}

// This function is called every time the device is connected to the Blynk.Cloud


// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.)
  // Blynk.virtualWrite(V7, "Thời gian kết nối:" + String(millis() / 1000) + "\n");
}

void setup() {
  // Debug console
  Serial.begin(115200);
  mySerial.begin(4800);
  wifiManager.autoConnect("Goilat1", "88888888");

  Serial.println();
  //Wire.setClock(400000);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pcf8574.begin();


  // Register a callback for the beat detection
  timer.setInterval(5000L, myTimerEvent);
  Serial.print("Init pcf8574...");

  pcf8574.write(0, HIGH);
  pcf8574.write(1, HIGH);
  pcf8574.write(2, HIGH);
  pcf8574.write(3, HIGH);
  pcf8574.write(4, HIGH);
  pcf8574.write(5, HIGH);
}

void loop() {
  Blynk.run();
  timer.run();

  if (millis() - lastsendData > 2000) {
    receivedData();
    lastsendData = millis();
  }


  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  btn1State = value1;  // nhan tren 3s ==> doi mode // nhan duoi 3s doi che do xem man hinh
  btn2State = value2;
  btn3State = value3;
  btn4State = value4;
  btn5State = value5;
  btn6State = value6;

  if (btn1State != lastBtn1State) {
    if (btn1State == HIGH) {
      btn1PressTime = millis();
    } else {
      if (millis() - btn1PressTime > longPressDuration) {
        // Serial.println("111111111111");
        nut1 = 1;
      }
    }
  }
  if (btn2State != lastBtn2State) {
    if (btn2State == HIGH) {
      btn2PressTime = millis();
    } else {
      if (millis() - btn2PressTime > longPressDuration) {
        // Serial.println("222222222222");
        nut2 = 1;
      }
    }
  }
  if (btn3State != lastBtn3State) {
    if (btn3State == HIGH) {
      btn3PressTime = millis();
    } else {
      if (millis() - btn3PressTime > longPressDuration) {
        // Serial.println("333333333333");
        nut3 = 1;
      }
    }
  }
  if (btn4State != lastBtn4State) {
    if (btn4State == HIGH) {
      btn4PressTime = millis();
    } else {
      if (millis() - btn4PressTime > longPressDuration) {
        // Serial.println("444444444444");
        nut4 = 1;
      }
    }
  }
  if (btn5State != lastBtn5State) {
    if (btn5State == HIGH) {
      btn5PressTime = millis();
    } else {
      if (millis() - btn5PressTime > longPressDuration) {
        ///Serial.println("555555555555");
        nut5 = 1;
      }
    }
  }
  if (btn6State != lastBtn6State) {
    if (btn6State == HIGH) {
      btn6PressTime = millis();
    } else {
      if (millis() - btn6PressTime > longPressDuration) {
        // Serial.println("666666666666");
        nut6 = 1;
      }
    }
  }
  PCF8574Control();


  if (IsOnOff) {
    if (dir == 1) {
      motorControl(0, speed);
    } else {
      motorControl(speed, 0);
    }
  } else {
    motorControl(0, 0);
  }



  // Serial.print(nut1);
  // Serial.print("\t");
  // Serial.print(nut2);
  // Serial.print("\t");
  // Serial.print(nut3);
  // Serial.print("\t");
  // Serial.print(nut4);
  // Serial.print("\t");
  // Serial.print(nut5);
  // Serial.print("\t");
  // Serial.println(nut6);
  lastBtn1State = btn1State;
  lastBtn2State = btn2State;
  lastBtn3State = btn3State;
  lastBtn4State = btn4State;
  lastBtn5State = btn5State;
  lastBtn6State = btn6State;
  nut1 = 0;
  nut2 = 0;
  nut3 = 0;
  nut4 = 0;
  nut5 = 0;
  nut6 = 0;
}

void PCF8574Control() {

  if (nut1 == 1) {
    pcf8574.write(0, LOW);
    delay(200);
    pcf8574.write(0, HIGH);
  } else if (nut2 == 1) {
    pcf8574.write(1, LOW);
    delay(200);
    pcf8574.write(1, HIGH);
  } else if (nut3 == 1) {
    pcf8574.write(2, LOW);
    delay(200);
    pcf8574.write(2, HIGH);
  } else if (nut4 == 1) {
    pcf8574.write(3, LOW);
    delay(200);
    pcf8574.write(3, HIGH);
  } else if (nut5 == 1) {
    pcf8574.write(4, LOW);
    delay(200);
    pcf8574.write(4, HIGH);
  } else if (nut6 == 1) {
    pcf8574.write(5, LOW);
    delay(200);
    pcf8574.write(5, HIGH);
  } else {
  }
}




void motorControl(int len, int xuong) {
  analogWrite(L_PWM, len);
  analogWrite(R_PWM, xuong);
}

void receivedData() {
  String data[7];
  if (mySerial.available() > 0) {                          // Kiểm tra xem có dữ liệu đang được gửi đến không
    String receivedData = mySerial.readStringUntil('\n');  // Đọc chuỗi dữ liệu cho đến khi gặp ký tự '\n'

    // Tách dữ liệu bằng dấu phẩy

    int startIndex = 0;
    int commaIndex;

    for (int i = 0; i < 7; i++) {
      commaIndex = receivedData.indexOf(',', startIndex);
      if (commaIndex != -1) {
        data[i] = receivedData.substring(startIndex, commaIndex);
        startIndex = commaIndex + 1;
      } else {
        data[i] = receivedData.substring(startIndex);
        break;
      }
    }

    // // Xử lý dữ liệu nhận được
    for (int i = 0; i < 7; i++) {
      Serial.print("\tData " + String(i + 1) + ": ");
      Serial.print(data[i]);
    }
    Serial.println();
    h = data[0];
    t = data[1];
    MQ135 = data[2];
    Dust = data[3];
    Water = data[4].toInt();
    dongColen = data[5];
    dongCoxuong = data[6];
    chiSo = "NHIỆT ĐỘ:" + t + " ĐỘ ẨM:" + h + " KHÔNG KHÍ:" + MQ135 + " BỤI:" + Dust + " NƯỚC:" + Water_value + "\n";
    Blynk.virtualWrite(V7, chiSo);
  }


  if (Water > 500) {

    Water_value = "Có";
  } else {
    Water_value = "Không";
  }

  if (dongColen == "1") {
    motorControl(255, 0);
    delay(5000);
    motorControl(0, 0);
  } else {
  }
  if (dongCoxuong == "1") {
    motorControl(0, 255);
    delay(5000);
    motorControl(0, 0);
  } else {
  }
}
