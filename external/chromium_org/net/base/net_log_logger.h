// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_LOG_LOGGER_H_
#define NET_BASE_NET_LOG_LOGGER_H_

#include <stdio.h>

#include "base/memory/scoped_handle.h"
#include "net/base/net_log.h"

namespace base {
class FilePath;
class Value;
}

namespace net {

class NET_EXPORT NetLogLogger : public NetLog::ThreadSafeObserver {
 public:
  
  
  
  NetLogLogger(FILE* file, const base::Value& constants);
  virtual ~NetLogLogger();

  
  
  void StartObserving(NetLog* net_log);

  
  void StopObserving();

  
  virtual void OnAddEntry(const NetLog::Entry& entry) OVERRIDE;

  
  
  static base::DictionaryValue* GetConstants();

 private:
  ScopedStdioHandle file_;

  
  bool added_events_;

  DISALLOW_COPY_AND_ASSIGN(NetLogLogger);
};

}  

#endif  
