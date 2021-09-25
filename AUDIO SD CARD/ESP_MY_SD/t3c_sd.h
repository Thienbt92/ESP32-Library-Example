class mSD_Class
{
private:
    /* data */
    bool debugView = false;
public:
    bool mSD_begin(uint8_t ssPin, SPIClass &spi, uint32_t frequency, const char * mountpoint, uint8_t max_files,bool format_if_empty);
    uint8_t mSD_cardType();
    void mSD_listDirSerial(const char * dirname, uint8_t levels);
    bool mSD_createDir(const char * path);
    bool mSD_removeDir(const char * path);
    bool mSD_writeFileString(const char * path, const char * message);
    bool mSD_appendFile(const char * path, const char * message);
    bool mSD_renameFile(const char * path1, const char * path2);
    bool mSD_deleteFile(const char * path);
    /*-------------------------------------------------------*/
    bool mSD_open(File  *file,const char * path,const char* mode);
    bool mSD_read(File  *file,uint8_t *buffer,uint32_t _size);
    bool mSD_seek(File *file,uint32_t pos);
    void mSD_Class::mSD_close(File  *file);
};
