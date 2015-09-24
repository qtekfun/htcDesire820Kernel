// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_PROFILE_WRITER_H_
#define CHROME_BROWSER_IMPORTER_PROFILE_WRITER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "chrome/browser/history/history_types.h"
#include "url/gurl.h"

struct ImportedBookmarkEntry;
struct ImportedFaviconUsage;
class Profile;
class TemplateURL;

namespace autofill {
struct PasswordForm;
}

#if defined(OS_WIN)
struct IE7PasswordInfo;
#endif

class ProfileWriter : public base::RefCountedThreadSafe<ProfileWriter> {
 public:
  explicit ProfileWriter(Profile* profile);

  
  
  
  virtual bool BookmarkModelIsLoaded() const;
  virtual bool TemplateURLServiceIsLoaded() const;

  
  virtual void AddPasswordForm(const autofill::PasswordForm& form);

#if defined(OS_WIN)
  virtual void AddIE7PasswordInfo(const IE7PasswordInfo& info);
#endif

  virtual void AddHistoryPage(const history::URLRows& page,
                              history::VisitSource visit_source);

  virtual void AddHomepage(const GURL& homepage);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void AddBookmarks(
      const std::vector<ImportedBookmarkEntry>& bookmarks,
      const base::string16& top_level_folder_name);

  virtual void AddFavicons(
      const std::vector<ImportedFaviconUsage>& favicons);

  
  
  
  
  
  
  
  
  virtual void AddKeywords(ScopedVector<TemplateURL> template_urls,
                           bool unique_on_host_and_path);

 protected:
  friend class base::RefCountedThreadSafe<ProfileWriter>;

  virtual ~ProfileWriter();

 private:
  Profile* const profile_;

  DISALLOW_COPY_AND_ASSIGN(ProfileWriter);
};

#endif  
