// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CHROME_VIEWS_EXPORT_H_
#define CHROME_BROWSER_UI_VIEWS_CHROME_VIEWS_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(CHROME_VIEWS_IMPLEMENTATION)
#define CHROME_VIEWS_EXPORT __declspec(dllexport)
#else
#define CHROME_VIEWS_EXPORT __declspec(dllimport)
#endif  

#else  

#if defined(CHROME_VIEWS_IMPLEMENTATION)
#define CHROME_VIEWS_EXPORT __attribute__((visibility("default")))
#else
#define CHROME_VIEWS_EXPORT
#endif

#endif  

#else  

#define CHROME_VIEWS_EXPORT

#endif  

#endif  
