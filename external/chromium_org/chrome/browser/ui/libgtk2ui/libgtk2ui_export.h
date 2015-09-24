// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LIBGTK2UI_LIBGTK2UI_EXPORT_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_LIBGTK2UI_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#error "LIBGTK2UI does not build on Windows."

#else  
#if defined(LIBGTK2UI_IMPLEMENTATION)
#define LIBGTK2UI_EXPORT __attribute__((visibility("default")))
#else
#define LIBGTK2UI_EXPORT
#endif
#endif

#else  
#define LIBGTK2UI_EXPORT
#endif

#endif  
