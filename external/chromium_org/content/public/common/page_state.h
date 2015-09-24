// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_PAGE_STATE_H_
#define CONTENT_PUBLIC_COMMON_PAGE_STATE_H_

#include <string>
#include <vector>

#include "content/common/content_export.h"

class GURL;

namespace base {
class FilePath;
}

namespace content {

class CONTENT_EXPORT PageState {
 public:
  static PageState CreateFromEncodedData(const std::string& data);
  static PageState CreateFromURL(const GURL& url);

  static PageState CreateForTesting(
      const GURL& url,
      bool body_contains_password_data,
      const char* optional_body_data,
      const base::FilePath* optional_body_file_path);

  PageState();

  bool IsValid() const;
  bool Equals(const PageState& page_state) const;
  const std::string& ToEncodedData() const;

  std::vector<base::FilePath> GetReferencedFiles() const;
  PageState RemovePasswordData() const;
  PageState RemoveScrollOffset() const;

 private:
  PageState(const std::string& data);

  std::string data_;
};

inline bool operator==(const PageState& a, const PageState& b) {
  return a.Equals(b);
}
inline bool operator!=(const PageState& a, const PageState& b) {
  return !(a == b);
}

}  

#endif  
