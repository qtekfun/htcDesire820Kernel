// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CONTENT_BROWSER_DOWNLOAD_SAVE_ITEM_H_
#define CONTENT_BROWSER_DOWNLOAD_SAVE_ITEM_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "content/browser/download/save_types.h"
#include "content/public/common/referrer.h"
#include "url/gurl.h"

namespace content {
class SavePackage;

class SaveItem {
 public:
  enum SaveState {
    WAIT_START,
    IN_PROGRESS,
    COMPLETE,
    CANCELED
  };

  SaveItem(const GURL& url,
           const Referrer& referrer,
           SavePackage* package,
           SaveFileCreateInfo::SaveFileSource save_source);

  ~SaveItem();

  void Start();

  
  void Update(int64 bytes_so_far);

  
  void Cancel();

  
  void Finish(int64 size, bool is_success);

  
  
  int PercentComplete() const;

  
  void Rename(const base::FilePath& full_path);

  void SetSaveId(int32 save_id);

  void SetTotalBytes(int64 total_bytes);

  
  SaveState state() const { return state_; }
  const base::FilePath& full_path() const { return full_path_; }
  const base::FilePath& file_name() const { return file_name_; }
  const GURL& url() const { return url_; }
  const Referrer& referrer() const { return referrer_; }
  int64 total_bytes() const { return total_bytes_; }
  int64 received_bytes() const { return received_bytes_; }
  int32 save_id() const { return save_id_; }
  bool has_final_name() const { return has_final_name_; }
  bool success() const { return is_success_; }
  SaveFileCreateInfo::SaveFileSource save_source() const {
    return save_source_;
  }
  SavePackage* package() const { return package_; }

 private:
  
  void UpdateSize(int64 size);

  
  int32 save_id_;

  
  base::FilePath full_path_;

  
  base::FilePath file_name_;

  
  GURL url_;
  Referrer referrer_;

  
  int64 total_bytes_;

  
  int64 received_bytes_;

  
  SaveState state_;

  
  bool has_final_name_;

  
  bool is_success_;

  SaveFileCreateInfo::SaveFileSource save_source_;

  
  SavePackage* package_;

  DISALLOW_COPY_AND_ASSIGN(SaveItem);
};

}  

#endif  
