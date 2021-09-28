#include "main.h"

mSD_Class SDcard;
File fil;
File root;
WAV_Class WAV;
String _pathFile="",_pathFileOld="";

uint8_t buffer_Read[2048];
String List_WAV[10];
uint32_t  Index_read = 0,Count_Read=0,_size_read=0,Count_playList=1;
bool Flag_Ok=false,First_Read=false,Start_Run=false;
uint8_t Led_stt=0;

void Read_Data()
{
  SDcard.mSD_seek(&fil,Index_read,SeekSet);
  if(First_Read==false)
  {
    _size_read = 2048;
    First_Read = true;
  }
  else
    _size_read = 1024;

  if((WAV.WAV_getSizeData()-Count_Read)>=_size_read)
  {
    SDcard.mSD_read(&fil,buffer_Read,_size_read);
    WAV.WAV_UpdateBuffer(buffer_Read,WAV.Pointer_Update,_size_read);
    Count_Read += _size_read;
    Index_read+=_size_read;
  }
  else
  {
    SDcard.mSD_read(&fil,buffer_Read,(WAV.WAV_getSizeData()-Count_Read));
    WAV.WAV_UpdateBuffer(buffer_Read,WAV.Pointer_Update,(WAV.WAV_getSizeData()-Count_Read));
    Count_Read += (WAV.WAV_getSizeData()-Count_Read);
    Index_read += (WAV.WAV_getSizeData()-Count_Read);
  }
}

void setup() 
{
  pinMode(16,OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (!SDcard.mSD_begin(5)) 
  {
    Serial.println("Card Mount Failed");
    return;
  }
  Start_Run=true;
  WAV.WAV_Init();
  WAV.DAC_Audio_Init(25,0,&WAV);
  // Lấy địa chỉ file WAV
  //while(strcmp(_pathFile==_pathFileOld)
  {
    SDcard.mSD_listFile(&root,"/",&_pathFile);
  }
  Serial.println(_pathFile[0]);
}

void loop() 
{
/*   if(Start_Run==true)
  {
    SDcard.mSD_open(&fil,SDcard.List_File.pathFile[Count_playList],FILE_READ);
    Serial.println(SDcard.List_File.pathFile[Count_playList]);
    if(SDcard.mSD_read(&fil,buffer_Read,WAV.WAV_getSizeHeader())!=0)
    {
      if(WAV.WAV_UpdateHeader(buffer_Read,WAV.WAV_getSizeHeader())==true)
      {
        Index_read = WAV.WAV_getSizeHeader();
        Serial.println(WAV.WAV_getSizeData());
        Flag_Ok = true;
        Start_Run = false;
      }
    }
  }
  if(WAV.WAV_Playing==false)
  {
    WAV.DAC_playWav(&WAV,true);
    Read_Data();
  }
  else
  {
    if(Flag_Ok==true)
    {
      if(WAV.WAV_GetflagUpdate()==true)
      {
        Read_Data();
        WAV.WAV_ClearflagUpdate(false);
      }
    }
  }
  if(WAV.Play_Finish==true)
  {
    
    WAV.Play_Finish=false;
    SDcard.mSD_close(&fil);
    Start_Run = true;
    if(++Count_playList>=SDcard.List_File.NumberFile)
      Count_playList = 0;
  }  */
  // put your main code here, to run repeatedly:
}