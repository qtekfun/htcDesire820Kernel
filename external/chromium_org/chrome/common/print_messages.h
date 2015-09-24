// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "base/memory/shared_memory.h"
#include "base/values.h"
#include "ipc/ipc_message_macros.h"
#include "printing/page_size_margins.h"
#include "printing/print_job_constants.h"
#include "third_party/WebKit/public/web/WebPrintScalingOption.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

#ifndef CHROME_COMMON_PRINT_MESSAGES_H_
#define CHROME_COMMON_PRINT_MESSAGES_H_

struct PrintMsg_Print_Params {
  PrintMsg_Print_Params();
  ~PrintMsg_Print_Params();

  
  void Reset();

  gfx::Size page_size;
  gfx::Size content_size;
  gfx::Rect printable_area;
  int margin_top;
  int margin_left;
  double dpi;
  double min_shrink;
  double max_shrink;
  int desired_dpi;
  int document_cookie;
  bool selection_only;
  bool supports_alpha_blend;
  int32 preview_ui_id;
  int preview_request_id;
  bool is_first_request;
  blink::WebPrintScalingOption print_scaling_option;
  bool print_to_pdf;
  bool display_header_footer;
  base::string16 title;
  base::string16 url;
  bool should_print_backgrounds;
};

struct PrintMsg_PrintPages_Params {
  PrintMsg_PrintPages_Params();
  ~PrintMsg_PrintPages_Params();

  
  void Reset();

  PrintMsg_Print_Params params;
  std::vector<int> pages;
};

struct PrintHostMsg_RequestPrintPreview_Params {
  PrintHostMsg_RequestPrintPreview_Params();
  ~PrintHostMsg_RequestPrintPreview_Params();
  bool is_modifiable;
  bool webnode_only;
  bool has_selection;
  bool selection_only;
};

#endif  

#define IPC_MESSAGE_START PrintMsgStart

IPC_ENUM_TRAITS(printing::MarginType)
IPC_ENUM_TRAITS(blink::WebPrintScalingOption)

IPC_STRUCT_TRAITS_BEGIN(PrintMsg_Print_Params)
  
  
  IPC_STRUCT_TRAITS_MEMBER(page_size)

  
  IPC_STRUCT_TRAITS_MEMBER(content_size)

  
  IPC_STRUCT_TRAITS_MEMBER(printable_area)

  
  IPC_STRUCT_TRAITS_MEMBER(margin_top)

  
  IPC_STRUCT_TRAITS_MEMBER(margin_left)

  
  IPC_STRUCT_TRAITS_MEMBER(dpi)

  
  IPC_STRUCT_TRAITS_MEMBER(min_shrink)

  
  IPC_STRUCT_TRAITS_MEMBER(max_shrink)

  
  IPC_STRUCT_TRAITS_MEMBER(desired_dpi)

  
  IPC_STRUCT_TRAITS_MEMBER(document_cookie)

  
  IPC_STRUCT_TRAITS_MEMBER(selection_only)

  
  IPC_STRUCT_TRAITS_MEMBER(supports_alpha_blend)

  

  
  IPC_STRUCT_TRAITS_MEMBER(preview_ui_id)

  
  IPC_STRUCT_TRAITS_MEMBER(preview_request_id)

  
  IPC_STRUCT_TRAITS_MEMBER(is_first_request)

  
  IPC_STRUCT_TRAITS_MEMBER(print_scaling_option)

  
  IPC_STRUCT_TRAITS_MEMBER(print_to_pdf)

  
  IPC_STRUCT_TRAITS_MEMBER(display_header_footer)

  
  IPC_STRUCT_TRAITS_MEMBER(title)

  
  IPC_STRUCT_TRAITS_MEMBER(url)

  
  IPC_STRUCT_TRAITS_MEMBER(should_print_backgrounds)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(PrintMsg_PrintPage_Params)
  
  
  IPC_STRUCT_MEMBER(PrintMsg_Print_Params, params)

  
  
  IPC_STRUCT_MEMBER(int, page_number)
