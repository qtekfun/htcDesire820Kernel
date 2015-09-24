// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DOWNLOAD_DESTINATION_OBSERVER_H_
#define CONTENT_PUBLIC_BROWSER_DOWNLOAD_DESTINATION_OBSERVER_H_

#include <string>

#include "content/public/browser/download_interrupt_reasons.h"

namespace content {

class DownloadDestinationObserver {
 public:
  virtual void DestinationUpdate(int64 bytes_so_far,
                                 int64 bytes_per_sec,
                                 const std::string& hash_state) = 0;

  virtual void DestinationError(DownloadInterruptReason reason) = 0;

  virtual void DestinationCompleted(const std::string& final_hash) = 0;
};

}  

#endif  
