// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_PYTHON_DESCRIPTOR_H__
#define GOOGLE_PROTOBUF_PYTHON_DESCRIPTOR_H__

#include <Python.h>
#include <structmember.h>

#include <google/protobuf/descriptor.h>

#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#endif

namespace google {
namespace protobuf {
namespace python {

typedef struct {
  PyObject_HEAD

  
  const google::protobuf::FieldDescriptor* descriptor;

  
  PyObject* full_name;

  
  PyObject* name;

  
  PyObject* cpp_type;

  
  PyObject* label;

  
  PyObject* id;
} CFieldDescriptor;

extern PyTypeObject CFieldDescriptor_Type;

extern PyTypeObject CDescriptorPool_Type;


PyObject* Python_NewCDescriptorPool(PyObject* ignored, PyObject* args);
PyObject* Python_BuildFile(PyObject* ignored, PyObject* args);
bool InitDescriptor();
google::protobuf::DescriptorPool* GetDescriptorPool();

}  
}  

}  
#endif  