IPC_STRUCT_END()

IPC_STRUCT_TRAITS_BEGIN(PrintHostMsg_RequestPrintPreview_Params)
  IPC_STRUCT_TRAITS_MEMBER(is_modifiable)
  IPC_STRUCT_TRAITS_MEMBER(webnode_only)
  IPC_STRUCT_TRAITS_MEMBER(has_selection)
  IPC_STRUCT_TRAITS_MEMBER(selection_only)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(printing::PageSizeMargins)
  IPC_STRUCT_TRAITS_MEMBER(content_width)
  IPC_STRUCT_TRAITS_MEMBER(content_height)
  IPC_STRUCT_TRAITS_MEMBER(margin_left)
  IPC_STRUCT_TRAITS_MEMBER(margin_right)
  IPC_STRUCT_TRAITS_MEMBER(margin_top)
  IPC_STRUCT_TRAITS_MEMBER(margin_bottom)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PrintMsg_PrintPages_Params)
  
  
  IPC_STRUCT_TRAITS_MEMBER(params)

  
  IPC_STRUCT_TRAITS_MEMBER(pages)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(PrintHostMsg_DidPreviewDocument_Params)
  
  IPC_STRUCT_MEMBER(base::SharedMemoryHandle, metafile_data_handle)

  
  IPC_STRUCT_MEMBER(uint32, data_size)

  
  IPC_STRUCT_MEMBER(int, document_cookie)

  
  IPC_STRUCT_MEMBER(int, expected_pages_count)

  
  IPC_STRUCT_MEMBER(bool, modifiable)

  
  IPC_STRUCT_MEMBER(int, preview_request_id)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintHostMsg_DidPreviewPage_Params)
  
  IPC_STRUCT_MEMBER(base::SharedMemoryHandle, metafile_data_handle)

  
  IPC_STRUCT_MEMBER(uint32, data_size)

  
  
  IPC_STRUCT_MEMBER(int, page_number)

  
  IPC_STRUCT_MEMBER(int, preview_request_id)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintHostMsg_DidGetPreviewPageCount_Params)
  
  IPC_STRUCT_MEMBER(int, document_cookie)

  
  IPC_STRUCT_MEMBER(int, page_count)

  
  IPC_STRUCT_MEMBER(bool, is_modifiable)

  
  IPC_STRUCT_MEMBER(int, preview_request_id)

  
  IPC_STRUCT_MEMBER(bool, clear_preview_data)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintHostMsg_DidPrintPage_Params)
  
  
  IPC_STRUCT_MEMBER(base::SharedMemoryHandle, metafile_data_handle)

  
  IPC_STRUCT_MEMBER(uint32, data_size)

  
  IPC_STRUCT_MEMBER(int, document_cookie)

  
  IPC_STRUCT_MEMBER(int, page_number)

  
  IPC_STRUCT_MEMBER(double, actual_shrink)

  
  IPC_STRUCT_MEMBER(gfx::Size, page_size)

  
  IPC_STRUCT_MEMBER(gfx::Rect, content_area)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintHostMsg_ScriptedPrint_Params)
  IPC_STRUCT_MEMBER(int, cookie)
  IPC_STRUCT_MEMBER(int, expected_pages_count)
  IPC_STRUCT_MEMBER(bool, has_selection)
  IPC_STRUCT_MEMBER(printing::MarginType, margin_type)
IPC_STRUCT_END()



IPC_MESSAGE_ROUTED1(PrintMsg_InitiatePrintPreview, bool )

IPC_MESSAGE_ROUTED0(PrintMsg_PrintNodeUnderContextMenu)

IPC_MESSAGE_ROUTED1(PrintMsg_PrintForPrintPreview,
                    base::DictionaryValue )

IPC_MESSAGE_ROUTED0(PrintMsg_PrintPages)

IPC_MESSAGE_ROUTED1(PrintMsg_PrintingDone,
                    bool )

IPC_MESSAGE_ROUTED1(PrintMsg_SetScriptedPrintingBlocked,
                    bool )

