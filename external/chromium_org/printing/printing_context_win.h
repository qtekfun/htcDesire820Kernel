// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINTING_CONTEXT_WIN_H_
#define PRINTING_PRINTING_CONTEXT_WIN_H_

#include <ocidl.h>
#include <commdlg.h>

#include <string>

#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "printing/printing_context.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/print_settings_dialog_win.h"

namespace printing {

class PRINTING_EXPORT PrintingContextWin
    : public PrintingContext,
      public ui::PrintSettingsDialogWin::Observer {
 public:
  explicit PrintingContextWin(const std::string& app_locale);
  ~PrintingContextWin();

  
  virtual void AskUserForSettings(
      gfx::NativeView parent_view,
      int max_pages,
      bool has_selection,
      const PrintSettingsCallback& callback) OVERRIDE;
  virtual Result UseDefaultSettings() OVERRIDE;
  virtual gfx::Size GetPdfPaperSizeDeviceUnits() OVERRIDE;
  virtual Result UpdatePrinterSettings(bool external_preview) OVERRIDE;
  virtual Result InitWithSettings(const PrintSettings& settings) OVERRIDE;
  virtual Result NewDocument(const base::string16& document_name) OVERRIDE;
  virtual Result NewPage() OVERRIDE;
  virtual Result PageDone() OVERRIDE;
  virtual Result DocumentDone() OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual void ReleaseContext() OVERRIDE;
  virtual gfx::NativeDrawingContext context() const OVERRIDE;

  
  virtual void PrintSettingsConfirmed(PRINTDLGEX* dialog_options) OVERRIDE;
  virtual void PrintSettingsCancelled(PRINTDLGEX* dialog_options) OVERRIDE;

#if defined(UNIT_TEST) || defined(PRINTING_IMPLEMENTATION)
  
  void SetPrintDialog(HRESULT (__stdcall *print_dialog_func)(LPPRINTDLGEX)) {
    print_dialog_func_ = print_dialog_func;
  }
#endif  

  
  static bool AllocateContext(const std::wstring& printer_name,
                              const DEVMODE* dev_mode,
                              gfx::NativeDrawingContext* context);

 private:
  
  
  class CallbackHandler;

  
  static BOOL CALLBACK AbortProc(HDC hdc, int nCode);

  
  
  bool InitializeSettings(const DEVMODE& dev_mode,
                          const std::wstring& new_device_name,
                          const PRINTPAGERANGE* ranges,
                          int number_ranges,
                          bool selection_only);

  
  
  bool GetPrinterSettings(HANDLE printer,
                          const std::wstring& device_name);

  
  Result ParseDialogResultEx(const PRINTDLGEX& dialog_options);
  Result ParseDialogResult(const PRINTDLG& dialog_options);

  
  HDC context_;

  
  volatile HWND dialog_box_;

  
  
  HRESULT (__stdcall *print_dialog_func_)(LPPRINTDLGEX);

  
  PrintSettingsCallback callback_;

  
  scoped_refptr<ui::PrintSettingsDialogWin> print_settings_dialog_;

  DISALLOW_COPY_AND_ASSIGN(PrintingContextWin);
};

}  

#endif  
