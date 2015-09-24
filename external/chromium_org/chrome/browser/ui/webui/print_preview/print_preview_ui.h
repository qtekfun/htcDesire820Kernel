// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_PRINT_PREVIEW_UI_H_
#define CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_PRINT_PREVIEW_UI_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "chrome/browser/ui/webui/constrained_web_dialog_ui.h"

class PrintPreviewDataService;
class PrintPreviewHandler;
struct PrintHostMsg_DidGetPreviewPageCount_Params;
struct PrintHostMsg_RequestPrintPreview_Params;

namespace base {
class RefCountedBytes;
}

namespace gfx {
class Rect;
}

namespace printing {
struct PageSizeMargins;
}

class PrintPreviewUI : public ConstrainedWebDialogUI {
 public:
  explicit PrintPreviewUI(content::WebUI* web_ui);
  virtual ~PrintPreviewUI();

  
  
  
  void GetPrintPreviewDataForIndex(int index,
                                   scoped_refptr<base::RefCountedBytes>* data);

  
  
  
  void SetPrintPreviewDataForIndex(int index,
                                   const base::RefCountedBytes* data);

  
  void ClearAllPreviewData();

  
  int GetAvailableDraftPageCount();

  
  void SetInitiatorTitle(const base::string16& initiator_title);

  base::string16 initiator_title() { return initiator_title_; }

  bool source_is_modifiable() { return source_is_modifiable_; }

  bool source_has_selection() { return source_has_selection_; }

  bool print_selection_only() { return print_selection_only_; }

  
  static void SetInitialParams(
      content::WebContents* print_preview_dialog,
      const PrintHostMsg_RequestPrintPreview_Params& params);

  
  
  
  static void GetCurrentPrintPreviewStatus(int32 preview_ui_id,
                                           int request_id,
                                           bool* cancel);

  
  int32 GetIDForPrintPreviewUI() const;

  
  void OnPrintPreviewRequest(int request_id);

  
  void OnShowSystemDialog();

  
  void OnDidGetPreviewPageCount(
      const PrintHostMsg_DidGetPreviewPageCount_Params& params);

  
  
  void OnDidGetDefaultPageLayout(const printing::PageSizeMargins& page_layout,
                                 const gfx::Rect& printable_area,
                                 bool has_custom_page_size_style);

  
  
  void OnDidPreviewPage(int page_number, int preview_request_id);

  
  
  
  void OnPreviewDataIsAvailable(int expected_pages_count,
                                int preview_request_id);

  
  
  
  void OnPrintPreviewDialogDestroyed();

  
  void OnPrintPreviewFailed();

  
  
  void OnPrintPreviewDialogClosed();

  
  
  void OnInitiatorClosed();

  
  void OnFileSelectionCancelled();

  
  
  void OnInvalidPrinterSettings();

  
  void OnCancelPendingPreviewRequest();

  
  void OnHidePreviewDialog();

  
  void OnClosePrintPreviewDialog();

  
  void OnReloadPrintersList();

  
  
  void OnPrintPreviewScalingDisabled();

  
  
  class TestingDelegate {
   public:
    virtual bool IsAutoCancelEnabled() = 0;
    virtual void DidGetPreviewPageCount(int page_count) = 0;
    virtual void DidRenderPreviewPage(
        const content::WebContents& preview_dialog) = 0;
  };

  static void SetDelegateForTesting(TestingDelegate* delegate);

 private:
  friend class PrintPreviewHandlerTest;
  FRIEND_TEST_ALL_PREFIXES(PrintPreviewHandlerTest, StickyMarginsCustom);
  FRIEND_TEST_ALL_PREFIXES(PrintPreviewHandlerTest, StickyMarginsDefault);
  FRIEND_TEST_ALL_PREFIXES(PrintPreviewHandlerTest,
                           StickyMarginsCustomThenDefault);
  FRIEND_TEST_ALL_PREFIXES(PrintPreviewHandlerTest,
                           GetLastUsedMarginSettingsCustom);
  FRIEND_TEST_ALL_PREFIXES(PrintPreviewHandlerTest,
                           GetLastUsedMarginSettingsDefault);
  FRIEND_TEST_ALL_PREFIXES(PrintPreviewDialogControllerUnitTest,
                           TitleAfterReload);

  
  PrintPreviewDataService* print_preview_data_service();

  base::TimeTicks initial_preview_start_time_;

  
  
  const int32 id_;

  
  PrintPreviewHandler* handler_;

  
  bool source_is_modifiable_;

  
  bool source_has_selection_;

  
  bool print_selection_only_;

  
  
  base::string16 initiator_title_;

  
  bool dialog_closed_;

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewUI);
};

#endif  
