#include "process.h"

mSD_Class SDcard;
File fil;
File root;
WAV_Class WAV;
string_process _STRING;
std::string _pathFile="",_pathFileOld="",__file="";
uint16_t _countSearch=0;
DATA_PLAYER  Data_Player;

void Music_PlayMode (MODE_PLAY _mode)
{
  Data_Player._modePlay = (uint8_t)_mode;
  if(_mode==PLAY_NOMAL)
    Serial.println("MODE PLAY NOMAL");
  else
    Serial.println("MODE PLAY LOOP");
}
void Read_Data()
{
  SDcard.mSD_seek(&fil,Data_Player._indexRead,SeekSet);
  if(Data_Player.flag_NewSong==true)
  {
    Data_Player._numByteRead = 2048;
    Data_Player.flag_NewSong = false;
  }
  else
    Data_Player._numByteRead = 1024;

  if((WAV.WAV_getSizeData()-Data_Player._countRead)>=Data_Player._numByteRead)
  {
    SDcard.mSD_read(&fil,Data_Player._bufferRead,Data_Player._numByteRead);
    WAV.WAV_UpdateBuffer(Data_Player._bufferRead,WAV.Pointer_Update,Data_Player._numByteRead);
    Data_Player._countRead += Data_Player._numByteRead;
    Data_Player._indexRead+=Data_Player._numByteRead;
  }
  else
  {
    SDcard.mSD_read(&fil,Data_Player._bufferRead,(WAV.WAV_getSizeData()-Data_Player._countRead));
    WAV.WAV_UpdateBuffer(Data_Player._bufferRead,WAV.Pointer_Update,(WAV.WAV_getSizeData()-Data_Player._countRead));
    Data_Player._countRead += (WAV.WAV_getSizeData()-Data_Player._countRead);
    Data_Player._indexRead += (WAV.WAV_getSizeData()-Data_Player._countRead);
  }
}
void Music_Intit()
{
  if (!SDcard.mSD_begin(5)) 
  {
    Serial.println("Card Mount Failed");
    Data_Player.flag_PlayerReady=false;   // Không đọc được File từ thẻ nhớ sẽ dùng phát nhạc
  }
  else
  {
    // Số lượng File có trong thẻ nhớ
    Data_Player._numFile_inSDCard = SDcard.mSD_listFileCount("/","");
    Serial.print("Number File In SD Card: ");
    Serial.println(Data_Player._numFile_inSDCard);
    // Lấy số lượng file WAV
    Data_Player._numFile_addPlaylist = SDcard.mSD_listFileCount("/",".wav");
    Serial.print("Number File .WAV In SD Card: ");
    Serial.println(Data_Player._numFile_addPlaylist);
    // Nếu có lớn hơn 1 file wav thì mới phát nhạc
    if(Data_Player._numFile_addPlaylist!=0)
    {
      Data_Player.flag_PlayerReady = true;  // Chế độ player sẵn sàng 
      WAV.WAV_Init();
      WAV.DAC_Audio_Init(25,0,&WAV);
      Data_Player.flag_BufferEmpty=true;
      Data_Player._modePlay = (uint8_t)PLAY_NOMAL;
    }
  }
}
void Music_Play()
{
  if(Data_Player.flag_PlayerReady==true)
  {
    if(Data_Player.flag_BufferEmpty==true)
    {
      while((_STRING.String_Compare(_pathFile,_pathFileOld)==0)&&(_countSearch<Data_Player._numFile_inSDCard))
      {
        _countSearch+=1;
        SDcard.mSD_listFile(&root,"/",&__file);
        if(_STRING.SearchArrayInArray(_STRING.String_toChar(__file),".wav",1,_STRING.String_Leng(__file))==true)
          _pathFile = __file;
      }
      if(_countSearch>=Data_Player._numFile_inSDCard)
      {
        Data_Player.flag_PlayerReady = false;
        return;
      }
      _pathFileOld = _pathFile;
      Serial.println(_STRING.String_toChar(_pathFile));
      /*-------------------------------------------------*/
      Data_Player.flag_NewSong=true;
      SDcard.mSD_open(&fil,_STRING.String_toChar(_pathFile),FILE_READ);
      if(SDcard.mSD_read(&fil,Data_Player._bufferRead,WAV.WAV_getSizeHeader())!=0)
      {
        if(WAV.WAV_UpdateHeader(Data_Player._bufferRead,WAV.WAV_getSizeHeader())==true)
        {
          Data_Player._indexRead = WAV.WAV_getSizeHeader();
          Data_Player._countRead=0;
          Data_Player.flag_BufferEmpty = false;
        }
      }
    }
    else
    {
      if(WAV.Play_Finish==true)
      {
        WAV.Play_Finish=false;
        SDcard.mSD_close(&fil);
        if(++Data_Player._countPlaylist >= Data_Player._numFile_addPlaylist)
        {
          Data_Player._countPlaylist = 0;
          if(Data_Player._modePlay == PLAY_LOOP)
            root.close();
          else if(Data_Player._modePlay == PLAY_NOMAL)
            Data_Player.flag_PlayerReady = false;
        }
        Data_Player.flag_BufferEmpty = true;
        _countSearch=0;
      } 
      else
      {
        if(WAV.WAV_Playing==false)
        {
          WAV.DAC_playWav(&WAV,true);
          Read_Data();
        }
        else
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
}