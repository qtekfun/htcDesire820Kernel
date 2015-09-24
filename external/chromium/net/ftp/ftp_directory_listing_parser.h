// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_DIRECTORY_LISTING_PARSER_H_
#define NET_FTP_FTP_DIRECTORY_LISTING_PARSER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "base/time.h"

namespace net {

struct FtpDirectoryListingEntry {
  enum Type {
    FILE,
    DIRECTORY,
    SYMLINK,
  };

  FtpDirectoryListingEntry();

  Type type;
  string16 name;  
  std::string raw_name;  
  int64 size;  

  
  base::Time last_modified;
};

int ParseFtpDirectoryListing(const std::string& text,
                             const base::Time& current_time,
                             std::vector<FtpDirectoryListingEntry>* entries);

}  

#endif  
