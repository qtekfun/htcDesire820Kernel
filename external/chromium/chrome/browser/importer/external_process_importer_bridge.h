// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
#define CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/importer/importer_bridge.h"
#include "chrome/browser/importer/profile_writer.h"

class DictionaryValue;
class GURL;
class ProfileImportThread;

class ExternalProcessImporterBridge : public ImporterBridge {
 public:
  ExternalProcessImporterBridge(ProfileImportThread* profile_import_thread,
                                const DictionaryValue& localized_strings);

  
  virtual void AddBookmarkEntries(
      const std::vector<ProfileWriter::BookmarkEntry>& bookmarks,
      const string16& first_folder_name,
      int options) OVERRIDE;

  virtual void AddHomePage(const GURL& home_page) OVERRIDE;

#if defined(OS_WIN)
  virtual void AddIE7PasswordInfo(
      const IE7PasswordInfo& password_info) OVERRIDE;
#endif

  virtual void SetFavicons(
      const std::vector<history::ImportedFaviconUsage>& favicons) OVERRIDE;

  virtual void SetHistoryItems(const std::vector<history::URLRow>& rows,
                               history::VisitSource visit_source) OVERRIDE;

  virtual void SetKeywords(const std::vector<TemplateURL*>& template_urls,
                           int default_keyword_index,
                           bool unique_on_host_and_path) OVERRIDE;

  virtual void SetPasswordForm(const webkit_glue::PasswordForm& form) OVERRIDE;

  virtual void NotifyStarted() OVERRIDE;
  virtual void NotifyItemStarted(importer::ImportItem item) OVERRIDE;
  virtual void NotifyItemEnded(importer::ImportItem item) OVERRIDE;
  virtual void NotifyEnded() OVERRIDE;

  virtual string16 GetLocalizedString(int message_id) OVERRIDE;
  

 private:
  virtual ~ExternalProcessImporterBridge();

  
  ProfileImportThread* const profile_import_thread_;

  
  
  scoped_ptr<DictionaryValue> localized_strings_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProcessImporterBridge);
};

#endif  
