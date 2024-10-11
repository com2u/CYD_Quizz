/*
   This sketch shows an example of sending a reading to data.sparkfun.com once per day.
   It uses the Sparkfun testing stream so the only customizing required is the WiFi SSID and password.
   The Harringay Maker Space
   License: Apache License v2
*/
#include <NTPtimeESP.h> // https://github.com/SensorsIot/NTPtimeESP
strDateTime mydateTime;

#define DEBUG_ON



NTPtime NTPch("ch.pool.ntp.org");   // Choose server pool as required

/*
 * The structure contains following fields:
 * struct strDateTime
{
  byte hour;
  byte minute;
  byte second;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
  boolean valid;
};
 */
strDateTime dateTime;

void initTime() {

  // first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time; 0 for no DST adjustment; (contributed by viewwer, not tested by me)
  dateTime = NTPch.getNTPtime(1.0, 1);

  // check dateTime.valid before using the returned time
  // Use "setSendInterval" or "setRecvTimeout" if required
  if(dateTime.valid){
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;
    byte actualMinute = dateTime.minute;
    byte actualsecond = dateTime.second;
    int actualyear = dateTime.year;
    byte actualMonth = dateTime.month;
    byte actualday =dateTime.day;
    byte actualdayofWeek = dateTime.dayofWeek;
    mydateTime = dateTime;
  } else {
    Serial.println("NO UPDATE initTime ! ! ! ");
  }
}

String getMyTime() {
  for ( int i = 0; i < 5; i++) { // Retry timer
      // first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time; 0 for no DST adjustment; (contributed by viewwer, not tested by me)
      dateTime = NTPch.getNTPtime(1.0, 1);
      // check dateTime.valid before using the returned time
      // Use "setSendInterval" or "setRecvTimeout" if required
      if(dateTime.valid){
        NTPch.printDateTime(dateTime);

        byte actualHour = dateTime.hour;
        byte actualMinute = dateTime.minute;
        byte actualsecond = dateTime.second;
        int actualyear = dateTime.year;
        byte actualMonth = dateTime.month;
        byte actualday =dateTime.day;
        byte actualdayofWeek = dateTime.dayofWeek;
        mydateTime = dateTime;
        char buffer[120];
        sprintf(buffer, "%d-%d-%d, %d:%d:%d ", mydateTime.year, mydateTime.month, mydateTime.day , mydateTime.hour, mydateTime.minute, mydateTime.second);
        Serial.println(buffer);
        return String(buffer);
      } else {
        Serial.println("NO UPDATE getMyTime ! ! ! ");
        delay(200);
        if (random(100) == 0) {
          ESP.restart();
        }
      }
  }
  return "";
}


