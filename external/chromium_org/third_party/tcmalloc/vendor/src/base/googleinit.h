// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef _GOOGLEINIT_H
#define _GOOGLEINIT_H

#include "base/logging.h"

class GoogleInitializer {
 public:
  typedef void (*VoidFunction)(void);
  GoogleInitializer(const char* name, VoidFunction ctor, VoidFunction dtor)
      : name_(name), destructor_(dtor) {
    RAW_VLOG(10, "<GoogleModuleObject> constructing: %s\n", name_);
    if (ctor)
      ctor();
  }
  ~GoogleInitializer() {
    RAW_VLOG(10, "<GoogleModuleObject> destroying: %s\n", name_);
    if (destructor_)
      destructor_();
  }

 private:
  const char* const name_;
  const VoidFunction destructor_;
};

#define REGISTER_MODULE_INITIALIZER(name, body)                 \
  namespace {                                                   \
    static void google_init_module_##name () { body; }          \
    GoogleInitializer google_initializer_module_##name(#name,   \
            google_init_module_##name, NULL);                   \
  }

#define REGISTER_MODULE_DESTRUCTOR(name, body)                  \
  namespace {                                                   \
    static void google_destruct_module_##name () { body; }      \
    GoogleInitializer google_destructor_module_##name(#name,    \
            NULL, google_destruct_module_##name);               \
  }


#endif 
