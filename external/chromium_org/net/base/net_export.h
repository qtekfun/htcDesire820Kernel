// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_EXPORT_H_
#define NET_BASE_NET_EXPORT_H_


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(NET_IMPLEMENTATION)
#define NET_EXPORT __declspec(dllexport)
#define NET_EXPORT_PRIVATE __declspec(dllexport)
#else
#define NET_EXPORT __declspec(dllimport)
#define NET_EXPORT_PRIVATE __declspec(dllimport)
#endif  

#else  
#if defined(NET_IMPLEMENTATION)
#define NET_EXPORT __attribute__((visibility("default")))
#define NET_EXPORT_PRIVATE __attribute__((visibility("default")))
#else
#define NET_EXPORT
#define NET_EXPORT_PRIVATE
#endif
#endif

#else  
#define NET_EXPORT
#define NET_EXPORT_PRIVATE
#endif

#endif  
