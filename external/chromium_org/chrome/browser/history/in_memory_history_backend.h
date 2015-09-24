// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_IN_MEMORY_HISTORY_BACKEND_H_
#define CHROME_BROWSER_HISTORY_IN_MEMORY_HISTORY_BACKEND_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GURL;
class Profile;

namespace base {
class FilePath;
}

namespace history {

class InMemoryDatabase;
class InMemoryURLIndex;
struct KeywordSearchUpdatedDetails;
struct KeywordSearchDeletedDetails;
class URLDatabase;
struct URLsDeletedDetails;
struct URLsModifiedDetails;

class InMemoryHistoryBackend : public content::NotificationObserver {
 public:
  InMemoryHistoryBackend();
  virtual ~InMemoryHistoryBackend();

  
  
  
  bool Init(const base::FilePath& history_filename, URLDatabase* db);

  
  
  
  void AttachToHistoryService(Profile* profile);

  
  // autocomplete code was written fro this, but it should probably be removed
  
  InMemoryDatabase* db() const {
    return db_.get();
  }

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(HistoryBackendTest, DeleteAll);

  
  void OnTypedURLsModified(const URLsModifiedDetails& details);

  
  void OnURLsDeleted(const URLsDeletedDetails& details);

  
  void OnKeywordSearchTermUpdated(const KeywordSearchUpdatedDetails& details);

  
  void OnKeywordSearchTermDeleted(const KeywordSearchDeletedDetails& details);

  
  bool HasKeyword(const GURL& url);

  content::NotificationRegistrar registrar_;

  scoped_ptr<InMemoryDatabase> db_;

  
  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(InMemoryHistoryBackend);
};

}  

#endif  
