/*
 * @file functions.h
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "defines.h"

/*
 * add the command to the command queue depending on address received.
 */


void setCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
 {
#ifdef LEARNING_BUTTON
  if (learningMode == HIGH)
   {
    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

#ifdef DEBUG_MSG
    MYSERIAL.println("setCommand");
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(Addr,DEC);
    MYSERIAL.print(F("Dir = ")); MYSERIAL.println(Direction,DEC);
    MYSERIAL.print(F("Op = ")); MYSERIAL.println(OutputPower,DEC);

#endif
                  
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
   }
  else
#endif

   {
    if(( Addr >= BaseTurnoutAddress ) && ( Addr < (BaseTurnoutAddress + NUM_COMMANDS )) && OutputPower )
     {
      if (debug)
       {
        MYSERIAL.print(F("addr: "));MYSERIAL.println(Addr);
        MYSERIAL.print(F("baseTurnoutAddr: "));MYSERIAL.println(BaseTurnoutAddress);
       }

// Direction = 1 move clockwise
// Direction = 0 move anti-clockwise

      thisCommand = (((Addr - BaseTurnoutAddress) * 10) + 1) + Direction;

      ttMover.addCommand(thisCommand);

#ifdef DEBUG_MSG_1
      MYSERIAL.println("");
      MYSERIAL.print("ndato thisCommand: ");
      MYSERIAL.println(thisCommand,DEC);
#endif

     }

   }

#ifdef  NOTIFY_TURNOUT_MSG
  MYSERIAL.println();
#endif

}




/*
 * setup the version number
 */


void setVersion() {
  const String versionString = VERSION;
  char versionArray[versionString.length() + 1];
  versionString.toCharArray(versionArray, versionString.length() + 1);
  version = strtok(versionArray, "."); // Split version on .
  versionBuffer[0] = atoi(version);  // Major first
  version = strtok(NULL, ".");
  versionBuffer[1] = atoi(version);  // Minor next
  version = strtok(NULL, ".");
  versionBuffer[2] = atoi(version);  // Patch last
}


/*
 * read the analogue pin as if it was a digital pin and return 1 or 0
 */

/*
 * soft reset function
 */

void(* resetFunc) (void) = 0;//declare reset function at address 0



/*
 * process serial commands
 */

#include "StringSplitter.h"

void doSerialCommand(String readString)
 {

  readString.trim();
  readString.toUpperCase();


  MYSERIAL.println(readString);  //so you can see the captured string

  if (readString == "<Z>")
   {
    MYSERIAL.println(F("Resetting"));
    delay(1000);
    resetFunc();
   }

  if (readString == "<F>")
   {
    MYSERIAL.println(F("Reset factory default CVs. Reset <Z> decoder next"));

    notifyCVResetFactoryDefault();
   }

  if (readString == "<D>")
   {
    MYSERIAL.println(F("Toggle debug"));
    debug = !debug;
    ttMover.setDebug(debug);
   }


  if (readString == "<C>")
   {
    MYSERIAL.println(F("Calibrating"));
    uint16_t fts = ttMover.calibrate();
    if (fts)
     {
      MYSERIAL.println(F("Changing full revolution steps"));

//      Dcc.setCV(CV_USER_ADDRESS + 2, ((fts >> 8) & 0xFF));
//      Dcc.setCV(CV_USER_ADDRESS + 3, fts & 0xFF);
     }
   }



  if (readString == "<?>")
   {

    MYSERIAL.println(F("Help Text"));
    MYSERIAL.println(F("Set decoder base address: <A address>"));
    MYSERIAL.println(F("Reset decoder to Factory Defaults: <F>"));
    MYSERIAL.println(F("Toggle debug output: <D>"));

//    MYSERIAL.println(F("Move to a track: <M address>"));
//    MYSERIAL.println(F("Close an address: <C address>"));
//    MYSERIAL.println(F("Throw an address: <T address>"));
//    MYSERIAL.println(F("Set decoder output pulse time: <P  mS / 10>"));

    MYSERIAL.println(F("Calibrate steps per revolution: <C>"));

    MYSERIAL.println(F("Show current CVs: <>"));
    MYSERIAL.println(F("Soft Reset: <Z>"));

   }
  else
   {
    if (readString.startsWith("<>"))
     {

      MYSERIAL.println(F("CVs are:"));
      MYSERIAL.print(F("CV"));
      MYSERIAL.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
      MYSERIAL.print(F(" = "));
      MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
      MYSERIAL.print(F("CV"));
      MYSERIAL.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
      MYSERIAL.print(F(" = "));
      MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));

      MYSERIAL.print(F("CV"));
      MYSERIAL.print(CV_ACCESSORY_DECODER_WAIT_TIME);
      MYSERIAL.print(F(" = "));
      MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_WAIT_TIME));

      MYSERIAL.print(F("CV"));
      MYSERIAL.print(CV_29_CONFIG);
      MYSERIAL.print(F(" = "));
      MYSERIAL.println(Dcc.getCV(CV_29_CONFIG));

     }
    else
     {
      if (readString.startsWith("<"))
       {
        // this is where commands are completed

        // command to close turnout <M address>

        if (readString.startsWith("<M"))
         {
/*
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            notifyDccAccTurnoutOutput( addr, 1, 1 );

           }
          else
           {
                        MYSERIAL.println(F("Invalid command: should be <C address>"));
           }
          delete splitter;
          splitter = NULL;
*/
         }


         // command to throw turnout <T address>
/*
        if (readString.startsWith("<T"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();


          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            notifyDccAccTurnoutOutput( addr, 0, 1 );

           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <T address>"));
           }
          delete splitter;
          splitter = NULL;
         }
*/

         // command to set address <A address>
         // address will be adjusted to the correct base turnout address
         // eg if address is 2 this will be corrected to 1 as the address are groups of 8 with an offset of 4
         // ie 1..8, 5..12, ...

        if (readString.startsWith("<A"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            byte L = (addr + 3) / 4;
            byte H = (addr + 3) / 1024;

#ifdef DEBUG_MSG
            MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
            MYSERIAL.print(F(" H = ")); MYSERIAL.println(H);
            MYSERIAL.print(F(" L = ")); MYSERIAL.println(L);
#endif
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <A address>"));
           }
          delete splitter;
          splitter = NULL;
         }

/*
        if (readString.startsWith("<P"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
#endif

            Dcc.setCV(CV_ACCESSORY_DECODER_WAIT_TIME, addr);

           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <P ms/10>"));
           }
          delete splitter;
          splitter = NULL;
          resetFunc();
         }
*/

       }
      else
       {
                MYSERIAL.println(F("ERROR: Unknown command"));
       }
     }
   }
 }



