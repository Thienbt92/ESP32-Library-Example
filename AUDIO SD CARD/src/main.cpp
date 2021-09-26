#include "main.h"

mSD_Class SDcard;
File fil;
WAV_Class WAV;
DAC_Audio_Class Audio(25,0);

uint8_t buffer_Read[2048];
uint32_t  Index_read = 0,Count_Read=0,_pointer=0,_size_read=0;
bool Flag_Ok=false,Start=false;
uint8_t Led_stt=0;

void Read_Data()
{
  SDcard.mSD_seek(&fil,Index_read);
  if(Start==false)
  {
    _size_read = 2048;
    Start = true;
  }
  else
    _size_read = 1024;

  if((WAV.WAV_getSizeData()-Count_Read)>=_size_read)
  {
    SDcard.mSD_read(&fil,buffer_Read,_size_read);
    WAV.WAV_UpdateBuffer(buffer_Read,_pointer,_size_read);
    Count_Read += _size_read;
    Index_read+=_size_read;
  }
  else
  {
    SDcard.mSD_read(&fil,buffer_Read,(WAV.WAV_getSizeData()-Count_Read));
    WAV.WAV_UpdateBuffer(buffer_Read,_pointer,(WAV.WAV_getSizeData()-Count_Read));
    Count_Read += (WAV.WAV_getSizeData()-Count_Read);
    Index_read += (WAV.WAV_getSizeData()-Count_Read);
  }
}

void setup() 
{
  pinMode(LED_BUILTIN,OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (!SDcard.mSD_begin(5)) 
  {
    Serial.println("Card Mount Failed");
    return;
  }
  SDcard.mSD_open(&fil,"/chan ai.wav",FILE_READ);
  if(SDcard.mSD_read(&fil,buffer_Read,WAV.WAV_getSizeHeader())==WAV.WAV_getSizeHeader())
  {
    if(WAV.WAV_Init(buffer_Read,WAV.WAV_getSizeHeader())==true)
    {
      Index_read = WAV.WAV_getSizeHeader();
      Flag_Ok = true;
    }
  }
  //SDcard.mSD_close(&fil);
  digitalWrite(LED_BUILTIN,0);
}

void loop() {
   if(WAV.Completed)
  {
    Audio.DAC_playWav(&WAV);
    Read_Data();
  }

  if(Flag_Ok==true)
  {
    if(WAV.WAV_GetflagUpdate()==true)
    {
      Read_Data();
      WAV.WAV_ClearflagUpdate(false);
      if(_pointer==0)
        _pointer = 1024;
      if(_pointer==1024)
        _pointer = 0;
      digitalWrite(LED_BUILTIN,1);
    }
  } 
  // put your main code here, to run repeatedly:
}