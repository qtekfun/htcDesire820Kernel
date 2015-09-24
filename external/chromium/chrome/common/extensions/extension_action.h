// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_ACTION_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_ACTION_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"

namespace gfx {
class Canvas;
class Rect;
}

class GURL;
class SkBitmap;

class ExtensionAction {
 public:
  
  
  static const int kDefaultTabId;

  ExtensionAction();
  ~ExtensionAction();

  
  std::string extension_id() const { return extension_id_; }
  void set_extension_id(const std::string& extension_id) {
    extension_id_ = extension_id;
  }

  
  std::string id() const { return id_; }
  void set_id(const std::string& id) { id_ = id; }

  
  std::vector<std::string>* icon_paths() { return &icon_paths_; }

  
  
  void SetPopupUrl(int tab_id, const GURL& url);

  
  bool HasPopup(int tab_id);

  
  GURL GetPopupUrl(int tab_id);

  
  void SetTitle(int tab_id, const std::string& title) {
    SetValue(&title_, tab_id, title);
  }

  
  
  std::string GetTitle(int tab_id) { return GetValue(&title_, tab_id); }

  
  
  
  
  
  

  
  void SetIcon(int tab_id, const SkBitmap& bitmap);

  
  SkBitmap GetIcon(int tab_id);

  
  
  void SetIconIndex(int tab_id, int index);

  
  
  int GetIconIndex(int tab_id) {
    return GetValue(&icon_index_, tab_id);
  }

  
  
  void set_default_icon_path(const std::string& path) {
    default_icon_path_ = path;
  }
  std::string default_icon_path() {
    return default_icon_path_;
  }

  
  void SetBadgeText(int tab_id, const std::string& text) {
    SetValue(&badge_text_, tab_id, text);
  }
  
  std::string GetBadgeText(int tab_id) {
    return GetValue(&badge_text_, tab_id);
  }

  
  void SetBadgeTextColor(int tab_id, const SkColor& text_color) {
    SetValue(&badge_text_color_, tab_id, text_color);
  }
  
  
  SkColor GetBadgeTextColor(int tab_id) {
    return GetValue(&badge_text_color_, tab_id);
  }

  
  void SetBadgeBackgroundColor(int tab_id, const SkColor& color) {
    SetValue(&badge_background_color_, tab_id, color);
  }
  
  
  SkColor GetBadgeBackgroundColor(int tab_id) {
    return GetValue(&badge_background_color_, tab_id);
  }

  
  void SetIsVisible(int tab_id, bool value) {
    SetValue(&visible_, tab_id, value);
  }
  
  
  bool GetIsVisible(int tab_id) {
    return GetValue(&visible_, tab_id);
  }

  
  void ClearAllValuesForTab(int tab_id);

  
  void PaintBadge(gfx::Canvas* canvas, const gfx::Rect& bounds, int tab_id);

 private:
  template <class T>
  struct ValueTraits {
    static T CreateEmpty() {
      return T();
    }
  };

  template<class T>
  void SetValue(std::map<int, T>* map, int tab_id, const T& val) {
    (*map)[tab_id] = val;
  }

  template<class T>
  T GetValue(std::map<int, T>* map, int tab_id) {
    typename std::map<int, T>::iterator iter = map->find(tab_id);
    if (iter != map->end()) {
      return iter->second;
    } else {
      iter = map->find(kDefaultTabId);
      return iter != map->end() ? iter->second : ValueTraits<T>::CreateEmpty();
    }
  }

  
  
  std::string extension_id_;

  
  
  std::map<int, GURL> popup_url_;
  std::map<int, std::string> title_;
  std::map<int, SkBitmap> icon_;
  std::map<int, int> icon_index_;  
  std::map<int, std::string> badge_text_;
  std::map<int, SkColor> badge_background_color_;
  std::map<int, SkColor> badge_text_color_;
  std::map<int, bool> visible_;

  std::string default_icon_path_;

  
  
  std::string id_;

  
  
  std::vector<std::string> icon_paths_;
};

template<>
struct ExtensionAction::ValueTraits<int> {
  static int CreateEmpty() {
    return -1;
  }
};

#endif  
