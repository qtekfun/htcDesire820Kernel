/*
 * libjingle
 * Copyright 2004--2006, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TALK_BASE_WIN32FILESYSTEM_H__
#define _TALK_BASE_WIN32FILESYSTEM_H__

#include "fileutils.h"

namespace talk_base {

class Win32Filesystem : public FilesystemInterface {
 public:
  
  
  virtual FileStream *OpenFile(const Pathname &filename, 
                               const std::string &mode);

  
  
  virtual bool CreatePrivateFile(const Pathname &filename);

  
  
  virtual bool DeleteFile(const Pathname &filename);

  
  
  virtual bool DeleteEmptyFolder(const Pathname &folder);

  
  
  
  virtual bool CreateFolder(const Pathname &pathname);
  
  
  
  
  
  virtual bool MoveFile(const Pathname &old_path, const Pathname &new_path);
  
  
  
  
  virtual bool MoveFolder(const Pathname &old_path, const Pathname &new_path);
  
  
  
  virtual bool CopyFile(const Pathname &old_path, const Pathname &new_path);

  
  virtual bool IsFolder(const Pathname& pathname);
  
  
  virtual bool IsFile(const Pathname &path);

  
  
  virtual bool IsAbsent(const Pathname& pathname);

  
  virtual bool IsTemporaryPath(const Pathname& pathname);

  
  
  
  

  virtual std::string TempFilename(const Pathname &dir, const std::string &prefix);

  virtual bool GetFileSize(const Pathname& path, size_t* size);
  virtual bool GetFileTime(const Pathname& path, FileTimeType which,
                           time_t* time);
 
  
  
  virtual bool GetTemporaryFolder(Pathname &path, bool create,
                                 const std::string *append);

  
  virtual bool GetAppPathname(Pathname* path);

  virtual bool GetAppDataFolder(Pathname* path, bool per_user);

  
  virtual bool GetAppTempFolder(Pathname* path);

  virtual bool GetDiskFreeSpace(const Pathname& path, int64 *freebytes);

  virtual Pathname GetCurrentDirectory();
};

}  

#endif  
