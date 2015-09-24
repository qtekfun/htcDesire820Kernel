// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_POLICY_SETTINGS_H_
#define CHROME_FRAME_POLICY_SETTINGS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/memory/singleton.h"

class PolicySettings {
 public:
  enum RendererForUrl {
    RENDERER_NOT_SPECIFIED = -1,
    RENDER_IN_HOST,
    RENDER_IN_CHROME_FRAME,
  };

  enum SkipMetadataCheck {
    SKIP_METADATA_CHECK_NOT_SPECIFIED = -1,
    SKIP_METADATA_CHECK_NO,
    SKIP_METADATA_CHECK_YES,
  };

  static PolicySettings* GetInstance();

  RendererForUrl default_renderer() const {
    return default_renderer_;
  }

  SkipMetadataCheck skip_metadata_check() const {
    return skip_metadata_check_;
  }

  RendererForUrl GetRendererForUrl(const wchar_t* url);

  RendererForUrl GetRendererForContentType(const wchar_t* content_type);

  
  
  const std::wstring& ApplicationLocale() const {
    return application_locale_;
  }

  
  
  
  const CommandLine& AdditionalLaunchParameters() const;

  
  bool suppress_turndown_prompt() const {
    return suppress_turndown_prompt_;
  }

  
  static void ReadUrlSettings(RendererForUrl* default_renderer,
      std::vector<std::wstring>* renderer_exclusion_list);
  static void ReadMetadataCheckSettings(SkipMetadataCheck* skip_metadata_check);
  static void ReadContentTypeSetting(
      std::vector<std::wstring>* content_type_list);
  static void ReadStringSetting(const char* value_name, std::wstring* value);
  static void ReadBoolSetting(const char* value_name, bool* value);

 protected:
  PolicySettings()
      : default_renderer_(RENDERER_NOT_SPECIFIED),
        skip_metadata_check_(SKIP_METADATA_CHECK_NOT_SPECIFIED),
        additional_launch_parameters_(CommandLine::NO_PROGRAM),
        suppress_turndown_prompt_(false) {
    RefreshFromRegistry();
  }

  ~PolicySettings() {
  }

  
  void RefreshFromRegistry();

 protected:
  RendererForUrl default_renderer_;
  SkipMetadataCheck skip_metadata_check_;
  std::vector<std::wstring> renderer_exclusion_list_;
  std::vector<std::wstring> content_type_list_;
  std::wstring application_locale_;
  CommandLine additional_launch_parameters_;
  bool suppress_turndown_prompt_;

 private:
  
  friend struct DefaultSingletonTraits<PolicySettings>;
  DISALLOW_COPY_AND_ASSIGN(PolicySettings);
};

#endif  
