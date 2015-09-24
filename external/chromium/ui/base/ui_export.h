// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_UI_EXPORT_H_
#define UI_UI_EXPORT_H_
#pragma once


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(UI_IMPLEMENTATION)
#define UI_EXPORT __declspec(dllexport)
#else
#define UI_EXPORT __declspec(dllimport)
#endif  

#else  
#define UI_EXPORT __attribute__((visibility("default")))
#endif

#else  
#define UI_EXPORT
#endif

#endif  
