// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_UI_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_UI_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "ui/base/layout.h"

class AutocompleteResult;

namespace base {
class DictionaryValue;
class ListValue;
class RefCountedMemory;
}

#if defined(OS_CHROMEOS)
namespace chromeos {
namespace system {
class PointerDeviceObserver;
}  
}  
#endif

namespace options {

class OptionsPageUIHandler : public content::WebUIMessageHandler,
                             public content::NotificationObserver {
 public:
  
  static const char kSettingsAppKey[];

  OptionsPageUIHandler();
  virtual ~OptionsPageUIHandler();

  
  virtual bool IsEnabled();

  
  virtual void GetLocalizedValues(base::DictionaryValue* localized_strings) = 0;

  virtual void PageLoadStarted() {}

  
  
  
  virtual void InitializeHandler() {}

  
  
  
  virtual void InitializePage() {}

  
  virtual void Uninitialize() {}

  
  virtual void RegisterMessages() OVERRIDE {}

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE {}

 protected:
  struct OptionsStringResource {
    
    const char* name;
    
    int id;
    
    
    int substitution_id;
  };

  
  
  static void RegisterStrings(base::DictionaryValue* localized_strings,
                              const OptionsStringResource* resources,
                              size_t length);

  
  static void RegisterTitle(base::DictionaryValue* localized_strings,
                            const std::string& variable_name,
                            int title_id);

  content::NotificationRegistrar registrar_;

 private:
  DISALLOW_COPY_AND_ASSIGN(OptionsPageUIHandler);
};

class OptionsPageUIHandlerHost {
 public:
  virtual void InitializeHandlers() = 0;

 protected:
  virtual ~OptionsPageUIHandlerHost() {}
};

class OptionsUI : public content::WebUIController,
                  public content::WebContentsObserver,
                  public OptionsPageUIHandlerHost {
 public:
  explicit OptionsUI(content::WebUI* web_ui);
  virtual ~OptionsUI();

  
  
  static void ProcessAutocompleteSuggestions(
      const AutocompleteResult& result,
      base::ListValue* const suggestions);

  static base::RefCountedMemory* GetFaviconResourceBytes(
      ui::ScaleFactor scale_factor);

  
  virtual void DidStartProvisionalLoadForFrame(
      int64 frame_id,
      int64 parent_frame_id,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc,
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  virtual void InitializeHandlers() OVERRIDE;

 private:
  
  void AddOptionsPageUIHandler(base::DictionaryValue* localized_strings,
                               OptionsPageUIHandler* handler);

  bool initialized_handlers_;

  std::vector<OptionsPageUIHandler*> handlers_;

#if defined(OS_CHROMEOS)
  scoped_ptr<chromeos::system::PointerDeviceObserver>
      pointer_device_observer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(OptionsUI);
};

}  

#endif  
