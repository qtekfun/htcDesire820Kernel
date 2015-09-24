// Copyright (c) 2006, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef _SYSINFO_H_
#define _SYSINFO_H_

#include <config.h>

#include <time.h>
#if (defined(_WIN32) || defined(__MINGW32__)) && (!defined(__CYGWIN__) && !defined(__CYGWIN32__))
#include <windows.h>   
#include <TlHelp32.h>  
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>    
#endif
#include <stddef.h>    
#include <limits.h>    
#include "base/basictypes.h"
#include "base/logging.h"   

extern const char* GetenvBeforeMain(const char* name);

extern bool GetUniquePathFromEnv(const char* env_name, char* path);

extern int NumCPUs();

void SleepForMilliseconds(int milliseconds);

extern double CyclesPerSecond(void);


bool HasPosixThreads();

#ifndef SWIG  

class ProcMapsIterator {
 public:
  struct Buffer {
#ifdef __FreeBSD__
    
    
    static const size_t kBufSize = 102400;
#else   
    static const size_t kBufSize = PATH_MAX + 1024;
#endif
    char buf_[kBufSize];
  };


  
  explicit ProcMapsIterator(pid_t pid);

  
  
  
  ProcMapsIterator(pid_t pid, Buffer *buffer);

  
  
  
  ProcMapsIterator(pid_t pid, Buffer *buffer,
                   bool use_maps_backing);

  
  bool Valid() const;

  
  
  
  
  const char *CurrentLine() const { return stext_; }

  
  // line to the passed-in buffer. Returns the number of bytes written,
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static int FormatLine(char* buffer, int bufsize,
                        uint64 start, uint64 end, const char *flags,
                        uint64 offset, int64 inode, const char *filename,
                        dev_t dev);

  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  bool Next(uint64 *start, uint64 *end, char **flags,
            uint64 *offset, int64 *inode, char **filename);

  bool NextExt(uint64 *start, uint64 *end, char **flags,
               uint64 *offset, int64 *inode, char **filename,
               uint64 *file_mapping, uint64 *file_pages,
               uint64 *anon_mapping, uint64 *anon_pages,
               dev_t *dev);

  ~ProcMapsIterator();

 private:
  void Init(pid_t pid, Buffer *buffer, bool use_maps_backing);

  char *ibuf_;        
  char *stext_;       
  char *etext_;       
  char *nextline_;    
  char *ebuf_;        
#if (defined(_WIN32) || defined(__MINGW32__)) && (!defined(__CYGWIN__) && !defined(__CYGWIN32__))
  HANDLE snapshot_;   
  
  
  
  
  
# ifdef MODULEENTRY32  
#   undef MODULEENTRY32
  MODULEENTRY32 module_;   
#   define MODULEENTRY32 MODULEENTRY32W
# else  
  MODULEENTRY32 module_;   
# endif
#elif defined(__MACH__)
  int current_image_; 
  int current_load_cmd_;   
#elif defined(__sun__)     
  int fd_;
  char current_filename_[PATH_MAX];
#else
  int fd_;            
#endif
  pid_t pid_;
  char flags_[10];
  Buffer* dynamic_buffer_;  
  bool using_maps_backing_; 
};

#endif  


namespace tcmalloc {
int FillProcSelfMaps(char buf[], int size, bool* wrote_all);
void DumpProcSelfMaps(RawFD fd);
}

#endif   
