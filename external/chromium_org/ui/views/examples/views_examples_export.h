// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_VIEWS_EXAMPLES_EXPORT_H_
#define UI_VIEWS_EXAMPLES_VIEWS_EXAMPLES_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(VIEWS_EXAMPLES_IMPLEMENTATION)
#define VIEWS_EXAMPLES_EXPORT __declspec(dllexport)
#else
#define VIEWS_EXAMPLES_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(VIEWS_EXAMPLES_IMPLEMENTATION)
#define VIEWS_EXAMPLES_EXPORT __attribute__((visibility("default")))
#else
#define VIEWS_EXAMPLES_EXPORT
#endif
#endif

#else  
#define VIEWS_EXAMPLES_EXPORT
#endif

#endif  
