/*
 * Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_UTILITY_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_UTILITY_H_

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/portability.h"
#include "native_client/src/shared/platform/nacl_threads.h"
#include "native_client/src/shared/platform/nacl_time.h"

#define SRPC_PLUGIN_DEBUG 1

namespace plugin {

bool IsValidIdentifierString(const char* strval, uint32_t* length);

extern int gNaClPluginDebugPrintEnabled;
extern FILE* gNaClPluginLogFile;
extern int NaClPluginPrintLog(const char *format, ...);
extern int NaClPluginDebugPrintCheckEnv();
extern FILE* NaClPluginLogFileEnv();
#if SRPC_PLUGIN_DEBUG
#define INIT_PLUGIN_LOGGING() do {                                    \
    if (-1 == ::plugin::gNaClPluginDebugPrintEnabled) {               \
      ::plugin::gNaClPluginDebugPrintEnabled =                        \
          ::plugin::NaClPluginDebugPrintCheckEnv();                   \
      ::plugin::gNaClPluginLogFile = ::plugin::NaClPluginLogFileEnv();\
    }                                                                 \
} while (0)

#define PLUGIN_PRINTF(args) do {                                      \
    INIT_PLUGIN_LOGGING();                                            \
    if (0 != ::plugin::gNaClPluginDebugPrintEnabled) {                \
      ::plugin::NaClPluginPrintLog("PLUGIN %" NACL_PRIu64 ": ",       \
                                   NaClGetTimeOfDayMicroseconds());   \
      ::plugin::NaClPluginPrintLog args;                              \
    }                                                                 \
  } while (0)

#define MODULE_PRINTF(args) do {                                      \
    INIT_PLUGIN_LOGGING();                                            \
    if (0 != ::plugin::gNaClPluginDebugPrintEnabled) {                \
      ::plugin::NaClPluginPrintLog("MODULE: ");                       \
      ::plugin::NaClPluginPrintLog args;                              \
    }                                                                 \
  } while (0)
#else
#  define PLUGIN_PRINTF(args) do { if (0) { printf args; } } while (0)
#  define MODULE_PRINTF(args) do { if (0) { printf args; } } while (0)
#endif

}  

#endif  
