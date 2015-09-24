// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_H_
#define CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_H_

#include "chrome/browser/printing/print_view_manager_base.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class RenderProcessHost;
}

namespace printing {

class PrintViewManagerObserver;

class PrintViewManager : public PrintViewManagerBase,
                         public content::WebContentsUserData<PrintViewManager> {
 public:
  virtual ~PrintViewManager();

  
  
  bool PrintForSystemDialogNow();

  
  
  
  bool AdvancedPrintNow();

  
  
  
  bool PrintToDestination();

  
  
  
  
  bool PrintPreviewNow(bool selection_only);

  
  
  void PrintPreviewForWebNode();

  
  
  void PrintPreviewDone();

  
  
  
  void set_observer(PrintViewManagerObserver* observer);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

 private:
  explicit PrintViewManager(content::WebContents* web_contents);
  friend class content::WebContentsUserData<PrintViewManager>;

  enum PrintPreviewState {
    NOT_PREVIEWING,
    USER_INITIATED_PREVIEW,
    SCRIPTED_PREVIEW,
  };

  
  void OnDidShowPrintDialog();
  void OnSetupScriptedPrintPreview(IPC::Message* reply_msg);
  void OnShowScriptedPrintPreview(bool source_is_modifiable);
  void OnScriptedPrintPreviewReply(IPC::Message* reply_msg);

  
  
  PrintViewManagerObserver* observer_;

  
  PrintPreviewState print_preview_state_;

  
  content::RenderProcessHost* scripted_print_preview_rph_;

  DISALLOW_COPY_AND_ASSIGN(PrintViewManager);
};

}  

#endif  
