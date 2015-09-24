// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_LOGGING_H_
#define CHROME_TEST_CHROMEDRIVER_LOGGING_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/values.h"
#include "chrome/test/chromedriver/chrome/log.h"

struct Capabilities;
class DevToolsEventListener;
class ListValue;
class Status;

class WebDriverLog : public Log {
 public:
  static const char kBrowserType[];
  static const char kDriverType[];
  static const char kPerformanceType[];

  
  static bool NameToLevel(const std::string& name, Level* out_level);

  
  WebDriverLog(const std::string& type, Level min_level);
  virtual ~WebDriverLog();

  
  
  
  
  scoped_ptr<base::ListValue> GetAndClearEntries();

  
  virtual void AddEntryTimestamped(const base::Time& timestamp,
                                   Level level,
                                   const std::string& source,
                                   const std::string& message) OVERRIDE;

  const std::string& type() const;
  void set_min_level(Level min_level);
  Level min_level() const;

 private:
  const std::string type_;  
  Level min_level_;  
  scoped_ptr<base::ListValue> entries_;  

  DISALLOW_COPY_AND_ASSIGN(WebDriverLog);
};

bool InitLogging();

Status CreateLogs(const Capabilities& capabilities,
                  ScopedVector<WebDriverLog>* out_logs,
                  ScopedVector<DevToolsEventListener>* out_listeners);

#endif  
