// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CONSTRAINED_HTML_UI_H_
#define CHROME_BROWSER_UI_WEBUI_CONSTRAINED_HTML_UI_H_
#pragma once

#include <vector>

#include "content/browser/tab_contents/constrained_window.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/property_bag.h"

class HtmlDialogUIDelegate;
class Profile;
class RenderViewHost;
class TabContents;

class ConstrainedHtmlUIDelegate {
 public:
  virtual HtmlDialogUIDelegate* GetHtmlDialogUIDelegate() = 0;

  
  virtual void OnDialogClose() = 0;
};

class ConstrainedHtmlUI : public WebUI {
 public:
  explicit ConstrainedHtmlUI(TabContents* contents);
  virtual ~ConstrainedHtmlUI();

  virtual void RenderViewCreated(RenderViewHost* render_view_host);

  
  
  
  static ConstrainedWindow* CreateConstrainedHtmlDialog(
      Profile* profile,
      HtmlDialogUIDelegate* delegate,
      TabContents* overshadowed);

  
  
  static PropertyAccessor<ConstrainedHtmlUIDelegate*>&
      GetPropertyAccessor();

 private:
  
  
  ConstrainedHtmlUIDelegate* GetConstrainedDelegate();

  
  void OnDialogClose(const ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(ConstrainedHtmlUI);
};

#endif  
