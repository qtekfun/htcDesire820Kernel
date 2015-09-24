// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_EXPORT_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(MESSAGE_CENTER_IMPLEMENTATION)
#define MESSAGE_CENTER_EXPORT __declspec(dllexport)
#else
#define MESSAGE_CENTER_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(MESSAGE_CENTER_IMPLEMENTATION)
#define MESSAGE_CENTER_EXPORT __attribute__((visibility("default")))
#else
#define MESSAGE_CENTER_EXPORT
#endif
#endif

#else  
#define MESSAGE_CENTER_EXPORT
#endif

#endif  
