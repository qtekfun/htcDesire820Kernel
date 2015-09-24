// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_IN_MEMORY_DATABASE_H_
#define CHROME_BROWSER_HISTORY_IN_MEMORY_DATABASE_H_
#pragma once

#include "app/sql/connection.h"
#include "base/basictypes.h"
#include "chrome/browser/history/url_database.h"

class FilePath;

namespace history {

class InMemoryDatabase : public URLDatabase {
 public:
  InMemoryDatabase();
  virtual ~InMemoryDatabase();

  
  bool InitFromScratch();

  
  
  
  
  bool InitFromDisk(const FilePath& history_name);

 protected:
  
  virtual sql::Connection& GetDB();

 private:
  
  
  bool InitDB();

  sql::Connection db_;

  DISALLOW_COPY_AND_ASSIGN(InMemoryDatabase);
};

}  

#endif  
