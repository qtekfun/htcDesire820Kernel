// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_DISPLAY_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_DISPLAY_OPTIONS_HANDLER_H_

#include <vector>

#include "ash/display/display_controller.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace chromeos {
namespace options {

class DisplayOptionsHandler : public ::options::OptionsPageUIHandler,
                              public ash::DisplayController::Observer {
 public:
  DisplayOptionsHandler();
  virtual ~DisplayOptionsHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializePage() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnDisplayConfigurationChanging() OVERRIDE;
  virtual void OnDisplayConfigurationChanged() OVERRIDE;

 private:
  
  void SendAllDisplayInfo();

  
  void SendDisplayInfo(const std::vector<gfx::Display>& displays);

  
  void OnFadeOutForMirroringFinished(bool is_mirroring);

  
  
  
  
  void OnFadeOutForDisplayLayoutFinished(int layout, int offset);

  
  void HandleDisplayInfo(const base::ListValue* unused_args);
  void HandleMirroring(const base::ListValue* args);
  void HandleSetPrimary(const base::ListValue* args);
  void HandleDisplayLayout(const base::ListValue* args);
  void HandleSetUIScale(const base::ListValue* args);
  void HandleSetResolution(const base::ListValue* args);
  void HandleSetOrientation(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(DisplayOptionsHandler);
};

}  
}  

#endif  
