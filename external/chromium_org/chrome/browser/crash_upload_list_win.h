// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CRASH_UPLOAD_LIST_WIN_H_
#define CHROME_BROWSER_CRASH_UPLOAD_LIST_WIN_H_

#include "chrome/browser/crash_upload_list.h"
#include "base/compiler_specific.h"

class CrashUploadListWin : public CrashUploadList {
 public:
  CrashUploadListWin(Delegate* delegate, const base::FilePath& upload_log_path);

 protected:
  
  virtual void LoadUploadList() OVERRIDE;

 private:
  
  bool IsPossibleCrashLogRecord(EVENTLOGRECORD* record) const;

  
  void ProcessPossibleCrashLogRecord(EVENTLOGRECORD* record);

  DISALLOW_COPY_AND_ASSIGN(CrashUploadListWin);
};

#endif  
