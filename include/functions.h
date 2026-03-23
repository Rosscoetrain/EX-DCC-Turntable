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

#ifdef DUAL_MOTOR
    if(( Addr >= BaseTurnoutAddress + NUM_COMMANDS ) && ( Addr < (BaseTurnoutAddress + NUM_COMMANDS * 2 )) && OutputPower )
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


#endif


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
      MYSERIAL.print(F("Changing full revolution steps to "));
      MYSERIAL.println(fts);

      Dcc.setCV(CV_USER_ADDRESS + 2, ((fts >> 8) & 0xFF));
      Dcc.setCV(CV_USER_ADDRESS + 3, fts & 0xFF);

      MYSERIAL.println(F("<Z> to reset and activate new settings"));
     }
#ifdef DUAL_MOTOR
    fts = ttMover2.calibrate();
    if (fts)
     {
      MYSERIAL.print(F("Changing motor 2 full revolution steps to "));
      MYSERIAL.println(fts);

      Dcc.setCV(CV_USER_ADDRESS + 22, ((fts >> 8) & 0xFF));
      Dcc.setCV(CV_USER_ADDRESS + 23, fts & 0xFF);

      MYSERIAL.println(F("<Z> to reset and activate new settings"));
     }
#endif
   }


  if (readString == "<?>")
   {

    MYSERIAL.println(F("Help Text"));
    MYSERIAL.println(F("Set decoder base address: <A address>"));
    MYSERIAL.println(F("Reset decoder to Factory Defaults: <F>"));
    MYSERIAL.println(F("Toggle debug output: <D>"));

//    MYSERIAL.print(F("Turn on (1) off (0) Extra: <E [1 - 6] [0,1]"));

#ifdef DUAL_MOTOR
    MYSERIAL.print(F("Move motor 1 to a track: <M1 [1 - "));
    MYSERIAL.print(NUM_TRACKS);
    MYSERIAL.println(F("]>"));
    MYSERIAL.print(F("Move motor 2 to a track: <M2 [1 - "));
    MYSERIAL.print(NUM_TRACKS);
    MYSERIAL.println(F("]>"));

    MYSERIAL.println(F("Move motor 1 this number of steps: <S1 (-)steps>"));
    MYSERIAL.println(F("Move motor 2 this number of steps: <S2 (-)steps>"));

    MYSERIAL.println(F("Motor 1 track separation angle: <TA1 [0 - 3600]>"));
    MYSERIAL.println(F("Motor 2 track separation angle: <TA2 [0 - 3600]>"));
#else
    MYSERIAL.print(F("Move to a track: <M [1 - "));
    MYSERIAL.print(NUM_TRACKS);
    MYSERIAL.println(F("]>"));

    MYSERIAL.println(F("Move this number of steps: <S (-)steps>"));
    MYSERIAL.println(F("Track separation angle: <T [0 - 3600]>"));
#endif


    MYSERIAL.println(F("Set led fast flash time: <FF mS/10>"));
    MYSERIAL.println(F("Set led slow flash time: <FS mS/10>"));

    MYSERIAL.println(F("Calibrate steps per revolution: <C>"));


    MYSERIAL.println(F("Set led slow flash time: <FS  mS/10>"));


    MYSERIAL.println(F("Show current CVs: <>"));
    MYSERIAL.println(F("Soft Reset: <Z>"));

#if TURNTABLE_EX_MODE == TRAVERSER
    MYSERIAL.println(F("Set traverser number of tracks: <TT number>"));
#endif

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

      for (int i = 0; i < 16; i++)
       {
        MYSERIAL.print(F("CV"));
        MYSERIAL.print(CV_USER_ADDRESS + i);
        MYSERIAL.print(F(" = "));
        MYSERIAL.println(Dcc.getCV(CV_USER_ADDRESS + i));
       }

     }
    else
     {
      if (readString.startsWith("<"))
       {
        // this is where commands are completed

        // command to move to track <M track>

#ifdef DUAL_MOTOR
        if (readString.startsWith("<M1"))
#else
        if (readString.startsWith("<M"))
#endif
         {

          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            if ((addr > 0) && (addr <= NUM_TRACKS))
             {
              notifyDccAccTurnoutOutput( BaseTurnoutAddress + addr - 1, 1, 1 );
             }
            else
             {
              MYSERIAL.println(F("Invalid track number"));
             }

           }
          else
           {
#ifdef DUAL_MOTOR
            MYSERIAL.println(F("Invalid command: should be <M1 track>"));
#else
            MYSERIAL.println(F("Invalid command: should be <M track>"));
#endif
           }
          delete splitter;
          splitter = NULL;

         }

#ifdef DUAL_MOTOR
        if (readString.startsWith("<M2"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            if ((addr > 0) && (addr <= NUM_TRACKS))
             {
              notifyDccAccTurnoutOutput( BaseTurnoutAddress + NUM_COMMANDS + addr - 1, 1, 1 );
             }
            else
             {
              MYSERIAL.println(F("Invalid track number"));
             }

           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <M2 track>"));
           }
          delete splitter;
          splitter = NULL;
         }
#endif



// command to activate led, acc or ext outputs
// <E p 0/1>
// p = 49 - 55
// 0 = off 1 = on
//
// 49 led off / on
// 50 led flash fast/flash slow
// 51 acc on / off
// 52 ext1 off / on
// 53 ext2 off / on
// 54 ext3 off / on
// 55 ext4 off / on
// 
//

        if (readString.startsWith("<E"))
         {

          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 3)
           {
            int addr = splitter->getItemAtIndex(1).toInt();
            int onOff = splitter->getItemAtIndex(2).toInt();

            if ((addr > NUM_TRACKS) && (addr <= NUM_TRACKS + 7) && ((onOff == 0) || (onOff == 1)))
             {
              notifyDccAccTurnoutOutput( BaseTurnoutAddress + addr - 1, onOff, 1 );
             }
            else
             {
              MYSERIAL.println(F("Invalid address number or on/off"));
             }
           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <E [49 - 55] 0/1>"));
           }
          delete splitter;
          splitter = NULL;

         }




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


        if (readString.startsWith("<T"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            uint16_t angle = splitter->getItemAtIndex(1).toInt();

            if ((angle >= 0) && (angle <= 3600))
             {
              MYSERIAL.print(F("Set angle to : "));
              MYSERIAL.print(angle / 10);
              MYSERIAL.print(F("."));
              MYSERIAL.print(angle % 10);
              MYSERIAL.println(F(" deg"));
              uint8_t m = ((angle >> 8)  & 0xFF);
              Dcc.setCV(CV_USER_ADDRESS + 4, m);
              m = angle & 0xFF;
              Dcc.setCV(CV_USER_ADDRESS + 5, m);
              MYSERIAL.println(F("<Z> to reset and activate new settings"));
             }
            else
             {
              MYSERIAL.println(F("Invalid angle: should be <T [0 - 3600]>"));
             }
           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <T angle>"));
           }
          delete splitter;
          splitter = NULL;
         }


        if (readString.startsWith("<FF"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
#endif
            if ((addr > 1) && (addr < 256))
             {
              Dcc.setCV(CV_USER_ADDRESS + 6, addr);
             }
            else
             {
              MYSERIAL.println(F("Invalid command: should be <FF [1-255]"));
             }

           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <FF ms/10>"));
           }
          delete splitter;
          splitter = NULL;
         }

