// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_MHTML_GENERATION_MANAGER_H_
#define CONTENT_BROWSER_DOWNLOAD_MHTML_GENERATION_MANAGER_H_

#include <map>

#include "base/memory/singleton.h"
#include "base/platform_file.h"
#include "base/process/process.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ipc/ipc_platform_file.h"

namespace base {
class FilePath;
}

namespace content {
class WebContents;

class MHTMLGenerationManager : public NotificationObserver {
 public:
  static MHTMLGenerationManager* GetInstance();

  typedef base::Callback<void(int64 )>
      GenerateMHTMLCallback;

  
  
  void SaveMHTML(WebContents* web_contents,
                 const base::FilePath& file,
                 const GenerateMHTMLCallback& callback);

  
  
  void StreamMHTML(WebContents* web_contents,
                   const base::PlatformFile file,
                   const GenerateMHTMLCallback& callback);

  
  
  
  void MHTMLGenerated(int job_id, int64 mhtml_data_size);

 private:
  friend struct DefaultSingletonTraits<MHTMLGenerationManager>;

  struct Job{
    Job();
    ~Job();

    
    
    base::PlatformFile browser_file;
    IPC::PlatformFileForTransit renderer_file;

    
    int process_id;
    int routing_id;

    
    GenerateMHTMLCallback callback;
  };

  MHTMLGenerationManager();
  virtual ~MHTMLGenerationManager();

  
  void CreateFile(int job_id,
                  const base::FilePath& file,
                  base::ProcessHandle renderer_process);

  
  
  
  
  void FileHandleAvailable(int job_id,
                           base::PlatformFile browser_file,
                           IPC::PlatformFileForTransit renderer_file);

  
  void CloseFile(base::PlatformFile file);

  
  
  
  void JobFinished(int job_id, int64 mhtml_data_size);

  
  int NewJob(WebContents* web_contents, const GenerateMHTMLCallback& callback);

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  typedef std::map<int, Job> IDToJobMap;
  IDToJobMap id_to_job_;
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(MHTMLGenerationManager);
};

}  

#endif  
