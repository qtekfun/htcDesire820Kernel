// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_NPOBJECT_LIFETIME_TEST_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_NPOBJECT_LIFETIME_TEST_H_

#include "base/compiler_specific.h"
#include "build/build_config.h"
#include "content/test/plugin/plugin_test.h"

namespace NPAPIClient {

class NPObjectLifetimeTest : public PluginTest {
 public:
  
  NPObjectLifetimeTest(NPP id, NPNetscapeFuncs *host_functions);

  
  virtual NPError SetWindow(NPWindow* pNPWindow) OVERRIDE;

  virtual void  URLNotify(const char* url,
                          NPReason reason,
                          void* data) OVERRIDE;

 protected:
  NPObject* other_plugin_instance_object_;

#if defined(OS_WIN)
  static void CALLBACK TimerProc(HWND window, UINT message, UINT_PTR timer_id,
                                 DWORD elapsed_milli_seconds);
  UINT_PTR timer_id_;
#endif
  DISALLOW_IMPLICIT_CONSTRUCTORS(NPObjectLifetimeTest);
};

class NPObjectLifetimeTestInstance2 : public PluginTest {
 public:
  
  NPObjectLifetimeTestInstance2(NPP id, NPNetscapeFuncs *host_functions);
  virtual ~NPObjectLifetimeTestInstance2();

  
  virtual NPError SetWindow(NPWindow* pNPWindow) OVERRIDE;
 protected:
  static NPObject* plugin_instance_object_;
  friend class NPObjectLifetimeTest;

  DISALLOW_IMPLICIT_CONSTRUCTORS(NPObjectLifetimeTestInstance2);
};

class NPObjectDeletePluginInNPN_Evaluate : public PluginTest {
 public:
  
  NPObjectDeletePluginInNPN_Evaluate(NPP id, NPNetscapeFuncs *host_functions);
  virtual ~NPObjectDeletePluginInNPN_Evaluate();

  
  virtual NPError SetWindow(NPWindow* pNPWindow) OVERRIDE;

 protected:
  NPObject* plugin_instance_object_;
#if defined(OS_WIN)
  static void CALLBACK TimerProc(HWND window, UINT message, UINT_PTR timer_id,
                                 DWORD elapsed_milli_seconds);
  UINT_PTR timer_id_;
#endif

 private:
  static NPObjectDeletePluginInNPN_Evaluate* g_npn_evaluate_test_instance_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(NPObjectDeletePluginInNPN_Evaluate);
};

}  

#endif  
