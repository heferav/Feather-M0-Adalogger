/*
  Simple RTC Alarm for Adafruit Feather M0 modified
  with repeated serial output each time the board wakes
  By: CaveMoa
  Date: 30/12/15
*/

//Arduino ZERO needs this. Featherwing send SERIAL to USB by default
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

#include <RTCZero.h>


/* Create an rtc object */
RTCZero rtcM0;
int AlarmTime;

void setup()
{
  rtcM0.begin();
  //Serial.begin(9600);
  //while (! Serial); // Wait until Serial is ready
  //Serial.println("Ready!");
  //show_M0_rtc_date_and_time();
  //Serial.end();
}

void loop()
{
  pinMode(12, INPUT_PULLUP); // turns the LED on   
  delay(1000); 
  pinMode(12, INPUT); // turns the LED off
  AlarmTime = rtcM0.getSeconds()+5; // Adds 5 seconds to alarm time
  AlarmTime = AlarmTime % 60; // checks for roll over 60 seconds and corrects
  show_M0_rtc_date_and_time();
  //Serial.print("Next Alarm Time (seconds):");
  //Serial.println(AlarmTime);
  
  rtcM0.setAlarmSeconds(AlarmTime); // Wakes at next alarm time
  rtcM0.enableAlarm(rtcM0.MATCH_SS); // Match seconds only
  rtcM0.attachInterrupt(alarmMatch);
  
  //Serial.end();
  USBDevice.detach(); // Safely detach the USB prior to sleeping
  
  rtcM0.standbyMode();    // Sleep until next alarm match

  USBDevice.attach();   // Re-attach the USB, audible sound on windows machines

  // Simple indication of being awake
  
  pinMode(12, INPUT_PULLUP); // turns the LED on   
  delay(1000); 
  pinMode(12, INPUT); // turns the LED off
  pinMode(12, INPUT_PULLUP); // turns the LED on   
  delay(1000); 
  pinMode(12, INPUT); // turns the LED off
  pinMode(12, INPUT_PULLUP); // turns the LED on   
  delay(1000); 
  pinMode(12, INPUT); // turns the LED off

  delay(1000);  // Delay added to make serial more reliable
  
  //Serial.begin(9600);
  //while (! Serial); // Wait until Serial is ready
  //Serial.println("Awake");  
}

void alarmMatch() // Do something when interrupt called
{
  
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

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
