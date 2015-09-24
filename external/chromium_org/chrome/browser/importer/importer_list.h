// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_LIST_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_LIST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "content/public/browser/browser_thread.h"

namespace importer {
class ImporterListObserver;
struct SourceProfile;
}

class ImporterList : public base::RefCountedThreadSafe<ImporterList> {
 public:
  ImporterList();

  
  
  
  
  
  void DetectSourceProfiles(const std::string& locale,
                            bool include_interactive_profiles,
                            importer::ImporterListObserver* observer);

  
  
  
  void set_observer(importer::ImporterListObserver* observer) {
    observer_ = observer;
  }

  
  
  
  void DetectSourceProfilesHack(const std::string& locale,
                                bool include_interactive_profiles);

  
  size_t count() const { return source_profiles_.size(); }

  
  
  
  const importer::SourceProfile& GetSourceProfileAt(size_t index) const;

  
  const importer::SourceProfile& GetSourceProfileForImporterType(
      int importer_type) const;

  
  bool source_profiles_loaded() const { return source_profiles_loaded_; }

 private:
  friend class base::RefCountedThreadSafe<ImporterList>;

  ~ImporterList();

  
  
  
  
  void DetectSourceProfilesWorker(const std::string& locale,
                                  bool include_interactive_profiles);

  
  
  
  void SourceProfilesLoaded(
      const std::vector<importer::SourceProfile*>& profiles);

  
  ScopedVector<importer::SourceProfile> source_profiles_;

  
  
  
  content::BrowserThread::ID source_thread_id_;

  
  
  importer::ImporterListObserver* observer_;

  
  
  
  
  bool is_observed_;

  
  bool source_profiles_loaded_;

  DISALLOW_COPY_AND_ASSIGN(ImporterList);
};

#endif  
