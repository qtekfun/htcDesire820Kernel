// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_EXTENSIONS_BROWSER_CLIENT_H_
#define EXTENSIONS_BROWSER_EXTENSIONS_BROWSER_CLIENT_H_

#include "base/memory/scoped_ptr.h"

class CommandLine;
class PrefService;

namespace content {
class BrowserContext;
class JavaScriptDialogManager;
}

namespace extensions {

class AppSorting;

class ExtensionsBrowserClient {
 public:
  virtual ~ExtensionsBrowserClient() {}

  
  virtual bool IsShuttingDown() = 0;

  
  
  virtual bool AreExtensionsDisabled(const CommandLine& command_line,
                                     content::BrowserContext* context) = 0;

  
  virtual bool IsValidContext(content::BrowserContext* context) = 0;

  
  
  virtual bool IsSameContext(content::BrowserContext* first,
                             content::BrowserContext* second) = 0;

  
  virtual bool HasOffTheRecordContext(content::BrowserContext* context) = 0;

  
  
  
  
  virtual content::BrowserContext* GetOffTheRecordContext(
      content::BrowserContext* context) = 0;

  
  
  virtual content::BrowserContext* GetOriginalContext(
      content::BrowserContext* context) = 0;

  
  virtual PrefService* GetPrefServiceForContext(
      content::BrowserContext* context) = 0;

  
  virtual bool DeferLoadingBackgroundHosts(
      content::BrowserContext* context) const = 0;

  virtual bool IsBackgroundPageAllowed(
      content::BrowserContext* context) const = 0;

  
  
  
  
  virtual bool DidVersionUpdate(content::BrowserContext* context) = 0;

  
  virtual scoped_ptr<AppSorting> CreateAppSorting() = 0;

  
  virtual bool IsRunningInForcedAppMode() = 0;

  
  
  virtual content::JavaScriptDialogManager* GetJavaScriptDialogManager() = 0;

  
  static ExtensionsBrowserClient* Get();

  
  static void Set(ExtensionsBrowserClient* client);
};

}  

#endif  
