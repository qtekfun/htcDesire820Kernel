// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_IMPORTER_URL_ROW_H_
#define CHROME_COMMON_IMPORTER_IMPORTER_URL_ROW_H_

#include "base/strings/string16.h"
#include "base/time/time.h"
#include "url/gurl.h"

struct ImporterURLRow {
 public:
  ImporterURLRow();
  explicit ImporterURLRow(const GURL& url);

  GURL url;
  base::string16 title;

  
  int visit_count;

  
  int typed_count;

  
  
  base::Time last_visit;

  
  
  bool hidden;
};

#endif  
