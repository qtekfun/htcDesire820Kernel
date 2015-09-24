// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GEOLOCATION_GEOLOCATION_EXCEPTIONS_TABLE_MODEL_H_
#define CHROME_BROWSER_GEOLOCATION_GEOLOCATION_EXCEPTIONS_TABLE_MODEL_H_
#pragma once

#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/geolocation/geolocation_content_settings_map.h"
#include "chrome/browser/remove_rows_table_model.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"

class GeolocationExceptionsTableModel : public RemoveRowsTableModel {
 public:
  explicit GeolocationExceptionsTableModel(
      GeolocationContentSettingsMap* map);
  virtual ~GeolocationExceptionsTableModel();

  

  
  
  
  virtual bool CanRemoveRows(const Rows& rows) const;

  
  
  
  
  
  virtual void RemoveRows(const Rows& rows);

  
  virtual void RemoveAll();

  
  virtual int RowCount() OVERRIDE;
  virtual string16 GetText(int row, int column_id) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;
  virtual int CompareValues(int row1, int row2, int column_id) OVERRIDE;

 private:
  void AddEntriesForOrigin(
      const GURL& origin,
      const GeolocationContentSettingsMap::OneOriginSettings& settings);

  GeolocationContentSettingsMap* map_;

  struct Entry;
  typedef std::vector<Entry> EntriesVector;
  EntriesVector entries_;

  ui::TableModelObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(GeolocationExceptionsTableModel);
};

#endif  
