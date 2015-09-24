// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_THEME_HELPERS_H__
#define CHROME_BROWSER_UI_VIEWS_THEME_HELPERS_H__
#pragma once

#include <windows.h>

#include "third_party/skia/include/core/SkColor.h"

void GetRebarGradientColors(int width, int x1, int x2,
                            SkColor* c1, SkColor* c2);


void GetDarkLineColor(SkColor* dark_color);

#endif  
