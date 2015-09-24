// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_METRO_DRIVER_PRINT_HANDLER_H_
#define CHROME_BROWSER_UI_METRO_DRIVER_PRINT_HANDLER_H_

#include <windows.media.playto.h>
#include <windows.graphics.printing.h>
#include <windows.ui.core.h>

#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "win8/metro_driver/print_document_source.h"
#include "win8/metro_driver/winrt_utils.h"

namespace base {

class Lock;

}  

namespace metro_driver {

class PrintHandler {
 public:
  PrintHandler();
  ~PrintHandler();

  HRESULT Initialize(winui::Core::ICoreWindow* window);

  
  static void EnablePrinting(bool printing_enabled);
  static void SetPageCount(size_t page_count);
  static void AddPage(size_t page_number, IStream* metafile_stream);
  static void ShowPrintUI();

 private:
  
  HRESULT OnPrintRequested(
      wingfx::Printing::IPrintManager* print_mgr,
      wingfx::Printing::IPrintTaskRequestedEventArgs* event_args);

  HRESULT OnPrintTaskSourceRequest(
      wingfx::Printing::IPrintTaskSourceRequestedArgs* args);

  HRESULT OnCompleted(wingfx::Printing::IPrintTask* task,
                      wingfx::Printing::IPrintTaskCompletedEventArgs* args);
  
  void ClearPrintTask();
  float GetLogicalDpi();

  
  HRESULT LogicalDpiChanged(IInspectable *sender);
  static void OnLogicalDpiChanged(float dpi);

  
  static void SetPrintDocumentSource(
      const mswr::ComPtr<PrintDocumentSource>& print_document_source);
  static void ReleasePrintDocumentSource();

  
  static void OnEnablePrinting(bool printing_enabled);
  static void OnSetPageCount(size_t page_count);
  static void OnAddPage(size_t page_number,
                        mswr::ComPtr<IStream> metafile_stream);

  
  static void OnShowPrintUI();

  mswr::ComPtr<wingfx::Printing::IPrintTask> print_task_;
  EventRegistrationToken print_requested_token_;
  EventRegistrationToken print_completed_token_;
  EventRegistrationToken dpi_change_token_;

  mswr::ComPtr<wingfx::Printing::IPrintManager> print_manager_;
  PrintDocumentSource::DirectXContext directx_context_;

  
  
  
  static mswr::ComPtr<PrintDocumentSource> current_document_source_;

  
  
  static bool printing_enabled_;

  
  
  
  static base::Lock* lock_;

  
  
  static base::Thread* thread_;
};

}  

extern "C" __declspec(dllexport)
void MetroEnablePrinting(BOOL printing_enabled);

extern "C" __declspec(dllexport)
void MetroSetPrintPageCount(size_t page_count);

extern "C" __declspec(dllexport)
void MetroSetPrintPageContent(size_t current_page,
                              void* data,
                              size_t data_size);

extern "C" __declspec(dllexport)
void MetroShowPrintUI();

#endif  
