// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_H_
#define CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "printing/printed_pages_source.h"

class RenderViewHost;
class TabContents;
struct PrintHostMsg_DidPrintPage_Params;

namespace printing {

class JobEventDetails;
class PrintJob;
class PrintJobWorkerOwner;

class PrintViewManager : public NotificationObserver,
                         public PrintedPagesSource,
                         public TabContentsObserver {
 public:
  explicit PrintViewManager(TabContents* tab_contents);
  virtual ~PrintViewManager();

  
  
  void OverrideTitle(TabContents* tab_contents);

  
  
  
  bool PrintNow();

  
  virtual string16 RenderSourceName();
  virtual GURL RenderSourceUrl();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  virtual void RenderViewGone();

  
  virtual void StopNavigation();

 private:
  void OnDidGetPrintedPagesCount(int cookie, int number_pages);
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

  NotificationRegistrar registrar_;

  
  scoped_refptr<PrintJob> print_job_;

  
  int number_pages_;

  
  
  
  bool waiting_to_print_;

  
  bool printing_succeeded_;

  
  
  
  bool inside_inner_message_loop_;

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  
  bool expecting_first_page_;
#endif

  
  bool is_title_overridden_;
  string16 overridden_title_;

  DISALLOW_COPY_AND_ASSIGN(PrintViewManager);
};

}  

#endif  
