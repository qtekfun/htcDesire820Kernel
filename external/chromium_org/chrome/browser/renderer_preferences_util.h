// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_PREFERENCES_UTIL_H_
#define CHROME_BROWSER_RENDERER_PREFERENCES_UTIL_H_

class Profile;

namespace content {
struct RendererPreferences;
}

namespace renderer_preferences_util {

void UpdateFromSystemSettings(content::RendererPreferences* prefs,
                              Profile* profile);

}  

#endif  
