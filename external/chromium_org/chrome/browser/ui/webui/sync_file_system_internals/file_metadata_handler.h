// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SYNC_FILE_SYSTEM_INTERNALS_FILE_METADATA_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_SYNC_FILE_SYSTEM_INTERNALS_FILE_METADATA_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/sync_file_system/remote_file_sync_service.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "content/public/browser/web_ui_message_handler.h"

class Profile;

namespace syncfs_internals {

class FileMetadataHandler : public content::WebUIMessageHandler {
 public:
  explicit FileMetadataHandler(Profile* profile);
  virtual ~FileMetadataHandler();

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  void GetExtensions(const base::ListValue* args);

  void GetFileMetadata(const base::ListValue* args);
  void DidGetFileMetadata(const base::ListValue* files);

  Profile* profile_;
  base::WeakPtrFactory<FileMetadataHandler> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(FileMetadataHandler);
};
}  

#endif  
