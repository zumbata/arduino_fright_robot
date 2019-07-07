#include <RH_ASK.h>
#include <SPI.h>
#include <MFRC522.h>

#define speed_A_pin 5
#define dir_A_pin 6
#define speed_B_pin 3
#define dir_B_pin 4
#define RST_PIN 7
#define SS_PIN 53

int speed_A = 0;
int speed_B = 0;
int x = 512, y = 512;
bool sw = false;
RH_ASK driver;
MFRC522 mfrc522(SS_PIN, RST_PIN);
uint8_t buf[11];
uint8_t buflen = sizeof(buf);

int *f(String a) {
  String b = a.substring(0, a.indexOf('*'));
  int *sa;
  sa[0] = b.toInt();
  String c = b.substring(2, b.indexOf(' ', 2));
  sa[1] = c.toInt();
  String d = b.substring(b.lastIndexOf(' '));
  sa[2] = d.toInt();
  return sa;
}

void setup() {
  pinMode(speed_A_pin, OUTPUT);
  pinMode(dir_A_pin, OUTPUT);
  pinMode(speed_B_pin, OUTPUT);
  pinMode(dir_B_pin, OUTPUT);
  if (!driver.init())
    Serial.println("init failed");
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  while(1)
  {
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      continue;
    }
    else if ( ! mfrc522.PICC_ReadCardSerial())
    {
      continue;
    }
    else if (mfrc522.uid.uidByte[0] != 0x37 || 
     mfrc522.uid.uidByte[1] != 0x89 ||
     mfrc522.uid.uidByte[2] != 0x1f ||
     mfrc522.uid.uidByte[3] != 0x1b)
    {
      continue;
    }
    else
      break;
  }
}

void loop()
{
  if (driver.recv(buf, &buflen))
  {
    String str((char*)buf);
    Serial.println(str);
    int * a = f(str);
    sw = !((bool) a[0]);
    x = a[1];
    y = a[2];
    Serial.println(sw);
    Serial.println(x);
    Serial.println(y);

  }
  int x_cpy = x, y_cpy = y;
  y -= 512;
  x -= 512;
  
  if (y >= -20 && y <= 20)
  {
    speed_A = 0;
    speed_B = 0;
  }
  else if (y > 20)
  {
    y /= 2;
    speed_A = y;
    speed_B = y;
    digitalWrite(dir_A_pin, HIGH);
    digitalWrite(dir_B_pin, HIGH);
  }
  else
  {
    y = -y;
    y /= 2;
    speed_A = y;
    speed_B = y;
    digitalWrite(dir_A_pin, LOW);
    digitalWrite(dir_B_pin, LOW);
  }
  if (x > 20) {
    x /= 2;
    speed_A += x;
    speed_B -= x;
    if (speed_A > 255)
      speed_A = 255;
    if (speed_B < 0)
      speed_B = 0;
  }
  else if (x < -20) {
    x = -x;
    x /= 2;
    speed_A -= x;
    speed_B += x;
    if (speed_A < 0)
      speed_A = 0;
    if (speed_B > 255)
      speed_B = 255;
  }
  analogWrite(speed_A_pin, speed_A);
  analogWrite(speed_B_pin, speed_B);
  x = x_cpy;
  y = y_cpy;
}
