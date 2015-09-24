// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_NIX_XDG_UTIL_H_
#define BASE_NIX_XDG_UTIL_H_
#pragma once


#ifdef nix
#error asdf
#endif

class FilePath;

namespace base {

class Environment;

namespace nix {

FilePath GetXDGDirectory(Environment* env, const char* env_name,
                         const char* fallback_dir);

FilePath GetXDGUserDirectory(Environment* env, const char* dir_name,
                             const char* fallback_dir);

enum DesktopEnvironment {
  DESKTOP_ENVIRONMENT_OTHER,
  DESKTOP_ENVIRONMENT_GNOME,
  
  
  DESKTOP_ENVIRONMENT_KDE3,
  DESKTOP_ENVIRONMENT_KDE4,
  DESKTOP_ENVIRONMENT_XFCE,
};

DesktopEnvironment GetDesktopEnvironment(Environment* env);

const char* GetDesktopEnvironmentName(DesktopEnvironment env);
const char* GetDesktopEnvironmentName(Environment* env);

}  
}  

#endif  
