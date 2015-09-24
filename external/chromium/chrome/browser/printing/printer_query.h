// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINTER_QUERY_H_
#define CHROME_BROWSER_PRINTING_PRINTER_QUERY_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/printing/print_job_worker_owner.h"
#include "ui/gfx/native_widget_types.h"

class CancelableTask;
class DictionaryValue;
class MessageLoop;

namespace base {
class Thread;
}

namespace printing {

class PrintJobWorker;

class PrinterQuery : public PrintJobWorkerOwner {
 public:
  
  enum GetSettingsAskParam {
    DEFAULTS,
    ASK_USER,
  };

  PrinterQuery();

  
  virtual void GetSettingsDone(const PrintSettings& new_settings,
                               PrintingContext::Result result);
  virtual PrintJobWorker* DetachWorker(PrintJobWorkerOwner* new_owner);
  virtual MessageLoop* message_loop();
  virtual const PrintSettings& settings() const;
  virtual int cookie() const;

  
  
  
  
  void GetSettings(GetSettingsAskParam ask_user_for_settings,
                   gfx::NativeView parent_view,
                   int expected_page_count,
                   bool has_selection,
                   bool use_overlays,
                   CancelableTask* callback);

  
  void SetSettings(const DictionaryValue& new_settings,
                   CancelableTask* callback);

  
  void StopWorker();

  
  bool is_callback_pending() const;

  PrintingContext::Result last_status() const { return last_status_; }

  
  bool is_valid() const;

 private:
  virtual ~PrinterQuery();

  
  
  bool StartWorker(CancelableTask* callback);

  
  
  MessageLoop* const io_message_loop_;

  
  
  
  scoped_ptr<PrintJobWorker> worker_;

  
  PrintSettings settings_;

  
  bool is_print_dialog_box_shown_;

  
  int cookie_;

  
  PrintingContext::Result last_status_;

  
  scoped_ptr<CancelableTask> callback_;

  DISALLOW_COPY_AND_ASSIGN(PrinterQuery);
};

}  

#endif  
