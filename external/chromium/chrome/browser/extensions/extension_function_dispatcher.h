// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_DISPATCHER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_DISPATCHER_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class Extension;
class ExtensionFunction;
class ListValue;
class Profile;
class RenderViewHost;
class TabContents;
struct ExtensionHostMsg_DomMessage_Params;

typedef ExtensionFunction* (*ExtensionFunctionFactory)();

class ExtensionFunctionDispatcher {
 public:
  class Delegate {
   public:
    
    
    virtual Browser* GetBrowser() = 0;

    
    
    virtual gfx::NativeView GetNativeViewOfHost() = 0;

    
    
    
    
    virtual TabContents* associated_tab_contents() const = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  struct Peer : public base::RefCounted<Peer> {
    explicit Peer(ExtensionFunctionDispatcher* dispatcher)
        : dispatcher_(dispatcher) {}
    ExtensionFunctionDispatcher* dispatcher_;

   private:
    friend class base::RefCounted<Peer>;

    ~Peer() {}
  };

  
  static void GetAllFunctionNames(std::vector<std::string>* names);

  
  
  static bool OverrideFunction(const std::string& name,
                               ExtensionFunctionFactory factory);

  
  static void ResetFunctions();

  
  
  static ExtensionFunctionDispatcher* Create(RenderViewHost* render_view_host,
                                             Delegate* delegate,
                                             const GURL& url);

  ~ExtensionFunctionDispatcher();

  Delegate* delegate() { return delegate_; }

  
  void HandleRequest(const ExtensionHostMsg_DomMessage_Params& params);

  
  void SendResponse(ExtensionFunction* api, bool success);

  
  
  
  
  
  
  Browser* GetCurrentBrowser(bool include_incognito);

  
  
  void HandleBadMessage(ExtensionFunction* api);

  
  const GURL& url() { return url_; }

  
  const std::string extension_id() { return extension_id_; }

  
  Profile* profile();

  
  RenderViewHost* render_view_host() { return render_view_host_; }

 private:
  ExtensionFunctionDispatcher(RenderViewHost* render_view_host,
                              Delegate* delegate,
                              const Extension* extension,
                              const GURL& url);

  
  
  
  Profile* profile_;

  RenderViewHost* render_view_host_;

  Delegate* delegate_;

  GURL url_;

  std::string extension_id_;

  scoped_refptr<Peer> peer_;
};

#endif  
