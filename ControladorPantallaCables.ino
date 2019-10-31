#include <Arduino.h>
#include <string.h>
#include <SD.h>
#include <SPI.h>
#include "SevenSegmentTM1637.h"
#include "TMRpcm.h"

#define WIRES_NUMBER 4
#define WIN_WIRE 3
#define PLAY_WIN 10
#define PLAY_LOSE 11
#define PIN_CLK 9
#define PIN_DIO 8

TMRpcm tmrpcm;
SevenSegmentTM1637 display(PIN_CLK, PIN_DIO);

int DELAY = 1000;
int gameDuration = 3600;
int wiresValues[4];

void printWires() {
  for (int i = 0; i < WIRES_NUMBER; i++ ) {
    Serial.print(wiresValues[i]);
    Serial.print("\t");
  }
  Serial.println();
}

char parseData(unsigned long n, uint8_t base) {
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);
  return str[0];
}

const char * parseTime(int gameDuration)
{
  static char value[5];
  char m = gameDuration / 60;
  char s = gameDuration % 60;

  char minutes[2] = {0};
  char seconds[2]= {0};

  if (m >= 10)
  {
    minutes[0] = m / 10;
    minutes[1] = m % 10;
  }
  else
  {
    minutes[0] = 0;
    minutes[1] = m % 10;
  }

  if (s >= 10)
  {
    seconds[0] = s / 10;
    seconds[1] = s % 10;
  }
  else
  {
    seconds[0] = 0;
    seconds[1] = s % 10;
  }

  value[0] = parseData(minutes[0],10);
  value[1] = parseData(minutes[1],10);
  value[2] = parseData(seconds[0],10);
  value[3] = parseData(seconds[1],10);
  value[4] = '\0';

  return value;
}

void win(){
  // play win music
  digitalWrite(PLAY_WIN, HIGH);
  while(true) {
    // to do sleep
  }
}
void lose(){
  // play lose music
  digitalWrite(PLAY_LOSE, HIGH);

  DELAY = 1;
  if ( gameDuration == 0 ) {
    display.print("XXXX");
    display.blink();
    delay(10000);
    display.off();
    while(true) {
      // todo sleep
    }
  }
}


void readWires() {
  wiresValues[0] = digitalRead(4);
  wiresValues[1] = digitalRead(5);
  wiresValues[2] = digitalRead(6);
  wiresValues[3] = digitalRead(7);
}

int checkWires() {

  // WIN
  if (wiresValues[WIN_WIRE] == 1 ) {  
    win();
  }
  
  for (int i = 0; i < WIRES_NUMBER; i++ ) {
    if (i != WIN_WIRE && wiresValues[i] == 1 ) {
      lose();
    }
  }
  return(0);
}


// run setup code
void setup()
{
  Serial.begin(9600);
  display.begin();
  display.setBacklight(100); // set the brightness to 100 %

};

void loop()
{
  bool colonOn = false;

  do
  {
    display.print(parseTime(gameDuration));
    display.setColonOn(colonOn);

    readWires();
    printWires();
    checkWires();

    colonOn = !colonOn;
    gameDuration--;
    delay(DELAY);
  } while (gameDuration > 0);
};
