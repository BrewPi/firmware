/* 
 * File:   FlashIO.h
 * Author: mat
 *
 * Created on 22 June 2014, 00:20
 */

#ifndef FLASHIO_H
#define	FLASHIO_H

#include "ffconf.h"
#include "ff.h"

#define MAX_PATH_LEN 64

class FSObject {
protected:
    const TCHAR* path;
public:

    FSObject(const TCHAR* path_) : path(path_) {}
        
#if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    FRESULT set_timestamp (int year, int month, int mday, int hour, int min, int sec) {        
        FILINFO fno;
        fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
        fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);
        return f_utime(path, &fno);
    }
#endif    
    
};

class FSFile : public FSObject {
    FIL fil;
        
protected:
    static char* make_path(const TCHAR* path, const TCHAR* file, TCHAR buf[MAX_PATH_LEN]) {
        buf = strcpy(buf, path);
        buf += strlen(path);
        *buf++ = TCHAR('\\');
        strcpy(buf, file);
        return buf;
    }
    
    inline char* fqpn(const TCHAR* file, char buf[MAX_PATH_LEN]) {
        return make_path(path, file, buf);
    }
    
    friend class FSDir;
    
public:
    ~FSFile() {
        close();
    }
    
    FRESULT open(BYTE mode) {
        return f_open(&fil, path, mode);
    }
    
    FRESULT close() {
        return f_close(&fil);
    }
    
    FRESULT read(void* buf, UINT count, UINT* result) {
        return f_read(&fil, buf, count, result);
    }
    
#if _FS_READONLY == 0    
    FRESULT write(const void* buf, UINT count, UINT* result) {
        return f_write(&fil, buf, count, result);
    }
#endif    
    
#if  _FS_MINIMIZE <= 2    
    FRESULT seek(DWORD offset) {
        return f_lseek(&fil, offset);
    }
#endif    
    
#if _FS_READONLY == 0 && _FS_MINIMIZE == 0    
    FRESULT truncate() {
        return f_truncate(&fil);
    }
#endif    

#if _FS_READONLY == 0    
    FRESULT sync() {
        return f_sync(&fil);
    }
#endif    
    
#if _USE_FORWARD == 1 && _FS_TINY == 1    
    /**
     * Streams the file contents to the given callback. 
     */
    FRESULT streamTo(UINT (*func)(const BYTE*,UINT), UINT count, UINT* pCount) {
        return f_forward(&fil, func, count, pCount);
    }    
#endif
    
    DWORD tell() {
        return f_tell(&fil);
    }
    
    bool eof() {
        return f_eof(&fil);
    }
    
    DWORD size() {
        return f_size(&fil);
    }
    
    bool error() {
        return f_error(&fil);
    }
};





#if _FS_MINIMIZE <= 1

class FSDir : FSObject {
    DIR dir;
    
public:    
    FSDir(const TCHAR* path) : FSObject(path) {}
    
    FRESULT open() {
        return f_opendir(&dir, path);
    }
    
    FRESULT close() {
        return f_closedir(&dir);
    }
    
#if _FS_READONLY == 0    
    static FSDir* mkdir(const TCHAR* path, FRESULT* result) {
        FRESULT fr = f_mkdir(path);
        if (result) 
            *result = fr;
        return fr==FR_OK ? new FSDir(path) : NULL;
    }
#endif    
    
    friend class FSDirList;
};

class FSDirList {
    FSDir& dir;
    FILINFO fno;
        
private:
    
    FSDirList(FSDir& dir_) : dir(dir) {}
    
    bool next() {
        return f_readdir(&dir.dir, &fno)!=FR_OK || !*fno.fname;        
    }
    
    const FILINFO& dirinfo() {
        return fno;
    }
};

#endif // _FS_MINIMIZE <= 1

class FSVolume {
   
public:    
#if _FS_MINIMIZE == 0
    FRESULT stat(const TCHAR* path, FILINFO* info) {
        return f_stat(path, info);
    }    
#endif

#if _FS_READONLY == 0    
    FRESULT mkdir(const TCHAR* path) {
        return f_mkdir(path);
    }    
#endif

#if _FS_READONLY == 0 && _FS_MINIMIZE == 0
    FRESULT unlink(const TCHAR* path) {
        return f_unlink(path);
    }    
    
    FRESULT chmod(const TCHAR* path, BYTE attr, BYTE mask) {
        return f_chmod(path, attr, mask);
    }    
    
    FRESULT rename(const TCHAR* old_name, /* [IN] Old object name */
        const TCHAR* new_name  /* [IN] New object name */) {
        return f_rename(old_name, new_name);
    }
    
    FRESULT getfree(DWORD* bytes, DWORD* total) {
        FATFS* fs;
        DWORD free = 0;
        DWORD max = 0;
        FRESULT fr;
        if (FR_OK==(fr=f_getfree("", &free, &fs))) {
            free *= fs->csize << 9;
            max = (fs->n_fatent - 2) * fs->csize << 9;
        }    
        if (bytes)
            *bytes = free;
        if (total)
            *total = max;
        return fr;
    }
#endif    // _FS_READONLY == 0 && _FS_MINIMIZE == 0
    
};

namespace Flashee {
    class FlashDevice;
    
    enum FormatCmd {
        FORMAT_CMD_NONE,
        FORMAT_CMD_FORMAT,
        FORMAT_CMD_FORMAT_IF_NEEDED        
    };

    FRESULT f_setFlashDevice(FlashDevice* device, FATFS* pfs, FormatCmd cmd=FORMAT_CMD_FORMAT_IF_NEEDED);    
}


#endif	/* FLASHIO_H */

