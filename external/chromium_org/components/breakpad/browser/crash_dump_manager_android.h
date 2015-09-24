// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BREAKPAD_BROWSER_CRASH_DUMP_MANAGER_ANDROID_H_
#define COMPONENTS_BREAKPAD_BROWSER_CRASH_DUMP_MANAGER_ANDROID_H_

#include <map>

#include "base/files/file_path.h"
#include "base/platform_file.h"
#include "base/process/process.h"
#include "base/synchronization/lock.h"
#include "content/public/browser/browser_child_process_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace content {
class RenderProcessHost;
}

namespace breakpad {

class CrashDumpManager : public content::BrowserChildProcessObserver,
                         public content::NotificationObserver {
 public:
  
  static CrashDumpManager* GetInstance();

  
  explicit CrashDumpManager(const base::FilePath& crash_dump_dir);

  virtual ~CrashDumpManager();

  
  
  int CreateMinidumpFile(int child_process_id);

 private:
  typedef std::map<int, base::FilePath> ChildProcessIDToMinidumpPath;

  static void ProcessMinidump(const base::FilePath& minidump_path,
                              base::ProcessHandle pid);

  
  virtual void BrowserChildProcessHostDisconnected(
      const content::ChildProcessData& data) OVERRIDE;
  virtual void BrowserChildProcessCrashed(
      const content::ChildProcessData& data) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnChildExit(int child_process_id, base::ProcessHandle pid);

  content::NotificationRegistrar notification_registrar_;

  
  
  base::Lock child_process_id_to_minidump_path_lock_;
  ChildProcessIDToMinidumpPath child_process_id_to_minidump_path_;

  base::FilePath crash_dump_dir_;

  static CrashDumpManager* instance_;

  DISALLOW_COPY_AND_ASSIGN(CrashDumpManager);
};

}  

#endif  
