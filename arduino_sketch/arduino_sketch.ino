/*
*
* Program for Arduino nano to receive volume up and volume down command from Philips IR TV remote (RC6 protocol).
* Command is used to set a digital MCP4151 potentiometer for external volume control.
* Additional serial output for debugging.
* Code by pandora
*
*/

#include <IRremote.h>
#include <MCP4151.h>

#define CS     10
#define MOSI   11
#define MISO   12
#define SCK    13

MCP4151 pot(CS, MOSI, MISO, SCK);

// Pin where the IR receiver is connected to
const int irReceivePin = 2;

IRrecv irrecv(irReceivePin);
decode_results results;

// Initial pot value after restart
int value = 128;

void setup()
{
  pot.writeValue(value);
  Serial.begin(9600);
  irrecv.enableIRIn();
}


void loop() {
  Serial.println(value, DEC);
  if (irrecv.decode(&results)){
      if (results.decode_type == RC6) {
        Serial.print("-decoded RC6: ");
        Serial.print("Value = ");
        Serial.println(results.value, DEC);

        // Lower pot value if volume up
        if (results.value == 17 || results.value == 65553){
            Serial.println("lauter");
            value -= 5;
            if (value < 0)
              value = 0;
            pot.writeValue(value);
            }
        // Higher pot value if volume down
        else if (results.value == 16 || results.value == 65552){
          Serial.println("leiser");
            value += 5;
            if (value > 254)
              value = 254;
          pot.writeValue(value);
          }
      }

    
      // showReceivedData();
      irrecv.resume();
  }
 
  delay(200);
}

// Serial output for debugging
void showReceivedData()
{
  if (results.decode_type == UNKNOWN){
    Serial.println("-Could not decode message");
    }
  else{
    if (results.decode_type == NEC) {
      Serial.print("- decoded NEC: ");
    }
  else if (results.decode_type == SONY) {
      Serial.print("- decoded SONY: ");
    }
  else if (results.decode_type == RC5) {
      Serial.print("-decoded RC5: ");
    }
  else if (results.decode_type == RC6) {
      Serial.print("-decoded RC6: ");
    }
  Serial.print("Value = ");
  Serial.println(results.value, HEX);
  }
}
