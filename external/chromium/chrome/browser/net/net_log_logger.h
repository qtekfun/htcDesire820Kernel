// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_NET_LOG_LOGGER_H_
#define CHROME_BROWSER_NET_NET_LOG_LOGGER_H_
#pragma once

#include "base/memory/scoped_handle.h"
#include "chrome/browser/net/chrome_net_log.h"

class FilePath;

class NetLogLogger : public ChromeNetLog::ThreadSafeObserver {
 public:
  
  
  
  explicit NetLogLogger(const FilePath &log_path);
  ~NetLogLogger();

  
  virtual void OnAddEntry(net::NetLog::EventType type,
                          const base::TimeTicks& time,
                          const net::NetLog::Source& source,
                          net::NetLog::EventPhase phase,
                          net::NetLog::EventParameters* params);

 private:
  ScopedStdioHandle file_;

  DISALLOW_COPY_AND_ASSIGN(NetLogLogger);
};

#endif  

