// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_STATS_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_STATS_OPTIONS_HANDLER_H_
#pragma once

#include "chrome/browser/ui/webui/options/chromeos/cros_options_page_ui_handler.h"

namespace chromeos {

class MetricsCrosSettingsProvider;

class StatsOptionsHandler : public CrosOptionsPageUIHandler {
 public:
  StatsOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Initialize();

  
  virtual void RegisterMessages();

 private:
  MetricsCrosSettingsProvider* provider() const;
  void HandleMetricsReportingCheckbox(const ListValue* args);
  void SetupMetricsReportingCheckbox(bool user_changed);

  DISALLOW_COPY_AND_ASSIGN(StatsOptionsHandler);
};

}  

#endif  
