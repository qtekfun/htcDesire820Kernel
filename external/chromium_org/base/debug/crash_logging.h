// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_CRASH_LOGGING_H_
#define BASE_DEBUG_CRASH_LOGGING_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/strings/string_piece.h"


namespace base {
namespace debug {

class StackTrace;

BASE_EXPORT void SetCrashKeyValue(const base::StringPiece& key,
                                  const base::StringPiece& value);
BASE_EXPORT void ClearCrashKey(const base::StringPiece& key);

BASE_EXPORT void SetCrashKeyToStackTrace(const base::StringPiece& key,
                                         const StackTrace& trace);

BASE_EXPORT void SetCrashKeyFromAddresses(const base::StringPiece& key,
                                          const void* const* addresses,
                                          size_t count);

class BASE_EXPORT ScopedCrashKey {
 public:
  ScopedCrashKey(const base::StringPiece& key, const base::StringPiece& value);
  ~ScopedCrashKey();

 private:
  std::string key_;

  DISALLOW_COPY_AND_ASSIGN(ScopedCrashKey);
};

struct BASE_EXPORT CrashKey {
  
  const char* key_name;

  
  
  
  
  size_t max_length;
};

BASE_EXPORT size_t InitCrashKeys(const CrashKey* const keys, size_t count,
                                 size_t chunk_max_length);

BASE_EXPORT const CrashKey* LookupCrashKey(const base::StringPiece& key);

typedef void (*SetCrashKeyValueFuncT)(const base::StringPiece&,
                                      const base::StringPiece&);
typedef void (*ClearCrashKeyValueFuncT)(const base::StringPiece&);

BASE_EXPORT void SetCrashKeyReportingFunctions(
    SetCrashKeyValueFuncT set_key_func,
    ClearCrashKeyValueFuncT clear_key_func);

BASE_EXPORT std::vector<std::string> ChunkCrashKeyValue(
    const CrashKey& crash_key,
    const base::StringPiece& value,
    size_t chunk_max_length);

BASE_EXPORT void ResetCrashLoggingForTesting();

}  
}  

#endif  
