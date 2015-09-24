// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_NET_URL_TRANSLATOR_H_
#define CHROME_BROWSER_SYNC_ENGINE_NET_URL_TRANSLATOR_H_
#pragma once

#include <string>

namespace browser_sync {

std::string CgiEscapeString(const char* src);
std::string CgiEscapeString(const std::string& src);

std::string MakeSyncServerPath(const std::string& path,
                               const std::string& query_string);

std::string MakeSyncQueryString(const std::string& client_id);

}  

#endif  
