#include "main.h"


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Music_Intit();
  Music_PlayMode(PLAY_LOOP);
}

void loop() 
{
  Music_Play();
  // put your main code here, to run repeatedly:
}