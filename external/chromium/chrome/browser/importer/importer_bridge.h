// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_BRIDGE_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_BRIDGE_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "build/build_config.h"
#include "chrome/browser/importer/importer_data_types.h"
#include "chrome/browser/importer/profile_writer.h"

class Toolbar5Importer;

class ImporterBridge : public base::RefCountedThreadSafe<ImporterBridge> {
 public:
  ImporterBridge();

  virtual void AddBookmarkEntries(
      const std::vector<ProfileWriter::BookmarkEntry>& bookmarks,
      const string16& first_folder_name,
      int options) = 0;

  virtual void AddHomePage(const GURL& home_page) = 0;

#if defined(OS_WIN)
  virtual void AddIE7PasswordInfo(const IE7PasswordInfo& password_info) = 0;
#endif

  virtual void SetFavicons(
      const std::vector<history::ImportedFaviconUsage>& favicons) = 0;

  virtual void SetHistoryItems(const std::vector<history::URLRow>& rows,
                               history::VisitSource visit_source) = 0;

  virtual void SetKeywords(const std::vector<TemplateURL*>& template_urls,
                           int default_keyword_index,
                           bool unique_on_host_and_path) = 0;

  virtual void SetPasswordForm(const webkit_glue::PasswordForm& form) = 0;

  
  virtual void NotifyStarted() = 0;

  
  
  virtual void NotifyItemStarted(importer::ImportItem item) = 0;

  
  
  virtual void NotifyItemEnded(importer::ImportItem item) = 0;

  
  virtual void NotifyEnded() = 0;

  
  
  
  
  virtual string16 GetLocalizedString(int message_id) = 0;

 protected:
  friend class base::RefCountedThreadSafe<ImporterBridge>;
  
  
  
  friend class Toolbar5Importer;

  virtual ~ImporterBridge();

  DISALLOW_COPY_AND_ASSIGN(ImporterBridge);
};

#endif  
