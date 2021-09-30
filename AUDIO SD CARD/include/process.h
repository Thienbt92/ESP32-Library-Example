#ifndef _PROCESS_H
#define _PROCESS_H

#include "main.h"

#define SIZE_BUFFER_READ      2048
typedef struct
{
  /****** Đọc File Thẻ Nhớ ***********/
  uint8_t   _bufferRead[SIZE_BUFFER_READ];      // Bộ đệm chứa dữ liệu đọc từ thẻ nhớ 
  uint32_t  _indexRead;                         // Vị trí đọc trong file WAV
  uint32_t  _countRead;                         // Số byte đã đọc từ thẻ nhớ
  uint32_t  _numByteRead;                       // Số byte cần đọc từ thẻ nhớ
  /****** Điều khiển phát nhạc *********/
  uint16_t  _countPlaylist;                     // Đếm số bài hát được phát
  uint16_t  _numFile_addPlaylist;               // Số file nhạc được thêm vào danh sách
  uint16_t  _numFile_inSDCard;                  // Số file có trong thẻ nhớ
  uint8_t   _modePlay;                          // Chế độ phát nhạc: Thường hay vòng lặp
  /****** Cờ báo ********/
  bool flag_NewSong;                           // Báo bài hát mới
  bool flag_BufferEmpty;                       // Báo bộ nhớ buffer trống
  bool flag_PlayerReady;                       // Báo trình phát nhạc đã sẵn sàng.
}DATA_PLAYER;
extern DATA_PLAYER  Data_Player;

typedef enum
{
  PLAY_NOMAL    = 0x00,
  PLAY_LOOP     = 0x01,
}MODE_PLAY;

void Music_PlayMode (MODE_PLAY _mode);
void Read_Data();
void Music_Intit();
void Music_Play();

#endif