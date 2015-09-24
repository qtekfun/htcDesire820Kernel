// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_WEB_DIALOGS_WEB_DIALOG_WEB_CONTENTS_DELEGATE_H_
#define UI_WEB_DIALOGS_WEB_DIALOG_WEB_CONTENTS_DELEGATE_H_

#include "base/compiler_specific.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ui/web_dialogs/web_dialogs_export.h"

namespace ui {

class WEB_DIALOGS_EXPORT WebDialogWebContentsDelegate
    : public content::WebContentsDelegate {
 public:
  
  class WebContentsHandler {
   public:
    virtual ~WebContentsHandler() {}
    virtual content::WebContents* OpenURLFromTab(
        content::BrowserContext* context,
        content::WebContents* source,
        const content::OpenURLParams& params) = 0;
    virtual void AddNewContents(content::BrowserContext* context,
                                content::WebContents* source,
                                content::WebContents* new_contents,
                                WindowOpenDisposition disposition,
                                const gfx::Rect& initial_pos,
                                bool user_gesture) = 0;
  };

  
  
  WebDialogWebContentsDelegate(content::BrowserContext* context,
                               WebContentsHandler* handler);

  virtual ~WebDialogWebContentsDelegate();

  
  content::BrowserContext* browser_context() const {
    return browser_context_;
  }

  
  
  
  void Detach();

  
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;

  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual bool IsPopupOrPanel(
      const content::WebContents* source) const OVERRIDE;

 private:
  
  content::BrowserContext* browser_context_;

  scoped_ptr<WebContentsHandler> handler_;

  DISALLOW_COPY_AND_ASSIGN(WebDialogWebContentsDelegate);
};

}  

#endif  
