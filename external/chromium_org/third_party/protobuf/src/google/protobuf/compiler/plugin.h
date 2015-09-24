// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

// Front-end for protoc code generator plugins written in C++.

#ifndef GOOGLE_PROTOBUF_COMPILER_PLUGIN_H__
#define GOOGLE_PROTOBUF_COMPILER_PLUGIN_H__

#include <google/protobuf/stubs/common.h>
namespace google {
namespace protobuf {
namespace compiler {

class CodeGenerator;    

LIBPROTOC_EXPORT int PluginMain(int argc, char* argv[], const CodeGenerator* generator);

}  
}  

}  
#endif  
