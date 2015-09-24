// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_PREFERENCES_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_PREFERENCES_HELPER_H_

#include "base/files/file_path.h"
#include "base/values.h"

#include <string>

class PrefService;

namespace preferences_helper {

PrefService* GetPrefs(int index);

PrefService* GetVerifierPrefs();

void ChangeBooleanPref(int index, const char* pref_name);

void ChangeIntegerPref(int index, const char* pref_name, int new_value);

void ChangeInt64Pref(int index, const char* pref_name, int64 new_value);

void ChangeDoublePref(int index, const char* pref_name, double new_value);

void ChangeStringPref(int index,
                      const char* pref_name,
                      const std::string& new_value);

void AppendStringPref(int index,
                      const char* pref_name,
                      const std::string& append_value);

void ChangeFilePathPref(int index,
                        const char* pref_name,
                        const base::FilePath& new_value);

void ChangeListPref(int index,
                    const char* pref_name,
                    const ListValue& new_value);

bool BooleanPrefMatches(const char* pref_name) WARN_UNUSED_RESULT;

bool IntegerPrefMatches(const char* pref_name) WARN_UNUSED_RESULT;

bool Int64PrefMatches(const char* pref_name) WARN_UNUSED_RESULT;

bool DoublePrefMatches(const char* pref_name) WARN_UNUSED_RESULT;

bool StringPrefMatches(const char* pref_name) WARN_UNUSED_RESULT;

bool FilePathPrefMatches(const char* pref_name) WARN_UNUSED_RESULT;

bool ListPrefMatches(const char* pref_name) WARN_UNUSED_RESULT;

}  

#endif  
