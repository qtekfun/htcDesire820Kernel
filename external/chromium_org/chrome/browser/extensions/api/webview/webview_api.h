// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEBVIEW_WEBVIEW_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEBVIEW_WEBVIEW_API_H_

#include "chrome/browser/extensions/api/capture_web_contents_function.h"
#include "chrome/browser/extensions/api/execute_code_function.h"
#include "chrome/browser/guestview/webview/webview_guest.h"

namespace extensions {

class WebviewExtensionFunction : public AsyncExtensionFunction {
 public:
   WebviewExtensionFunction() {}

 protected:
  virtual ~WebviewExtensionFunction() {}

  
  virtual bool RunImpl() OVERRIDE FINAL;

 private:
  virtual bool RunImplSafe(WebViewGuest* guest) = 0;
};

class WebviewClearDataFunction : public WebviewExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webview.clearData", WEBVIEW_CLEARDATA);

  WebviewClearDataFunction();

 protected:
  virtual ~WebviewClearDataFunction();

 private:
  
  virtual bool RunImplSafe(WebViewGuest* guest) OVERRIDE;

  uint32 GetRemovalMask();
  void ClearDataDone();

  
  base::Time remove_since_;
  
  uint32 remove_mask_;
  
  bool bad_message_;

  DISALLOW_COPY_AND_ASSIGN(WebviewClearDataFunction);
};

class WebviewExecuteCodeFunction : public extensions::ExecuteCodeFunction {
 public:
  WebviewExecuteCodeFunction();

 protected:
  virtual ~WebviewExecuteCodeFunction();

  
  virtual bool Init() OVERRIDE;
  virtual bool ShouldInsertCSS() const OVERRIDE;
  virtual bool CanExecuteScriptOnPage() OVERRIDE;
  
  virtual extensions::ScriptExecutor* GetScriptExecutor() OVERRIDE FINAL;
  virtual bool IsWebView() const OVERRIDE;

 private:
  
  
  extensions::ExtensionResource resource_;

  int guest_instance_id_;

  DISALLOW_COPY_AND_ASSIGN(WebviewExecuteCodeFunction);
};

class WebviewExecuteScriptFunction : public WebviewExecuteCodeFunction {
 public:
  WebviewExecuteScriptFunction();

 protected:
  virtual ~WebviewExecuteScriptFunction() {}

  virtual void OnExecuteCodeFinished(const std::string& error,
                                     int32 on_page_id,
                                     const GURL& on_url,
                                     const base::ListValue& result) OVERRIDE;

  DECLARE_EXTENSION_FUNCTION("webview.executeScript", WEBVIEW_EXECUTESCRIPT)

 private:
  DISALLOW_COPY_AND_ASSIGN(WebviewExecuteScriptFunction);
};

class WebviewInsertCSSFunction : public WebviewExecuteCodeFunction {
 public:
  WebviewInsertCSSFunction();

 protected:
  virtual ~WebviewInsertCSSFunction() {}

  virtual bool ShouldInsertCSS() const OVERRIDE;

  DECLARE_EXTENSION_FUNCTION("webview.insertCSS", WEBVIEW_INSERTCSS)

 private:
  DISALLOW_COPY_AND_ASSIGN(WebviewInsertCSSFunction);
};

class WebviewCaptureVisibleRegionFunction
    : public extensions::CaptureWebContentsFunction {
  DECLARE_EXTENSION_FUNCTION("webview.captureVisibleRegion",
                             WEBVIEW_CAPTUREVISIBLEREGION);

  WebviewCaptureVisibleRegionFunction();

 protected:
  virtual ~WebviewCaptureVisibleRegionFunction();

 private:
  
  virtual bool IsScreenshotEnabled() OVERRIDE;
  virtual content::WebContents* GetWebContentsForID(int id) OVERRIDE;
  virtual void OnCaptureFailure(FailureReason reason) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewCaptureVisibleRegionFunction);
};

class WebviewGoFunction : public WebviewExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webview.go", WEBVIEW_GO);

  WebviewGoFunction();

 protected:
  virtual ~WebviewGoFunction();

 private:
  
  virtual bool RunImplSafe(WebViewGuest* guest) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewGoFunction);
};

class WebviewReloadFunction : public WebviewExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webview.reload", WEBVIEW_RELOAD);

  WebviewReloadFunction();

 protected:
  virtual ~WebviewReloadFunction();

 private:
  
  virtual bool RunImplSafe(WebViewGuest* guest) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewReloadFunction);
};

class WebviewSetPermissionFunction : public WebviewExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webview.setPermission", WEBVIEW_SETPERMISSION);

  WebviewSetPermissionFunction();

 protected:
  virtual ~WebviewSetPermissionFunction();

 private:
  
  virtual bool RunImplSafe(WebViewGuest* guest) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewSetPermissionFunction);
};

class WebviewOverrideUserAgentFunction: public WebviewExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webview.overrideUserAgent",
                             WEBVIEW_OVERRIDEUSERAGENT);

  WebviewOverrideUserAgentFunction();

 protected:
  virtual ~WebviewOverrideUserAgentFunction();

 private:
  
  virtual bool RunImplSafe(WebViewGuest* guest) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewOverrideUserAgentFunction);
};

class WebviewStopFunction : public WebviewExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webview.stop", WEBVIEW_STOP);

  WebviewStopFunction();

 protected:
  virtual ~WebviewStopFunction();

 private:
  
  virtual bool RunImplSafe(WebViewGuest* guest) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewStopFunction);
};

class WebviewTerminateFunction : public WebviewExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("webview.terminate", WEBVIEW_TERMINATE);

  WebviewTerminateFunction();

 protected:
  virtual ~WebviewTerminateFunction();

 private:
  
  virtual bool RunImplSafe(WebViewGuest* guest) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewTerminateFunction);
};

}  

#endif  
