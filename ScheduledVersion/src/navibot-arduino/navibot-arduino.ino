/*------------------------------------------------------------------------------\
 *  Navibot remote control by Arduino/Teensyduino                               *
 *                                                                              *
 *  Copyright (C) 2015 VirtualGit https://github.com/VirtualGit/NavibotArduino  *
 *                                                                              *
 *  Licensed under the Apache License, Version 2.0 (the "License");             *
 *  you may not use this file except in compliance with the License.            *
 *  You may obtain a copy of the License at                                     *
 *                                                                              *
 *     http://www.apache.org/licenses/LICENSE-2.0                               *
 *                                                                              *
 *  Unless required by applicable law or agreed to in writing, software         *
 *  distributed under the License is distributed on an "AS IS" BASIS,           *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    *
 *  See the License for the specific language governing permissions and         *
 *  limitations under the License.                                              *
 \------------------------------------------------------------------------------*/


// Includes for real time clock. Download lib from https://github.com/adafruit/RTClib
#include <Wire.h>
#include <RTClib.h>

// Include for IR Emitter. Download lib from http://github.com/shirriff/Arduino-IRremote
#include <IRremote.h>

// Raw IR sequence for basic commands
unsigned int AutoCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 590 , 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
unsigned int SpotCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 590 , 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 590 , 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
unsigned int EdgeCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 590 , 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
unsigned int MaxCommand[68]  = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 590 , 590, 1690, 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };

// Schedule Edge mode
//   0h    1h    2h    3h    4h    5h    6h    7h    8h    9h    10h   11h   12h   13h   14h   15h   16h   17h   18h   19h   20h   21h   22h   23h
bool scheduleEdge[7][24] = {
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Sunday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false,false,false,false,false}, // Monday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,true ,false,false,false,false,false,false,false,false}, // Tuesday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,true ,false,false,false,false,false,false,false,false}, // Wednesday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,true ,false,false,false,false,false,false,false,false}, // Thursday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false,false,false,false,false}, // Friday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}  // Saterday
};

// Schedule Max mode
//   0h    1h    2h    3h    4h    5h    6h    7h    8h    9h    10h   11h   12h   13h   14h   15h   16h   17h   18h   19h   20h   21h   22h   23h
bool scheduleMax[7][24] = {
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Sunday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false}, // Monday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Tuesday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Wednesday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Thursday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false}, // Friday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}  // Saterday
};

// Global variable for IR emitter
IRsend irsend;
// Global variable for accessing DS1307
RTC_DS1307 rtc;

// Program startup
void setup() {
   // Init serial port
   Serial.begin(9600);

   // Start communication with DS1307
   Wire.begin();
   rtc.begin();
   if (! rtc.isrunning()) {
      Serial.println("RTC is NOT running ! Sending time");
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
}


// Main program
void loop() {
   
   DateTime now = rtc.now();
       
   if ( rtc.isrunning() ) {
      // On success, write the date so pluging it allows you to check the current date
      printDate( now );
      // Check if some command should be send now
      checkScheduler( now );
   } else {
      Serial.println("RTC is NOT running !");
   }



   // No need to check faster than once per minute. On teensy boards, this delay should
   // set the board in a deep sleep mode so the consumption is near 0mA. On arduino
   // it seems to be different and you should consider using the LowPower library
   // at : https://github.com/rocketscream/Low-Power
   delay(60000);
}


// Print the given date
void printDate( const DateTime& dt )
{
   Serial.print("Ok, WDay = ");
   Serial.print( dt.dayOfWeek() );
   Serial.print(" Time = ");
   print2digits( dt.hour() );
   Serial.write(':');
   print2digits( dt.minute() );
   Serial.write(':');
   print2digits( dt.second() );
   Serial.print(", Date (D/M/Y) = ");
   Serial.print( dt.day() );
   Serial.write('/');
   Serial.print( dt.month() );
   Serial.write('/');
   Serial.print( dt.year() );
   Serial.println();
}

// Check if a command should be send at the given date, and send it
// if needed.
void checkScheduler( const DateTime& dt )
{
   // Check bounds
   if( dt.dayOfWeek() < 0 || dt.dayOfWeek() > 6 || dt.hour() < 0 || dt.hour() > 23 )
   {
      Serial.println("An error occured : weekday or hour out of bounds.");
      return;
   }
  
   // Check if some IR command should be send now
   if( scheduleMax[dt.dayOfWeek()][dt.hour()] && dt.minute() == 0 )
   {
      // Send maximum cleaning command
      Serial.println("Start maximum cleaning");
      irsend.sendRaw(MaxCommand, 68, 38);
   }
   if( scheduleEdge[dt.dayOfWeek()][dt.hour()] && dt.minute() == 0 )
   {
      // Send edge cleaning command
      Serial.println("Start edge cleaning");
      irsend.sendRaw(EdgeCommand, 68, 38);
   }
}

// Print a number with a leading zero if needed
void print2digits( int number ) {
   if (number >= 0 && number < 10) {
      Serial.write('0');
   }
   Serial.print(number);
}

