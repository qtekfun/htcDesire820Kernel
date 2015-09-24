// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_JOB_H_
#define CHROME_BROWSER_PRINTING_PRINT_JOB_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "chrome/browser/printing/print_job_worker_owner.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class Thread;

namespace printing {

class JobEventDetails;

class PrintedDocument;
class PrintedPage;
class PrintedPagesSource;
class PrintJobWorker;
class PrinterQuery;

class PrintJob : public PrintJobWorkerOwner,
                 public NotificationObserver,
                 public MessageLoop::DestructionObserver {
 public:
  
  
  PrintJob();

  
  
  void Initialize(PrintJobWorkerOwner* job, PrintedPagesSource* source,
                  int page_count);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void GetSettingsDone(const PrintSettings& new_settings,
                               PrintingContext::Result result);
  virtual PrintJobWorker* DetachWorker(PrintJobWorkerOwner* new_owner);
  virtual MessageLoop* message_loop();
  virtual const PrintSettings& settings() const;
  virtual int cookie() const;

  
  virtual void WillDestroyCurrentMessageLoop();

  
  
  void StartPrinting();

  
  
  
  
  void Stop();

  
  void Cancel();

  
  
  
  bool FlushJob(int timeout_ms);

  
  
  void DisconnectSource();

  
  
  bool is_job_pending() const;

  
  PrintedDocument* document() const;

 protected:
  virtual ~PrintJob();

 private:
  
  void UpdatePrintedDocument(PrintedDocument* new_document);

  
  void OnNotifyPrintJobEvent(const JobEventDetails& event_details);

  
  
  void OnDocumentDone();

  
  
  void ControlledWorkerShutdown();

  NotificationRegistrar registrar_;

  
  
  MessageLoop* const ui_message_loop_;

  
  
  PrintedPagesSource* source_;

  
  
  
  scoped_ptr<PrintJobWorker> worker_;

  
  PrintSettings settings_;

  
  scoped_refptr<PrintedDocument> document_;

  
  bool is_job_pending_;

  
  
  bool is_canceling_;

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
