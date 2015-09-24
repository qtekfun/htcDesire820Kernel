// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BUG_REPORT_DATA_H_
#define CHROME_BROWSER_BUG_REPORT_DATA_H_

#include <string>
#include <vector>

#include "base/utf_string_conversions.h"
#include "chrome/browser/bug_report_util.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/cros/syslogs_library.h"
#endif

class BugReportData {
 public:
  
  
  
  
  
  
  BugReportData();
  ~BugReportData();

  
  void SendReport();

  void UpdateData(Profile* profile,
                  const std::string& target_tab_url,
                  const int problem_type,
                  const std::string& page_url,
                  const std::string& description,
                  const std::vector<unsigned char>& image
#if defined(OS_CHROMEOS)
                  , const std::string& user_email
                  , const bool send_sys_info
                  , const bool sent_report
#endif
                  );

#if defined(OS_CHROMEOS)
  void SyslogsComplete(chromeos::LogDictionaryType* logs,
                       std::string* zip_content);
#endif

  const std::string& target_tab_url() const { return target_tab_url_; }

  int problem_type() const { return problem_type_; }
  const std::string& page_url() const { return page_url_; }
  const std::string& description() const { return description_; }
  const std::vector<unsigned char>& image() const { return image_; }
#if defined(OS_CHROMEOS)
  const std::string& user_email() const { return user_email_; }
  chromeos::LogDictionaryType* sys_info() const { return sys_info_; }
  bool send_sys_info() const { return send_sys_info_; }
  bool sent_report() const { return sent_report_; }
  std::string* zip_content() const { return zip_content_; }
#endif


 private:
  Profile* profile_;

  
  std::string target_tab_url_;

  int problem_type_;
  std::string page_url_;
  std::string description_;
  std::vector<unsigned char> image_;

#if defined(OS_CHROMEOS)
  
  std::string user_email_;
  chromeos::LogDictionaryType* sys_info_;
  
  std::string* zip_content_;
  
  
  bool sent_report_;
  
  bool send_sys_info_;
#endif
};

#endif  
