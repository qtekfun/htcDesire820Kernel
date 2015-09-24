// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_TESTING_FILE_H__
#define GOOGLE_PROTOBUF_TESTING_FILE_H__

#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {

const int DEFAULT_FILE_MODE = 0777;

class File {
 public:
  
  static bool Exists(const string& name);

  
  
  static bool ReadFileToString(const string& name, string* output);

  
  static void ReadFileToStringOrDie(const string& name, string* output);

  
  static void WriteStringToFileOrDie(const string& contents,
                                     const string& name);

  
  static bool CreateDir(const string& name, int mode);

  
  static bool RecursivelyCreateDir(const string& path, int mode);

  
  
  
  
  
  static void DeleteRecursively(const string& name,
                                void* dummy1, void* dummy2);

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(File);
};

}  
}  

#endif  
