/*
1/15/2016--CADI 1.1 PulseBoard MIDI Code

1/22/2016: Implemented dynamic on time scaling
  Note: could do this for each individual arm as well as global

Note: Code needs to be streamlined to use arrays for arms

Code designed for use with EMMI's Mosfet Pulse Board and EMMIduini 

Copyright 2016, Steven Kemper
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <MIDI.h> //Francois Best's MIDI Library, Version 3.2

//Pins:  {10,4,3,2,1,19,16,15,17,11,12,13,18,9,5,6,7,8};
//Index number = which socket (in 20 pin connector)
//Consult EMMI_MOSFET_Pulse_Board1F_pcb.png for layout

#ifndef cbi //This allows us to enable pin 1 for use
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#include "Arm.h" //class for each arm 

//MIDI Setup
int midiChan = 1;  //Sets MIDI Channel
int AllNotesOff = 123;  //Sets Controller 123 as All Notes Off
int OnTimeLowCCnum = 120; //Stores low and high values for in time
int OnTimeHighCCnum = 121;
int MIDI_LED = 14; //Pulse board blinks with assigned notes received

int midiNN[8] = {36,37,38,39,40,41,42,43}; //Array of MIDI note numbers

int onTimeScaleLow = 10;  //Low value for on-time in ms (velocity 1 = Xms)
int onTimeScaleHigh = 60;  //High value for on-time in ms (velocity 127 = Xms)


//Arrays to store timing information for each arm
unsigned long currentMillis[8] = {0,0,0,0,0,0,0,0};
unsigned long previousMillis[8] = {0,0,0,0,0,0,0,0};

//Individual arm setup
int solenoidPins[8] = {10,4,3,2,1,19,16,15}; //CADI 1.1 uses first 8 pins on EMMI pulse board

arm Arm1(midiNN[0], solenoidPins[0]); //nn, solenoid pin#--(Eventually need to make this an array)
arm Arm2(midiNN[1], solenoidPins[1]);
arm Arm3(midiNN[2], solenoidPins[2]); 
arm Arm4(midiNN[3], solenoidPins[3]);
arm Arm5(midiNN[4], solenoidPins[4]); 
arm Arm6(midiNN[5], solenoidPins[5]);
arm Arm7(midiNN[6], solenoidPins[6]); 
arm Arm8(midiNN[7], solenoidPins[7]);

void setup() 
{ 
  pinMode(Arm1.striker, OUTPUT);  //Initialize digital pins as output
  pinMode(Arm2.striker, OUTPUT); 
  pinMode(Arm3.striker, OUTPUT); 
  pinMode(Arm4.striker, OUTPUT); 
  pinMode(Arm5.striker, OUTPUT); 
  pinMode(Arm6.striker, OUTPUT); 
  pinMode(Arm7.striker, OUTPUT); 
  pinMode(Arm8.striker, OUTPUT); 
  MIDI.begin(midiChan);  //set MIDI channel to that specified above 
    cbi(UCSR0B, TXEN0);  //Disables pin 1 so we can use it as an output
}

void loop() 
{ 
  if (MIDI.read()) 
  {  
      switch(MIDI.getType())  		 // Get the type of the message we caught
      { 
          case ControlChange: 
          {
              int tCtlNum = MIDI.getData1(); //Get Controller Number
              int tCtlVal = MIDI.getData2(); //Get Value
              
              //[Need code to allow changing of low/high on-times]
              
              if (tCtlNum == OnTimeLowCCnum) //Set low on time value
               {
                 onTimeScaleLow = tCtlVal;
               }
               
               if (tCtlNum == OnTimeHighCCnum) //Set low on time value
               {
                 onTimeScaleHigh = tCtlVal;
               }
             
             //If all notes off message received, turn off all arms
              if( tCtlNum == AllNotesOff && tCtlVal == 0 ) //Controller 123 sends AllOff
              { 
 
                  Arm1.strikerState = LOW;
                  digitalWrite(Arm1.striker, Arm1.strikerState);
                  
                  Arm2.strikerState = LOW;
                  digitalWrite(Arm2.striker, Arm2.strikerState);
                  
                  Arm3.strikerState = LOW;
                  digitalWrite(Arm3.striker, Arm3.strikerState);
                  
                  Arm4.strikerState = LOW;
                  digitalWrite(Arm4.striker, Arm4.strikerState);
                  
                  Arm5.strikerState = LOW;
                  digitalWrite(Arm5.striker, Arm5.strikerState);
                  
                  Arm6.strikerState = LOW;
                  digitalWrite(Arm6.striker, Arm6.strikerState);
                  
                  Arm7.strikerState = LOW;
                  digitalWrite(Arm7.striker, Arm7.strikerState);
                  
                  Arm8.strikerState = LOW;
                  digitalWrite(Arm8.striker, Arm8.strikerState);
              } 
           }
           
           break;
           
           case NoteOn: {
               int tNoteNum = MIDI.getData1(); //Get Note Number, scale for our arrays
               int tNoteVel = MIDI.getData2(); //Get Velocity
               int onTime = 0;
              
               if (tNoteNum == Arm1.note) //ARM 1
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn1(0, Arm1, onTime); //init, stringNumber, on time);
                   }
               }
               if (tNoteNum == Arm2.note) //ARM 2
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn2(0, Arm2, onTime); //init, stringNumber, on time);
                   }
               }
               if (tNoteNum == Arm3.note) //ARM 3
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn3(0, Arm3, onTime); //init, stringNumber, on time);
                   }
               }
               if (tNoteNum == Arm4.note) //ARM 4
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn4(0, Arm4, onTime); //init, stringNumber, on time);
                   }
               }
               if (tNoteNum == Arm5.note) //ARM 5
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn5(0, Arm5, onTime); //init, stringNumber, on time);
                   }
               }
               if (tNoteNum == Arm6.note) //ARM 6
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn6(0, Arm6, onTime); //init, stringNumber, on time);
                   }
               }
               if (tNoteNum == Arm7.note) //ARM 7
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn7(0, Arm7, onTime); //init, stringNumber, on time);
                   }
               }
               if (tNoteNum == Arm8.note) //ARM 8
               {
                   if(tNoteVel > 0)
                   {
                       onTime = map(tNoteVel, 0, 127, onTimeScaleLow, onTimeScaleHigh); //Scales from velocity numbers to on time
                       turnSolenoidsOn8(0, Arm8, onTime); //init, stringNumber, on time);
                   }
               }
               
               
          }    
          break;
             
          default:  
         
          break;
         }
  }
  
  
}

//Function to turn solenoids on/off based on millis()
//[Need to condense this function so we only need one]
void turnSolenoidsOn1(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH; //Set striking arm state to ON
      digitalWrite(ArmNum.striker, ArmNum.strikerState);  //Turn on striking arm
      previousMillis[0] = millis(); //Keep track of when we turned on arm
  }
  
  currentMillis[0] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[0] - previousMillis[0] >= onTime) //If the difference between current time/time solenoid turned on > on time, turn off arm
  {
      if(ArmNum.strikerState == HIGH) //If arm is on
      {
          ArmNum.strikerState = LOW; //Set arm state to OFF
          digitalWrite(ArmNum.striker, ArmNum.strikerState); //Turn arm off
          previousMillis[0] = currentMillis[0]; //Reset counter
      }
  }
  else
  {
      turnSolenoidsOn1(1, ArmNum, onTime); //Arm stays on (init is on)
  }
}

void turnSolenoidsOn2(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH;
      digitalWrite(ArmNum.striker, ArmNum.strikerState);
      previousMillis[1] = millis();
  }
  
  currentMillis[1] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[1] - previousMillis[1] >= onTime)
  {
      if(ArmNum.strikerState == HIGH)
      {
          ArmNum.strikerState = LOW;
          digitalWrite(ArmNum.striker, ArmNum.strikerState);
          previousMillis[1] = currentMillis[1];
      }
  }
  else
  {
      turnSolenoidsOn2(1, ArmNum, onTime);
  }
}

void turnSolenoidsOn3(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH;
      digitalWrite(ArmNum.striker, ArmNum.strikerState);
      previousMillis[2] = millis();
  }
  
  currentMillis[2] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[2] - previousMillis[2] >= onTime)
  {
      if(ArmNum.strikerState == HIGH)
      {
          ArmNum.strikerState = LOW;
          digitalWrite(ArmNum.striker, ArmNum.strikerState);
          previousMillis[2] = currentMillis[2];
      }
  }
  else
  {
      turnSolenoidsOn3(1, ArmNum, onTime);
  }
}

void turnSolenoidsOn4(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH;
      digitalWrite(ArmNum.striker, ArmNum.strikerState);
      previousMillis[3] = millis();
  }
  
  currentMillis[3] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[3] - previousMillis[3] >= onTime)
  {
      if(ArmNum.strikerState == HIGH)
      {
          ArmNum.strikerState = LOW;
          digitalWrite(ArmNum.striker, ArmNum.strikerState);
          previousMillis[3] = currentMillis[3];
      }
  }
  else
  {
      turnSolenoidsOn4(1, ArmNum, onTime);
  }
}

void turnSolenoidsOn5(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH;
      digitalWrite(ArmNum.striker, ArmNum.strikerState);
      previousMillis[4] = millis();
  }
  
  currentMillis[4] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[4] - previousMillis[4] >= onTime)
  {
      if(ArmNum.strikerState == HIGH)
      {
          ArmNum.strikerState = LOW;
          digitalWrite(ArmNum.striker, ArmNum.strikerState);
          previousMillis[4] = currentMillis[4];
      }
  }
  else
  {
      turnSolenoidsOn5(1, ArmNum, onTime);
  }
}
void turnSolenoidsOn6(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH;
      digitalWrite(ArmNum.striker, ArmNum.strikerState);
      previousMillis[5] = millis();
  }
  
  currentMillis[5] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[5] - previousMillis[5] >= onTime)
  {
      if(ArmNum.strikerState == HIGH)
      {
          ArmNum.strikerState = LOW;
          digitalWrite(ArmNum.striker, ArmNum.strikerState);
          previousMillis[5] = currentMillis[5];
      }
  }
  else
  {
      turnSolenoidsOn6(1, ArmNum, onTime);
  }
}
void turnSolenoidsOn7(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH;
      digitalWrite(ArmNum.striker, ArmNum.strikerState);
      previousMillis[6] = millis();
  }
  
  currentMillis[6] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[6] - previousMillis[6] >= onTime)
  {
      if(ArmNum.strikerState == HIGH)
      {
          ArmNum.strikerState = LOW;
          digitalWrite(ArmNum.striker, ArmNum.strikerState);
          previousMillis[6] = currentMillis[6];
      }
  }
  else
  {
      turnSolenoidsOn7(1, ArmNum, onTime);
  }
}
void turnSolenoidsOn8(int init, arm ArmNum, int onTime)
{
  if(init == 0) //On the first loop, turn on the solenoids
  {
      ArmNum.strikerState = HIGH;
      digitalWrite(ArmNum.striker, ArmNum.strikerState);
      previousMillis[7] = millis();
  }
  
  currentMillis[7] = millis(); //The amount of milliseconds since the program started
  
  if(currentMillis[7] - previousMillis[7] >= onTime)
  {
      if(ArmNum.strikerState == HIGH)
      {
          ArmNum.strikerState = LOW;
          digitalWrite(ArmNum.striker, ArmNum.strikerState);
          previousMillis[7] = currentMillis[7];
      }
  }
  else
  {
      turnSolenoidsOn8(1, ArmNum, onTime);
  }
}


