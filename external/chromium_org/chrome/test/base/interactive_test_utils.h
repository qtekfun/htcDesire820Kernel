// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_INTERACTIVE_TEST_UTILS_H_
#define CHROME_TEST_BASE_INTERACTIVE_TEST_UTILS_H_

#include "chrome/browser/ui/view_ids.h"
#include "chrome/test/base/ui_test_utils.h"
#include "ui/base/test/ui_controls.h"

#if defined(TOOLKIT_VIEWS)
#include "ui/views/view.h"
#endif

namespace gfx {
class Point;
}

namespace ui_test_utils {

bool BringBrowserWindowToFront(const Browser* browser) WARN_UNUSED_RESULT;

bool IsViewFocused(const Browser* browser, ViewID vid);

void ClickOnView(const Browser* browser, ViewID vid);


void HideNativeWindow(gfx::NativeWindow window);

bool ShowAndFocusNativeWindow(gfx::NativeWindow window) WARN_UNUSED_RESULT;

bool SendKeyPressSync(const Browser* browser,
                      ui::KeyboardCode key,
                      bool control,
                      bool shift,
                      bool alt,
                      bool command) WARN_UNUSED_RESULT;

bool SendKeyPressToWindowSync(const gfx::NativeWindow window,
                              ui::KeyboardCode key,
                              bool control,
                              bool shift,
                              bool alt,
                              bool command) WARN_UNUSED_RESULT;

bool SendKeyPressAndWait(const Browser* browser,
                         ui::KeyboardCode key,
                         bool control,
                         bool shift,
                         bool alt,
                         bool command,
                         int type,
                         const content::NotificationSource& source)
                             WARN_UNUSED_RESULT;

bool SendMouseMoveSync(const gfx::Point& location) WARN_UNUSED_RESULT;
bool SendMouseEventsSync(ui_controls::MouseButton type,
                         int state) WARN_UNUSED_RESULT;

template <class U>
bool SendKeyPressAndWaitWithDetails(
    const Browser* browser,
    ui::KeyboardCode key,
    bool control,
    bool shift,
    bool alt,
    bool command,
    int type,
    const content::NotificationSource& source,
    const content::Details<U>& details) WARN_UNUSED_RESULT;

template <class U>
bool SendKeyPressAndWaitWithDetails(
    const Browser* browser,
    ui::KeyboardCode key,
    bool control,
    bool shift,
    bool alt,
    bool command,
    int type,
    const content::NotificationSource& source,
    const content::Details<U>& details) {
  WindowedNotificationObserverWithDetails<U> observer(type, source);

  if (!SendKeyPressSync(browser, key, control, shift, alt, command))
    return false;

  observer.Wait();

  U my_details;
  if (!observer.GetDetailsFor(source.map_key(), &my_details))
    return false;

  return *details.ptr() == my_details && !testing::Test::HasFatalFailure();
}

void MoveMouseToCenterAndPress(
#if defined(TOOLKIT_VIEWS)
    views::View* view,
#elif defined(TOOLKIT_GTK)
    GtkWidget* widget,
#elif defined(OS_IOS)
    UIView* view,
#elif defined(OS_MACOSX)
    NSView* view,
#endif
    ui_controls::MouseButton button,
    int state,
    const base::Closure& task);

namespace internal {

void ClickTask(ui_controls::MouseButton button,
               int state,
               const base::Closure& followup);

}  

}  

#endif  
