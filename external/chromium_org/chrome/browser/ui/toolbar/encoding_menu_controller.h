// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_ENCODING_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_TOOLBAR_ENCODING_MENU_CONTROLLER_H_

#include <utility>
#include <string>
#include <vector>

#include "base/basictypes.h"  
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"

class Profile;

class EncodingMenuController {
  FRIEND_TEST_ALL_PREFIXES(EncodingMenuControllerTest, EncodingIDsBelongTest);
  FRIEND_TEST_ALL_PREFIXES(EncodingMenuControllerTest, IsItemChecked);

 public:
  typedef std::pair<int, base::string16> EncodingMenuItem;
  typedef std::vector<EncodingMenuItem> EncodingMenuItemList;

 public:
  EncodingMenuController() {}

  
  bool DoesCommandBelongToEncodingMenu(int id);

  
  
  
  bool IsItemChecked(Profile* browser_profile,
                     const std::string& current_tab_encoding,
                     int item_id);

  
  
  void GetEncodingMenuItems(Profile* profile,
                            EncodingMenuItemList* menu_items);

 private:
  
  static const int kValidEncodingIds[];
  const int* ValidGUIEncodingIDs();
  int NumValidGUIEncodingIDs();

  DISALLOW_COPY_AND_ASSIGN(EncodingMenuController);
};

#endif  
