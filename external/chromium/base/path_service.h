// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PATH_SERVICE_H_
#define BASE_PATH_SERVICE_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/base_paths.h"
#include "build/build_config.h"

class FilePath;

class BASE_API PathService {
 public:
  
  
  
  
  
  
  
  static bool Get(int key, FilePath* path);

  
  
  
  
  
  
  
  
  
  
  static bool Override(int key, const FilePath& path);

  
  
  
  
  
  
  
  
  typedef bool (*ProviderFunc)(int, FilePath*);

  
  
  static void RegisterProvider(ProviderFunc provider,
                               int key_start,
                               int key_end);
 private:
  static bool GetFromCache(int key, FilePath* path);
  static bool GetFromOverrides(int key, FilePath* path);
  static void AddToCache(int key, const FilePath& path);
};

#endif  
