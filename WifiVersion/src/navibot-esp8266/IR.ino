// Use library to send IR commands
#include <IRsend.h>

// The IR LED pin used
#define IR_LED 5

// An object to handle IR emission
IRsend irsend(IR_LED);

// Raw IR sequence for basic commands of Samsung Navitor SR8855
uint16_t AutoCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 590 , 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
uint16_t SpotCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 590 , 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 590 , 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
uint16_t EdgeCommand[68] = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 590 , 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590, 590, 1690, 590, 590 , 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };
uint16_t MaxCommand[68]  = {4590, 4590, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 590 , 590, 590 , 590, 1690, 590, 590 , 590, 590, 590, 590, 590, 590, 590, 590, 590, 1690, 590, 1690, 590, 590, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 1690, 590, 65750 };

// Function for IR initializations
void IRBegin()
{
  irsend.begin();
}

// Send the command for the robot to start cleaning in MAXIMUM mode
void IRSendStartComplete()
{
    irsend.sendRaw(MaxCommand, 68, 38);
}

// Send the command for the robot to start cleaning in EDGE mode
void IRSendStartEdge()
{
    irsend.sendRaw(EdgeCommand, 68, 38);
}
