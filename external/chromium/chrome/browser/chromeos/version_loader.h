// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_VERSION_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_VERSION_LOADER_H_
#pragma once

#include <string>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "content/browser/cancelable_request.h"

class FilePath;

namespace chromeos {

class VersionLoader : public CancelableRequestProvider {
 public:
  VersionLoader();

  enum VersionFormat {
    VERSION_SHORT,
    VERSION_SHORT_WITH_DATE,
    VERSION_FULL,
  };

  
  typedef Callback2<Handle, std::string>::Type GetVersionCallback;
  typedef CancelableRequest<GetVersionCallback> GetVersionRequest;

  typedef Callback2<Handle, std::string>::Type GetFirmwareCallback;
  typedef CancelableRequest<GetFirmwareCallback> GetFirmwareRequest;

  
  
  
  Handle GetVersion(CancelableRequestConsumerBase* consumer,
                    GetVersionCallback* callback,
                    VersionFormat format);

  Handle GetFirmware(CancelableRequestConsumerBase* consumer,
                     GetFirmwareCallback* callback);

  
  
  
  
  void EnablePlatformVersions(bool enable);

  static const char kFullVersionPrefix[];
  static const char kVersionPrefix[];
  static const char kFirmwarePrefix[];

 private:
  FRIEND_TEST_ALL_PREFIXES(VersionLoaderTest, ParseFullVersion);
  FRIEND_TEST_ALL_PREFIXES(VersionLoaderTest, ParseVersion);
  FRIEND_TEST_ALL_PREFIXES(VersionLoaderTest, ParseFirmware);

  
  
  class Backend : public base::RefCountedThreadSafe<Backend> {
   public:
    Backend() : parse_as_platform_(false) {}

    
    
    
    void GetVersion(scoped_refptr<GetVersionRequest> request,
                    VersionFormat format);

    
    
    void GetFirmware(scoped_refptr<GetFirmwareRequest> request);

    void set_parse_as_platform(bool value) { parse_as_platform_ = value; }

   private:
    friend class base::RefCountedThreadSafe<Backend>;

    bool parse_as_platform_;

    ~Backend() {}

    DISALLOW_COPY_AND_ASSIGN(Backend);
  };

  
  
  static std::string ParseVersion(const std::string& contents,
                                  const std::string& prefix);

  
  static std::string ParseFirmware(const std::string& contents);

  scoped_refptr<Backend> backend_;

  DISALLOW_COPY_AND_ASSIGN(VersionLoader);
};

}  

#endif  
