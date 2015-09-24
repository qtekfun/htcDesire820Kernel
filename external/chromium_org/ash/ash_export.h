// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ASH_EXPORT_H_
#define ASH_ASH_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(ASH_IMPLEMENTATION)
#define ASH_EXPORT __declspec(dllexport)
#else
#define ASH_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(ASH_IMPLEMENTATION)
#define ASH_EXPORT __attribute__((visibility("default")))
#else
#define ASH_EXPORT
#endif
#endif

#else  
#define ASH_EXPORT
#endif

#endif  
