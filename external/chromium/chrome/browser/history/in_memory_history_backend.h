// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_IN_MEMORY_HISTORY_BACKEND_H_
#define CHROME_BROWSER_HISTORY_IN_MEMORY_HISTORY_BACKEND_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class FilePath;
class GURL;
class HistoryDatabase;
class Profile;

namespace history {

class InMemoryDatabase;
class InMemoryURLIndex;
struct KeywordSearchTermDetails;
class URLDatabase;
struct URLsDeletedDetails;
struct URLsModifiedDetails;

class InMemoryHistoryBackend : public NotificationObserver {
 public:
  InMemoryHistoryBackend();
  ~InMemoryHistoryBackend();

  
  
  
  
  
  
  
  bool Init(const FilePath& history_filename,
            const FilePath& history_dir,
            URLDatabase* db,
            const std::string& languages);

  
  
  
  void AttachToHistoryService(Profile* profile);

  
  // autocomplete code was written fro this, but it should probably be removed
  
  InMemoryDatabase* db() const {
    return db_.get();
  }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  history::InMemoryURLIndex* InMemoryIndex() const { return index_.get(); }

 private:
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteAll);

  
  void OnTypedURLsModified(const URLsModifiedDetails& details);

  
  void OnURLsDeleted(const URLsDeletedDetails& details);

  
  void OnKeywordSearchTermUpdated(const KeywordSearchTermDetails& details);

  
  bool HasKeyword(const GURL& url);

  NotificationRegistrar registrar_;

  scoped_ptr<InMemoryDatabase> db_;

  
  
  Profile* profile_;

  
  scoped_ptr<history::InMemoryURLIndex> index_;

  DISALLOW_COPY_AND_ASSIGN(InMemoryHistoryBackend);
};

}  

#endif  