/*
 * these are the NMRA dcc override functions
 */

void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};



#ifdef ENABLE_DCC_ACK

// This function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read 
void notifyCVAck(void)
{
    MYSERIAL.println("notifyCVAck") ;

  digitalWrite( ENABLE_DCC_ACK, HIGH );
  delay( 10 );  
  digitalWrite( ENABLE_DCC_ACK, LOW );
}
#endif


#ifdef NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
 {
  if ((Msg->Data[0] == 0xFF) && (Msg->Data[1] == 0x00) && (Msg->Data[2] == 0xFF))
   {

   }
  else
   {
    MYSERIAL.print("notifyDccMsg: ");
    for(uint8_t i = 0; i < Msg->Size; i++)
     {
      MYSERIAL.print(Msg->Data[i], HEX);
      MYSERIAL.write(' ');
     }
    MYSERIAL.println();
   }
 }
#endif



/*
// This function is called whenever a normal DCC Turnout Packet is received and we're in Board Addressing Mode
void notifyDccAccTurnoutBoard( uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower )
 {

  uint16_t Addr = ((BoardAddr - 1) * 4) + OutputPair + 1;

  MYSERIAL.print("notifyDccAccTurnoutBoard: ") ;
  MYSERIAL.print(BoardAddr,DEC) ;
  MYSERIAL.print(',');
  MYSERIAL.print(OutputPair,DEC) ;
  MYSERIAL.print(',');
  MYSERIAL.print(Direction,DEC) ;
  MYSERIAL.print(',');
  MYSERIAL.println(OutputPower, HEX) ;

  setCommand(Addr, Direction, OutputPower);
 }
*/


/*
// This function is called whenever a DCC Signal Aspect Packet is received
void notifyDccSigOutputState( uint16_t Addr, uint8_t State)
 {
  MYSERIAL.print("notifyDccSigOutputState: ");
  MYSERIAL.print(Addr,DEC);
  MYSERIAL.print(',');
  MYSERIAL.println(State, HEX);

  setCommand(Addr + State - 1, 1, 1);
 }
*/



// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
 {
#ifdef  NOTIFY_TURNOUT_MSG
  MYSERIAL.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  MYSERIAL.print(Addr,DEC) ;
  MYSERIAL.print(" Direction: ");
  MYSERIAL.print(Direction ? "Closed" : "Thrown") ;
  MYSERIAL.print(" Output: ");
  MYSERIAL.println(OutputPower ? "On" : "Off") ;
#endif

  setCommand(Addr, Direction, OutputPower);

 }


#endif
