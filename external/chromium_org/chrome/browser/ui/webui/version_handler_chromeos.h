// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_VERSION_HANDLER_CHROMEOS_H_
#define CHROME_BROWSER_UI_WEBUI_VERSION_HANDLER_CHROMEOS_H_

#include <string>

#include "chrome/browser/chromeos/version_loader.h"
#include "chrome/browser/ui/webui/version_handler.h"

class VersionHandlerChromeOS : public VersionHandler {
 public:
  VersionHandlerChromeOS();
  virtual ~VersionHandlerChromeOS();

  
  virtual void HandleRequestVersionInfo(const ListValue* args) OVERRIDE;

  
  void OnVersion(const std::string& version);

 private:
  
  chromeos::VersionLoader loader_;

  
  CancelableTaskTracker tracker_;

  DISALLOW_COPY_AND_ASSIGN(VersionHandlerChromeOS);
};

#endif  
