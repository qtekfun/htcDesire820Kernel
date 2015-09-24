// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_SUBPROCESS_H__
#define GOOGLE_PROTOBUF_COMPILER_SUBPROCESS_H__

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN   
#include <windows.h>
#else  
#include <sys/types.h>
#include <unistd.h>
#endif  
#include <google/protobuf/stubs/common.h>

#include <string>


namespace google {
namespace protobuf {

class Message;

namespace compiler {

class Subprocess {
 public:
  Subprocess();
  ~Subprocess();

  enum SearchMode {
    SEARCH_PATH,   
    EXACT_NAME     
  };

  
  
  void Start(const string& program, SearchMode search_mode);

  
  
  
  
  
  bool Communicate(const Message& input, Message* output, string* error);

#ifdef _WIN32
  
  
  static string Win32ErrorMessage(DWORD error_code);
#endif

 private:
#ifdef _WIN32
  DWORD process_start_error_;
  HANDLE child_handle_;

  
  
  HANDLE child_stdin_;
  HANDLE child_stdout_;

#else  
  pid_t child_pid_;

  
  
  int child_stdin_;
  int child_stdout_;

#endif  
};

}  
}  

}  
#endif  
