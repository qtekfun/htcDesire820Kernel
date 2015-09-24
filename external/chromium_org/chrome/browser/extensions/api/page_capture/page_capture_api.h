// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PAGE_CAPTURE_PAGE_CAPTURE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_PAGE_CAPTURE_PAGE_CAPTURE_API_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/common/extensions/api/page_capture.h"
#include "webkit/common/blob/shareable_file_reference.h"

namespace base {
class FilePath;
}

namespace content {
class WebContents;
}

namespace extensions {

class PageCaptureSaveAsMHTMLFunction : public ChromeAsyncExtensionFunction {
 public:
  PageCaptureSaveAsMHTMLFunction();

  
  class TestDelegate {
   public:
    
    
    virtual void OnTemporaryFileCreated(const base::FilePath& temp_file) = 0;
  };
  static void SetTestDelegate(TestDelegate* delegate);

 private:
  virtual ~PageCaptureSaveAsMHTMLFunction();
  virtual bool RunImpl() OVERRIDE;
  virtual bool OnMessageReceivedFromRenderView(
      const IPC::Message& message) OVERRIDE;

  
  void CreateTemporaryFile();

  
  void TemporaryFileCreated(bool success);
  void ReturnFailure(const std::string& error);
  void ReturnSuccess(int64 file_size);

  
  void MHTMLGenerated(int64 mhtml_file_size);

  
  content::WebContents* GetWebContents();

  scoped_ptr<extensions::api::page_capture::SaveAsMHTML::Params> params_;

  
  base::FilePath mhtml_path_;

  
  scoped_refptr<webkit_blob::ShareableFileReference> mhtml_file_;

  DECLARE_EXTENSION_FUNCTION("pageCapture.saveAsMHTML", PAGECAPTURE_SAVEASMHTML)
};

}  

#endif  
