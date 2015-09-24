// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_METRICS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_METRICS_HANDLER_H_

#include "base/compiler_specific.h"
#include "content/public/browser/web_ui_message_handler.h"



namespace base {
class ListValue;
}

class MetricsHandler : public content::WebUIMessageHandler {
 public:
  MetricsHandler();
  virtual ~MetricsHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  
  void HandleRecordAction(const base::ListValue* args);

  

  
  
  
  
  
  void HandleRecordInHistogram(const base::ListValue* args);

  
  void HandleLogEventTime(const base::ListValue* args);

  
  void HandleLogMouseover(const base::ListValue* args);

 private:
  DISALLOW_COPY_AND_ASSIGN(MetricsHandler);
};

#endif  
