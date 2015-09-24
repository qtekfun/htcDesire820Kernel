// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SANDBOXED_ZIP_ANALYZER_H_
#define CHROME_BROWSER_SAFE_BROWSING_SANDBOXED_ZIP_ANALYZER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "content/public/browser/utility_process_host.h"
#include "content/public/browser/utility_process_host_client.h"

namespace IPC {
class Message;
}

namespace safe_browsing {
namespace zip_analyzer {
struct Results;
}

class SandboxedZipAnalyzer : public content::UtilityProcessHostClient {
 public:
  
  typedef base::Callback<void(const zip_analyzer::Results&)> ResultCallback;

  SandboxedZipAnalyzer(const base::FilePath& zip_file,
                       const ResultCallback& result_callback);

  
  void Start();

 private:
  virtual ~SandboxedZipAnalyzer();

  
  
  void AnalyzeInSandbox();

  
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  void OnUtilityProcessStarted();

  
  
  void OnAnalyzeZipFileFinished(const zip_analyzer::Results& results);

  
  void StartProcessOnIOThread();

  const base::FilePath zip_file_;
  
  
  base::PlatformFile zip_platform_file_;
  base::WeakPtr<content::UtilityProcessHost> utility_process_host_;
  const ResultCallback callback_;
  
  bool callback_called_;

  DISALLOW_COPY_AND_ASSIGN(SandboxedZipAnalyzer);
};

}  

#endif  
