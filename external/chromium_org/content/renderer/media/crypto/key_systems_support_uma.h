// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_CRYPTO_KEY_SYSTEMS_SUPPORT_UMA_H_
#define CONTENT_RENDERER_MEDIA_CRYPTO_KEY_SYSTEMS_SUPPORT_UMA_H_

#include <string>

#include "base/containers/scoped_ptr_hash_map.h"

namespace content {

class KeySystemsSupportUMA {
 public:
  KeySystemsSupportUMA();
  ~KeySystemsSupportUMA();

  
  
  
  void AddKeySystemToReport(const std::string& key_system);

  
  
  void ReportKeySystemQuery(const std::string& key_system, bool has_type);

  
  
  
  void ReportKeySystemSupport(const std::string& key_system, bool has_type);

 private:
  class Reporter;

  
  
  Reporter* GetReporter(const std::string& key_system);

  
  typedef base::ScopedPtrHashMap<std::string, Reporter> Reporters;
  Reporters reporters_;
};

}  

#endif  
