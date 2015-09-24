// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_MEDIA_GALLERIES_IPHOTO_LIBRARY_H_
#define CHROME_COMMON_MEDIA_GALLERIES_IPHOTO_LIBRARY_H_

#include <map>
#include <set>

#include "base/files/file_path.h"

namespace iphoto {
namespace parser {

struct Photo {
  Photo();
  Photo(uint64 id,
        const base::FilePath& location,
        const base::FilePath& original_location);
  bool operator<(const Photo& other) const;

  uint64 id;
  base::FilePath location;
  base::FilePath original_location;
};

typedef std::set<uint64> Album;
typedef std::map<std::string , Album> Albums;

struct Library {
  Library();
  Library(const Albums& albums, const std::set<Photo>& all_photos);
  ~Library();

  Albums albums;
  std::set<Photo> all_photos;
};

}  
}  

#endif  

