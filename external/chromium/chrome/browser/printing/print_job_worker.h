// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_JOB_WORKER_H__
#define CHROME_BROWSER_PRINTING_PRINT_JOB_WORKER_H__
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "base/threading/thread.h"
#include "printing/page_number.h"
#include "printing/printing_context.h"
#include "ui/gfx/native_widget_types.h"

class DictionaryValue;

namespace printing {

class PrintedDocument;
class PrintedPage;
class PrintJob;
class PrintJobWorkerOwner;

class PrintJobWorker : public base::Thread {
 public:
  explicit PrintJobWorker(PrintJobWorkerOwner* owner);
  ~PrintJobWorker();

  void SetNewOwner(PrintJobWorkerOwner* new_owner);

  
  
  void GetSettings(bool ask_user_for_settings,
                   gfx::NativeView parent_view,
                   int document_page_count,
                   bool has_selection,
                   bool use_overlays);

  
  
  void SetSettings(const DictionaryValue* const new_settings);

  
  
  void StartPrinting(PrintedDocument* new_document);

  
  void OnDocumentChanged(PrintedDocument* new_document);

  
  
  
  void OnNewPage();

  
  void Cancel();

 protected:
  
  PrintingContext* printing_context() { return printing_context_.get(); }

 private:
  
  
  
  
  
  class NotificationTask;
  friend struct RunnableMethodTraits<PrintJobWorker>;

  
  void SpoolPage(PrintedPage& page);

  
  void OnDocumentDone();

  
  
  void OnFailure();

  
  
  
  void GetSettingsWithUI(gfx::NativeView parent_view,
                         int document_page_count,
                         bool has_selection);

  
  
  
  void GetSettingsWithUIDone(PrintingContext::Result result);

  
  
  void UpdatePrintSettings(const DictionaryValue* const new_settings);

  
  void GetSettingsDone(PrintingContext::Result result);

  
  
  
  void UseDefaultSettings();

  
  scoped_ptr<PrintingContext> printing_context_;

  
  scoped_refptr<PrintedDocument> document_;

  
  
  PrintJobWorkerOwner* owner_;

  
  PageNumber page_number_;

  DISALLOW_COPY_AND_ASSIGN(PrintJobWorker);
};

}  

template <>
struct RunnableMethodTraits<printing::PrintJobWorker> {
  void RetainCallee(printing::PrintJobWorker* obj);
  void ReleaseCallee(printing::PrintJobWorker* obj);
 private:
  scoped_refptr<printing::PrintJobWorkerOwner> owner_;
};

#endif  
