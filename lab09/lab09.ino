#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700 //DAC speed (Hz)
/*freq0 : frequency of carrier sine wave (Hz)*/
#define freq0 500
/*A[0]-A[3] : ASK Amplitude (0,5] (V)*/
const float A[4] = {0.25,0.5,0.75,1};
/*S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270*/
const uint16_t S_DAC[4] = {0, 2047, 4095, 2047};
int delay0;
char inData[20]; // Allocate some space for the string

void setup( ) {
/* set buadrate serial is 115200 */
  Serial.begin(115200);  
  dac.begin(0x62); // set to default
  delay0 = (1000000/freq0 - 1000000/defaultFreq)/4;
  // delay for sampling period of sine
  // (Tsine – delayfrom DAC processing speed)
  Serial.flush(); // for clear buffer serial
}

void loop( ) {
  
  if(Serial.available()>0){
    
    /*use a cycle loop receive inData : message input */
    for (int i=0; i==0 || inData[i-1]!=0xFFFFFFFF; i++){
      inData[i] = Serial.read(); // Read a character
    }

    /*use a cycle loop i for send data 8 bits*/
    for (int i=0; inData[i]!=0xFFFFFFFF; i++ ){
    /*
    use a cycle loop k for 1 ASK signal element (2 bit)
    - map inData[i] to tmp (2 bit)
    - from LSB to MSB
    */
      for (int k=7; k>=0; k-=2){
        int tmp = inData[i] & B11000000 ; // 00, 01, 10, 11
        /*use a cycle loop sl to send 5 cycle/baud*/
        for(int s2=0; s2<5; s2++ ){
        /*use a cycle loop s to send 4 sample/cycle*/
          for(int sl=0; sl<4; sl++ ){
            /*
            Use the selected amplitude above to modify
            sine amplitude
            */
            dac.setVoltage(S_DAC[sl]*A[tmp],false);
            delayMicroseconds(delay0); // sampling delay
          }
        }
      inData[i] <<= 2;
      }
    }
    dac.setVoltage(0,false); // for don't send
  }
}
