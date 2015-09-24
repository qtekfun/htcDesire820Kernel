// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_PREVIEW_MESSAGE_HANDLER_H_
#define CHROME_BROWSER_PRINTING_PRINT_PREVIEW_MESSAGE_HANDLER_H_

#include "base/compiler_specific.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class PrintPreviewUI;
struct PrintHostMsg_DidGetPreviewPageCount_Params;
struct PrintHostMsg_DidPreviewDocument_Params;
struct PrintHostMsg_DidPreviewPage_Params;
struct PrintHostMsg_RequestPrintPreview_Params;

namespace content {
class WebContents;
}

namespace gfx {
class Rect;
}

namespace printing {

struct PageSizeMargins;

class PrintPreviewMessageHandler
    : public content::WebContentsObserver,
      public content::WebContentsUserData<PrintPreviewMessageHandler> {
 public:
  virtual ~PrintPreviewMessageHandler();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  explicit PrintPreviewMessageHandler(content::WebContents* web_contents);
  friend class content::WebContentsUserData<PrintPreviewMessageHandler>;

  
  
  content::WebContents* GetPrintPreviewDialog();

  
  PrintPreviewUI* GetPrintPreviewUI();

  
  void OnRequestPrintPreview(
      const PrintHostMsg_RequestPrintPreview_Params& params);
  void OnDidGetDefaultPageLayout(
      const printing::PageSizeMargins& page_layout_in_points,
      const gfx::Rect& printable_area_in_points,
      bool has_custom_page_size_style);
  void OnDidGetPreviewPageCount(
      const PrintHostMsg_DidGetPreviewPageCount_Params& params);
  void OnDidPreviewPage(const PrintHostMsg_DidPreviewPage_Params& params);
  void OnMetafileReadyForPrinting(
      const PrintHostMsg_DidPreviewDocument_Params& params);
  void OnPrintPreviewFailed(int document_cookie);
  void OnPrintPreviewCancelled(int document_cookie);
  void OnInvalidPrinterSettings(int document_cookie);
  void OnPrintPreviewScalingDisabled();

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewMessageHandler);
};

}  

#endif  
