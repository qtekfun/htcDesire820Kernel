/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Mork Reader.
 *
 * The Initial Developer of the Original Code is
 * Google Inc.
 * Portions created by the Initial Developer are Copyright (C) 2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Brian Ryner <bryner@brianryner.com> (original author)
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


#ifndef CHROME_BROWSER_IMPORTER_MORK_READER_H_
#define CHROME_BROWSER_IMPORTER_MORK_READER_H_
#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"

class FilePath;
class ImporterBridge;

class MorkReader {
 public:
  
  
  
  typedef std::string IDString;

  
  typedef std::vector<std::string> ColumnDataList;

  
  struct MorkColumn {
    MorkColumn(const IDString& i, const std::string& n) : id(i), name(n) { }

    IDString id;
    std::string name;
  };
  typedef std::vector<MorkColumn> MorkColumnList;

  
  
  typedef std::map<IDString, ColumnDataList*> RowMap;

  typedef RowMap::const_iterator iterator;

  MorkReader();
  ~MorkReader();

  
  
  bool Read(const FilePath& filename);

  
  const MorkColumnList& columns() const { return columns_; }

  
  
  
  
  const ColumnDataList& meta_row() const { return meta_row_; }

  
  
  void NormalizeValue(std::string* value) const;

  
  
  
  iterator begin() const { return table_.begin(); }
  iterator end() const { return table_.end(); }

 private:
  
  typedef std::map<IDString, std::string> StringMap;

  
  
  typedef std::map<IDString, int> IndexMap;

  
  
  
  
  bool ParseMap(const std::string& first_line,
                size_t start_index,
                StringMap* map);

  
  
  
  
  
  
  void ParseTable(const std::string& first_line,
                  size_t start_index,
                  const IndexMap* column_map);

  
  
  bool ReadLine(std::string* line);

  std::ifstream stream_;

  
  MorkColumnList columns_;

  
  StringMap value_map_;

  
  ColumnDataList meta_row_;

  
  
  RowMap table_;
};

void ImportHistoryFromFirefox2(const FilePath& file, ImporterBridge* bridge);

#endif  
