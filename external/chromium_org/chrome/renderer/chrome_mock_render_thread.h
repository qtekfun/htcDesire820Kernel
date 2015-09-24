// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_CHROME_MOCK_RENDER_THREAD_H_
#define CHROME_RENDERER_CHROME_MOCK_RENDER_THREAD_H_

#include <string>

#include "base/compiler_specific.h"
#include "content/public/test/mock_render_thread.h"

namespace base {
class DictionaryValue;
}

class MockPrinter;
struct ExtensionMsg_ExternalConnectionInfo;
struct PrintHostMsg_DidGetPreviewPageCount_Params;
struct PrintHostMsg_DidPreviewPage_Params;
struct PrintHostMsg_DidPrintPage_Params;
struct PrintHostMsg_ScriptedPrint_Params;
struct PrintMsg_PrintPages_Params;
struct PrintMsg_Print_Params;

class ChromeMockRenderThread : public content::MockRenderThread {
 public:
  ChromeMockRenderThread();
  virtual ~ChromeMockRenderThread();

  
  virtual scoped_refptr<base::MessageLoopProxy> GetIOMessageLoopProxy()
      OVERRIDE;

  
  

  
  void set_io_message_loop_proxy(
      const scoped_refptr<base::MessageLoopProxy>& proxy);

#if defined(ENABLE_PRINTING)
  
  MockPrinter* printer();

  
  
  void set_print_dialog_user_response(bool response);

  
  void set_print_preview_cancel_page_number(int page);

  
  int print_preview_pages_remaining() const;
#endif

 private:
  
  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  void OnOpenChannelToExtension(int routing_id,
                                const ExtensionMsg_ExternalConnectionInfo& info,
                                const std::string& channel_name,
                                bool include_tls_channel_id,
                                int* port_id);

#if defined(ENABLE_PRINTING)
#if defined(OS_CHROMEOS) || defined(OS_ANDROID)
  void OnAllocateTempFileForPrinting(int render_view_id,
                                     base::FileDescriptor* renderer_fd,
                                     int* browser_fd);
  void OnTempFileForPrintingWritten(int render_view_id, int browser_fd);
#endif

  
  void OnGetDefaultPrintSettings(PrintMsg_Print_Params* setting);

  
  void OnScriptedPrint(const PrintHostMsg_ScriptedPrint_Params& params,
                       PrintMsg_PrintPages_Params* settings);

  void OnDidGetPrintedPagesCount(int cookie, int number_pages);
  void OnDidPrintPage(const PrintHostMsg_DidPrintPage_Params& params);
  void OnDidGetPreviewPageCount(
      const PrintHostMsg_DidGetPreviewPageCount_Params& params);
  void OnDidPreviewPage(const PrintHostMsg_DidPreviewPage_Params& params);
  void OnCheckForCancel(int32 preview_ui_id,
                        int preview_request_id,
                        bool* cancel);


  
  void OnUpdatePrintSettings(int document_cookie,
                             const base::DictionaryValue& job_settings,
                             PrintMsg_PrintPages_Params* params);

  
  scoped_ptr<MockPrinter> printer_;

  
  bool print_dialog_user_response_;

  
  
  int print_preview_cancel_page_number_;

  
  int print_preview_pages_remaining_;
#endif

  scoped_refptr<base::MessageLoopProxy> io_message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(ChromeMockRenderThread);
};

#endif  
