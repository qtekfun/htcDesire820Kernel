// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_SAVE_TYPES_H_
#define CONTENT_BROWSER_DOWNLOAD_SAVE_TYPES_H_

#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "url/gurl.h"

namespace content {
typedef std::vector<std::pair<int, base::FilePath> > FinalNameList;
typedef std::vector<int> SaveIDList;

struct SaveFileCreateInfo {
  enum SaveFileSource {
    
    SAVE_FILE_FROM_UNKNOWN = -1,
    
    SAVE_FILE_FROM_NET = 0,
    
    
    SAVE_FILE_FROM_DOM,
    
    
    SAVE_FILE_FROM_FILE
  };

  SaveFileCreateInfo(const base::FilePath& path,
                     const GURL& url,
                     SaveFileSource save_source,
                     int32 save_id);

  SaveFileCreateInfo();

  ~SaveFileCreateInfo();

  
  
  base::FilePath path;
  
  GURL url;
  
  GURL final_url;
  
  
  int save_id;
  
  int render_process_id;
  int render_view_id;
  
  int request_id;
  
  std::string content_disposition;
  
  int64 total_bytes;
  
  SaveFileSource save_source;
};

}  

#endif  
