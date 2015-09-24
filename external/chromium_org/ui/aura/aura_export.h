// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_AURA_EXPORT_H
#define UI_AURA_AURA_EXPORT_H


#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(AURA_IMPLEMENTATION)
#define AURA_EXPORT __declspec(dllexport)
#else
#define AURA_EXPORT __declspec(dllimport)
#endif  

#else  
#if defined(AURA_IMPLEMENTATION)
#define AURA_EXPORT __attribute__((visibility("default")))
#else
#define AURA_EXPORT
#endif
#endif

#else  
#define AURA_EXPORT
#endif

#endif  
