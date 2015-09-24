// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_PREFS_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_PREFS_H_
#pragma once

#include <set>

#include "base/file_path.h"
#include "chrome/browser/prefs/pref_member.h"

class PrefService;

class DownloadPrefs {
 public:
  explicit DownloadPrefs(PrefService* prefs);
  ~DownloadPrefs();

  static void RegisterUserPrefs(PrefService* prefs);

  FilePath download_path() const { return *download_path_; }
  int save_file_type() const { return *save_file_type_; }

  
  
  
  bool PromptForDownload() const;

  
  bool IsDownloadPathManaged() const;

  
  
  bool IsAutoOpenUsed() const;

  bool IsAutoOpenEnabledForExtension(
      const FilePath::StringType& extension) const;

  
  
  bool EnableAutoOpenBasedOnExtension(const FilePath& file_name);

  
  void DisableAutoOpenBasedOnExtension(const FilePath& file_name);

  void ResetToDefaults();
  void ResetAutoOpen();

 private:
  void SaveAutoOpenState();

  PrefService* prefs_;

  BooleanPrefMember prompt_for_download_;
  FilePathPrefMember download_path_;
  IntegerPrefMember save_file_type_;

  
  struct AutoOpenCompareFunctor {
    bool operator()(const FilePath::StringType& a,
                    const FilePath::StringType& b) const;
  };
  typedef std::set<FilePath::StringType, AutoOpenCompareFunctor> AutoOpenSet;
  AutoOpenSet auto_open_;

  DISALLOW_COPY_AND_ASSIGN(DownloadPrefs);
};

#endif  
