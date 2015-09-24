// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_LOG_PRIVATE_FILTER_HANDLER_H_
#define CHROME_BROWSER_EXTENSIONS_API_LOG_PRIVATE_FILTER_HANDLER_H_

#include <string>
#include <vector>

#include "chrome/common/extensions/api/log_private.h"

namespace extensions {
class FilterHandler {
 public:
  explicit FilterHandler(const api::log_private::Filter& filter);
  ~FilterHandler();

  
  
  bool IsValidLogEntry(const api::log_private::LogEntry& entry) const;
  
  
  bool IsValidTime(double time) const;
  
  
  bool IsValidSource(const std::string& source) const;
  
  
  bool IsValidLevel(const std::string& level) const;
  
  
  bool IsValidProcess(const std::string& process) const;

  const api::log_private::Filter* GetFilter() const { return &filter_; }
 private:
  api::log_private::Filter filter_;

  DISALLOW_COPY_AND_ASSIGN(FilterHandler);
};

}  

#endif  
