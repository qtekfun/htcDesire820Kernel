// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_FONT_SETTINGS_FONTS_LIST_LOADER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_FONT_SETTINGS_FONTS_LIST_LOADER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"

class FontSettingsFontsListLoader
    : public base::RefCountedThreadSafe<FontSettingsFontsListLoader> {
 public:
  
  
  class Observer {
   public:
    virtual void FontsListHasLoaded() = 0;

   protected:
    virtual ~Observer() {}
  };

  
  explicit FontSettingsFontsListLoader(Observer* observer);

  
  
  ListValue* GetFontsList();

  
  
  void StartLoadFontsList();

  
  
  void SetObserver(Observer* observer);

 private:
  friend class base::RefCountedThreadSafe<FontSettingsFontsListLoader>;

  ~FontSettingsFontsListLoader();

  void GetFontsListOnFileThread();
  void FinishFontsListOnUIThread();

  scoped_ptr<ListValue> fonts_list_;
  Observer* observer_;  

  DISALLOW_COPY_AND_ASSIGN(FontSettingsFontsListLoader);
};

#endif  

