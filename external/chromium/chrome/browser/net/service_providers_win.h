// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_SERVICE_PROVIDERS_WIN_H_
#define CHROME_BROWSER_NET_SERVICE_PROVIDERS_WIN_H_
#pragma once

#include <vector>

class Value;

struct WinsockNamespaceProvider {
  std::wstring name;
  int version;
  bool active;
  int type;
};
typedef std::vector<WinsockNamespaceProvider> WinsockNamespaceProviderList;

struct WinsockLayeredServiceProvider {
  std::wstring name;
  std::wstring path;
  int version;
  int chain_length;
  int socket_type;
  int socket_protocol;
};
typedef std::vector<WinsockLayeredServiceProvider>
    WinsockLayeredServiceProviderList;

void GetWinsockNamespaceProviders(
    WinsockNamespaceProviderList* namespace_list);

void GetWinsockLayeredServiceProviders(
    WinsockLayeredServiceProviderList* service_list);

#endif  

