// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SNAPSHOT_SNAPSHOT_EXPORT_H
#define UI_SNAPSHOT_SNAPSHOT_EXPORT_H


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(SNAPSHOT_IMPLEMENTATION)
#define SNAPSHOT_EXPORT __declspec(dllexport)
#else
#define SNAPSHOT_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(SNAPSHOT_IMPLEMENTATION)
#define SNAPSHOT_EXPORT __attribute__((visibility("default")))
#else
#define SNAPSHOT_EXPORT
#endif
#endif

#else  
#define SNAPSHOT_EXPORT
#endif

#endif  
