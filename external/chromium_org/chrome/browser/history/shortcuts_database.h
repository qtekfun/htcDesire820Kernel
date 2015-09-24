// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_SHORTCUTS_DATABASE_H_
#define CHROME_BROWSER_HISTORY_SHORTCUTS_DATABASE_H_

#include <map>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "chrome/browser/history/shortcuts_backend.h"
#include "sql/connection.h"
#include "url/gurl.h"

namespace history {

class ShortcutsDatabase : public base::RefCountedThreadSafe<ShortcutsDatabase> {
 public:
  typedef std::map<std::string, ShortcutsBackend::Shortcut> GuidToShortcutMap;

  explicit ShortcutsDatabase(const base::FilePath& database_path);

  bool Init();

  
  bool AddShortcut(const ShortcutsBackend::Shortcut& shortcut);

  
  bool UpdateShortcut(const ShortcutsBackend::Shortcut& shortcut);

  
  bool DeleteShortcutsWithIds(const std::vector<std::string>& shortcut_ids);

  
  bool DeleteShortcutsWithUrl(const std::string& shortcut_url_spec);

  
  bool DeleteAllShortcuts();

  
  void LoadShortcuts(GuidToShortcutMap* shortcuts);

 private:
  friend class base::RefCountedThreadSafe<ShortcutsDatabase>;
  friend class ShortcutsDatabaseTest;
  FRIEND_TEST_ALL_PREFIXES(ShortcutsDatabaseTest, AddShortcut);
  FRIEND_TEST_ALL_PREFIXES(ShortcutsDatabaseTest, UpdateShortcut);
  FRIEND_TEST_ALL_PREFIXES(ShortcutsDatabaseTest, DeleteShortcutsWithIds);
  FRIEND_TEST_ALL_PREFIXES(ShortcutsDatabaseTest, DeleteShortcutsWithUrl);
  FRIEND_TEST_ALL_PREFIXES(ShortcutsDatabaseTest, LoadShortcuts);

  virtual ~ShortcutsDatabase();

  
  bool EnsureTable();

  
  sql::Connection db_;
  base::FilePath database_path_;

  static const base::FilePath::CharType kShortcutsDatabaseName[];

  DISALLOW_COPY_AND_ASSIGN(ShortcutsDatabase);
};

}  

#endif  
