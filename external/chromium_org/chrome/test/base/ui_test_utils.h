// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_UI_TEST_UTILS_H_
#define CHROME_TEST_BASE_UI_TEST_UTILS_H_

#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "chrome/browser/history/history_service.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_source.h"
#include "content/public/test/test_utils.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/window_open_disposition.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

class AppModalDialog;
class Browser;
class LocationBar;
class Profile;
class SkBitmap;
class TemplateURLService;

namespace base {
class FilePath;
}

namespace chrome {
struct NavigateParams;
}

namespace content {
class MessageLoopRunner;
class RenderViewHost;
class RenderWidgetHost;
class WebContents;
}

namespace gfx {
class Rect;
class Size;
}

namespace ui_test_utils {

enum BrowserTestWaitFlags {
  BROWSER_TEST_NONE = 0,                      
  BROWSER_TEST_WAIT_FOR_BROWSER = 1 << 0,     
  BROWSER_TEST_WAIT_FOR_TAB = 1 << 1,         
  BROWSER_TEST_WAIT_FOR_NAVIGATION = 1 << 2,  

  BROWSER_TEST_MASK = BROWSER_TEST_WAIT_FOR_BROWSER |
                      BROWSER_TEST_WAIT_FOR_TAB |
                      BROWSER_TEST_WAIT_FOR_NAVIGATION
};

bool GetCurrentTabTitle(const Browser* browser, string16* title);

Browser* OpenURLOffTheRecord(Profile* profile, const GURL& url);

void NavigateToURL(chrome::NavigateParams* params);

void NavigateToURLWithPost(Browser* browser, const GURL& url);

void NavigateToURL(Browser* browser, const GURL& url);

void NavigateToURLWithDisposition(Browser* browser,
                                  const GURL& url,
                                  WindowOpenDisposition disposition,
                                  int browser_test_flags);

void NavigateToURLBlockUntilNavigationsComplete(Browser* browser,
                                                const GURL& url,
                                                int number_of_navigations);

base::FilePath GetTestFilePath(const base::FilePath& dir,
                               const base::FilePath& file);

GURL GetTestUrl(const base::FilePath& dir, const base::FilePath& file);

bool GetRelativeBuildDirectory(base::FilePath* build_dir);

AppModalDialog* WaitForAppModalDialog();

int FindInPage(content::WebContents* tab,
               const string16& search_string,
               bool forward,
               bool case_sensitive,
               int* ordinal,
               gfx::Rect* selection_rect);

void WaitForTemplateURLServiceToLoad(TemplateURLService* service);

void WaitForHistoryToLoad(HistoryService* history_service);

void DownloadURL(Browser* browser, const GURL& download_url);

void SendToOmniboxAndSubmit(LocationBar* location_bar,
                            const std::string& input);

Browser* GetBrowserNotInSet(std::set<Browser*> excluded_browsers);

class WindowedTabAddedNotificationObserver
    : public content::WindowedNotificationObserver {
 public:
  
  
  
  explicit WindowedTabAddedNotificationObserver(
      const content::NotificationSource& source);

  
  content::WebContents* GetTab() { return added_tab_; }

  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::WebContents* added_tab_;

  DISALLOW_COPY_AND_ASSIGN(WindowedTabAddedNotificationObserver);
};

template <class U>
class WindowedNotificationObserverWithDetails
    : public content::WindowedNotificationObserver {
 public:
  WindowedNotificationObserverWithDetails(
      int notification_type,
      const content::NotificationSource& source)
      : content::WindowedNotificationObserver(notification_type, source) {}

  
  bool GetDetailsFor(uintptr_t source, U* details) {
    typename std::map<uintptr_t, U>::const_iterator iter =
        details_.find(source);
    if (iter == details_.end())
      return false;
    *details = iter->second;
    return true;
  }

  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE {
    const U* details_ptr = content::Details<U>(details).ptr();
    if (details_ptr)
      details_[source.map_key()] = *details_ptr;
    content::WindowedNotificationObserver::Observe(type, source, details);
  }

 private:
  std::map<uintptr_t, U> details_;

  DISALLOW_COPY_AND_ASSIGN(WindowedNotificationObserverWithDetails);
};

class UrlLoadObserver : public content::WindowedNotificationObserver {
 public:
  
  
  
  UrlLoadObserver(const GURL& url, const content::NotificationSource& source);
  virtual ~UrlLoadObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  GURL url_;

  DISALLOW_COPY_AND_ASSIGN(UrlLoadObserver);
};

class BrowserAddedObserver {
 public:
  BrowserAddedObserver();
  ~BrowserAddedObserver();

  
  Browser* WaitForSingleNewBrowser();

 private:
  content::WindowedNotificationObserver notification_observer_;
  std::set<Browser*> original_browsers_;

  DISALLOW_COPY_AND_ASSIGN(BrowserAddedObserver);
};

bool TakeRenderWidgetSnapshot(content::RenderWidgetHost* rwh,
                              const gfx::Size& page_size,
                              SkBitmap* bitmap) WARN_UNUSED_RESULT;

bool TakeEntirePageSnapshot(content::RenderViewHost* rvh,
                            SkBitmap* bitmap) WARN_UNUSED_RESULT;

#if defined(OS_WIN)
bool SaveScreenSnapshotToDirectory(const base::FilePath& directory,
                                   base::FilePath* screenshot_path);

bool SaveScreenSnapshotToDesktop(base::FilePath* screenshot_path);
#endif

void OverrideGeolocation(double latitude, double longitude);

class HistoryEnumerator {
 public:
  explicit HistoryEnumerator(Profile* profile);
  ~HistoryEnumerator();

  std::vector<GURL>& urls() { return urls_; }

 private:
  void HistoryQueryComplete(
      const base::Closure& quit_task,
      HistoryService::Handle request_handle,
      history::QueryResults* results);

  std::vector<GURL> urls_;

  CancelableRequestConsumer consumer_;

  DISALLOW_COPY_AND_ASSIGN(HistoryEnumerator);
};

}  

#endif  
