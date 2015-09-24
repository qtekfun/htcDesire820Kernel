// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_PROFILE_IMPORT_PROCESS_HOST_H_
#define CHROME_BROWSER_IMPORTER_PROFILE_IMPORT_PROCESS_HOST_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/importer/importer_data_types.h"
#include "chrome/browser/importer/profile_writer.h"
#include "content/browser/browser_child_process_host.h"
#include "content/browser/browser_thread.h"

class ProfileImportProcessClient;

namespace webkit_glue {
struct PasswordForm;
}

class ProfileImportProcessHost : public BrowserChildProcessHost {
 public:
  
  
  
  
  
  
  ProfileImportProcessHost(ProfileImportProcessClient* import_process_client,
                           BrowserThread::ID thread_id);
  virtual ~ProfileImportProcessHost();

  
  
  bool StartProfileImportProcess(const importer::SourceProfile& source_profile,
                                 uint16 items,
                                 bool import_to_bookmark_bar);

  
  bool CancelProfileImportProcess();

  
  
  
  bool ReportImportItemFinished(importer::ImportItem item);

 protected:
  
  virtual FilePath GetProfileImportProcessCmd();

 private:
  
  bool StartProcess();

  
  
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;
  virtual bool CanShutdown() OVERRIDE;

  
  scoped_refptr<ProfileImportProcessClient> import_process_client_;

  
  BrowserThread::ID thread_id_;

  DISALLOW_COPY_AND_ASSIGN(ProfileImportProcessHost);
};

#endif  
