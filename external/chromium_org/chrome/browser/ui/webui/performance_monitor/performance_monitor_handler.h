// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
class Time;
class Value;
}  

namespace performance_monitor {

class PerformanceMonitorHandler
    : public content::WebUIMessageHandler,
      public base::SupportsWeakPtr<PerformanceMonitorHandler> {
 public:
  PerformanceMonitorHandler();

 private:
  virtual ~PerformanceMonitorHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void ReturnResults(const std::string& callback, const base::Value* results);

  
  
  void HandleGetActiveIntervals(const base::ListValue* args);

  
  
  void HandleGetFlagEnabled(const base::ListValue* args);

  
  
  void HandleGetAggregationTypes(const base::ListValue* args);

  
  
  void HandleGetEventTypes(const base::ListValue* args);

  
  
  void HandleGetEvents(const base::ListValue* args);

  
  
  void HandleGetMetricTypes(const base::ListValue* args);

  
  
  
  
  void HandleGetMetrics(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(PerformanceMonitorHandler);
};

}  

#endif  
