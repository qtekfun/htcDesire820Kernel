// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_PROFILE_WRITER_H_
#define CHROME_BROWSER_IMPORTER_PROFILE_WRITER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "build/build_config.h"
#include "chrome/browser/history/history_types.h"
#include "googleurl/src/gurl.h"

class BookmarkModel;
class Profile;
class TemplateURL;

namespace webkit_glue {
struct PasswordForm;
}

#if defined(OS_WIN)
struct IE7PasswordInfo;
#endif

class ProfileWriter : public base::RefCountedThreadSafe<ProfileWriter> {
 public:
  
  enum BookmarkOptions {
    
    
    
    
    ADD_IF_UNIQUE = 1 << 0,

    
    IMPORT_TO_BOOKMARK_BAR = 1 << 1,

    
    BOOKMARK_BAR_DISABLED = 1 << 2
  };

  struct BookmarkEntry {
    BookmarkEntry();
    ~BookmarkEntry();

    bool in_toolbar;
    bool is_folder;
    GURL url;
    std::vector<string16> path;
    string16 title;
    base::Time creation_time;
  };

  explicit ProfileWriter(Profile* profile);

  
  
  
  virtual bool BookmarkModelIsLoaded() const;
  virtual bool TemplateURLModelIsLoaded() const;

  
  virtual void AddPasswordForm(const webkit_glue::PasswordForm& form);

#if defined(OS_WIN)
  virtual void AddIE7PasswordInfo(const IE7PasswordInfo& info);
#endif

  virtual void AddHistoryPage(const std::vector<history::URLRow>& page,
                              history::VisitSource visit_source);

  virtual void AddHomepage(const GURL& homepage);

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void AddBookmarkEntry(const std::vector<BookmarkEntry>& bookmark,
                                const string16& first_folder_name,
                                int options);

  virtual void AddFavicons(
      const std::vector<history::ImportedFaviconUsage>& favicons);

  
  
  
  
  
  
  
  
  
  

  virtual void AddKeywords(const std::vector<TemplateURL*>& template_urls,
                           int default_keyword_index,
                           bool unique_on_host_and_path);

  
  void ShowBookmarkBar();

 protected:
  friend class base::RefCountedThreadSafe<ProfileWriter>;

  virtual ~ProfileWriter();

 private:
  
  
  string16 GenerateUniqueFolderName(BookmarkModel* model,
                                    const string16& folder_name);

  
  
  
  bool DoesBookmarkExist(BookmarkModel* model,
                         const BookmarkEntry& entry,
                         const string16& first_folder_name,
                         bool import_to_bookmark_bar);

  Profile* const profile_;

  DISALLOW_COPY_AND_ASSIGN(ProfileWriter);
};

#endif  
