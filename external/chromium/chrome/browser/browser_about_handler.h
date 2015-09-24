// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSER_ABOUT_HANDLER_H_
#define CHROME_BROWSER_BROWSER_ABOUT_HANDLER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/process.h"
#include "base/string_util.h"

template <typename T> struct DefaultSingletonTraits;
class GURL;
class Profile;

bool WillHandleBrowserAboutURL(GURL* url, Profile* profile);

void InitializeAboutDataSource(Profile* profile);

bool HandleNonNavigationAboutURL(const GURL& url);

std::vector<std::string> AboutPaths();

#if defined(USE_TCMALLOC)
typedef std::map<std::string, std::string> AboutTcmallocOutputsType;

class AboutTcmallocOutputs {
 public:
  
  static AboutTcmallocOutputs* GetInstance();

  AboutTcmallocOutputsType* outputs() { return &outputs_; }

  
  void SetOutput(const std::string& header, const std::string& output) {
    outputs_[header] = output;
  }

  
  
  
  void RendererCallback(base::ProcessId pid, const std::string& output) {
    SetOutput(StringPrintf("Renderer PID %d", static_cast<int>(pid)), output);
  }

 private:
  AboutTcmallocOutputs();
  ~AboutTcmallocOutputs();

  AboutTcmallocOutputsType outputs_;

  friend struct DefaultSingletonTraits<AboutTcmallocOutputs>;

  DISALLOW_COPY_AND_ASSIGN(AboutTcmallocOutputs);
};

void AboutTcmallocRendererCallback(base::ProcessId pid,
                                   const std::string& output);
#endif

#endif  
