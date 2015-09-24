// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_BASE_H_
#define CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_BASE_H_

#include "base/memory/ref_counted.h"
#include "base/prefs/pref_member.h"
#include "base/strings/string16.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "printing/printed_pages_source.h"

struct PrintHostMsg_DidPrintPage_Params;

namespace content {
class RenderViewHost;
}

namespace printing {

class JobEventDetails;
class PrintJob;
class PrintJobWorkerOwner;
class PrintQueriesQueue;

class PrintViewManagerBase : public content::NotificationObserver,
                             public PrintedPagesSource,
                             public content::WebContentsObserver {
 public:
  virtual ~PrintViewManagerBase();

  
  
  
  virtual bool PrintNow();

  
  void UpdateScriptedPrintingBlocked();

  
  virtual base::string16 RenderSourceName() OVERRIDE;

 protected:
  explicit PrintViewManagerBase(content::WebContents* web_contents);

  
  bool PrintNowInternal(IPC::Message* message);

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void OnPrintingFailed(int cookie);

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void DidStartLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  virtual void NavigationStopped() OVERRIDE;

  
  void OnDidGetPrintedPagesCount(int cookie, int number_pages);
  void OnDidGetDocumentCookie(int cookie);
  void OnDidPrintPage(const PrintHostMsg_DidPrintPage_Params& params);

  
  void OnNotifyPrintJobEvent(const JobEventDetails& event_details);

  
  
  
  bool RenderAllMissingPagesNow();

  
  
  
  
  
  void ShouldQuitFromInnerMessageLoop();

  
  
  
  
  bool CreateNewPrintJob(PrintJobWorkerOwner* job);

  
  
  void DisconnectFromCurrentPrintJob();

  
  void PrintingDone(bool success);

  
  
  
  void TerminatePrintJob(bool cancel);

  
  
  void ReleasePrintJob();

  
  
  
  
  bool RunInnerMessageLoop();

  
  
  
  bool OpportunisticallyCreatePrintJob(int cookie);

  
  void ReleasePrinterQuery();

  content::NotificationRegistrar registrar_;

  
  scoped_refptr<PrintJob> print_job_;

  
  int number_pages_;

  
  bool printing_succeeded_;

  
  
  
  bool inside_inner_message_loop_;

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  
  bool expecting_first_page_;
#endif

  
  int cookie_;

  
  BooleanPrefMember printing_enabled_;

  scoped_refptr<printing::PrintQueriesQueue> queue_;

  DISALLOW_COPY_AND_ASSIGN(PrintViewManagerBase);
};

}  

#endif  
