// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once
#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_USER_IMAGES_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_USER_IMAGES_H_

#include <cstddef>  
#include <string>

namespace chromeos {

std::string GetDefaultImagePath(int index);

bool IsDefaultImagePath(const std::string& path, int* image_id);

std::string GetDefaultImageUrl(int index);

bool IsDefaultImageUrl(const std::string url, int* image_id);

extern const int kDefaultImageResources[];

extern const int kDefaultImagesCount;

}  

#endif  
