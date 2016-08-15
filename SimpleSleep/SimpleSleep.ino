/*
  Simple RTC Alarm for Adafruit Feather M0 modified from 
  RTCzero library example at https://github.com/arduino-libraries/RTCZero
  By: CaveMoa
  Date: 30/12/15

  Modified by:  Andre Hefer
  Date:        15 Aug 2016
  Test to see of sketch works on Adafruit Feather M0 proto
*/

#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;
int AlarmTime;

void setup()
{
  rtc.begin();
  
}

void loop()
{

  // Simple indication of being awake
  digitalWrite(12, HIGH);   // turn the LED on 
  delay(200);              
  digitalWrite(12, LOW);    // turn the LED off
  delay(200);
  digitalWrite(12, HIGH);   // turn the LED on 
  delay(200);
  digitalWrite(12, LOW);    // turn the LED off
  

  AlarmTime += 10; // Adds 10 seconds to alarm time
  AlarmTime = AlarmTime % 60; // checks for roll over 60 seconds and corrects
  rtc.setAlarmSeconds(AlarmTime); // Wakes at next alarm time, i.e. every 5 secs
  
  rtc.enableAlarm(rtc.MATCH_SS); // Match seconds only
  rtc.attachInterrupt(alarmMatch); // Attach function to interupt
  rtc.standbyMode();    // Sleep until next alarm match
  
}

void alarmMatch() // Do something when interrupt called
{
  
}
