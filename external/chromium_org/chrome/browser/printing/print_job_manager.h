// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_JOB_MANAGER_H_
#define CHROME_BROWSER_PRINTING_PRINT_JOB_MANAGER_H_

#include <set>
#include <vector>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "printing/print_destination_interface.h"

namespace printing {

class JobEventDetails;
class PrintJob;
class PrinterQuery;

class PrintQueriesQueue : public base::RefCountedThreadSafe<PrintQueriesQueue> {
 public:
  PrintQueriesQueue();

  
  void SetDestination(PrintDestinationInterface* destination);

  
  
  
  void QueuePrinterQuery(PrinterQuery* job);

  
  
  scoped_refptr<PrinterQuery> PopPrinterQuery(int document_cookie);

  
  scoped_refptr<PrinterQuery> CreatePrinterQuery();

  void Shutdown();

 private:
  friend class base::RefCountedThreadSafe<PrintQueriesQueue>;
  typedef std::vector<scoped_refptr<PrinterQuery> > PrinterQueries;

  virtual ~PrintQueriesQueue();

  
  base::Lock lock_;

  PrinterQueries queued_queries_;

  scoped_refptr<PrintDestinationInterface> destination_;

  DISALLOW_COPY_AND_ASSIGN(PrintQueriesQueue);
};

class PrintJobManager : public content::NotificationObserver {
 public:
  PrintJobManager();
  virtual ~PrintJobManager();

  
  void Shutdown();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  scoped_refptr<PrintQueriesQueue> queue();

 private:
  typedef std::set<scoped_refptr<PrintJob> > PrintJobs;

  
  void OnPrintJobEvent(PrintJob* print_job,
                       const JobEventDetails& event_details);

  
  
  void StopJobs(bool wait_for_finish);

  content::NotificationRegistrar registrar_;

  
  PrintJobs current_jobs_;

  scoped_refptr<PrintQueriesQueue> queue_;

  bool is_shutdown_;

  DISALLOW_COPY_AND_ASSIGN(PrintJobManager);
};

}  

#endif  
