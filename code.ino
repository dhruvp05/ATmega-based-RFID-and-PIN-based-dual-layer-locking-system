#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>


#define RST_PIN 9                                
#define SS_PIN 10                                
byte accessUID[4] = { 0xC1, 0xB5, 0xF5, 0x19 };  //Change the UID according to the tag
byte recordUID[4];
MFRC522 mfrc522(SS_PIN, RST_PIN);                //Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 16, 2);              //Create LCD instance
Servo sg90;                                      //Create servo instance
const int servo_pin = 3;                         //Servo's signal pin
const int buzzer_pin = 2;                        //Buzzer pin
const byte ROWS = 4;                             //four rows
const byte COLS = 4;                             //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A'},
  { '4', '5', '6', 'B'},
  { '7', '8', '9', 'C'},
  { '*', '0', '#', 'D'}
};
byte rowPins[ROWS] = { 4, 8, 7, 6 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, A1, A2, A0};  //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char customKey;  //Stores the character for pressed key

const int Password_Length = 5;  //4 character password
char Data[Password_Length];
const char Master[Password_Length] = "1A2B";
byte data_count = 0;

void setup() {
  Serial.begin(9600);                 // Initialize serial communications with the PC
  SPI.begin();                        // Init SPI bus
  mfrc522.PCD_Init();                 // Init MFRC522
  delay(4);                           // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan the card"));
  sg90.attach(servo_pin);
  sg90.write(180);
  lcd.backlight();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scan your card");
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // See if the data has been read successfully
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  //See if UID matches the saved UID C1 B5 F5 19
  if (mfrc522.uid.uidByte[0] == accessUID[0] && mfrc522.uid.uidByte[1] == accessUID[1] && mfrc522.uid.uidByte[2] == accessUID[2] && mfrc522.uid.uidByte[3] == accessUID[3]) {
    Serial.println("Card Matched");
    //Update the recordUID byte with UID scanned
    recordUID[0]=accessUID[0];
    recordUID[1]=accessUID[1];
    recordUID[2]=accessUID[2];
    recordUID[3]=accessUID[3];
    }
    else{
      //If UID doesn't match activate the buzzer of 2 seconds
      digitalWrite(buzzer_pin, HIGH);
      delay(2000);
      digitalWrite(buzzer_pin, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Card is not");
      lcd.setCursor(0,1);
      lcd.print("matched");
      delay(5000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Scan your card");
    }
    if(recordUID[0] == accessUID[0] && recordUID[1] == accessUID[1] && recordUID[2] == accessUID[2] && recordUID[3] == accessUID[3]){
      //If card is matched we need to enter the password
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter Password");
      lcd.setCursor(0,1);
      while(true){
        //Keep on continuously checking if there is a character is pressed
        customKey = customKeypad.getKey();
        if(customKey){
          //If a character is pressed save it in Data array and print a '*' on LCD
          lcd.print("*");
          Data[data_count] = customKey;
          data_count++;
        }
        if(data_count == Password_Length -1){
          //Break the loop when required no. of characters are pressed
          break;
        }
      }
      }
      
      if (data_count == Password_Length - 1) {
        if (!strcmp(Data, Master)) {
          //If Data array matches Master array unlock the servo for 10 seconds
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Password Matched");
          sg90.write(90);
          delay(10000);
          sg90.write(180);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Scan your card");
          }
        else {
          //If Data array doesn't match Master array activate the buzzer for two seconds
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Password");
          lcd.setCursor(0,1);
          lcd.print("not matched");
          digitalWrite(buzzer_pin, HIGH);
          delay(2000);
          digitalWrite(buzzer_pin, LOW);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Scan your card");
        }
 
    // Clear Data array and LCD display
    clearData();
  }

  mfrc522.PICC_HaltA();
}
  

void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
