// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/values.h"
#include "base/shared_memory.h"
#include "ipc/ipc_message_macros.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

#define IPC_MESSAGE_START PrintMsgStart

IPC_STRUCT_BEGIN(PrintMsg_Print_Params)
  
  
  IPC_STRUCT_MEMBER(gfx::Size, page_size)

  
  IPC_STRUCT_MEMBER(gfx::Size, printable_size)

  
  IPC_STRUCT_MEMBER(int, margin_top)

  
  IPC_STRUCT_MEMBER(int, margin_left)

  
  IPC_STRUCT_MEMBER(double, dpi)

  
  IPC_STRUCT_MEMBER(double, min_shrink)

  
  IPC_STRUCT_MEMBER(double, max_shrink)

  
  IPC_STRUCT_MEMBER(int, desired_dpi)

  
  IPC_STRUCT_MEMBER(int, document_cookie)

  
  IPC_STRUCT_MEMBER(bool, selection_only)

  
  IPC_STRUCT_MEMBER(bool, supports_alpha_blend)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintMsg_PrintPage_Params)
  
  
  IPC_STRUCT_MEMBER(PrintMsg_Print_Params, params)

  
  
  IPC_STRUCT_MEMBER(int, page_number)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintMsg_PrintPages_Params)
  
  
  IPC_STRUCT_MEMBER(PrintMsg_Print_Params, params)

  
  IPC_STRUCT_MEMBER(std::vector<int>, pages)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintHostMsg_DidPreviewDocument_Params)
  
  IPC_STRUCT_MEMBER(base::SharedMemoryHandle, metafile_data_handle)

  
  IPC_STRUCT_MEMBER(uint32, data_size)

  
  IPC_STRUCT_MEMBER(int, document_cookie)

  
  IPC_STRUCT_MEMBER(int, expected_pages_count)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintHostMsg_DidPrintPage_Params)
  
  
  IPC_STRUCT_MEMBER(base::SharedMemoryHandle, metafile_data_handle)

  
  IPC_STRUCT_MEMBER(uint32, data_size)

  
  IPC_STRUCT_MEMBER(int, document_cookie)

  
  IPC_STRUCT_MEMBER(int, page_number)

  
  IPC_STRUCT_MEMBER(double, actual_shrink)

  
  IPC_STRUCT_MEMBER(gfx::Size, page_size)

  
  IPC_STRUCT_MEMBER(gfx::Rect, content_area)

  
  IPC_STRUCT_MEMBER(bool, has_visible_overlays)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PrintHostMsg_ScriptedPrint_Params)
  IPC_STRUCT_MEMBER(int, routing_id)
  IPC_STRUCT_MEMBER(gfx::NativeViewId, host_window_id)
  IPC_STRUCT_MEMBER(int, cookie)
  IPC_STRUCT_MEMBER(int, expected_pages_count)
  IPC_STRUCT_MEMBER(bool, has_selection)
  IPC_STRUCT_MEMBER(bool, use_overlays)
IPC_STRUCT_END()



IPC_MESSAGE_ROUTED0(PrintMsg_PrintNodeUnderContextMenu)

IPC_MESSAGE_ROUTED1(PrintMsg_PrintForPrintPreview,
                    DictionaryValue )

IPC_MESSAGE_ROUTED0(PrintMsg_PrintPages)

IPC_MESSAGE_ROUTED2(PrintMsg_PrintingDone,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED1(PrintMsg_PrintPreview,
                    DictionaryValue )

IPC_MESSAGE_ROUTED0(PrintMsg_ResetScriptedPrintCount)



#if defined(OS_WIN)
IPC_SYNC_MESSAGE_ROUTED1_1(PrintHostMsg_DuplicateSection,
                           base::SharedMemoryHandle ,
                           base::SharedMemoryHandle )
#endif

IPC_MESSAGE_ROUTED2(PrintHostMsg_DidGetPrintedPagesCount,
                    int ,
                    int )

IPC_MESSAGE_ROUTED1(PrintHostMsg_DidPrintPage,
                    PrintHostMsg_DidPrintPage_Params )

IPC_SYNC_MESSAGE_ROUTED0_1(PrintHostMsg_GetDefaultPrintSettings,
                           PrintMsg_Print_Params )

IPC_SYNC_MESSAGE_ROUTED2_1(PrintHostMsg_UpdatePrintSettings,
                           int ,
                           DictionaryValue ,
                           PrintMsg_PrintPages_Params )

IPC_SYNC_MESSAGE_ROUTED1_1(PrintHostMsg_ScriptedPrint,
                           PrintHostMsg_ScriptedPrint_Params,
                           PrintMsg_PrintPages_Params
                               )

#if defined(USE_X11)
IPC_SYNC_MESSAGE_CONTROL0_2(PrintHostMsg_AllocateTempFileForPrinting,
                            base::FileDescriptor ,
                            int )
IPC_MESSAGE_CONTROL1(PrintHostMsg_TempFileForPrintingWritten,
                     int )
#endif

IPC_MESSAGE_ROUTED0(PrintHostMsg_PrintPreviewNodeUnderContextMenu)

IPC_MESSAGE_ROUTED0(PrintHostMsg_ScriptInitiatedPrintPreview)

IPC_MESSAGE_ROUTED1(PrintHostMsg_PagesReadyForPreview,
                    PrintHostMsg_DidPreviewDocument_Params )
