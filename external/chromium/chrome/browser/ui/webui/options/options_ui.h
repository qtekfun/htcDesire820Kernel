// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_UI_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_UI_H_
#pragma once

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"

class GURL;
class PrefService;
struct UserMetricsAction;

class OptionsPageUIHandler : public WebUIMessageHandler,
                             public NotificationObserver {
 public:
  OptionsPageUIHandler();
  virtual ~OptionsPageUIHandler();

  
  virtual bool IsEnabled();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings) = 0;

  
  
  virtual void Initialize() {}

  
  virtual void Uninitialize() {}

  
  virtual void RegisterMessages() {}

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) {}

  void UserMetricsRecordAction(const UserMetricsAction& action);

 protected:
  struct OptionsStringResource {
    
    const char* name;
    
    int id;
  };
  
  static void RegisterStrings(DictionaryValue* localized_strings,
                              const OptionsStringResource* resources,
                              size_t length);

  
  static void RegisterTitle(DictionaryValue* localized_strings,
                            const std::string& variable_name,
                            int title_id);

  NotificationRegistrar registrar_;

 private:
  DISALLOW_COPY_AND_ASSIGN(OptionsPageUIHandler);
};

class OptionsPageUIHandlerHost {
 public:
  virtual void InitializeHandlers() = 0;
};

class OptionsUI : public WebUI,
                  public OptionsPageUIHandlerHost {
 public:
  explicit OptionsUI(TabContents* contents);
  virtual ~OptionsUI();

  static RefCountedMemory* GetFaviconResourceBytes();

  
  virtual void RenderViewCreated(RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidBecomeActiveForReusedRenderView() OVERRIDE;

  
  virtual void InitializeHandlers() OVERRIDE;

 private:
  
  void AddOptionsPageUIHandler(DictionaryValue* localized_strings,
                               OptionsPageUIHandler* handler);

  bool initialized_handlers_;

  DISALLOW_COPY_AND_ASSIGN(OptionsUI);
};

#endif  
