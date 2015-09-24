// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINTING_CONTEXT_ANDROID_H_
#define PRINTING_PRINTING_CONTEXT_ANDROID_H_

#include <jni.h>

#include <string>

#include "base/android/scoped_java_ref.h"
#include "printing/printing_context.h"

namespace printing {

class PRINTING_EXPORT PrintingContextAndroid : public PrintingContext {
 public:
  explicit PrintingContextAndroid(const std::string& app_locale);
  virtual ~PrintingContextAndroid();

  // Called when the page is successfully written to a PDF using the file
  
  static void PdfWritingDone(int fd, bool success);

  
  
  void AskUserForSettingsReply(JNIEnv* env, jobject obj, jboolean success);

  
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

  
  static bool RegisterPrintingContext(JNIEnv* env);

 private:
  base::android::ScopedJavaGlobalRef<jobject> j_printing_context_;

  
  
  PrintSettingsCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(PrintingContextAndroid);
};

}  

#endif  

