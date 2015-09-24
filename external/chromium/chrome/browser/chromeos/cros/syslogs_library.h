// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_SYSLOGS_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_SYSLOGS_LIBRARY_H_
#pragma once

#include "base/memory/singleton.h"
#include "content/browser/cancelable_request.h"
#include "third_party/cros/chromeos_syslogs.h"

class CancelableRequestConsumerBase;

namespace chromeos {

class SyslogsLibrary : public CancelableRequestProvider {
 public:
  typedef Callback2<LogDictionaryType*,
                    std::string*>::Type ReadCompleteCallback;

  SyslogsLibrary() {}
  virtual ~SyslogsLibrary() {}

  
  
  
  
  
  virtual Handle RequestSyslogs(
      bool compress_logs, bool feedback_context,
      CancelableRequestConsumerBase* consumer,
      ReadCompleteCallback* callback) = 0;

  
  
  static SyslogsLibrary* GetImpl(bool stub);
};

}  

#endif  
