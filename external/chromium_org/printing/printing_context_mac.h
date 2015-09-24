// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINTING_CONTEXT_MAC_H_
#define PRINTING_PRINTING_CONTEXT_MAC_H_

#include <string>

#include "base/mac/scoped_nsobject.h"
#include "printing/print_job_constants.h"
#include "printing/printing_context.h"

#ifdef __OBJC__
@class NSPrintInfo;
#else
class NSPrintInfo;
#endif  

namespace printing {

class PRINTING_EXPORT PrintingContextMac : public PrintingContext {
 public:
  explicit PrintingContextMac(const std::string& app_locale);
  virtual ~PrintingContextMac();

  
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

 private:
  
  
  
  
  
  void InitPrintSettingsFromPrintInfo();

  
  PageRanges GetPageRangesFromPrintInfo();

  
  
  bool SetPrinter(const std::string& device_name);

  
  
  bool UpdatePageFormatWithPaperInfo();

  
  
  bool SetPrintPreviewJob();

  
  
  bool SetCopiesInPrintSettings(int copies);

  
  
  bool SetCollateInPrintSettings(bool collate);

  
  
  bool SetOrientationIsLandscape(bool landscape);

  
  
  bool SetDuplexModeInPrintSettings(DuplexMode mode);

  
  
  bool SetOutputColor(int color_mode);

  
  base::scoped_nsobject<NSPrintInfo> print_info_;

  
  
  CGContext* context_;

  DISALLOW_COPY_AND_ASSIGN(PrintingContextMac);
};

}  

#endif  
