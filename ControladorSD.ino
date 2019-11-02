
/*
http://audio.online-convert.com/convert-to-wav
Go to The Link
Upload your audio you want to convert to WAV
Change bit resolution to " 8bit ".
Change sampling rate to " 16000Hz ".
Change audio channels " mono " .
Click on " Show advanced options ".
PCM format " unsigned 8 bit ".
Convert file.
At next page click on "direct download link"
 */


/*
 CS ---------------------->> 10
SCK ---------------------->> 13
MOSI -------------------->> 11
MISO--------------------->> 12
VCC --------------------->> +5v
GND--------------------->> Arduino's Ground
 */

#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"
#define SD_ChipSelectPin 4
#define initPin 10



TMRpcm tmrpcm;

void playWin() {
  Serial.println("WIN");

    tmrpcm.play("defused.wav");

}

void playLose() {
  Serial.println("LOSE");

      tmrpcm.play("boom.wav");



}

void setup(){
  pinMode(initPin, INPUT);
  tmrpcm.speakerPin = 9;
  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
  
  attachInterrupt(digitalPinToInterrupt(2), playLose, RISING);
  attachInterrupt(digitalPinToInterrupt(3), playWin, RISING);
  
  tmrpcm.setVolume(5);

}


void loop(){
  
  int initGame = digitalRead(initPin);
  if ( initGame == HIGH ) {
     // tmrpcm.play("bomb_explosion.wav");
     tmrpcm.play("planted.wav");
     delay(1500);
     tmrpcm.play("bombgame.wav");
     delay(3600000);
  }
  
}
