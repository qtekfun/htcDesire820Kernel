// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_HOST_H_
#define CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_HOST_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/importer/importer_host.h"

class ExternalProcessImporterClient;
class Profile;
class ProfileWriter;

namespace importer {
struct SourceProfile;
}

class ExternalProcessImporterHost : public ImporterHost {
 public:
  ExternalProcessImporterHost();

  
  virtual void Cancel() OVERRIDE;

 private:
  
  virtual void StartImportSettings(
      const importer::SourceProfile& source_profile,
      Profile* target_profile,
      uint16 items,
      ProfileWriter* writer,
      bool first_run) OVERRIDE;
  virtual void InvokeTaskIfDone() OVERRIDE;
  virtual void Loaded(BookmarkModel* model) OVERRIDE;

  
  ExternalProcessImporterClient* client_;

  
  const importer::SourceProfile* source_profile_;

  
  uint16 items_;

  
  bool import_to_bookmark_bar_;

  
  bool cancelled_;

  
  
  bool import_process_launched_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProcessImporterHost);
};

#endif  
