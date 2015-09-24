// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_COLLECTED_COOKIES_UI_DELEGATE_H_
#define CHROME_BROWSER_UI_WEBUI_COLLECTED_COOKIES_UI_DELEGATE_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/webui/cookies_tree_model_adapter.h"
#include "chrome/browser/ui/webui/html_dialog_ui.h"
#include "chrome/common/content_settings.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class TabContents;

namespace gfx {
class Size;
}

class CollectedCookiesUIDelegate : public HtmlDialogUIDelegate,
                                          WebUIMessageHandler,
                                          NotificationObserver {
 public:
  virtual ~CollectedCookiesUIDelegate();

  
  static void Show(TabContents* tab_contents);

  
  virtual bool IsDialogModal() const;
  virtual std::wstring GetDialogTitle() const;
  virtual GURL GetDialogContentURL() const;
  virtual void GetWebUIMessageHandlers(
      std::vector<WebUIMessageHandler*>* handlers) const;
  virtual void GetDialogSize(gfx::Size* size) const;
  virtual std::string GetDialogArgs() const;
  virtual void OnDialogClosed(const std::string& json_retval);
  virtual void OnCloseContents(TabContents* source, bool* out_close_dialog) {}
  virtual bool ShouldShowDialogTitle() const;

  
  virtual void RegisterMessages();

 private:
  explicit CollectedCookiesUIDelegate(TabContents* tab_contents);

  
  void CloseDialog();

  
  void SetInfobarLabel(const std::string& text);

  
  void AddContentException(CookieTreeOriginNode* origin_node,
                           ContentSetting setting);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void BindCookiesTreeModel(const ListValue* args);

  
  void Block(const ListValue* args);
  void Allow(const ListValue* args);
  void AllowThisSession(const ListValue* args);

  NotificationRegistrar registrar_;
  TabContents* tab_contents_;
  bool closed_;

  scoped_ptr<CookiesTreeModel> allowed_cookies_tree_model_;
  scoped_ptr<CookiesTreeModel> blocked_cookies_tree_model_;

  CookiesTreeModelAdapter allowed_cookies_adapter_;
  CookiesTreeModelAdapter blocked_cookies_adapter_;

  DISALLOW_COPY_AND_ASSIGN(CollectedCookiesUIDelegate);
};

#endif  
