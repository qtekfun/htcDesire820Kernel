// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PROFILE_ERROR_DIALOG_H_
#define CHROME_BROWSER_UI_PROFILE_ERROR_DIALOG_H_

enum ProfileErrorType {
  PROFILE_ERROR_HISTORY,
  PROFILE_ERROR_PREFERENCES,
  PROFILE_ERROR_DB_AUTOFILL_WEB_DATA,
  PROFILE_ERROR_DB_TOKEN_WEB_DATA,
  PROFILE_ERROR_DB_WEB_DATA,
  PROFILE_ERROR_END
};

void ShowProfileErrorDialog(ProfileErrorType type, int message_id);

#endif  
