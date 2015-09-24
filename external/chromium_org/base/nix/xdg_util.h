// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_NIX_XDG_UTIL_H_
#define BASE_NIX_XDG_UTIL_H_


#include "base/base_export.h"

#ifdef nix
#error asdf
#endif

namespace base {

class Environment;
class FilePath;

namespace nix {

BASE_EXPORT extern const char kDotConfigDir[];

BASE_EXPORT extern const char kXdgConfigHomeEnvVar[];

BASE_EXPORT FilePath GetXDGDirectory(Environment* env, const char* env_name,
                                     const char* fallback_dir);

BASE_EXPORT FilePath GetXDGUserDirectory(const char* dir_name,
                                         const char* fallback_dir);

enum DesktopEnvironment {
  DESKTOP_ENVIRONMENT_OTHER,
  DESKTOP_ENVIRONMENT_GNOME,
  
  
  DESKTOP_ENVIRONMENT_KDE3,
  DESKTOP_ENVIRONMENT_KDE4,
  DESKTOP_ENVIRONMENT_UNITY,
  DESKTOP_ENVIRONMENT_XFCE,
};

BASE_EXPORT DesktopEnvironment GetDesktopEnvironment(Environment* env);

BASE_EXPORT const char* GetDesktopEnvironmentName(DesktopEnvironment env);
BASE_EXPORT const char* GetDesktopEnvironmentName(Environment* env);

}  
}  

#endif  