IPC_MESSAGE_ROUTED1(PrintMsg_PrintPreview,
                    base::DictionaryValue )

IPC_MESSAGE_ROUTED0(PrintMsg_PrintForSystemDialog)

IPC_MESSAGE_ROUTED0(PrintMsg_ResetScriptedPrintCount)


#if defined(OS_WIN)
IPC_SYNC_MESSAGE_ROUTED1_1(PrintHostMsg_DuplicateSection,
                           base::SharedMemoryHandle ,
                           base::SharedMemoryHandle )
#endif

IPC_SYNC_MESSAGE_ROUTED0_1(PrintHostMsg_IsPrintingEnabled,
                           bool )

IPC_MESSAGE_ROUTED2(PrintHostMsg_DidGetPrintedPagesCount,
                    int ,
                    int )

IPC_MESSAGE_ROUTED1(PrintHostMsg_DidGetDocumentCookie,
                    int )

IPC_MESSAGE_ROUTED0(PrintHostMsg_DidShowPrintDialog)

IPC_MESSAGE_ROUTED1(PrintHostMsg_DidPrintPage,
                    PrintHostMsg_DidPrintPage_Params )

IPC_SYNC_MESSAGE_ROUTED0_1(PrintHostMsg_GetDefaultPrintSettings,
                           PrintMsg_Print_Params )

IPC_SYNC_MESSAGE_ROUTED2_1(PrintHostMsg_UpdatePrintSettings,
                           int ,
                           base::DictionaryValue ,
                           PrintMsg_PrintPages_Params )

IPC_SYNC_MESSAGE_ROUTED1_1(PrintHostMsg_ScriptedPrint,
                           PrintHostMsg_ScriptedPrint_Params,
                           PrintMsg_PrintPages_Params
                               )

#if defined(OS_CHROMEOS) || defined(OS_ANDROID)
IPC_SYNC_MESSAGE_CONTROL1_2(PrintHostMsg_AllocateTempFileForPrinting,
                            int ,
                            base::FileDescriptor ,
                            int ) 
IPC_MESSAGE_CONTROL2(PrintHostMsg_TempFileForPrintingWritten,
                     int ,
                     int ) 
#endif

IPC_MESSAGE_ROUTED1(PrintHostMsg_RequestPrintPreview,
                    PrintHostMsg_RequestPrintPreview_Params )

IPC_MESSAGE_ROUTED1(PrintHostMsg_DidGetPreviewPageCount,
                    PrintHostMsg_DidGetPreviewPageCount_Params )

IPC_MESSAGE_ROUTED3(PrintHostMsg_DidGetDefaultPageLayout,
                    printing::PageSizeMargins ,
                    gfx::Rect ,
                    bool )

IPC_MESSAGE_ROUTED1(PrintHostMsg_DidPreviewPage,
                    PrintHostMsg_DidPreviewPage_Params )

IPC_SYNC_MESSAGE_ROUTED2_1(PrintHostMsg_CheckForCancel,
                           int32 ,
                           int ,
                           bool )

IPC_MESSAGE_ROUTED1(PrintHostMsg_MetafileReadyForPrinting,
                    PrintHostMsg_DidPreviewDocument_Params )

IPC_MESSAGE_ROUTED1(PrintHostMsg_PrintingFailed,
                    int )

IPC_MESSAGE_ROUTED1(PrintHostMsg_PrintPreviewFailed,
                    int )

IPC_MESSAGE_ROUTED1(PrintHostMsg_PrintPreviewCancelled,
                    int )

IPC_MESSAGE_ROUTED1(PrintHostMsg_PrintPreviewInvalidPrinterSettings,
                    int )

IPC_SYNC_MESSAGE_ROUTED0_0(PrintHostMsg_SetupScriptedPrintPreview)

IPC_MESSAGE_ROUTED1(PrintHostMsg_ShowScriptedPrintPreview,
                    bool )

IPC_MESSAGE_ROUTED0(PrintHostMsg_PrintPreviewScalingDisabled)
