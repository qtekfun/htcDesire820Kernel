// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_WEB_UI_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_WEB_UI_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_bookmark_manager_api.h"
#include "chrome/browser/extensions/extension_function_dispatcher.h"
#include "chrome/browser/favicon_service.h"
#include "chrome/common/extensions/extension.h"
#include "content/browser/webui/web_ui.h"

class GURL;
class ListValue;
class PrefService;
class Profile;
class RenderViewHost;
class TabContents;
struct ExtensionHostMsg_DomMessage_Params;

class ExtensionWebUI
    : public WebUI,
      public ExtensionFunctionDispatcher::Delegate {
 public:
  static const char kExtensionURLOverrides[];

  explicit ExtensionWebUI(TabContents* tab_contents, const GURL& url);

  virtual ~ExtensionWebUI();

  ExtensionFunctionDispatcher* extension_function_dispatcher() const {
    return extension_function_dispatcher_.get();
  }

  
  virtual void RenderViewCreated(RenderViewHost* render_view_host);
  virtual void RenderViewReused(RenderViewHost* render_view_host);
  virtual void ProcessWebUIMessage(
      const ExtensionHostMsg_DomMessage_Params& params);

  
  virtual Browser* GetBrowser();
  virtual gfx::NativeView GetNativeViewOfHost();
  virtual gfx::NativeWindow GetCustomFrameNativeWindow();
  virtual TabContents* associated_tab_contents() const;

  virtual ExtensionBookmarkManagerEventRouter*
      extension_bookmark_manager_event_router();

  
  static bool HandleChromeURLOverride(GURL* url, Profile* profile);

  
  
  
  static void RegisterChromeURLOverrides(Profile* profile,
      const Extension::URLOverrideMap& overrides);
  static void UnregisterChromeURLOverrides(Profile* profile,
      const Extension::URLOverrideMap& overrides);
  static void UnregisterChromeURLOverride(const std::string& page,
                                          Profile* profile,
                                          Value* override);

  
  static void RegisterUserPrefs(PrefService* prefs);

  
  static void GetFaviconForURL(Profile* profile,
                               FaviconService::GetFaviconRequest* request,
                               const GURL& page_url);

 private:
  
  
  static void UnregisterAndReplaceOverride(const std::string& page,
                                           Profile* profile,
                                           ListValue* list,
                                           Value* override);

  
  
  
  void ResetExtensionFunctionDispatcher(RenderViewHost* render_view_host);

  void ResetExtensionBookmarkManagerEventRouter();

  scoped_ptr<ExtensionFunctionDispatcher> extension_function_dispatcher_;

  
  
  scoped_ptr<ExtensionBookmarkManagerEventRouter>
      extension_bookmark_manager_event_router_;

  
  GURL url_;
};

#endif  
