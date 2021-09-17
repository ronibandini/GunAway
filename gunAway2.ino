// Gun Away
// Roni Bandini, Buenos Aires, Argentina, Sep 2021
// @RoniBandini
// MIT license

// HUSKYLENS green line >> Pin 10; blue line >> Pin 11
// Relay to D2
// Button to D7

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

int relayPin=2;
int buttonPin=7;
int sirenDelay=3000;

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX

void printResult(HUSKYLENSResult result);

void setup() {
    
    pinMode(relayPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    digitalWrite(relayPin, HIGH);
    
    Serial.begin(115200);
    mySerial.begin(9600);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));        
        delay(100);
    }

    while (!huskylens.setCustomName("Gun ",1))   
    {
      Serial.println(F("ID1 customname failed!")); 
      delay(100);
    }
  Serial.println(F("System started")); 
    
}

void loop() {

    int buttonRead=digitalRead(buttonPin);

    if (buttonRead==1){

          if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
          else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));          
          else
          {                   
                
              //Serial.println(F("###########"));
              while (huskylens.available())
              {
                  HUSKYLENSResult result = huskylens.read();
                  printResult(result);
              }    
          }

    } // button on
    
}

void printResult(HUSKYLENSResult result){
  
    if (result.command == COMMAND_RETURN_BLOCK){
        
        //Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
        Serial.println("Gun detected");       
         
        // Enable siren
        digitalWrite(relayPin, LOW);
        delay(sirenDelay);
        digitalWrite(relayPin, HIGH);     

         // save screenshot to SD Card
        huskylens.saveScreenshotToSDCard(); 
        delay(500);
        
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    
}
