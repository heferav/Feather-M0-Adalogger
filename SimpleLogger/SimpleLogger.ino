/*
  Simple Logger using internal RTC for Arduino Zero

 Created by:  Jonathan Davies
 Date:        21 Dec 2015
 Version:     0.3
 
 Modified by:  Andre Hefer
 Date:        15 Aug 2016
 Version:     0.3a
 Test to see of sketch works with Adafruit Feather / Adalogger featherwing
 Added external LED flasher on pin 12
 changed date/time to YYYY/MM/DD hh:mm:ss format
*/


#include <SPI.h>
#include <SD.h>
#include "RTClib.h"  // using adalogger RTC
#include <RTCZero.h> // using M0 onboard RTC


#define cardSelect 10  // Set (Adalogger) pin [Changed from 4]
#define VBATPIN A7    // Battery Voltage on Pin A7

/* Not required for Feather M0 + adalogger
#ifdef ARDUINO_SAMD_ZERO
   #define Serial SerialUSB
#endif
********************/

File logfile;   // Create file object
RTC_PCF8523 rtcWNG; // Create RTC object on the adalogger featherwing
RTCZero rtcM0;    // Create RTC object for Feather M0 onboard RTC

/*
// Change these values to set the current initial time
const byte hours = 16;
const byte minutes = 23;
const byte seconds = 0;
// Change these values to set the current initial date
const byte day = 15;
const byte month = 8;
const byte year = 16;
*/

//////////////    Setup   ///////////////////
void setup() {

  rtcM0.begin();    // Start the RTC
  //rtcM0.setTime(hours, minutes, seconds);   // Set the time
  //rtcM0.setDate(day, month, year);    // Set the date
  
  /* If you want output for debugging and the Feather to wait for you
  to open a serial port uncomment the the lines below.
  */
   while (! Serial); // Wait until Serial is ready
   Serial.begin(115200);
   Serial.println("\r\nAnalog logger test");
  
  pinMode(13, OUTPUT);


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);     // Two red flashes means no card or card init failed.
  }
  char filename[15];
  strcpy(filename, "ANALOG00.CSV");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  pinMode(13, OUTPUT);
  //pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.println("Ready!");

   //******************* START RTC_PCF8523 INITIAL SETUP *********************
  if (! rtcWNG.begin()) {
    Serial.println("Couldn't find RTC (Adalogger)");
    while (1);
  }
  //******************* END RTC_PCF8523 INITIAL SETUP ***********************

  Serial.println("Before syncing to featherwing RTC: ");
  show_M0_rtc_date_and_time();
  show_adalogger_rtc_date_and_time();
  //Read featherwing RTC and set onboard RTC to match
  DateTime now = rtcWNG.now(); // Read Adalogger RTC and use it to set featherwing M0 RTC
  rtcM0.setTime(now.hour(), now.minute(), now.second());
  rtcM0.setDate(now.day(), now.month(), now.year() - 2000);

  Serial.println("After syncing to featherwing RTC: ");
  show_M0_rtc_date_and_time();
  show_adalogger_rtc_date_and_time();
}

uint8_t i=0;

///////////////   Loop    //////////////////
void loop() {
  //digitalWrite(8, HIGH);  // Turn the green LED on
  digitalWrite(12, HIGH);  // Rigged up external LED on pin12 / no internal led
  
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  
    
  // Print data and RTC (M0) time followed by battery voltage to SD card
  logfile.print("20"); //YYYY year format
  logfile.print(rtcM0.getYear());
  logfile.print("/");
  print2digits_F(rtcM0.getMonth());
  logfile.print("/");
  print2digits_F(rtcM0.getDay());
  logfile.print("\t");
  print2digits_F(rtcM0.getHours());
  logfile.print(":");
  print2digits_F(rtcM0.getMinutes());
  logfile.print(":");
  print2digits_F(rtcM0.getSeconds());
  logfile.print(", ");
  logfile.println(measuredvbat);   // Print battery voltage
  logfile.flush();
  
  //digitalWrite(8, LOW);   // Turn the green LED off
  digitalWrite(12, LOW);   // Turn the green LED off
  
  delay(10000);   // Simple 10 second delay
}

///////////////   Functions   //////////////////

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

//Padding date/time fields two two digits -- to logfile
void print2digits_F(int number) {
  if (number >= 0 && number < 10) {
    logfile.print('0');
  }
  logfile.print(number);
}

//Print onboard RTC date/time to terminal
void show_M0_rtc_date_and_time(){   
    Serial.print("Feather M0 onboard RTC : 20");
    Serial.print(rtcM0.getYear());
    Serial.print("/");
    print2digits(rtcM0.getMonth());
    Serial.print("/");
    print2digits(rtcM0.getDay());
    Serial.print("\t");
    print2digits(rtcM0.getHours());
    Serial.print(":");
    print2digits(rtcM0.getMinutes());
    Serial.print(":");
    print2digits(rtcM0.getSeconds());
    Serial.println();   
}

void show_adalogger_rtc_date_and_time(){   
    DateTime now = rtcWNG.now();
    Serial.print("Adalogger RTC (PCF8523): ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    print2digits(now.month());
    Serial.print('/');
    print2digits(now.day());
    Serial.print("\t");
    print2digits(now.hour());
    Serial.print(':');
    print2digits(now.minute());
    Serial.print(':');
    print2digits(now.second());
    Serial.println();  
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
