// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_JOB_WORKER_H_
#define CHROME_BROWSER_PRINTING_PRINT_JOB_WORKER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread.h"
#include "printing/page_number.h"
#include "printing/print_destination_interface.h"
#include "printing/printing_context.h"
#include "printing/print_job_constants.h"

class PrintingUIWebContentsObserver;

namespace base {
class DictionaryValue;
}

namespace printing {

class PrintedDocument;
class PrintedPage;
class PrintJob;
class PrintJobWorkerOwner;

class PrintJobWorker : public base::Thread {
 public:
  explicit PrintJobWorker(PrintJobWorkerOwner* owner);
  virtual ~PrintJobWorker();

  void SetNewOwner(PrintJobWorkerOwner* new_owner);

  
  
  void SetPrintDestination(PrintDestinationInterface* destination);

  
  
  void GetSettings(
      bool ask_user_for_settings,
      scoped_ptr<PrintingUIWebContentsObserver> web_contents_observer,
      int document_page_count,
      bool has_selection,
      MarginType margin_type);

  
  
  void SetSettings(const base::DictionaryValue* const new_settings);

  
  
  void StartPrinting(PrintedDocument* new_document);

  
  void OnDocumentChanged(PrintedDocument* new_document);

  
  
  
  void OnNewPage();

  
  void Cancel();

 protected:
  
  PrintingContext* printing_context() { return printing_context_.get(); }

 private:
  
  
  
  
  
  class NotificationTask;

  
  void SpoolPage(PrintedPage* page);

  
  void OnDocumentDone();

  
  
  void OnFailure();

  
  
  
  void GetSettingsWithUI(
      scoped_ptr<PrintingUIWebContentsObserver> web_contents_observer,
      int document_page_count,
      bool has_selection);

  
  
  
  void GetSettingsWithUIDone(PrintingContext::Result result);

  
  
  void UpdatePrintSettings(const base::DictionaryValue* const new_settings);

  
  void GetSettingsDone(PrintingContext::Result result);

  
  
  
  void UseDefaultSettings();

  
  scoped_ptr<PrintingContext> printing_context_;

  
  scoped_refptr<PrintedDocument> document_;

  
  scoped_refptr<PrintDestinationInterface> destination_;

  
  
  PrintJobWorkerOwner* owner_;

  
  PageNumber page_number_;

  
  base::WeakPtrFactory<PrintJobWorker> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PrintJobWorker);
};

}  

#endif  
