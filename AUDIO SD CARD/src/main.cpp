#include "main.h"

mSD_Class SDcard;
File fil;
uint8_t buf[20];
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (!SDcard.mSD_begin(5)) 
  {
    Serial.println("Card Mount Failed");
    return;
  }
  SDcard.mSD_open(&fil,"/chan ai.wav",FILE_READ);
  SDcard.mSD_seek(&fil,10);
  SDcard.mSD_read(&fil,buf,20);
  SDcard.mSD_close(&fil);
  for(int i=0;i<20;i++)
  {
    Serial.print(buf[i],HEX);
    Serial.print(' ');
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}