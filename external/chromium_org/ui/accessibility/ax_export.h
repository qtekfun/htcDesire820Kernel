// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_AX_EXPORT_H_
#define UI_ACCESSIBILITY_AX_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(ACCESSIBILITY_IMPLEMENTATION)
#define AX_EXPORT __declspec(dllexport)
#else
#define AX_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(ACCESSIBILITY_IMPLEMENTATION)
#define AX_EXPORT __attribute__((visibility("default")))
#else
#define AX_EXPORT
#endif
#endif

#else  
#define AX_EXPORT
#endif

#endif  
