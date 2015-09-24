// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_ABOUT_PAGE_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_ABOUT_PAGE_HANDLER_H_

#include <string>

#include "chrome/browser/ui/webui/options/options_ui.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/cros/update_library.h"
#include "chrome/browser/chromeos/version_loader.h"
#endif

class AboutPageHandler : public OptionsPageUIHandler {
 public:
  AboutPageHandler();
  virtual ~AboutPageHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void RegisterMessages();

 private:
  
  void PageReady(const ListValue* args);

  
  
  void SetReleaseTrack(const ListValue* args);

#if defined(OS_CHROMEOS)
  
  void CheckNow(const ListValue* args);

  
  void RestartNow(const ListValue* args);

  
  void OnOSVersion(chromeos::VersionLoader::Handle handle,
                   std::string version);
  void OnOSFirmware(chromeos::VersionLoader::Handle handle,
                    std::string firmware);
  void UpdateStatus(const chromeos::UpdateLibrary::Status& status);

  
  static void UpdateSelectedChannel(void* user_data, const char* channel);

  
  chromeos::VersionLoader loader_;

  
  CancelableRequestConsumer consumer_;

  
  class UpdateObserver;
  scoped_ptr<UpdateObserver> update_observer_;

  int progress_;
  bool sticky_;
  bool started_;
#endif

  DISALLOW_COPY_AND_ASSIGN(AboutPageHandler);
};

#endif  
