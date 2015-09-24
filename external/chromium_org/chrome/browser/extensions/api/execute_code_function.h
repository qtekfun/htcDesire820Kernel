// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_EXECUTE_CODE_FUNCTION_H_
#define CHROME_BROWSER_EXTENSIONS_API_EXECUTE_CODE_FUNCTION_H_

#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/extensions/script_executor.h"
#include "chrome/common/extensions/api/tabs.h"

namespace extensions {

class ExecuteCodeFunction : public ChromeAsyncExtensionFunction {
 public:
  ExecuteCodeFunction();

 protected:
  virtual ~ExecuteCodeFunction();

  
  virtual bool HasPermission() OVERRIDE;
  virtual bool RunImpl() OVERRIDE;

  
  virtual bool Init() = 0;
  virtual bool ShouldInsertCSS() const = 0;
  virtual bool CanExecuteScriptOnPage() = 0;
  virtual ScriptExecutor* GetScriptExecutor() = 0;
  virtual bool IsWebView() const = 0;

  virtual void OnExecuteCodeFinished(const std::string& error,
                                     int32 on_page_id,
                                     const GURL& on_url,
                                     const base::ListValue& result);

  
  scoped_ptr<api::tabs::InjectDetails> details_;

 private:
  
  
  void DidLoadFile(bool success, const std::string& data);

  
  
  void GetFileURLAndLocalizeCSS(
      ScriptExecutor::ScriptType script_type,
      const std::string& data,
      const std::string& extension_id,
      const base::FilePath& extension_path,
      const std::string& extension_default_locale);

  
  void DidLoadAndLocalizeFile(bool success, const std::string& data);

  
  
  bool Execute(const std::string& code_string);

  
  
  ExtensionResource resource_;

  
  GURL file_url_;
};

}  

#endif  

