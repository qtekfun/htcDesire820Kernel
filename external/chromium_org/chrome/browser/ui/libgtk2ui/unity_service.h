// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LIBGTK2UI_UNITY_SERVICE_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_UNITY_SERVICE_H_

namespace unity {

bool IsRunning();

void SetDownloadCount(int count);

void SetProgressFraction(float percentage);

}  

#endif  
