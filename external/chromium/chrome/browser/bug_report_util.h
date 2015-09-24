// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BUG_REPORT_UTIL_H_
#define CHROME_BROWSER_BUG_REPORT_UTIL_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/userfeedback/proto/common.pb.h"
#include "chrome/browser/userfeedback/proto/extension.pb.h"
#include "chrome/browser/userfeedback/proto/math.pb.h"
#include "ui/gfx/rect.h"

#if defined(OS_MACOSX)
#include "base/sys_info.h"
#elif defined(OS_WIN)
#include "base/win/windows_version.h"
#elif defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/cros/syslogs_library.h"
#include "chrome/browser/chromeos/cros/cros_library.h"
#endif

class Profile;
class TabContents;

class BugReportUtil {
 public:

#if defined(OS_MACOSX)
  enum BugType {
    PAGE_WONT_LOAD = 0,
    PAGE_LOOKS_ODD,
    PHISHING_PAGE,
    CANT_SIGN_IN,
    CHROME_MISBEHAVES,
    SOMETHING_MISSING,
    BROWSER_CRASH,
    OTHER_PROBLEM
  };
#endif


  
  
  
  
  
  
  
  
  
  static void SetOSVersion(std::string *os_version);

  
  static void SetFeedbackServer(const std::string& server);

  
  static void DispatchFeedback(Profile* profile, std::string* feedback_data,
                               int64 delay);


  
  static void SendReport(Profile* profile,
      int problem_type,
      const std::string& page_url_text,
      const std::string& description,
      const char* png_data,
      int png_data_length,
      int png_width,
#if defined(OS_CHROMEOS)
      int png_height,
      const std::string& user_email_text,
      const char* zipped_logs_data,
      int zipped_logs_length,
      const chromeos::LogDictionaryType* const sys_info);
#else
      int png_height);
#endif

  
  static void ReportPhishing(TabContents* currentTab,
                             const std::string& phishing_url);

  class PostCleanup;

 private:
  
  static void AddFeedbackData(
      userfeedback::ExternalExtensionSubmit* feedback_data,
      const std::string& key, const std::string& value);

  
  static void SendFeedback(Profile* profile, std::string* feedback_data,
                           int64 previous_delay);

#if defined(OS_CHROMEOS)
  static bool ValidFeedbackSize(const std::string& content);
#endif

  static std::string feedback_server_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(BugReportUtil);
};

#endif  
