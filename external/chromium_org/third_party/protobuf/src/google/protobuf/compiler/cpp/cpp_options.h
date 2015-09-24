// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_OPTIONS_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_OPTIONS_H__

#include <string>

#include <google/protobuf/stubs/common.h>
namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {

struct Options {
  Options() : safe_boundary_check(false) {
  }
  string dllexport_decl;
  bool safe_boundary_check;
};

}  
}  
}  


}  
#endif  
