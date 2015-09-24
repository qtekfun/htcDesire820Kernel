// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_HELP_HELP_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_HELP_HELP_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/help/version_updater.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"

#if defined(OS_CHROMEOS)
#include "base/platform_file.h"
#include "chrome/browser/chromeos/version_loader.h"
#endif  

namespace content {
class WebUIDataSource;
}

class HelpHandler : public content::WebUIMessageHandler,
                    public content::NotificationObserver {
 public:
  HelpHandler();
  virtual ~HelpHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void GetLocalizedValues(content::WebUIDataSource* source);

  
  virtual void Observe(int type, const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  void OnPageLoaded(const base::ListValue* args);

#if defined(OS_MACOSX)
  
  void PromoteUpdater(const base::ListValue* args);
#endif

  
  void RelaunchNow(const base::ListValue* args);

  
  void OpenFeedbackDialog(const base::ListValue* args);

  
  void OpenHelpPage(const base::ListValue* args);

#if defined(OS_CHROMEOS)
  
  void SetChannel(const base::ListValue* args);

  
  void RelaunchAndPowerwash(const base::ListValue* args);
#endif

  
  void SetUpdateStatus(VersionUpdater::Status status, int progress,
                       const base::string16& fail_message);

#if defined(OS_MACOSX)
  
  void SetPromotionState(VersionUpdater::PromotionState state);
#endif

#if defined(OS_CHROMEOS)
  
  void OnOSVersion(const std::string& version);
  void OnOSFirmware(const std::string& firmware);
  void OnCurrentChannel(const std::string& channel);
  void OnTargetChannel(const std::string& channel);
#endif

  
  scoped_ptr<VersionUpdater> version_updater_;

  
  content::NotificationRegistrar registrar_;

#if defined(OS_CHROMEOS)
  
  chromeos::VersionLoader loader_;

  
  CancelableTaskTracker tracker_;
#endif  

  
  base::WeakPtrFactory<HelpHandler> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(HelpHandler);
};

#endif  
