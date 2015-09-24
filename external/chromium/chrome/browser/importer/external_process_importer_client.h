// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
#define CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/string16.h"
#include "chrome/browser/importer/importer_data_types.h"
#include "chrome/browser/importer/profile_writer.h"
#include "chrome/browser/importer/profile_import_process_client.h"
#include "content/browser/browser_thread.h"

class ExternalProcessImporterHost;
class InProcessImporterBridge;
class ProfileImportProcessHost;

namespace history {
class URLRow;
struct ImportedFaviconUsage;
}

class ExternalProcessImporterClient : public ProfileImportProcessClient {
 public:
  ExternalProcessImporterClient(ExternalProcessImporterHost* importer_host,
                                const importer::SourceProfile& source_profile,
                                uint16 items,
                                InProcessImporterBridge* bridge,
                                bool import_to_bookmark_bar);
  virtual ~ExternalProcessImporterClient();

  
  void Start();

  
  void Cancel();

  
  void Cleanup();

 private:
  
  void StartImportProcessOnIOThread(BrowserThread::ID thread_id);

  
  void CancelImportProcessOnIOThread();

  
  void NotifyItemFinishedOnIOThread(importer::ImportItem import_item);

  
  virtual void OnProcessCrashed(int exit_status) OVERRIDE;
  virtual void OnImportStart() OVERRIDE;
  virtual void OnImportFinished(bool succeeded,
                                const std::string& error_msg) OVERRIDE;
  virtual void OnImportItemStart(int item) OVERRIDE;
  virtual void OnImportItemFinished(int item) OVERRIDE;

  
  
  virtual void OnHistoryImportStart(size_t total_history_rows_count) OVERRIDE;

  
  virtual void OnHistoryImportGroup(
      const std::vector<history::URLRow>& history_rows_group,
      int visit_source) OVERRIDE;

  
  virtual void OnHomePageImportReady(const GURL& home_page) OVERRIDE;

  
  
  
  
  virtual void OnBookmarksImportStart(const string16& first_folder_name,
                                      int options,
                                      size_t total_bookmarks_count) OVERRIDE;

  
  virtual void OnBookmarksImportGroup(
      const std::vector<ProfileWriter::BookmarkEntry>& bookmarks_group)
          OVERRIDE;

  
  
  virtual void OnFaviconsImportStart(size_t total_favicons_count) OVERRIDE;

  
  virtual void OnFaviconsImportGroup(
      const std::vector<history::ImportedFaviconUsage>& favicons_group)
          OVERRIDE;

  
  virtual void OnPasswordFormImportReady(
      const webkit_glue::PasswordForm& form) OVERRIDE;

  
  virtual void OnKeywordsImportReady(
      const std::vector<TemplateURL>& template_urls,
      int default_keyword_index,
      bool unique_on_host_and_path) OVERRIDE;

  

  
  // entire group has been collected and is ready to be written to the profile.
  std::vector<history::URLRow> history_rows_;
  std::vector<ProfileWriter::BookmarkEntry> bookmarks_;
  std::vector<history::ImportedFaviconUsage> favicons_;

  
  
  string16 bookmarks_first_folder_name_;

  
  int bookmarks_options_;

  
  size_t total_bookmarks_count_;

  
  size_t total_history_rows_count_;

  
  size_t total_favicons_count_;

  
  
  
  
  ExternalProcessImporterHost* process_importer_host_;

  
  
  ProfileImportProcessHost* profile_import_process_host_;

  
  const importer::SourceProfile& source_profile_;
  uint16 items_;
  bool import_to_bookmark_bar_;

  // Takes import data coming over IPC and delivers it to be written by the
  
  
  InProcessImporterBridge* bridge_;

  
  bool cancelled_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProcessImporterClient);
};

#endif  
