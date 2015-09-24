// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_JOB_MANAGER_H_
#define CHROME_BROWSER_PRINTING_PRINT_JOB_MANAGER_H_
#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace printing {

class JobEventDetails;
class PrintedDocument;
class PrintJob;
class PrintedPage;
class PrinterQuery;

class PrintJobManager : public NotificationObserver {
 public:
  PrintJobManager();
  ~PrintJobManager();

  
  void OnQuit();

  
  
  void StopJobs(bool wait_for_finish);

  
  
  
  void QueuePrinterQuery(PrinterQuery* job);

  
  
  void PopPrinterQuery(int document_cookie, scoped_refptr<PrinterQuery>* job);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  bool printing_enabled();

  void set_printing_enabled(bool printing_enabled);

 private:
  typedef std::vector<scoped_refptr<PrintJob> > PrintJobs;
  typedef std::vector<scoped_refptr<PrinterQuery> > PrinterQueries;

  
  void OnPrintJobEvent(PrintJob* print_job,
                       const JobEventDetails& event_details);

  NotificationRegistrar registrar_;

  
  base::Lock lock_;

  
  base::Lock enabled_lock_;

  PrinterQueries queued_queries_;

  
  PrintJobs current_jobs_;

  
  
  
  
  bool printing_enabled_;

  DISALLOW_COPY_AND_ASSIGN(PrintJobManager);
};

}  

#endif  
