// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CRASH_UPLOAD_LIST_H_
#define CHROME_BROWSER_CRASH_UPLOAD_LIST_H_

#include "chrome/browser/upload_list.h"

class CrashUploadList : public UploadList {
 public:
  
  
  static CrashUploadList* Create(Delegate* delegate);

  
  
  static const char* kReporterLogFilename;

  
  CrashUploadList(Delegate* delegate, const base::FilePath& upload_log_path);

 protected:
  virtual ~CrashUploadList();

 private:
  DISALLOW_COPY_AND_ASSIGN(CrashUploadList);
};

#endif  
