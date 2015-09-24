// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_SYNCABLE_COLUMNS_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_SYNCABLE_COLUMNS_H_
#pragma once

#include "chrome/browser/sync/syncable/syncable.h"
#include "chrome/browser/sync/syncable/syncable_changes_version.h"

namespace syncable {

struct ColumnSpec {
  const char* name;
  const char* spec;
};

static const ColumnSpec g_metas_columns[] = {
  
  
  {"metahandle", "bigint primary key ON CONFLICT FAIL"},
  {"base_version", "bigint default " CHANGES_VERSION_STRING},
  {"server_version", "bigint default 0"},
  
  
  {"mtime", "bigint default 0"},
  {"server_mtime", "bigint default 0"},
  {"ctime", "bigint default 0"},
  {"server_ctime", "bigint default 0"},
  {"server_position_in_parent", "bigint default 0"},
  
  {"local_external_id", "bigint default 0"},
  
  
  {"id", "varchar(255) default \"r\""},
  {"parent_id", "varchar(255) default \"r\""},
  {"server_parent_id", "varchar(255) default \"r\""},
  {"prev_id", "varchar(255) default \"r\""},
  {"next_id", "varchar(255) default \"r\""},
  
  
  {"is_unsynced", "bit default 0"},
  {"is_unapplied_update", "bit default 0"},
  {"is_del", "bit default 0"},
  {"is_dir", "bit default 0"},
  {"server_is_dir", "bit default 0"},
  {"server_is_del", "bit default 0"},
  
  
  {"non_unique_name", "varchar"},
  {"server_non_unique_name", "varchar(255)"},
  {"unique_server_tag", "varchar"},
  {"unique_client_tag", "varchar"},
  
  
  {"specifics", "blob"},
  {"server_specifics", "blob"},
};

COMPILE_ASSERT(arraysize(g_metas_columns) >= FIELD_COUNT, missing_column_name);
COMPILE_ASSERT(arraysize(g_metas_columns) <= FIELD_COUNT, extra_column_names);

static inline const char* ColumnName(int field) {
  DCHECK(field < BEGIN_TEMPS);
  return g_metas_columns[field].name;
}

}  

#endif  
