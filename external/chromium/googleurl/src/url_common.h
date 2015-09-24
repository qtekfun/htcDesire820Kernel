// Copyright 2010, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GOOGLEURL_SRC_URL_COMMON_H__
#define GOOGLEURL_SRC_URL_COMMON_H__

#if !defined(GURL_IMPLEMENTATION)
#define GURL_IMPLEMENTATION 0
#endif

#if defined(GURL_DLL)
#if defined(WIN32)
#if GURL_IMPLEMENTATION
#define GURL_API __declspec(dllexport)
#else
#define GURL_API __declspec(dllimport)
#endif
#else
#define GURL_API __attribute__((visibility("default")))
#endif
#else
#define GURL_API
#endif

#endif  