// set led slow flash rate in ms / 10

        if (readString.startsWith("<FS"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
#endif
            if ((addr > 1) && (addr < 256))
             {
              Dcc.setCV(CV_USER_ADDRESS + 7, addr);
             }
            else
             {
              MYSERIAL.println(F("Invalid command: should be <FS [1-255]"));
             }

           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <FS ms/10>"));
           }
          delete splitter;
          splitter = NULL;
         }

// set track one steps

        if (readString.startsWith("<T1"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
#endif
            if ((addr > 1) && (addr < 256))
             {
              Dcc.setCV(CV_USER_ADDRESS + 7, addr);
             }
            else
             {
              MYSERIAL.println(F("Invalid command: should be <T1 [1 - FULL_TURN_STEPS]"));
             }

           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <T1 (number of steps)>"));
           }
          delete splitter;
          splitter = NULL;
         }


#if TURNTABLE_EX_MODE == TRAVERSER
        if (readString.startsWith("<TT"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
#endif
            if ((addr > 1) && (addr < 256))
             {
              Dcc.setCV(CV_USER_ADDRESS + 8, addr);
             }
            else
             {
              MYSERIAL.println(F("Invalid command: should be <TT [1-255]"));
             }

           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <TT [1-255]>"));
           }
          delete splitter;
          splitter = NULL;
         }

#endif

// move a number of steps

        if (readString.startsWith("<S"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int steps = splitter->getItemAtIndex(1).toInt();
            int endStep = 0;

            if ((steps >= -100) && (steps <= 100))
             {
              MYSERIAL.print(F("Start step position : "));
              MYSERIAL.println(ttMover.currentPosition());

              if (ttMover.currentPosition() + steps < 0)
               {
                endStep = ttMover.getFullTurnSteps() + (ttMover.currentPosition() + steps);
               }
              else
               {
                if (ttMover.currentPosition() + steps > ttMover.getFullTurnSteps())
                 {
                  endStep = (ttMover.currentPosition() + steps) - ttMover.getFullTurnSteps();
                 }
                else
                 {
                  endStep = ttMover.currentPosition() + steps;
                 }
               }
              if (debug)
               {
                MYSERIAL.print(F("end step : "));
                MYSERIAL.println(endStep);
               }
              ttMover.enableOutputs();
//              uint16_t currentStep = ttMover.currentPosition();
              ttMover.moveTo(endStep);
              while (ttMover.currentPosition() != endStep)
               {
                ttMover.run();
               }
              ttMover.disableOutputs();
              MYSERIAL.print(F("At step : "));
              MYSERIAL.println(ttMover.currentPosition(), DEC);

             }
            else
             {
              MYSERIAL.println(F("Invalid steps: should be <S [-100 to 100]>"));
             }
           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <S [-100 - 100]>"));
           }
          delete splitter;
          splitter = NULL;
         }


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
