// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_IMPORTER_BRIDGE_H_
#define CHROME_COMMON_IMPORTER_IMPORTER_BRIDGE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "build/build_config.h"
#include "chrome/common/importer/importer_data_types.h"
#include "chrome/common/importer/importer_url_row.h"

class GURL;
struct ImportedBookmarkEntry;
struct ImportedFaviconUsage;

namespace autofill {
struct PasswordForm;
}

namespace importer {
#if defined(OS_WIN)
struct ImporterIE7PasswordInfo;
#endif
struct ImporterURlRow;
struct URLKeywordInfo;
}

class ImporterBridge : public base::RefCountedThreadSafe<ImporterBridge> {
 public:
  ImporterBridge();

  virtual void AddBookmarks(
      const std::vector<ImportedBookmarkEntry>& bookmarks,
      const base::string16& first_folder_name) = 0;

  virtual void AddHomePage(const GURL& home_page) = 0;

#if defined(OS_WIN)
  virtual void AddIE7PasswordInfo(
      const importer::ImporterIE7PasswordInfo& password_info) = 0;
#endif

  virtual void SetFavicons(
      const std::vector<ImportedFaviconUsage>& favicons) = 0;

  virtual void SetHistoryItems(const std::vector<ImporterURLRow>& rows,
                               importer::VisitSource visit_source) = 0;

  virtual void SetKeywords(
      const std::vector<importer::URLKeywordInfo>& url_keywords,
      bool unique_on_host_and_path) = 0;

  
  
  virtual void SetFirefoxSearchEnginesXMLData(
      const std::vector<std::string>& search_engine_data) = 0;

  virtual void SetPasswordForm(const autofill::PasswordForm& form) = 0;

  
  virtual void NotifyStarted() = 0;

  
  
  virtual void NotifyItemStarted(importer::ImportItem item) = 0;

  
  
  virtual void NotifyItemEnded(importer::ImportItem item) = 0;

  
  virtual void NotifyEnded() = 0;

  
  
  
  
  virtual base::string16 GetLocalizedString(int message_id) = 0;

 protected:
  friend class base::RefCountedThreadSafe<ImporterBridge>;

  virtual ~ImporterBridge();

  DISALLOW_COPY_AND_ASSIGN(ImporterBridge);
};

#endif  
