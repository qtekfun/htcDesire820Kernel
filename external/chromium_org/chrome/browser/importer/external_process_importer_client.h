// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
#define CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/common/importer/importer_data_types.h"
#include "chrome/common/importer/importer_url_row.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/utility_process_host_client.h"

class ExternalProcessImporterHost;
struct ImportedBookmarkEntry;
struct ImportedFaviconUsage;
class InProcessImporterBridge;

namespace autofill {
struct PasswordForm;
}

namespace content{
class UtilityProcessHost;
}

namespace importer {
#if defined(OS_WIN)
struct ImporterIE7PasswordInfo;
#endif
struct URLKeywordInfo;
}

class ExternalProcessImporterClient : public content::UtilityProcessHostClient {
 public:
  ExternalProcessImporterClient(
      base::WeakPtr<ExternalProcessImporterHost> importer_host,
      const importer::SourceProfile& source_profile,
      uint16 items,
      InProcessImporterBridge* bridge);

  
  void Start();

  
  void Cancel();

  
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void OnImportStart();
  void OnImportFinished(bool succeeded, const std::string& error_msg);
  void OnImportItemStart(int item);
  void OnImportItemFinished(int item);
  void OnHistoryImportStart(size_t total_history_rows_count);
  void OnHistoryImportGroup(
      const std::vector<ImporterURLRow>& history_rows_group,
      int visit_source);
  void OnHomePageImportReady(const GURL& home_page);
  void OnBookmarksImportStart(const base::string16& first_folder_name,
                              size_t total_bookmarks_count);
  void OnBookmarksImportGroup(
      const std::vector<ImportedBookmarkEntry>& bookmarks_group);
  void OnFaviconsImportStart(size_t total_favicons_count);
  void OnFaviconsImportGroup(
      const std::vector<ImportedFaviconUsage>& favicons_group);
  void OnPasswordFormImportReady(const autofill::PasswordForm& form);
  void OnKeywordsImportReady(
      const std::vector<importer::URLKeywordInfo>& url_keywords,
      bool unique_on_host_and_path);
  void OnFirefoxSearchEngineDataReceived(
      const std::vector<std::string> search_engine_data);
#if defined(OS_WIN)
  void OnIE7PasswordReceived(
        const importer::ImporterIE7PasswordInfo& importer_password_info);
#endif

 protected:
  virtual ~ExternalProcessImporterClient();

 private:
  
  void Cleanup();

  
  void CancelImportProcessOnIOThread();

  
  void NotifyItemFinishedOnIOThread(importer::ImportItem import_item);

  
  void StartProcessOnIOThread(content::BrowserThread::ID thread_id);

  
  // entire group has been collected and is ready to be written to the profile.
  std::vector<ImporterURLRow> history_rows_;
  std::vector<ImportedBookmarkEntry> bookmarks_;
  std::vector<ImportedFaviconUsage> favicons_;

  
  
  base::string16 bookmarks_first_folder_name_;

  
  size_t total_bookmarks_count_;

  
  size_t total_history_rows_count_;

  
  size_t total_favicons_count_;

  
  
  
  
  
  
  base::WeakPtr<ExternalProcessImporterHost> process_importer_host_;

  
  
  
  base::WeakPtr<content::UtilityProcessHost> utility_process_host_;

  
  importer::SourceProfile source_profile_;
  uint16 items_;

  // Takes import data coming over IPC and delivers it to be written by the
  
  scoped_refptr<InProcessImporterBridge> bridge_;

  
  bool cancelled_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProcessImporterClient);
};

#endif  
