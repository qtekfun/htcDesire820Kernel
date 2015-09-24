// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_BROWSER_ACTION_DRAG_DATA_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_BROWSER_ACTION_DRAG_DATA_H_

#include <string>

#include "base/basictypes.h"

#if defined(TOOLKIT_VIEWS)
#include "ui/base/dragdrop/os_exchange_data.h"
#endif

class Pickle;
class Profile;

class BrowserActionDragData {
 public:
  BrowserActionDragData();
  BrowserActionDragData(const std::string& id, int index);

  const std::string& id() const { return id_; }

  size_t index() const { return index_; }

  
  bool IsFromProfile(Profile* profile) const;

#if defined(TOOLKIT_VIEWS)
  void Write(Profile* profile, ui::OSExchangeData* data) const;

  
  bool Read(const ui::OSExchangeData& data);

  
  static const ui::OSExchangeData::CustomFormat& GetBrowserActionCustomFormat();
#endif

 private:
  void WriteToPickle(Profile* profile, Pickle* pickle) const;
  bool ReadFromPickle(Pickle* pickle);

  
  void* profile_;

  
  std::string id_;

  
  size_t index_;

  
  static const char* kClipboardFormatString;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionDragData);
};

#endif  
