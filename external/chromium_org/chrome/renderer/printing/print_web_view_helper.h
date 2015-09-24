// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_PRINTING_PRINT_WEB_VIEW_HELPER_H_
#define CHROME_RENDERER_PRINTING_PRINT_WEB_VIEW_HELPER_H_

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/public/renderer/render_view_observer_tracker.h"
#include "printing/metafile_impl.h"
#include "third_party/WebKit/public/platform/WebCanvas.h"
#include "third_party/WebKit/public/web/WebNode.h"
#include "third_party/WebKit/public/web/WebPrintParams.h"
#include "ui/gfx/size.h"

struct PrintMsg_Print_Params;
struct PrintMsg_PrintPage_Params;
struct PrintMsg_PrintPages_Params;

namespace base {
class DictionaryValue;
}

namespace blink {
class WebFrame;
class WebView;
}

namespace printing {

struct PageSizeMargins;
class PrepareFrameAndViewForPrint;

class FrameReference {
 public:
  explicit FrameReference(const blink::WebFrame* frame);
  FrameReference();
  ~FrameReference();

  void Reset(const blink::WebFrame* frame);

  blink::WebFrame* GetFrame();
  blink::WebView* view();

 private:
  blink::WebView* view_;
  blink::WebString frame_name_;
};

class PrintWebViewHelper
    : public content::RenderViewObserver,
      public content::RenderViewObserverTracker<PrintWebViewHelper> {
 public:
  explicit PrintWebViewHelper(content::RenderView* render_view);
  virtual ~PrintWebViewHelper();

  bool IsPrintingEnabled();

  void PrintNode(const blink::WebNode& node);

 private:
  friend class PrintWebViewHelperTestBase;
  FRIEND_TEST_ALL_PREFIXES(PrintWebViewHelperTest,
                           BlockScriptInitiatedPrinting);
  FRIEND_TEST_ALL_PREFIXES(PrintWebViewHelperTest,
                           BlockScriptInitiatedPrintingFromPopup);
  FRIEND_TEST_ALL_PREFIXES(PrintWebViewHelperTest, OnPrintPages);

#if defined(OS_WIN) || defined(OS_MACOSX)
  FRIEND_TEST_ALL_PREFIXES(PrintWebViewHelperTest, PrintLayoutTest);
  FRIEND_TEST_ALL_PREFIXES(PrintWebViewHelperTest, PrintWithIframe);
#endif  

  enum PrintingResult {
    OK,
    FAIL_PRINT_INIT,
    FAIL_PRINT,
    FAIL_PREVIEW,
  };

  enum PrintPreviewErrorBuckets {
    PREVIEW_ERROR_NONE,  
    PREVIEW_ERROR_BAD_SETTING,
    PREVIEW_ERROR_METAFILE_COPY_FAILED,
    PREVIEW_ERROR_METAFILE_INIT_FAILED,
    PREVIEW_ERROR_ZERO_PAGES,
    PREVIEW_ERROR_MAC_DRAFT_METAFILE_INIT_FAILED,
    PREVIEW_ERROR_PAGE_RENDERED_WITHOUT_METAFILE,
    PREVIEW_ERROR_UPDATING_PRINT_SETTINGS,
    PREVIEW_ERROR_INVALID_PRINTER_SETTINGS,
    PREVIEW_ERROR_LAST_ENUM  
  };

  enum PrintPreviewRequestType {
    PRINT_PREVIEW_USER_INITIATED_ENTIRE_FRAME,
    PRINT_PREVIEW_USER_INITIATED_SELECTION,
    PRINT_PREVIEW_USER_INITIATED_CONTEXT_NODE,
    PRINT_PREVIEW_SCRIPTED  
  };

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void PrintPage(blink::WebFrame* frame, bool user_initiated) OVERRIDE;
  virtual void DidStartLoading() OVERRIDE;
  virtual void DidStopLoading() OVERRIDE;

  

  
  void OnPrintPages();

  
  void OnPrintForSystemDialog();

  
  
  void GetPageSizeAndContentAreaFromPageLayout(
      const PageSizeMargins& page_layout_in_points,
      gfx::Size* page_size,
      gfx::Rect* content_area);

  
  void UpdateFrameMarginsCssInfo(const base::DictionaryValue& settings);

  
  bool IsPrintToPdfRequested(const base::DictionaryValue& settings);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  blink::WebPrintScalingOption GetPrintScalingOption(
      bool source_is_html,
      const base::DictionaryValue& job_settings,
      const PrintMsg_Print_Params& params);

  
  void OnInitiatePrintPreview(bool selection_only);

  
  void OnPrintPreview(const base::DictionaryValue& settings);

  
  void PrepareFrameForPreviewDocument();

  
  void OnFramePreparedForPreviewDocument();

  
  bool CreatePreviewDocument();

  
  
  bool RenderPreviewPage(int page_number,
                         const PrintMsg_Print_Params& print_params);

  
  bool FinalizePrintReadyDocument();

  
  void OnPrintNodeUnderContextMenu();

  
  
  void OnPrintForPrintPreview(const base::DictionaryValue& job_settings);

  void OnPrintingDone(bool success);

  
  
  void SetScriptedPrintBlocked(bool blocked);

  

  void Print(blink::WebFrame* frame, const blink::WebNode& node);

  
  void DidFinishPrinting(PrintingResult result);

  

  
  
  bool InitPrintSettings(bool fit_to_paper_size);

  
  bool CalculateNumberOfPages(blink::WebFrame* frame,
                              const blink::WebNode& node,
                              int* number_of_pages);

  
  
  
  bool UpdatePrintSettings(blink::WebFrame* frame,
                           const blink::WebNode& node,
                           const base::DictionaryValue& passed_job_settings);

  
  
  bool GetPrintSettingsFromUser(blink::WebFrame* frame,
                                const blink::WebNode& node,
                                int expected_pages_count);

  

  void OnFramePreparedForPrintPages();
  void PrintPages();
  bool PrintPagesNative(blink::WebFrame* frame,
                        int page_count,
                        const gfx::Size& canvas_size);
  void FinishFramePrinting();

  
#if defined(OS_LINUX) || defined(OS_ANDROID)
  void PrintPageInternal(const PrintMsg_PrintPage_Params& params,
                         const gfx::Size& canvas_size,
                         blink::WebFrame* frame,
                         Metafile* metafile);
#else
  void PrintPageInternal(const PrintMsg_PrintPage_Params& params,
                         const gfx::Size& canvas_size,
                         blink::WebFrame* frame);
#endif

  
  bool RenderPagesForPrint(blink::WebFrame* frame,
                           const blink::WebNode& node);

  
#if defined(OS_WIN)
  void RenderPage(const PrintMsg_Print_Params& params,
                  int page_number,
                  blink::WebFrame* frame,
                  bool is_preview,
                  Metafile* metafile,
                  double* scale_factor,
                  gfx::Size* page_size_in_dpi,
                  gfx::Rect* content_area_in_dpi);
#elif defined(OS_MACOSX)
  void RenderPage(const PrintMsg_Print_Params& params,
                  int page_number,
                  blink::WebFrame* frame,
                  bool is_preview,
                  Metafile* metafile,
                  gfx::Size* page_size,
                  gfx::Rect* content_rect);
#endif  

  
  
  
  
  static float RenderPageContent(blink::WebFrame* frame,
                                 int page_number,
                                 const gfx::Rect& canvas_area,
                                 const gfx::Rect& content_area,
                                 double scale_factor,
                                 blink::WebCanvas* canvas);

  

  bool CopyMetafileDataToSharedMem(Metafile* metafile,
                                   base::SharedMemoryHandle* shared_mem_handle);

  
  static void ComputePageLayoutInPointsForCss(
      blink::WebFrame* frame,
      int page_index,
      const PrintMsg_Print_Params& default_params,
      bool ignore_css_margins,
      double* scale_factor,
      PageSizeMargins* page_layout_in_points);

  
  
  static void PrintHeaderAndFooter(
      blink::WebCanvas* canvas,
      int page_number,
      int total_pages,
      float webkit_scale_factor,
      const PageSizeMargins& page_layout_in_points,
      const base::DictionaryValue& header_footer_info,
      const PrintMsg_Print_Params& params);

  bool GetPrintFrame(blink::WebFrame** frame);

  

  
  
  
  bool IsScriptInitiatedPrintAllowed(blink::WebFrame* frame,
                                     bool user_initiated);

  
  bool IsScriptInitiatedPrintTooFrequent(blink::WebFrame* frame);

  
  
  void ResetScriptedPrintCount();

  
  
  void IncrementScriptedPrintCount();

  
  void ShowScriptedPrintPreview();

  void RequestPrintPreview(PrintPreviewRequestType type);

  
  
  bool CheckForCancel();

  
  
  
  
  
  bool PreviewPageRendered(int page_number, Metafile* metafile);

  
  scoped_ptr<PrepareFrameAndViewForPrint> prep_frame_view_;
  bool reset_prep_frame_view_;

  scoped_ptr<PrintMsg_PrintPages_Params> print_pages_params_;
  bool is_preview_enabled_;
  bool is_scripted_print_throttling_disabled_;
  bool is_print_ready_metafile_sent_;
  bool ignore_css_margins_;

  
  base::Time last_cancelled_script_print_;
  int user_cancelled_scripted_print_count_;
  bool is_scripted_printing_blocked_;

  
  
  bool notify_browser_of_print_failure_;

  
  bool print_for_preview_;

  
  
  scoped_ptr<base::DictionaryValue> header_footer_info_;

  
  
  
  class PrintPreviewContext {
   public:
    PrintPreviewContext();
    ~PrintPreviewContext();

    
    
    void InitWithFrame(blink::WebFrame* web_frame);
    void InitWithNode(const blink::WebNode& web_node);

    
    void OnPrintPreview();

    
    
    bool CreatePreviewDocument(PrepareFrameAndViewForPrint* prepared_frame,
                               const std::vector<int>& pages);

    
    
    void RenderedPreviewPage(const base::TimeDelta& page_time);

    
    void AllPagesRendered();

    
    void FinalizePrintReadyDocument();

    
    void Finished();

    
    void Failed(bool report_error);

    
    int GetNextPageNumber();
    bool IsRendering() const;
    bool IsModifiable();
    bool HasSelection();
    bool IsLastPageOfPrintReadyMetafile() const;
    bool IsFinalPageRendered() const;

    
    void set_generate_draft_pages(bool generate_draft_pages);
    void set_error(enum PrintPreviewErrorBuckets error);

    
    
    blink::WebFrame* source_frame();
    
    const blink::WebNode& source_node() const;

    
    
    blink::WebFrame* prepared_frame();
    
    
    const blink::WebNode& prepared_node() const;

    int total_page_count() const;
    bool generate_draft_pages() const;
    PreviewMetafile* metafile();
    gfx::Size GetPrintCanvasSize() const;
    int last_error() const;

   private:
    enum State {
      UNINITIALIZED,  
      INITIALIZED,    
      RENDERING,      
      DONE            
    };

    
    void ClearContext();

    
    FrameReference source_frame_;
    blink::WebNode source_node_;

    scoped_ptr<PrepareFrameAndViewForPrint> prep_frame_view_;
    scoped_ptr<PreviewMetafile> metafile_;

    
    int total_page_count_;

    
    int current_page_index_;

    
    std::vector<int> pages_to_render_;

    
    bool generate_draft_pages_;

    
    int print_ready_metafile_page_count_;

    base::TimeDelta document_render_time_;
    base::TimeTicks begin_time_;

    enum PrintPreviewErrorBuckets error_;

    State state_;
  };

  bool print_node_in_progress_;
  PrintPreviewContext print_preview_context_;
  bool is_loading_;
  bool is_scripted_preview_delayed_;
  base::WeakPtrFactory<PrintWebViewHelper> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(PrintWebViewHelper);
};

}  

#endif  
