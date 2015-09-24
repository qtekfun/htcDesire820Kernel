// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_PREVIEW_MESSAGE_HANDLER_H_
#define CHROME_BROWSER_PRINTING_PRINT_PREVIEW_MESSAGE_HANDLER_H_
#pragma once

#include "content/browser/tab_contents/tab_contents_observer.h"

struct PrintHostMsg_DidPreviewDocument_Params;

namespace printing {

class PrintPreviewMessageHandler : public TabContentsObserver {
 public:
  explicit PrintPreviewMessageHandler(TabContents* tab_contents);
  virtual ~PrintPreviewMessageHandler();

  
  virtual bool OnMessageReceived(const IPC::Message& message);
  virtual void DidStartLoading();

 private:
  
  TabContents* GetPrintPreviewTab();

  void OnPagesReadyForPreview(
      const PrintHostMsg_DidPreviewDocument_Params& params);
  void OnPrintPreviewNodeUnderContextMenu();
  void OnScriptInitiatedPrintPreview();

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewMessageHandler);
};

}  

#endif  
