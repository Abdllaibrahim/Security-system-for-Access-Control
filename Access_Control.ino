#include <LiquidCrystal_I2C.h>  // lcd library 
#include <Keypad.h> // keypad library 
#include <Wire.h>    // I2C communication library
#include <SPI.h>
#include <MFRC522.h>
#include<SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>




SoftwareSerial GSM_Serial(5,6); 
SoftwareSerial Finger_Print_Serial(7, 8);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Finger_Print_Serial);

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

LiquidCrystal_I2C Lcd(0x27, 16, 2);  
const byte rows = 4; 
const byte cols = 4; 
char keys[rows][cols] = {
  {'1', '2', '3' , 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[rows] = {0,1,2,3}; 
byte colPins[cols] = {4,A3,A2,A1}; 
Keypad keypad ( makeKeymap(keys), rowPins, colPins, rows, cols );
const String password = "55555";   // change your password here
String input_password;
int count = 0;
byte relay=A0, wrong_attempt=0;     
byte x=0;

void setup() {
  serialData.begin(9600);
  GSM_Serial.begin(9600);
  Finger_Print_Serial.begin(57600);
  SPI.begin();      // Initiate  SPI bus
  rfid.PCD_Init();  // Init MFRC522 
  input_password.reserve(8); 
  Lcd.init();
  Lcd.backlight();
  Lcd.print("Enter Password");
  Lcd.setCursor(1, 0);
  pinMode(relay,1);
  digitalWrite(relay,0);
}


void loop() { 
  Finger_Print_Serial.listen();
 
   if(x==0){keypad1();  RFID(); FingerPrint();}
  else if(x==1){ keypad1();  RFID();  }
  else if(x==2) { FingerPrint(); }
  
  if(wrong_attempt==3){               //#--------------------------------
        Lcd.clear();
        Lcd.print("calling admin");
        GSM_Serial.listen();
        GSM_Serial.println("ATD+201145239935;");
        wrong_attempt=0;
        delay(5000);
        Lcd.clear();
        Lcd.print("Enter Password");
         
        }
  }
 

  void keypad1(){
  char key = keypad.getKey();
  if (key)
 { x=1;
    
    if (key == '*') 
    {
      if (count > 0)
       { 
         count--;
        input_password.remove(count);
        Lcd.setCursor(count, 1);
        Lcd.print (" ");
} }

 else if (key == '#') 
    {   
      if (password == input_password) 
      {   x=0;wrong_attempt=0;
          digitalWrite(relay,1);
          Lcd.clear();
          Lcd.print("pass is correct");
          delay(2000);
          Lcd.clear();
          digitalWrite(relay,0);
          Lcd.print("Enter Password");
          Lcd.setCursor(0, 1);
      } 
      else  {  x=0; wrong_attempt++;
              Lcd.clear();
              Lcd.print("Wrong Password");
              delay(2000);
              Lcd.clear();
              Lcd.print("Enter Password");
              Lcd.setCursor(0, 1); 
        }
      input_password = ""; 
      count = 0;
    }
 else 
    {
      input_password += key; 
      Lcd.setCursor(count, 1);
      Lcd.print(key);
      count++;

    }

  } // end of if(key)
} //end of void keypad1

void RFID()
{
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) { x=1;
  String UID="";
  for (byte i=0;i<rfid.uid.size;i++) 
  {
     UID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ")); // 0E c2 12 
     UID.concat(String(rfid.uid.uidByte[i], HEX));
  }
  
  UID.toUpperCase();  // E8 07 C7 D2
 
  if (UID.substring(1) == "62 0B 94 1F") //change here the UID of the card/cards that you want to give access
  {      x=0;wrong_attempt=0;
          digitalWrite(relay,1);
          Lcd.clear();
          Lcd.print("ST Smart");
          delay(2000);
          Lcd.clear();
          digitalWrite(relay,0);
          Lcd.print("Enter Password");
          Lcd.setCursor(0, 1);
          
  }
  
  else
          {   
             x=0; wrong_attempt++;
              Lcd.clear();
              Lcd.print("wrong ID card");
              delay(2000);
              Lcd.clear();
              Lcd.print("Enter Password");
              Lcd.setCursor(0, 1);
}
  
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  }}//end of void rfid1

  
void FingerPrint() { 
  uint8_t p = finger.getImage();
  switch (p) {
     case FINGERPRINT_OK:
          x=2;
          break;
     default:
      return p;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK: 
      break;
    default:
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {x=0;wrong_attempt=0;
          digitalWrite(relay,1);
          Lcd.clear();
          Lcd.print("Access");
          delay(2000);
          Lcd.clear();
          digitalWrite(relay,0);
          Lcd.print("Enter Password");
          Lcd.setCursor(0, 1);
          
  } else if (p == FINGERPRINT_NOTFOUND) {
         wrong_attempt++; x=0;
                Lcd.clear();
                Lcd.print("Try Again");
                delay(2000);
                Lcd.clear();
                Lcd.print("Enter Password");
                Lcd.setCursor(0, 1);

}}
