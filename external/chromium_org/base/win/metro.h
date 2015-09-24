// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_METRO_H_
#define BASE_WIN_METRO_H_

#include <windows.h>
#include <wpcapi.h>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"

namespace base {
namespace win {

enum MetroLaunchType {
  METRO_LAUNCH,
  METRO_SEARCH,
  METRO_SHARE,
  METRO_FILE,
  METRO_PROTOCOL,
  METRO_LAUNCH_ERROR,
  METRO_LASTLAUNCHTYPE,
};

enum MetroPreviousExecutionState {
  NOTRUNNING,
  RUNNING,
  SUSPENDED,
  TERMINATED,
  CLOSEDBYUSER,
  LASTEXECUTIONSTATE,
};

enum MetroSecondaryTilePinUmaResult {
  METRO_PIN_STATE_NONE,
  METRO_PIN_INITIATED,
  METRO_PIN_LOGO_READY,
  METRO_PIN_REQUEST_SHOW_ERROR,
  METRO_PIN_RESULT_CANCEL,
  METRO_PIN_RESULT_OK,
  METRO_PIN_RESULT_OTHER,
  METRO_PIN_RESULT_ERROR,
  METRO_UNPIN_INITIATED,
  METRO_UNPIN_REQUEST_SHOW_ERROR,
  METRO_UNPIN_RESULT_CANCEL,
  METRO_UNPIN_RESULT_OK,
  METRO_UNPIN_RESULT_OTHER,
  METRO_UNPIN_RESULT_ERROR,
  METRO_PIN_STATE_LIMIT
};

struct CurrentTabInfo {
  wchar_t* title;
  wchar_t* url;
};

BASE_EXPORT HMODULE GetMetroModule();

BASE_EXPORT bool IsMetroProcess();

BASE_EXPORT bool IsProcessImmersive(HANDLE process);

BASE_EXPORT bool IsTSFAwareRequired();

BASE_EXPORT void SetForceToUseTSF();

BASE_EXPORT wchar_t* LocalAllocAndCopyString(const string16& src);

BASE_EXPORT bool IsParentalControlActivityLoggingOn();

BASE_EXPORT MetroLaunchType GetMetroLaunchParams(string16* params);

typedef void (*MetroDialogButtonPressedHandler)();

typedef void (*MetroNotificationClickedHandler)(const wchar_t* context);

typedef void (*MetroNotification)(const char* origin_url,
                                  const char* icon_url,
                                  const wchar_t* title,
                                  const wchar_t* body,
                                  const wchar_t* display_source,
                                  const char* notification_id,
                                  MetroNotificationClickedHandler handler,
                                  const wchar_t* handler_context);

typedef bool (*MetroCancelNotification)(const char* notification_id);

typedef base::Callback<void(MetroSecondaryTilePinUmaResult)>
    MetroPinUmaResultCallback;

typedef void (*MetroPinToStartScreen)(
    const string16& tile_id,
    const string16& title,
    const string16& url,
    const FilePath& logo_path,
    const MetroPinUmaResultCallback& callback);

typedef void (*MetroUnPinFromStartScreen)(
    const string16& title_id,
    const MetroPinUmaResultCallback& callback);

}  
}  

#endif  
