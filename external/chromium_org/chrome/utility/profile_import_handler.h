// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_PROFILE_IMPORT_HANDLER_H_
#define CHROME_UTILITY_PROFILE_IMPORT_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/utility/utility_message_handler.h"

class ExternalProcessImporterBridge;
class Importer;

namespace base {
class DictionaryValue;
class Thread;
}

namespace importer {
struct SourceProfile;
}

namespace chrome {

class ProfileImportHandler : public UtilityMessageHandler {
 public:
  ProfileImportHandler();
  virtual ~ProfileImportHandler();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  void OnImportStart(
      const importer::SourceProfile& source_profile,
      uint16 items,
      const base::DictionaryValue& localized_strings);
  void OnImportCancel();
  void OnImportItemFinished(uint16 item);

  
  void ImporterCleanup();

  static bool Send(IPC::Message* message);

  
  
  scoped_ptr<base::Thread> import_thread_;

  
  
  scoped_refptr<ExternalProcessImporterBridge> bridge_;

  
  uint16 items_to_import_;

  
  scoped_refptr<Importer> importer_;
};

}  

#endif  
