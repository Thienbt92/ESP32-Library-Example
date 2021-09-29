#include "main.h"

mSD_Class SDcard;
File fil;
File root;
WAV_Class WAV;
string_process _STRING;

std::string _pathFile="",_pathFileOld="";

uint8_t buffer_Read[2048];
String List_WAV[10];
uint32_t  Index_read = 0,Count_Read=0,_size_read=0,Count_playList=0,NumFile_Playlist=0;
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
  NumFile_Playlist = SDcard.mSD_listFileCount("/");
  Serial.println(NumFile_Playlist);
}

void loop() 
{
  if(Start_Run==true)
  {
    Serial.println("Start Run");
    while(_STRING.String_Compare(_pathFile,_pathFileOld)==0)
    {
      SDcard.mSD_listFile(&root,"/",&_pathFile);
    }
    _pathFileOld = _pathFile;
    Serial.println(_STRING.String_toChar(_pathFile));
    SDcard.mSD_open(&fil,_STRING.String_toChar(_pathFile),FILE_READ);
    if(SDcard.mSD_read(&fil,buffer_Read,WAV.WAV_getSizeHeader())!=0)
    {
      Serial.println("Read OK");
      if(WAV.WAV_UpdateHeader(buffer_Read,WAV.WAV_getSizeHeader())==true)
      {
        Index_read = WAV.WAV_getSizeHeader();
        Count_Read=0;
        Flag_Ok = true;
        Start_Run = false;
      }
    }
  }
  else
  {
    if(WAV.Play_Finish==true)
    {
      WAV.Play_Finish=false;
      SDcard.mSD_close(&fil);
      delay(50);
      if(++Count_playList>=NumFile_Playlist)
      {
        Count_playList = 0;
        root.close();
      }
      Start_Run = true;
      Serial.print("Next List: ");
      Serial.println(Start_Run);
    } 
    else
    {
      if(WAV.WAV_Playing==false)
      {
        Serial.print("Playing: ");
        Serial.println(Start_Run);
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
    }
  } 
  // put your main code here, to run repeatedly:
}