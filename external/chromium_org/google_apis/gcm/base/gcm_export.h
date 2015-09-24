// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_GCM_EXPORT_H_
#define GOOGLE_APIS_GCM_GCM_EXPORT_H_

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(GCM_IMPLEMENTATION)
#define GCM_EXPORT __declspec(dllexport)
#else
#define GCM_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(GCM_IMPLEMENTATION)
#define GCM_EXPORT __attribute__((visibility("default")))
#else
#define GCM_EXPORT
#endif  
#endif

#else  
#define GCM_EXPORT
#endif

#endif  
