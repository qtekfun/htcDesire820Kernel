// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_JOB_H_
#define CHROME_BROWSER_PRINTING_PRINT_JOB_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "chrome/browser/printing/print_job_worker_owner.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Thread;

namespace printing {

class JobEventDetails;

class PrintedDocument;
class PrintedPage;
class PrintedPagesSource;
class PrintJobWorker;
class PrinterQuery;

class PrintJob : public PrintJobWorkerOwner,
                 public content::NotificationObserver,
                 public base::MessageLoop::DestructionObserver {
 public:
  
  
  PrintJob();

  
  
  void Initialize(PrintJobWorkerOwner* job, PrintedPagesSource* source,
                  int page_count);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void GetSettingsDone(const PrintSettings& new_settings,
                               PrintingContext::Result result) OVERRIDE;
  virtual PrintJobWorker* DetachWorker(PrintJobWorkerOwner* new_owner) OVERRIDE;
  virtual base::MessageLoop* message_loop() OVERRIDE;
  virtual const PrintSettings& settings() const OVERRIDE;
  virtual int cookie() const OVERRIDE;

  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

  
  
  void StartPrinting();

  
  
  
  
  
  void Stop();

  
  void Cancel();

  
  
  
  bool FlushJob(base::TimeDelta timeout);

  
  
  void DisconnectSource();

  
  
  bool is_job_pending() const;

  
  bool is_stopping() const;

  
  bool is_stopped() const;

  
  PrintedDocument* document() const;

 protected:
  virtual ~PrintJob();

 private:
  
  void UpdatePrintedDocument(PrintedDocument* new_document);

  
  void OnNotifyPrintJobEvent(const JobEventDetails& event_details);

  
  
  void OnDocumentDone();

  
  
  void ControlledWorkerShutdown();

  
  void Quit();

  void HoldUntilStopIsCalled(const scoped_refptr<PrintJob>& job);

  content::NotificationRegistrar registrar_;

  
  
  base::MessageLoop* const ui_message_loop_;

  
  
  PrintedPagesSource* source_;

  
  
  
  scoped_ptr<PrintJobWorker> worker_;

  
  PrintSettings settings_;

  
  scoped_refptr<PrintedDocument> document_;

  
  bool is_job_pending_;

  
  
  bool is_canceling_;

  
  bool is_stopping_;

  
  bool is_stopped_;

  
  base::WeakPtrFactory<PrintJob> quit_factory_;

  base::WeakPtrFactory<PrintJob> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PrintJob);
};

class JobEventDetails : public base::RefCountedThreadSafe<JobEventDetails> {
 public:
  
  enum Type {
    
    USER_INIT_DONE,

    
    USER_INIT_CANCELED,

    
    DEFAULT_INIT_DONE,

    
    NEW_DOC,

    
    NEW_PAGE,

    
    PAGE_DONE,

    
    
    DOC_DONE,

    
    
    JOB_DONE,

    
    ALL_PAGES_REQUESTED,

    
    FAILED,
  };

  JobEventDetails(Type type, PrintedDocument* document, PrintedPage* page);

  
  PrintedDocument* document() const;
  PrintedPage* page() const;
  Type type() const {
    return type_;
  }

 private:
  friend class base::RefCountedThreadSafe<JobEventDetails>;

  ~JobEventDetails();

  scoped_refptr<PrintedDocument> document_;
  scoped_refptr<PrintedPage> page_;
  const Type type_;

  DISALLOW_COPY_AND_ASSIGN(JobEventDetails);
};

}  

#endif  
