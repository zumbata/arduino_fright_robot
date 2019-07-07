#define X_pin 0
#define Y_pin 1
#define SW_pin 2
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver;

void setup()
{
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
    Serial.begin(9600);
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
  int sw = digitalRead(SW_pin);
  int x = analogRead(X_pin);
  int y = analogRead(Y_pin);
  String str1(sw), str2(x), str3(y);
  String str = str1 + " " + str2 + " " + str3;
  for(int i = str.length(); i < 11; i++)
    str += "*";
  const char *msg = str.c_str();
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  Serial.println(msg);
}

