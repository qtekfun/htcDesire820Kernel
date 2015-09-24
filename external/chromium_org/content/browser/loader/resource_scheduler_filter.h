// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_RESOURCE_SCHEDULER_FILTER_H_
#define CONTENT_BROWSER_LOADER_RESOURCE_SCHEDULER_FILTER_H_

#include "content/public/browser/browser_message_filter.h"

namespace content {

class ResourceSchedulerFilter : public BrowserMessageFilter {
 public:
  explicit ResourceSchedulerFilter(int child_id);

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

 private:
  virtual ~ResourceSchedulerFilter();

  int child_id_;
};

}  

#endif  
