// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_TAB_UTIL_H_
#define CHROME_BROWSER_TAB_CONTENTS_TAB_UTIL_H_
#pragma once

class TabContents;

namespace tab_util {

TabContents* GetTabContentsByID(int render_process_host_id, int routing_id);

}  

#endif  
