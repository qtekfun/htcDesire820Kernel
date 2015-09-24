// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_LIST_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_LIST_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/string16.h"
#include "content/browser/browser_thread.h"

namespace importer {
struct SourceProfile;
}

class ImporterList : public base::RefCountedThreadSafe<ImporterList> {
 public:
  
  
  class Observer {
   public:
    virtual void OnSourceProfilesLoaded() = 0;

   protected:
    virtual ~Observer() {}
  };

  ImporterList();

  
  
  
  
  void DetectSourceProfiles(Observer* observer);

  
  
  
  void SetObserver(Observer* observer);

  
  
  void DetectSourceProfilesHack();

  
  size_t count() const { return source_profiles_.size(); }

  
  
  
  const importer::SourceProfile& GetSourceProfileAt(size_t index) const;

  
  const importer::SourceProfile& GetSourceProfileForImporterType(
      int importer_type) const;

 private:
  friend class base::RefCountedThreadSafe<ImporterList>;

  ~ImporterList();

  
  
  void DetectSourceProfilesWorker();

  
  
  
  void SourceProfilesLoaded(
      const std::vector<importer::SourceProfile*>& profiles);

  
  ScopedVector<importer::SourceProfile> source_profiles_;

  
  
  
  BrowserThread::ID source_thread_id_;

  
  
  Observer* observer_;

  
  
  
  
  bool is_observed_;

  
  bool source_profiles_loaded_;

  DISALLOW_COPY_AND_ASSIGN(ImporterList);
};

#endif  
