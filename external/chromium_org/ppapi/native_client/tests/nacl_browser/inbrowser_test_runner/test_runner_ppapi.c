/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "native_client/tests/inbrowser_test_runner/test_runner.h"

#include <stdio.h>
#include <string.h>

#include "ppapi/native_client/src/shared/ppapi_proxy/ppruntime.h"

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_messaging.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppp.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppp_messaging.h"


static int (*g_test_func)(void);

static PPB_GetInterface g_get_browser_interface;
const static PPB_Messaging *g_browser_messaging;
const static PPB_Var *g_browser_var;


int RunTests(int (*test_func)(void)) {
  
  setvbuf(stdout, NULL, _IONBF, 0);
  if (getenv("OUTSIDE_BROWSER") != NULL) {
    return test_func();
  } else {
    g_test_func = test_func;
    return PpapiPluginMain();
  }
}

int TestRunningInBrowser(void) {
  return 1;
}

static PP_Bool PppInstanceDidCreate(PP_Instance instance,
                                    uint32_t argc,
                                    const char *argn[],
                                    const char *argv[]) {
  g_browser_messaging = (*g_get_browser_interface)(PPB_MESSAGING_INTERFACE);
  g_browser_var = (*g_get_browser_interface)(PPB_VAR_INTERFACE);
  return PP_TRUE;
}

static void PppInstanceDidDestroy(PP_Instance instance) {
  
}

static void PppInstanceDidChangeView(PP_Instance instance, PP_Resource view) {
  
}

void PppInstanceDidChangeFocus(PP_Instance instance, PP_Bool has_focus) {
  
}

PP_Bool PppInstanceHandleDocumentLoad(PP_Instance instance,
                                      PP_Resource url_loader) {
  
  return PP_FALSE;
}

static const PPP_Instance kPppInstance = {
  PppInstanceDidCreate,
  PppInstanceDidDestroy,
  PppInstanceDidChangeView,
  PppInstanceDidChangeFocus,
  PppInstanceHandleDocumentLoad
};

static void PppMessagingHandleMessage(PP_Instance instance,
                                      struct PP_Var message) {
  const char *data;
  uint32_t len;
  static const char kStartMessage[] = "run_tests";
  int num_fails;
  struct PP_Var result;

  
  data = g_browser_var->VarToUtf8(message, &len);
  if (len == 0) {
    return;
  }
  if (strcmp(data, kStartMessage) != 0) {
    return;
  }
  
  num_fails = (*g_test_func)();
  
  if (num_fails == 0) {
    static const char kPassed[] = "passed";
    result = g_browser_var->VarFromUtf8(kPassed, strlen(kPassed));
  } else {
    static const char kFailed[] = "failed";
    result = g_browser_var->VarFromUtf8(kFailed, strlen(kFailed));
  }

  fprintf(stderr, "\nEND_OF_LOG\n");

  g_browser_messaging->PostMessage(instance, result);
}

static const PPP_Messaging kPppMessaging = {
  PppMessagingHandleMessage
};

int32_t PPP_InitializeModule(PP_Module module,
                             PPB_GetInterface get_browser_interface) {
  g_get_browser_interface = get_browser_interface;
  return PP_OK;
}

void PPP_ShutdownModule(void) {
}

const void *PPP_GetInterface(const char *interface_name) {
  
  if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0) {
    return (const void*) &kPppInstance;
  }
  if (strcmp(interface_name, PPP_MESSAGING_INTERFACE) == 0) {
    return (const void*) &kPppMessaging;
  }
  return NULL;
}
