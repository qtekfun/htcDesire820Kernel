// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_PYTHON_PYTHON_PROTOBUF_H__
#define GOOGLE_PROTOBUF_PYTHON_PYTHON_PROTOBUF_H__

#include <Python.h>

namespace google {
namespace protobuf {

class Message;

namespace python {

const Message* GetCProtoInsidePyProto(PyObject* msg);
Message* MutableCProtoInsidePyProto(PyObject* msg);

}  
}  

}  
#endif  
