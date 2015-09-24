// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_RENDER_HOST_PRINT_MANAGER_H_
#define ANDROID_WEBVIEW_BROWSER_RENDER_HOST_PRINT_MANAGER_H_

#include "base/callback_forward.h"
#include "base/threading/non_thread_safe.h"
#include "content/public/browser/web_contents_observer.h"

class GURL;

namespace base {
struct FileDescriptor;
}

namespace printing {
class PrintSettings;
}

namespace android_webview {

class PrintManagerDelegate {
 public:
  virtual ~PrintManagerDelegate() { }
  virtual void DidExportPdf(bool success) = 0;
  virtual bool IsCancelled() = 0;

 private:
  
};

class PrintManager : public content::WebContentsObserver,
                     public base::NonThreadSafe {
 public:
  
  
  PrintManager(content::WebContents* contents,
               printing::PrintSettings* settings,
               int fd,
               PrintManagerDelegate* delegate);
  virtual ~PrintManager();

  
  
  
  
  
  
  bool PrintNow();

 private:
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  void OnDidGetPrintedPagesCount(int cookie, int number_pages);
  void OnDidGetDocumentCookie(int cookie);
  void OnPrintingFailed(int cookie);
  void OnGetDefaultPrintSettingsReply(IPC::Message* reply_msg);
  void OnGetDefaultPrintSettings(IPC::Message* reply_msg);
  void OnAllocateTempFileForPrinting(base::FileDescriptor* temp_file_fd,
                                     int* sequence_number);
  void OnTempFileForPrintingWritten(int sequence_number);

  
  printing::PrintSettings* settings_;

  
  int fd_;
  
  PrintManagerDelegate* delegate_;
  
  int number_pages_;
  
  int cookie_;
  
  int printing_;

  DISALLOW_COPY_AND_ASSIGN(PrintManager);
};

}  

#endif  
