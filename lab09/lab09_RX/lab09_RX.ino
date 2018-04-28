#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700 //DAC speed (Hz)
/*freq0 : frequency of carrier sine wave (Hz)*/
#define freq0 500
/*A[0]-A[3] : ASK Amplitude (0,5] (V)*/
const float A[4] = {0.25,0.5,0.75,1};
int delay0;
/* amin/amax : Amplitude in digital 10bit */

#define a0min 100 /* a0min <= a0 <= a0max */
#define a0max 300
#define a1min 400 /* a1min <= a1 <= a1max */
#define a1max 600
#define a2min 700/* a2min <= a2 <= a2max */
#define a2max 800
#define a3min 900 /* a3min <= a3 <= a3max */
#define a3max 1100
/* amplitude difference for detecting rising or falling
signal */
#define r_slope 50
int sum = 0;
int maxx = 0;
int prev = 0;
int check = false;
int output = -1;
int count = 0;

void setup() {
/* set serial baudrate the same as in TX */
  Serial.begin(115200);
 
}

void loop() {
  int tmp = analogRead(A0); // read signal from analog pin
  
 
  if( tmp-prev>r_slope &&check ==false){
    maxx = 0;
    check = true; // change check status is true
//     Serial.println("read temp:");
//    Serial.println(tmp);
//    Serial.println("check");
//    Serial.println(check);
  
    
  }
  if(tmp>maxx){ // update max value
    maxx=tmp;
  }
  if(maxx-tmp > r_slope){ // check for falling signal
    if(check == true){
//      Serial.print("Max");
//    Serial.println(maxx);
      if(a0min<maxx && maxx<a0max){
        Serial.print("0 0 ");
        count++;
      }
      else if(a1min<maxx && maxx<a1max){
        Serial.print("0 1 ");
        count++;
      }
      else if(a2min<maxx && maxx<a2max){
        Serial.print("1 0 ");
        count++;
      }
      else if(a3min<maxx && maxx<a3max){
        Serial.print("1 1 ");
        count++;
      }
      if(count == 5){
        Serial.println();
        count = 0;
      }
    }
  check = false; // change check status is false
  }
  prev = tmp; // assign temp value to previous
}
