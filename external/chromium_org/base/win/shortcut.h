// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_SHORTCUT_H_
#define BASE_WIN_SHORTCUT_H_

#include <windows.h>

#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/strings/string16.h"

namespace base {
namespace win {

enum ShortcutOperation {
  
  SHORTCUT_CREATE_ALWAYS = 0,
  
  
  
  SHORTCUT_REPLACE_EXISTING,
  
  SHORTCUT_UPDATE_EXISTING,
};

struct ShortcutProperties {
  enum IndividualProperties {
    PROPERTIES_TARGET = 1 << 0,
    PROPERTIES_WORKING_DIR = 1 << 1,
    PROPERTIES_ARGUMENTS = 1 << 2,
    PROPERTIES_DESCRIPTION = 1 << 3,
    PROPERTIES_ICON = 1 << 4,
    PROPERTIES_APP_ID = 1 << 5,
    PROPERTIES_DUAL_MODE = 1 << 6,
  };

  ShortcutProperties() : icon_index(-1), dual_mode(false), options(0U) {}

  void set_target(const FilePath& target_in) {
    target = target_in;
    options |= PROPERTIES_TARGET;
  }

  void set_working_dir(const FilePath& working_dir_in) {
    working_dir = working_dir_in;
    options |= PROPERTIES_WORKING_DIR;
  }

  void set_arguments(const string16& arguments_in) {
    
    DCHECK(arguments_in.size() < MAX_PATH);
    arguments = arguments_in;
    options |= PROPERTIES_ARGUMENTS;
  }

  void set_description(const string16& description_in) {
    
    DCHECK(description_in.size() < MAX_PATH);
    description = description_in;
    options |= PROPERTIES_DESCRIPTION;
  }

  void set_icon(const FilePath& icon_in, int icon_index_in) {
    icon = icon_in;
    icon_index = icon_index_in;
    options |= PROPERTIES_ICON;
  }

  void set_app_id(const string16& app_id_in) {
    app_id = app_id_in;
    options |= PROPERTIES_APP_ID;
  }

  void set_dual_mode(bool dual_mode_in) {
    dual_mode = dual_mode_in;
    options |= PROPERTIES_DUAL_MODE;
  }

  
  
  FilePath target;
  
  FilePath working_dir;
  
  
  string16 arguments;
  
  
  string16 description;
  
  
  FilePath icon;
  int icon_index;
  
  string16 app_id;
  
  bool dual_mode;
  
  
  uint32 options;
};

BASE_EXPORT bool CreateOrUpdateShortcutLink(
    const FilePath& shortcut_path,
    const ShortcutProperties& properties,
    ShortcutOperation operation);

BASE_EXPORT bool ResolveShortcut(const FilePath& shortcut_path,
                                 FilePath* target_path,
                                 string16* args);

BASE_EXPORT bool TaskbarPinShortcutLink(const wchar_t* shortcut);

BASE_EXPORT bool TaskbarUnpinShortcutLink(const wchar_t* shortcut);

}  
}  

#endif  
