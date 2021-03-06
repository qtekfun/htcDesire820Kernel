// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OAK_OAK_EXPORT_H_
#define UI_OAK_OAK_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(OAK_IMPLEMENTATION)
#define OAK_EXPORT __declspec(dllexport)
#else
#define OAK_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(OAK_IMPLEMENTATION)
#define OAK_EXPORT __attribute__((visibility("default")))
#else
#define OAK_EXPORT
#endif
#endif

#else  
#define OAK_EXPORT
#endif

#endif  
