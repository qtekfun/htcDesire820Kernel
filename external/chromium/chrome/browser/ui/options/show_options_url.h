// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OPTIONS_SHOW_OPTIONS_URL_H_
#define CHROME_BROWSER_UI_OPTIONS_SHOW_OPTIONS_URL_H_
#pragma once

class GURL;
class Profile;

namespace browser {

void ShowOptionsURL(Profile* profile, const GURL& url);

}  

#endif  
