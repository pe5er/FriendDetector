/* 
Friend Detector by Ricardo Oliveira, forked by Skickar 9/30/2018
Forked again by pe5er 17/11/2018 (dd mm yyyy because uk)
*/

#include "./esppl_functions.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LIST_SIZE 5

uint8_t friendmac[LIST_SIZE][ESPPL_MAC_LEN] = {
   {0x40, 0x88, 0x05, 0x51, 0x4d, 0xe6}
  ,{0x28, 0xed, 0x6a, 0x4f, 0xd3, 0xcc}
  ,{0xd0, 0x04, 0x01, 0x42, 0x30, 0xfa}
  ,{0x04, 0xd6, 0xaa, 0x32, 0x7e, 0x2d}
  ,{0x00, 0x34, 0xda, 0x85, 0xec, 0xe6}  
  };

String friendname[LIST_SIZE] = {
   "PeterPhone"
  ,"BrittanyPhone"
  ,"JacobPhone"
  ,"JoePhone"
  ,"StuartPhone"
  };

LiquidCrystal_I2C lcd(0x27, 16, 4);

int peterhome = 0;
int brittanyhome = 0;
int jacobhome = 0;
int joehome = 0;
int stuarthome = 0;

int petercountdown = 0;
int brittanycountdown = 0;
int jacobcountdown = 0;
int joecountdown = 0;
int stuartcountdown = 0;

int displayupdate = 0;
int peoplecounter = 0;

void setup() { 
  delay(500);
  pinMode(D4, OUTPUT); // sets the pins to output mode
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  Serial.begin(115200);
  esppl_init(cb);

  Wire.begin(D2, D1);
  lcd.clear();
  lcd.begin();
  lcd.home();
  lcd.print(" Who's at Home?");
}

void pin4() {
digitalWrite(D4, HIGH); 
peterhome = 1;
}
void pin5() {
digitalWrite(D5, HIGH); 
brittanyhome = 1;
}
void pin6() {
digitalWrite(D6, HIGH); 
jacobhome = 1;
}
void pin7() {
digitalWrite(D7, HIGH); 
joehome = 1;
}
void pin8() {
digitalWrite(D8, HIGH); 
stuarthome = 1;
}

void updatelcd() {

  lcd.setCursor(0,2);
  peoplecounter = 0;
  if (peterhome == 1){ peoplecounter++; }
  if (brittanyhome == 1){ peoplecounter++; }
  if (jacobhome == 1){ peoplecounter++; }
  if (joehome == 1){ peoplecounter++; }
  if (stuarthome == 1){ peoplecounter++; }

  if (peoplecounter == 0){ lcd.print("No-one"); 
  } else if (peoplecounter == 5) { lcd.print("Everyone"); 
  } else if (peoplecounter == 1) { lcd.print("1 person"); 
  } else { 
  lcd.print(peoplecounter);
  lcd.print(" people");
  }
}

bool maccmp(uint8_t *mac1, uint8_t *mac2) {
  for (int i=0; i < ESPPL_MAC_LEN; i++) {
    if (mac1[i] != mac2[i]) {
      return false;
    }
  }
  return true;
}

void cb(esppl_frame_info *info) {  
  for (int i=0; i<LIST_SIZE; i++) {
    if (maccmp(info->sourceaddr, friendmac[i]) || maccmp(info->receiveraddr, friendmac[i])) {

        if (displayupdate > 0) {          
        displayupdate--; 
        } else { 
        updatelcd(); 
        displayupdate = 200;
        }
      
      Serial.printf("\n%s", friendname[i].c_str());
      if (i == 0){        //peter
          pin4();
          petercountdown = 10000;
      } else if (i == 1){ //brittany
          pin5();
          brittanycountdown = 10000;
      } else if (i == 2){ //jacob
          pin6();
          jacobcountdown = 10000;
      } else if (i == 3){ //joe
          pin7();
          joecountdown = 10000;
      } else if (i == 4){ //stuart
          pin8();
          stuartcountdown = 2000;
      }

    } else { // this is for if the packet does not match any we are tracking

        if (displayupdate > 0) {          
        displayupdate--; 
        } else { 
        updatelcd(); 
        displayupdate = 200;
        }

      if (petercountdown > 0) {          
        petercountdown--; 
      } else { 
        digitalWrite(D4, LOW); 
        peterhome = 0;
      }

      if (brittanycountdown > 0) {          
        brittanycountdown--; 
      } else { 
        digitalWrite(D5, LOW); 
        brittanyhome = 0;
      }

      if (jacobcountdown > 0) {          
        jacobcountdown--; 
      } else {
        digitalWrite(D6, LOW); 
        jacobhome = 0;
      }

      if (joecountdown > 0) {          
        joecountdown--; 
      } else { 
        digitalWrite(D7, LOW); 
        joehome = 0;
      }

      if (stuartcountdown > 0) {          
        stuartcountdown--; 
      } else { 
        digitalWrite(D8, LOW); 
        stuarthome = 0;
      }

    } //if
  } //for  
} //void


void loop() { // I didn't write this part but it sure looks fancy.
  esppl_sniffing_start();
  while (true) {
    for (int i = ESPPL_CHANNEL_MIN; i <= ESPPL_CHANNEL_MAX; i++ ) {
      esppl_set_channel(i);
      while (esppl_process_frames()) {
        //
      }
    }
  }  
}
