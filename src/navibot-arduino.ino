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


// Include for real time clock
#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>

// Include for IR Emitter
#include <IRremote.h>

// Raw IR sequence for basic commands
unsigned int AutoCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 590 , 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
unsigned int SpotCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 590 , 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 590 , 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
unsigned int EdgeCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 590 , 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
unsigned int MaxCommand[68]  = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 590 , 590, 1690, 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };

// Schedule Edge mode
//   0h    1h    2h    3h    4h    5h    6h    7h    8h    9h    10h   11h   12h   13h   14h   15h   16h   17h   18h   19h   20h   21h   22h   23h
bool scheduleEdge[7][24] = {
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Saterday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Sunday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false,false,false,false,false}, // Monday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,true ,false,false,false,false,false,false,false,false}, // Tuesday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,true ,false,false,false,false,false,false,false,false}, // Wednesday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,true ,false,false,false,false,false,false,false,false}, // Thursday
   {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false,false,false,false,false}  // Friday
};

// Schedule Max mode
//   0h    1h    2h    3h    4h    5h    6h    7h    8h    9h    10h   11h   12h   13h   14h   15h   16h   17h   18h   19h   20h   21h   22h   23h
bool scheduleMax[7][24] = {
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Saterday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Sunday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false}, // Monday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Tuesday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Wednesday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false}, // Thursday
   {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false,false,false,false}  // Friday
};

// Global variable for IR emitter
IRsend irsend;


// Program startup
void setup() {
   // Init serial port
   Serial.begin(9600);
   // wait for serial
   while (!Serial) ; 
}


// Main program
void loop() {
   // Time elements
   tmElements_t tm;
  
   // Read time
   if (RTC.read(tm)) {
    
      // On success, write the date so pluging it allows you to check the current date
      printDate( &tm );
      // Check if some command should be send now
      checkScheduler( &tm );
    
   } else {
      if (RTC.chipPresent()) {
         Serial.println("The DS1307 is stopped.  Please run the SetTime");
         Serial.println("example to initialize the time and begin running.");
         Serial.println();
      } else {
         Serial.println("DS1307 read error!  Please check the circuitry.");
         Serial.println();
      }
   }

   // No need to check faster than once per minute. On teensy boards, this delay should
   // set the board in a deep sleep mode so the consumption is near 0mA. On arduino
   // it seems to be different and you should consider using the LowPower library
   // at : https://github.com/rocketscream/Low-Power
   delay(60000);
}


// Print the given date
void printDate( tmElements_t* tm )
{
   Serial.print("Ok, WDay = ");
   Serial.print(tm->Wday);
   Serial.print(" Time = ");
   print2digits(tm->Hour);
   Serial.write(':');
   print2digits(tm->Minute);
   Serial.write(':');
   print2digits(tm->Second);
   Serial.print(", Date (D/M/Y) = ");
   Serial.print(tm->Day);
   Serial.write('/');
   Serial.print(tm->Month);
   Serial.write('/');
   Serial.print(tmYearToCalendar(tm->Year));
   Serial.println();
}

// Check if a command should be send at the given date, and send it
// if needed.
void checkScheduler( tmElements_t* tm )
{
   // Check bounds
   if( tm->Wday < 0 || tm->Wday > 6 || tm->Hour < 0 || tm->Hour > 23 )
   {
      Serial.println("An error occured : weekday or hour out of bounds.");
      return;
   }
  
   // Check if some IR command should be send now
   if( scheduleMax[tm->Wday][tm->Hour] )
   {
      // Send maximum cleaning command
      Serial.println("Start maximum cleaning");
      irsend.sendRaw(MaxCommand, 68, 38);
   }
   if( scheduleEdge[tm->Wday][tm->Hour] )
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
