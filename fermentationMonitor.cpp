#include <OLEDFourBit.h>
#include "globals.h"
#include "enums.h"

void setup()
{
  lcdText[0][20]='\0';
  lcdText[1][20]='\0';
  lcdText[2][20]='\0';
  lcdText[3][20]='\0';

  pinMode(coolingPin, OUTPUT);
  pinMode(heatingPin, OUTPUT);
  pinMode(doorPin, INPUT);
  // analog pins are input by default
  
  analogReference(2); //set analog reference to internal 1.1V
  delay(100);
  lcd.begin(20, 4);
  lcd.clear(); 
  lcdPrintStationaryText();
  loadSettings(); //read previous settings from EEPROM
  
  lcdPrintMode();
  Serial.begin(9600);
  initFilters();
  initControl();
  updateSettings();
  serialFridgeMessage(ARDUINO_START);
}

void loop()
{
  if(millis()- sampleTimerFast > 200){ //update fast filters every 200 ms
    sampleTimerFast=millis();
    updateTemperatures();
  }
  if(millis()-sampleTimerSlow > 10000){ //update slow filter every 10 seconds
    sampleTimerSlow=millis();
    updateSlowFilteredTemperatures();
    detectPeaks();
    updateSettings();
  }
  if(millis()-settingsTimer >1000){ //update settings every second
    settingsTimer=millis();
    updateState();
    lcdPrintState();
    updateOutputs(); 
    lcdPrintAllTemperatures();
    lcdPrintMode();

  }
  if(millis()- slopeTimer > 60000){//update slope every minute. slope is tempdiff per 30 minutes
    slopeTimer = millis();
    updateSlope();
  }
 //listen for incoming serial connections while waiting top update
  handleSerialCommunication();  
  //Check if menu button is pressed
  if(analogRead(button3)>900){
    menu(MENU_TOP);
  }
}





