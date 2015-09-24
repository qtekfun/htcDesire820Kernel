// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_RENDERER_STATE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_RENDERER_STATE_H_

#include <map>
#include <set>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/memory/singleton.h"

class WebViewGuest;

class ExtensionRendererState {
 public:
  struct WebViewInfo {
    int embedder_process_id;
    int instance_id;
    std::string partition_id;
    std::string extension_id;
    bool allow_chrome_extension_urls;
  };

  static ExtensionRendererState* GetInstance();

  
  
  void Init();
  void Shutdown();

  
  
  bool GetWebViewInfo(int guest_process_id, int guest_routing_id,
                      WebViewInfo* webview_info);

  
  
  bool GetTabAndWindowId(
      int render_process_host_id, int routing_id, int* tab_id, int* window_id);

  
  bool IsWebViewRenderer(int render_process_id);

 private:
  class RenderViewHostObserver;
  class TabObserver;
  friend class TabObserver;
  friend class WebViewGuest;
  friend struct DefaultSingletonTraits<ExtensionRendererState>;

  typedef std::pair<int, int> RenderId;
  typedef std::pair<int, int> TabAndWindowId;
  typedef std::map<RenderId, TabAndWindowId> TabAndWindowIdMap;
  typedef std::map<RenderId, WebViewInfo> WebViewInfoMap;

  ExtensionRendererState();
  ~ExtensionRendererState();

  
  void SetTabAndWindowId(
      int render_process_host_id, int routing_id, int tab_id, int window_id);
  void ClearTabAndWindowId(
      int render_process_host_id, int routing_id);

  
  void AddWebView(int render_process_host_id, int routing_id,
                  const WebViewInfo& webview_info);
  void RemoveWebView(int render_process_host_id, int routing_id);

  TabObserver* observer_;
  TabAndWindowIdMap map_;
  WebViewInfoMap webview_info_map_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionRendererState);
};

#endif  
