// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_USER_IMAGES_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_USER_IMAGES_H_

#include <cstddef>  
#include <string>

#include "base/strings/string16.h"

namespace gfx {
class ImageSkia;
}

namespace chromeos {

std::string GetDefaultImagePath(int index);

bool IsDefaultImagePath(const std::string& path, int* image_id);

std::string GetDefaultImageUrl(int index);

bool IsDefaultImageUrl(const std::string& url, int* image_id);

const gfx::ImageSkia& GetDefaultImage(int index);

base::string16 GetDefaultImageDescription(int index);

extern const int kDefaultImageResourceIDs[];

extern const int kDefaultImageAuthorIDs[];

extern const int kDefaultImageWebsiteIDs[];

extern const int kDefaultImagesCount;

extern const int kFirstDefaultImageIndex;


extern const int kHistogramImageFromFile;

extern const int kHistogramImageFromCamera;

extern const int kHistogramImageOld;

extern const int kHistogramImageFromProfile;

extern const int kHistogramVideoFromCamera;

extern const int kHistogramVideoFromFile;

extern const int kHistogramImagesCount;

int GetDefaultImageHistogramValue(int index);

}  

#endif  
