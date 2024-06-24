#include <Arduino.h>
#include "ps2Keyboard.h"

void setup() 
{
  Serial.begin(9600);
  ps2_init();
}

void loop() 
{
  ps2_check_clock();
  
  unsigned char c = ps2_buf_pull();
  if (c) 
  {
    Serial.write(c);
    Serial.println();
  }
}
