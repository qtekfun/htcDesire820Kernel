// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_EXPORT_H_
#define UI_KEYBOARD_KEYBOARD_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(KEYBOARD_IMPLEMENTATION)
#define KEYBOARD_EXPORT __declspec(dllexport)
#else
#define KEYBOARD_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(KEYBOARD_IMPLEMENTATION)
#define KEYBOARD_EXPORT __attribute__((visibility("default")))
#else
#define KEYBOARD_EXPORT
#endif
#endif

#else  
#define KEYBOARD_EXPORT
#endif

#endif  
